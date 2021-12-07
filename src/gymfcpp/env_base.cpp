#include "gymfcpp/env_base.h"
#include "gymfcpp/gymfcpp_config.h"

namespace  gymfcpp{

EnvBase::EnvBase(std::string name, std::string version, std::string py_env_name, obj_t gym_namespace)
    :
      name_(name),
      version_(version),
      py_env_name_(py_env_name),
      gym_namespace_(gym_namespace),
      is_created_(false)
{}

EnvBase::~EnvBase(){
    close();
}

void
EnvBase::render(std::string mode){

#ifdef GYMFCPP_DEBUG
    assert(data_.is_created && "Environment has not been created");
#endif

    auto str = "screen = " + py_env_name_ + ".render(mode=" + mode + ")\n";
    boost::python::exec(str.c_str(), gym_namespace_);
}

void
EnvBase::close(){

    if(!is_created_){
        return;
    }

    auto str = py_env_name_ + ".close()\n";
    boost::python::exec(str.c_str(), gym_namespace_);
    is_created_ = false;

}

}
