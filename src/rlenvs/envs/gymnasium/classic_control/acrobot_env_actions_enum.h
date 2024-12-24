#ifndef ACROBOT_ENV_ACTIONS_ENUM_H
#define ACROBOT_ENV_ACTIONS_ENUM_H

#include "rlenvs/extern/enum.h"
#include "rlenvs/rlenvs_types_v2.h"

#include <vector>
namespace rlenvscpp{
namespace envs{
namespace gymnasium{
	
	BETTER_ENUM(AcrobotActionsEnum, int, ZERO=0, ONE=1, TWO=2, INVALID_ACTION=3);
	
	
	/**
     * @brief Convert the action index to a valid AcrobotActionsEnum
     *
     * */
	struct AcrobotActionsEnumUtils{
		static AcrobotActionsEnum action_from_int(uint_t aidx);
		static std::vector<AcrobotActionsEnum> actions_from_ints(const std::vector<uint_t> actions);
		static std::vector<int> actions_to_ints(const std::vector<AcrobotActionsEnum>& actions);
		static int action_to_int(const AcrobotActionsEnum action);
	};
    
}
	
}
	
}

#endif