#include "rlenvs/envs/gymnasium/vector/vector_cart_pole.h"
#include <string>


namespace rlenvs_cpp{
namespace envs{
namespace gymnasium{
namespace vector{


/*
VCartPole::VCartPole(const std::string& version, obj_t main_namespace,
                    uint_t n_copies,
                    bool do_create)
:
    version_(version),
    is_created(false),
    gym_namespace(main_namespace),
    n_copies_(n_copies)
{

    if(do_create){
        make();
    }
}

void
VCartPole::make(){

    if(is_created){
        return;
    }

    static const std::string name = "CartPole";
    static const std::string py_env_name("v_cart_pole");
    std::string cpp_str = "import gymnasium as gym \n";
    cpp_str += py_env_name + " = gym.vector.make('" + name +"-" + version_ + "', num_envs="+ std::to_string(n_copies_) + ").unwrapped\n";

    // create an environment
    auto ignored = boost::python::exec(cpp_str.c_str(), gym_namespace);
    env = boost::python::extract<boost::python::api::object>(gym_namespace[py_env_name]);
    is_created = true;


}
*/

}
}
}
}
