/**
 * @file
 * Implementation of the VisualizerData class.
 */

#include "VisualizerData.h"

namespace stride {

using namespace std;
using boost::property_tree::ptree;

void VisualizerData::AddDay(const std::shared_ptr<const Population>& pop)
{
	days.push_back({});
	for (const auto& p : *pop) {
		if (p.GetHealth().IsInfected()) {
			days.back()[pop->GetHometown(p).id]++;
		}
	}
}

const vector<map<size_t, int>>& VisualizerData::GetDays() const { return days; }

shared_ptr<ptree> VisualizerData::ToPtree() const
{
	auto daysTree = make_shared<ptree>();

	// Create a list: [{townId: deltaInfected}]
	for (int i = 0; i < days.size(); i++) {
		ptree data;
		const auto& currentDay = days[i];

		if (i == 0) {
			// First day: Just put the number of infected.
			for (const auto& p : currentDay)
				data.put(to_string(p.first), p.second);
		} else {
			// Subsequent days: Put the difference in infected from the previous day
			const auto& prevDay = days[i - 1];
			for (const auto& p : currentDay) {
				int diff = p.second;

				// If the previous day has no record it means there were 0 infected that day.
				const auto& prev = prevDay.find(p.first);
				if (prev != prevDay.end())
					diff -= prev->second;

				// If the difference between days is zero, don't actually put it in the file.
				if (diff != 0)
					data.put(to_string(p.first), diff);
			}
		}

		daysTree->push_back(make_pair("", data));
	}

	return daysTree;
}

} // end of namespace