#include "gymfcpp/time_step_type.h"

namespace gymfcpp {

std::string to_string(TimeStepTp type){

    switch(type){
    case TimeStepTp::FIRST:
        return "FIRST";
    case TimeStepTp::MID:
        return "MID";
    case TimeStepTp::LAST:
        return "LAST";
    }

    return "INVALID_TYPE";
}

}
