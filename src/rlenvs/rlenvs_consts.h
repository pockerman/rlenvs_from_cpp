#ifndef GYMFCPP_CONSTS_H
#define GYMFCPP_CONSTS_H

#include "rlenvs/rlenvs_types_v2.h"
#include <string>

namespace rlenvscpp {
	namespace consts{

		///
		/// \brief INVALID_ID
		///
		inline const uint_t INVALID_ID = static_cast<uint_t>(-1);

		///
		/// \brief Invalid string
		///
		inline const std::string INVALID_STR = std::string("INVALID");

		///
		/// \brief Tolerance to use around the library
		///
		inline const real_t TOLERANCE = 1.0e-8;
		
		namespace maths{
			
			///
			/// \brief The Pi constant
			///
			inline const real_t PI = 3.14159265359;
			
			///
			/// \brief Acceleration due to gravity m/secs
			///
			inline const real_t G = 9.82;
		}
		
	}

}

#endif // GYMFCPP_CONSTS_H
