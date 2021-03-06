#ifndef DISEASE_PROFILE_H_INCLUDED
#define DISEASE_PROFILE_H_INCLUDED
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
 *  Copyright 2015, Willem L, Kuylen E, Stijven S & Broeckhove J
 */

/**
 * @file
 * Disease profile.
 */

#include "sim/SimulationConfig.h"
#include <boost/property_tree/ptree.hpp>

namespace stride {

class DiseaseProfile
{
public:
        /// Initialize.
        DiseaseProfile() : m_transmission_rate(0.0) {}

        /// Return transmission rate.
        double GetTransmissionRate() { return m_transmission_rate;}

        /// Initialize.
        void Initialize(const SingleSimulationConfig& config, const boost::property_tree::ptree& pt_disease);

private:
        double m_transmission_rate;
};

} // namespace

#endif // end-of-include-guard
