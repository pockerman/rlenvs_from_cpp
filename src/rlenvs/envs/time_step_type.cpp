#include "rlenvs/envs/time_step_type.h"

#include <algorithm>
#include <iterator>

namespace rlenvscpp
{

/*TimeStepTp time_step_type_from_int(int type){
   return TimeStepEnumUtils::time_step_type_from_int(type);
}*/



TimeStepTp 
TimeStepEnumUtils::time_step_type_from_int(uint_t type){
	
	if(type == 0){

        return TimeStepTp::FIRST;
    }
    else if(type == 1){
        return TimeStepTp::MID;
    }
    else if(type == 2){

        return TimeStepTp::LAST;
    }

    return TimeStepTp::INVALID_TYPE;
}

TimeStepTp 
TimeStepEnumUtils::time_step_type_from_int(int type){
	
	return TimeStepEnumUtils::time_step_type_from_int(static_cast<uint_t>(type));
}

std::string 
TimeStepEnumUtils::to_string(TimeStepTp type){
	
	if(type == TimeStepTp::FIRST){

        return "FIRST";
    }
    else if(type == TimeStepTp::MID){
        return "MID";
    }
    else if(type == TimeStepTp::LAST){

        return "LAST";
    }

    return "INVALID_TYPE";
	
}

std::vector<TimeStepTp> 
TimeStepEnumUtils::time_step_type_from_int(const std::vector<uint_t>& types){
	
	
	std::vector<TimeStepTp> result_;
	result_.reserve(types.size());
	
	std::transform(types.begin(), types.end(),
	               std::back_inserter(result_),
				   [](auto& val){
					   return TimeStepEnumUtils::time_step_type_from_int(val); 
				   });
	return result_;
	
}


}
