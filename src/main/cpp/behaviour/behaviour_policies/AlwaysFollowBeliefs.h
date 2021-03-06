#ifndef SRC_MAIN_CPP_BEHAVIOUR_BEHAVIOUR_POLICIES_ALWAYSFOLLOWBELIEFS_H_
#define SRC_MAIN_CPP_BEHAVIOUR_BEHAVIOUR_POLICIES_ALWAYSFOLLOWBELIEFS_H_

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
 */

namespace stride {

class AlwaysFollowBeliefs {
	bool PracticesBehavior(bool beliefs_behavior) { return beliefs_behavior; }
};

} /* namespace stride */


#endif /* SRC_MAIN_CPP_BEHAVIOUR_BEHAVIOUR_POLICIES_ALWAYSFOLLOWBELIEFS_H_ */
