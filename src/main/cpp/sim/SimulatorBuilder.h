#ifndef SIMULATOR_BUILDER_H_INCLUDED
#define SIMULATOR_BUILDER_H_INCLUDED
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
 *  Copyright 2017, Willem L, Kuylen E, Stijven S, Broeckhove J
 *  Aerts S, De Haes C, Van der Cruysse J & Van Hauwe L
 */

/**
 * @file
 * Header for the SimulatorBuilder class.
 */

#include "Simulator.h"
#include "sim/SimulationConfig.h"

#include <spdlog/spdlog.h>
#include <boost/property_tree/ptree.hpp>
#include <memory>
#include <string>

namespace stride {

class Population;
class Calendar;

/**
 * Main class that contains and direct the virtual world.
 */
class SimulatorBuilder
{
public:
	/// Build simulator.
	static std::shared_ptr<Simulator> Build(
	        const std::string& config_file_name,
                const std::shared_ptr<spdlog::logger>& log,
	        unsigned int num_threads =1U,
	        bool track_index_case =false);

        /// Build simulator.
        static std::shared_ptr<Simulator> Build(
                const boost::property_tree::ptree& pt_config,
                const std::shared_ptr<spdlog::logger>& log,
                unsigned int num_threads =1U,
                bool track_index_case =false);

        /// Build simulator.
        static std::shared_ptr<Simulator> Build(
                const SingleSimulationConfig& config,
                const std::shared_ptr<spdlog::logger>& log,
                unsigned int num_threads =1U);

        /// Build simulator.
        static std::shared_ptr<Simulator> Build(
                const boost::property_tree::ptree& pt_config,
                const boost::property_tree::ptree& pt_disease,
                const boost::property_tree::ptree& pt_contact,
                const std::shared_ptr<spdlog::logger>& log,
                unsigned int number_of_threads = 1U,
                bool track_index_case =false);

        /// Build simulator.
        static std::shared_ptr<Simulator> Build(
                const SingleSimulationConfig& config,
                const boost::property_tree::ptree& pt_disease,
                const boost::property_tree::ptree& pt_contact,
                const std::shared_ptr<spdlog::logger>& log,
                unsigned int number_of_threads = 1U);

private:
        /// Initialize the clusters.
        static void InitializeClusters(std::shared_ptr<Simulator> sim);
};

} // end_of_namespace

#endif // end-of-include-guard
