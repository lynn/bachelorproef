/*
 * ThresholdData.h
 *
 *  Created on: May 7, 2017
 *      Author: elise
 */

#ifndef SRC_MAIN_CPP_BEHAVIOUR_BELIEF_DATA_THRESHOLDDATA_H_
#define SRC_MAIN_CPP_BEHAVIOUR_BELIEF_DATA_THRESHOLDDATA_H_

#include "behaviour/behaviour_policies/AlwaysFollowBeliefs.h"
#include <memory>

/*
 * Possible variants:
 * 		+ fraction adopted over entire simulation
 * 		+ with awareness
 * 		+ with history
 * 		+ and off course all combinations
 */
namespace stride {

template <typename BehaviourPolicy, typename BeliefPolicy>
class GenericPersonData;

template <bool threshold_infected, bool threshold_adopted>
class Threshold;

class ThresholdData {
public:
	/// Default constructor
	ThresholdData():
		m_num_contacts(0U), m_num_contacts_infected(0U), m_num_contacts_adopted(0U),
		m_threshold_infected(1), m_threshold_adopted(1) {}

	void SetThresholdInfected(double threshold) {
		m_threshold_infected = threshold;
	}

	double GetThresholdInfected() const {
		return m_threshold_infected;
	}

	void SetThresholdAdopted(double threshold) {
		m_threshold_adopted = threshold;
	}

	double GetThresholdAdopted() const {
		return m_threshold_adopted;
	}

	double GetFractionInfected() const {
		if (m_num_contacts == 0) {
			return 0;
		}
		return double(m_num_contacts_infected) / m_num_contacts;
	}

	double GetFractionAdopted() const {
		if (m_num_contacts == 0) {
			return 0;
		}
		return double(m_num_contacts_adopted) / m_num_contacts;
	}

	template <typename BehaviourPolicy, typename BeliefPolicy>
	void Contact(std::shared_ptr<const GenericPersonData<BehaviourPolicy, BeliefPolicy>> p);

private:
	unsigned int		m_num_contacts;				///<
	unsigned int		m_num_contacts_infected;	///<
	unsigned int		m_num_contacts_adopted; 	///<

	double 				m_threshold_infected;		///< Fraction of contacts that needs to be infected before person adopts belief.
	double				m_threshold_adopted;		///< Fraction of contacts that needs to have adopted the belief for person to also adopt.

};

extern template void ThresholdData::Contact<AlwaysFollowBeliefs, Threshold<true, false> >(std::shared_ptr<const GenericPersonData<AlwaysFollowBeliefs, Threshold<true, false> >> p);
extern template void ThresholdData::Contact<AlwaysFollowBeliefs, Threshold<false, true> >(std::shared_ptr<const GenericPersonData<AlwaysFollowBeliefs, Threshold<false, true> >> p);
extern template void ThresholdData::Contact<AlwaysFollowBeliefs, Threshold<true, true> >(std::shared_ptr<const GenericPersonData<AlwaysFollowBeliefs, Threshold<true, true> >> p);


//extern template return-type name < argument-list > ( parameter-list ) ;
//extern template ReallyBigFunction<int>();
//extern template void ThresholdData::Contact<AlwaysFollowBeliefs, Threshold<true, false> >(const GenericPersonData<AlwaysFollowBeliefs, Threshold<true, false> >);
//extern template<AlwaysFollowBeliefs, Threshold<true, false> >
//void ThresholdData::Contact(const GenericPersonData<AlwaysFollowBeliefs, Threshold<true, false> >);

} /* namespace stride */


#endif /* SRC_MAIN_CPP_BEHAVIOUR_BELIEF_DATA_THRESHOLDDATA_H_ */
