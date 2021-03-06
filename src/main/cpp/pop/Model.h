#ifndef POPULATION_MODEL_H_INCLUDED
#define POPULATION_MODEL_H_INCLUDED

/**
 * @file
 * Header file for the population model class
 */

#include <map>
#include <memory>
#include <vector>
#include <boost/property_tree/ptree.hpp>
#include "util/InclusiveRange.h"

namespace stride {
namespace population {

struct Model;
using ModelRef = std::shared_ptr<const Model>;

/**
 * Parameters for population generation.
 */
struct Model final
{
	// Read a population model from an ptree.
	// Throws `boost::property_tree::ptree_error` on invalid inputs.
	// See data/population_model_default.xml for the structure `pt` should have.
	static ModelRef Parse(const boost::property_tree::ptree& pt);

	Model(
	    int school_size, int school_cluster_size, int college_size, int college_cluster_size, int workplace_size,
	    int community_size, double search_radius, int population_size, double city_ratio,
	    std::map<util::InclusiveRange<int>, double> town_distribution, util::InclusiveRange<int> school_age,
	    util::InclusiveRange<int> college_age, double college_ratio, double college_commute_ratio,
	    util::InclusiveRange<int> employable_age, double employed_ratio, double work_commute_ratio)
	    : school_size(school_size), school_cluster_size(school_cluster_size), college_size(college_size),
	      college_cluster_size(college_cluster_size), workplace_size(workplace_size),
	      community_size(community_size), search_radius(search_radius), population_size(population_size),
	      city_ratio(city_ratio), town_distribution(town_distribution), school_age(school_age),
	      college_age(college_age), college_ratio(college_ratio), college_commute_ratio(college_commute_ratio),
	      employable_age(employable_age), employed_ratio(employed_ratio), work_commute_ratio(work_commute_ratio)
	{
	}

	// Is the given age in the mandatory education age range?
	bool IsSchoolAge(int age) const { return school_age.Includes(age); }

	// Is the given age in the college age range?
	bool IsCollegeAge(int age) const { return college_age.Includes(age); }

	// Is the given age in the employable age range?
	bool IsEmployableAge(int age) const { return employable_age.Includes(age); }

	// The size of a (non-college) school.
	int school_size;

	// The size of a (non-college) school cluster.
	int school_cluster_size;

	// The size of a college ("hogeschool").
	int college_size;

	// The size of a college cluster.
	int college_cluster_size;

	// The size of a workplace. Workplaces are their own clusters, so there's
	// no separate cluster size field.
	int workplace_size;

	// The size of a primary or secondary community.
	int community_size;

	// The radius to look for geopositions in when allocating a Person to one.
	// (When no geopositions are found in this radius, it is doubled
	// repeatedly until one is found.)
	double search_radius;

	// The size of the entire population.
	int population_size;

	// The proportion of people who live in cities. (The remaining portion
	// lives in randomly-generated towns.)
	double city_ratio;

	// The distribution for town population ranges. Every entry ([a, b], p)
	// in this map represents a statement of the form: with relative
	// probability p, a generated town has between a and b inhabitants.
	std::map<util::InclusiveRange<int>, double> town_distribution;

	// The age range for compulsory lower education.
	util::InclusiveRange<int> school_age;

	// The age range for college education.
	util::InclusiveRange<int> college_age;

	// The proportion of students in the above age range that actually goes
	// to college, instead of getting a job.
	double college_ratio;

	// The proportion of actual college students that commutes to a distant
	// college campus.
	double college_commute_ratio;

	// The age range for employable citizens.
	util::InclusiveRange<int> employable_age;

	// The proportion of citizens in the above age range that is actually
	// employed.
	double employed_ratio;

	// The proportion of actual employees that commutes to a distant workplace.
	double work_commute_ratio;
};

} // namespace population
} // namespace stride

#endif
