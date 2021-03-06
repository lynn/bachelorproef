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
 * Contact profile.
 */

#include "ContactProfile.h"

namespace stride {

using namespace std;
using namespace boost::property_tree;

ContactProfile::ContactProfile(ClusterType cluster_type,  const ptree& pt_contacts)
{
        const string key = "matrices." + ToString(cluster_type);
        ContactProfile mean_nums;
        unsigned int i = 0U;
        for(const auto& participant: pt_contacts.get_child(key)) {
                double total_contacts = 0;
                for (const auto& contact: participant.second.get_child("contacts")) {
                        total_contacts += contact.second.get<double>("rate");
                }
                (*this)[i++] = total_contacts;
        }
}

} // namespace

