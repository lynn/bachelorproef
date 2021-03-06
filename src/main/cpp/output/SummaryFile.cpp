/*
 *  This is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 *  The software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with the software. If not, see <http://www.gnu.org/licenses/>.
 *
 *  Copyright 2017, Willem L, Kuylen E, Stijven S & Broeckhove J
 */

/**
 * @file
 * Implementation of the SummaryFile class.
 */

#include "SummaryFile.h"

#include <fstream>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include "util/Parallel.h"

namespace stride {
namespace output {

using namespace std;

SummaryFile::SummaryFile(const std::string& file)
{
	m_fstream.open((file + "_summary.csv").c_str());

	// add header
	m_fstream << "pop_file,num_days,pop_size,seeding_rate,"
		  << "R0,transm_rate,immunity_rate,num_threads,rng_seed,run_time,"
		  << "total_time,num_cases,AR" << endl;
}

SummaryFile::~SummaryFile() { m_fstream.close(); }

void SummaryFile::Print(
    const SingleSimulationConfig& config, unsigned int population_size, unsigned int num_cases, unsigned int run_time,
    unsigned int total_time)
{
	unsigned int num_threads = util::parallel::get_number_of_threads();

	m_fstream << config.GetPopulationPath() << "," << config.common_config->number_of_days << "," << population_size
		  << "," << config.common_config->seeding_rate << "," << config.common_config->r0 << ","
		  << "NA"
		  << "," // << pt_config.get<double>("run.transmission_rate") << ";"
		  << config.common_config->immunity_rate << "," << num_threads << "," << config.common_config->rng_seed
		  << "," << run_time << "," << total_time << "," << num_cases << ","
		  << static_cast<double>(num_cases) / population_size << endl;
}

} // end namespace
} // end namespace
