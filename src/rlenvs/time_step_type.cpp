#include "rlenvs/time_step_type.h"

namespace rlenvs_cpp
{

TimeStepTp time_step_type_from_int(int type){

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

}
