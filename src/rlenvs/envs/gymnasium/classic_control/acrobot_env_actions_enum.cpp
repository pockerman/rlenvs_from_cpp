#include "rlenvs/envs/gymnasium/classic_control/acrobot_env_actions_enum.h"
#include <algorithm>
#include <iterator>

namespace rlenvscpp{
namespace envs{
namespace gymnasium{
	
	
AcrobotActionsEnum 
AcrobotActionsEnumUtils::action_from_int(uint_t aidx){
			
		if(aidx==0)
			return AcrobotActionsEnum::ZERO;

		if(aidx==1)
			return AcrobotActionsEnum::ONE;
			
		if(aidx == 2)
			return AcrobotActionsEnum::TWO;

		return AcrobotActionsEnum::INVALID_ACTION;

}

int 
AcrobotActionsEnumUtils::action_to_int(const AcrobotActionsEnum action){
	
	switch(action){
		case AcrobotActionsEnum::ZERO:
			return 0;
		case AcrobotActionsEnum::ONE:
			return 1;
		case AcrobotActionsEnum::TWO:
			return 2;
		default:
			return 3;
	}
}
	
std::vector<AcrobotActionsEnum> 
AcrobotActionsEnumUtils::actions_from_ints(const std::vector<uint_t> actions){
	
	std::vector<AcrobotActionsEnum> result_;
	result_.reserve(actions.size());
	
	std::transform(actions.begin(), actions.end(),
	               std::back_inserter(result_),
				   [](auto& val){
					   return AcrobotActionsEnumUtils::action_from_int(val); 
				   });
				   
	return result_;
}

std::vector<int> 
AcrobotActionsEnumUtils::actions_to_ints(const std::vector<AcrobotActionsEnum>& actions){
	
	std::vector<int> result_;
	result_.reserve(actions.size());
	
	std::transform(actions.begin(), actions.end(),
	               std::back_inserter(result_),
				   [](auto& val){
					   return AcrobotActionsEnumUtils::action_to_int(val); 
				   });
				   
	return result_;
}
	
    
}
	
}
	
}