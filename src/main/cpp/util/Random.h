#ifndef RANDOM_H_INCLUDED
#define RANDOM_H_INCLUDED
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
 * Header for the Random Number Generator class.
 */

#include <trng/mrg2.hpp>
#include <trng/uniform01_dist.hpp>
#include <trng/uniform_dist.hpp>
#include <trng/uniform_int_dist.hpp>
#include "Errors.h"
#include "InclusiveRange.h"

namespace stride {
namespace util {

/**
 * The random number generator.
 */
class Random
{
public:
	/// Constructor: initialize the random number engine and distribution.
	Random(const unsigned long seed)
	{
		m_engine.seed(seed);
		m_uniform_dist = trng::uniform01_dist<double>();
	}

	/// Get random double.
	double NextDouble() { return m_uniform_dist(m_engine); }

	/// Return true with chance p, and false with chance 1-p.
	double Chance(double p) { return NextDouble() < p; }

	/// Get random unsigned int from [0, max[.
	unsigned int operator()(unsigned int max)
	{
		trng::uniform_int_dist dis(0, max);
		return dis(m_engine);
	}

	/// Get random int from [a, b[ (excludes the endpoint).
	int operator()(int a, int b) { return trng::uniform_int_dist(a, b)(m_engine); }

	/// Get random double from [a, b].
	double operator()(double a, double b) { return trng::uniform_dist<double>(a, b)(m_engine); }

	/// Get random int from an integer InclusiveRange.
	int operator()(InclusiveRange<int> r) { return (*this)(r.minimum, r.maximum + 1); }

	/// Get random double from [a, b].
	double operator()(InclusiveRange<double> r) { return (*this)(r.minimum, r.maximum); }

	/// Sample a random element from a vector with a uniform distribution.
	template <typename T>
	const T& Sample(const std::vector<T>& vec)
	{
		if (vec.empty())
			FATAL_ERROR("Random::Sample called on empty vector.");
		return vec[(*this)(vec.size())];
	}

	/**
	 * Split random engines
	 * E. g. stream 0 1 2 3 4 5...
	 * => stream A: 0 2 4...
	 * => stream B: 1 3 5...
	 */
	void Split(unsigned int total, unsigned int id) { m_engine.split(total, id); }

private:
	trng::mrg2 m_engine;			     ///< The random number engine.
	trng::uniform01_dist<double> m_uniform_dist; ///< The random distribution.
};

} // end namespace
} // end namespace

#endif // include guard
