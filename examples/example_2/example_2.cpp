#include "gymfcpp/py_interpreter.h"
#include <iostream>
#include <string>
#include <any>

int main(){

    try
    {
        std::cout<<"Starting the interpreter..."<<std::endl;
        pfcpp::PyInterpreter interpreter(true);

        //auto main_module = interpreter.import("__main__");

        std::cout<<"Importing gym module..."<<std::endl;
        auto gym_module = interpreter.import("gym");

        std::cout<<"Getting the namespace..."<<std::endl;
        auto gym_namespace = gym_module.get_raw().attr("__dict__");

        std::cout<<"Module name "<<boost::python::extract<const char*>(gym_namespace["__name__"])<<std::endl;

        //auto gym_world = gym_module.get_raw()("make('FrozenLake-v0', slippery=True)";

        std::cout<<"Creating the environment..."<<std::endl;

        // create an environment
        auto ignored = interpreter.exec("import gym \n"
                                        "world = gym.make('FrozenLake-v0', is_slippery=True) \n"
                                        "world = world.unwrapped", gym_namespace);

        auto world =  boost::python::extract<boost::python::api::object>(gym_namespace["world"]);

        //world.observation_space;
        auto world_dict = boost::python::extract<boost::python::dict>(world().attr("__dict__"));
        auto keys = boost::python::list(world_dict().keys());

        for(auto i=0; i<boost::python::len(keys); ++i){

            std::cout<<boost::python::extract<std::string>(boost::python::object(keys[i]))()<<std::endl;;
        }

        auto observation_space = boost::python::extract<boost::python::api::object>(world_dict()["observation_space"]);
        std::cout<<"Number of states "<<boost::python::extract<int>(observation_space().attr("__dict__")["n"])<<std::endl;

        auto action_space = boost::python::extract<boost::python::api::object>(world_dict()["action_space"]);
        std::cout<<"Number of actions "<<boost::python::extract<int>(action_space().attr("__dict__")["n"])<<std::endl;

    }
    catch(boost::python::error_already_set const &)
    {
        PyErr_Print();
    }

    std::cout<<"Finilize..."<<std::endl;
    return 0;
}
