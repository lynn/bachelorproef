/**
 * Script for the visualizer webpage.
 */

$(document).ready(function(){
    // Register the file-loading event
    visualizer = new Visualizer('.file-input', '.day-control', '#view');
})

//--------------------//
// File reading stuff //
//--------------------//

/// Handler passed to the file selector, called when a file is selected.
function readSingleFile(f, handler) {
    var file = f.target.files[0];
    if (!file) {
        console.log("File not found!");
        return;
    }
    var reader = new FileReader();
    reader.onload = handler;
    reader.readAsText(file);
}

/// Cleans up the JSON that boost generated by parsing strings back to numerics where needed.
function cleanData(data){
    for(i in data.towns){
        town = data.towns[i];
        town.size = parseInt(town.size);
        town.lat = parseFloat(town.lat);
        town.long = parseFloat(town.long);
    }
    for(i in data.days){
        day = data.days[i];
        for(town in day)
            day[town] = parseInt(day[town]);
    }
}


//------------//
// Class: RGB //
//------------//

var RGB = function(r=0,g=0,b=0){
    this.r = r;
    this.g = g;
    this.b = b;
}

RGB.prototype.mix = function(other, p){
    var out = new RGB();
    out.r = this.r * p + other.r * (1 - p);
    out.g = this.g * p + other.g * (1 - p);
    out.b = this.b * p + other.b * (1 - p);
    return out;
}

RGB.prototype.toString = function(){
    return "rgb(" + Math.round(this.r) + "," + Math.round(this.g) + "," + Math.round(this.b) + ")";
}


//-----------------//
// Class: Gradient //
//-----------------//

function ValCol(val, colour){
    return {val: val, colour: colour};
}

var Gradient = function(colourMap){
    // colourMap is a List[Dict{val, colour}] sorted by increasing val
    this.colourMap = colourMap;
}

Gradient.prototype.get = function(val){
    // Check the upper and lower bounds first
    if(val <= this.colourMap[0].val)
        return this.colourMap[0].colour;
    if(val >= this.colourMap[this.colourMap.length-1].val)
        return this.colourMap[this.colourMap.length-1].colour;

    // It has to be somewhere in the gradient
    for(i in this.colourMap){
        upper = this.colourMap[i];
        if( val == upper.val)
            return upper.colour;
        if( val < upper.val ){
            lower = this.colourMap[i-1];
            p = (upper.val - val) / (upper.val - lower.val);
            return lower.colour.mix(upper.colour, p);
        }
    }   
}


//------------//
// Class: Map //
//------------//

/// Make a new Map by the following attributes
/// imageRef: ref to the map image
/// ratio: horizontal/vertical ratio of the image
/// min/maxLat/Long: bounding box of the map image
var Map = function(name, imageRef, imageRatio, minLat, maxLat, minLong, maxLong, overflow=false){
    this.name = name;
    this.imageRef = imageRef;
    this.imageRatio = imageRatio;
    this.box = {minLat: minLat, maxLat: maxLat, minLong: minLong, maxLong: maxLong};
}

/// Test if the given box fits inside the map.
Map.prototype.containsBox = function(box){
    if(this.box.minLat > box.minLat)
        return false;
    if(this.box.maxLat < box.maxLat)
        return false;
    if(this.box.minLong > box.minLong)
        return false;
    if(this.box.maxLong < box.maxLong)
        return false;
    return true;
}

Map.prototype.fitTo = function(width, height){
    if(this.imageRatio > width/height)
        return {width: width, height: width / this.imageRatio};
    if(this.imageRatio < width/height)
        return {width: height * this.imageRatio, height: height};
    return {width: width, height: height};
}

Map.belgium = new Map("Belgium", "resource/belgium.svg", 1135.92/987.997, 49.2, 51.77, 2.19, 6.87);
// Todo: Figure out smarter map-to-dot algorithms for non-mercator projections.

Map.maps = [Map.belgium];

//-------------------//
// Class: Visualizer //
//-------------------//

/// Make a new Visualizer bound to the given elements.
var Visualizer = function(inputSelector, controlSelector, viewSelector){
    // Place our hook into the file selector
    $(inputSelector).on('change', f => readSingleFile(f, this.handleFile.bind(this)));
    // Place our hooks into the controls
    this.initializeControls(controlSelector);
    // Remember our view
    this.$view = $(viewSelector);
}

/// Bind into the controls and set the appropriate events.
Visualizer.prototype.initializeControls = function (controlSelector){
    $c = $(controlSelector);
    this.control = c = Object();
    c.$prevDay = $c.find('.prev-day');
    c.$nextDay = $c.find('.next-day');
    c.$range = $c.find('.range-input');
    c.all = [c.$prevDay, c.$nextDay, c.$range];

    // Bind events
    c.$prevDay.on("click", this.prevDay.bind(this));
    c.$nextDay.on("click", this.nextDay.bind(this));
    c.$range.on("input", () => this.updateDay(parseInt(this.control.$range.val())));

    // Disable the controls, they're not ready yet.
    this.disableControls();
}

/// Set up and enable the controls based on the simulation data.
Visualizer.prototype.configureControls = function(){
    this.control.$range.prop("max", this.maxDays);
    // Enable the controls
    this.disableControls(false);
}

/// Control interface methods:
Visualizer.prototype.prevDay = function(){ this.updateDay(visualizer.day - 1); }
Visualizer.prototype.nextDay = function(){ this.updateDay(visualizer.day + 1); }

/// Enable or disable the controls.
Visualizer.prototype.disableControls = function(val=true){
    for(i in this.control.all)
        this.control.all[i].prop("disabled", val);
}

/// Handler passed to the FileReader, called with the contents of the selected file.
Visualizer.prototype.handleFile = function(e) {
    var data = JSON.parse(e.target.result);
    cleanData(data);
    this.initialize(data);
}

/// Actually initialize the Visualizer with the data retrieved from the file.
Visualizer.prototype.initialize = function(data){
    // Grab the data parsed from the file
    this.days = data.days;
    this.towns = data.towns;

    // Put the total number of days
    this.maxDays = this.days.length;
    $('.days').text(this.maxDays);
    
    // set up the view
    this.makeView();
    this.updateDay(0);

    // enable the controls
    this.configureControls();
}

/// Prepare the HTML document with the basic frameworks for our view.
Visualizer.prototype.makeView = function(){
    this.makeTable();
    this.makeMap();
}

/// Prepare the HTML document with a basic table.
Visualizer.prototype.makeTable = function(){
    // Find and clear the view target
    $target = this.$view.find(".table-view");
    $target.html('');

    // Make a table
    this.$table = $table = $('<table>', {class:'table table-striped table-condensed'});
    $target.append($table);

    // Header
    $row = $('<tr>');
    $row.append($('<th>Name</th>'));
    $row.append($('<th>Inhabitants</th>'));
    $row.append($('<th>Infected</th>'));
    $row.append($('<th>Percentage</th>'));
    $table.append($row);

    // Rows
    for(town in this.towns){
        $row = $('<tr>', {id:noSpace(town)});
        $row.append($('<td>' + town + '</td>'));
        $row.append($('<td>' + this.towns[town].size + '</td>'));
        $row.append($('<td>', {class:'infected'}));
        $row.append($('<td>', {class:'percent'}));
        $table.append($row);
    }
}

/// Prepare the HTML document with a basic table.
Visualizer.prototype.makeMap = function(){
    // Find and clear the view target
    var $target = this.$view.find(".map-view");
    $target.svg("destroy");
    $target.html("");

    // Set up colour stuff
    // TODO: those left hand values should scale.
    var colourMap = [
        ValCol(0,new RGB()),
        ValCol(5,new RGB(255,0,0)),
        ValCol(10,new RGB(255,255,0)),
        ValCol(15,new RGB(255,255,255)),
    ];
    this.gradient = new Gradient(colourMap);

    var box = this.findBox();
    var width = 800;
    var height = 600;

    // Check if any of the data fits inside a known map.
    for(i in Map.maps){
        theMap = Map.maps[i];
        if(theMap.containsBox(box)){
            console.log("Found map: " + theMap.name);
            box = theMap.box;

            // Find the appropriate size for the map image
            var o = theMap.fitTo(width, height);
            width = o.width;
            height = o.height;

            // Place it on the page
            $img = $("<img>",{src:theMap.imageRef,width:width,height:height});
            $target.append($img);
            break;
        }
    }

    // Functions converting lat/longitudes into percentages.
    var latFunc = lat => (lat - box.minLat) / (box.maxLat - box.minLat);
    var longFunc = long => (long - box.minLong) / (box.maxLong - box.minLong);

    // Make an SVG object
    $target.svg({settings: {width:width,height:height}});
    var svgMap = $target.svg("get");

    // Fill it with circles
    for(town in this.towns){
        // Determine its features
        var x = percentFormat(longFunc(this.towns[town].long));
        var y = percentFormat(1- latFunc(this.towns[town].lat));
        var radius = 5;
        var fillColour = "red";

        // Make and attach the circle
        var dot = svgMap.circle(x, y, radius, {fill: fillColour});

        // Remember the circle
        this.towns[town].dot = dot;

        // Add a little tooltip
        dot.setAttribute("title", town);
        dot.setAttribute("data-toggle", "tooltip");
        dot.setAttribute("data-container", "body");
    }
    refreshTooltips();
}

Visualizer.prototype.findBox = function(){
    var out = {minLat: 1000, maxLat: -1000, minLong: 1000, maxLong: -1000};

    for(town in this.towns){
        out.minLat = Math.min(this.towns[town].lat, out.minLat);
        out.maxLat = Math.max(this.towns[town].lat, out.maxLat);
        out.minLong = Math.min(this.towns[town].long, out.minLong);
        out.maxLong = Math.max(this.towns[town].long, out.maxLong);
    }

    return out;
}

/// If given a valid day, update the view to match the info at that day.
Visualizer.prototype.updateDay = function(day){
    // clamp day to the valid range
    day = clamp(day, 0, this.maxDays-1);

    this.day = day;
    $('.current-day').text(1 + day);
    this.control.$range.prop("value", day);

    this.updateView();
}

/// Update the view to reflect the currently selected day.
Visualizer.prototype.updateView = function(){
    this.updateTable();
    this.updateMap();
}

/// Update the table to reflect the currently selected day.
Visualizer.prototype.updateTable = function(){
    var currentDay = this.days[this.day];
    var total = 0;

    // Update the view further
    for(town in this.towns){
        val = currentDay[town] || 0;
        total += val;
        // Find the table column for the given town
        $col = this.$table.find('#' + noSpace(town));

        // Put the amount of infected
        $col.find(".infected").text(val);
        // Write the percentage infected if any
        var percent = val/this.towns[town].size;
        $col.find(".percent").text(percent? percentFormat(percent) : '');
    }
    // Put the total infected where we can see it.
    $('.total-infected').text(total);
}

/// Update the map to reflect the currently selected day.
Visualizer.prototype.updateMap = function(){
    var currentDay = this.days[this.day];

    for(town in this.towns){
        val = currentDay[town] || 0;
        dot = this.towns[town].dot;
        dot.setAttribute("fill", this.gradient.get(val).toString());
        dot.setAttribute("r",Math.sqrt(val) * 2 + 2);
    }
}


//------//
// Etc. //
//------//

// Remove all spaces from a given string.
var noSpace = s => s.split(" ").join('');

// Nicely format a given value p as a percentage showing d digits after the period.
var percentFormat = (p, d=1) => (100*p).toFixed(d)+'%';

// Clamp val to be on [l, r]
var clamp = (val, l, r) => val > r ? r : val < l ? l : val;

// Refresh Bootstrap's tooltips
var refreshTooltips = () => $('[data-toggle="tooltip"]').tooltip(); 