#include <boost/python.hpp>
#include <iostream>


int main(){

    try
    {
        std::cout<<"Starting the interpreter..."<<std::endl;

        Py_Initialize();

        std::cout<<"Importing module..."<<std::endl;
        auto gym_module = boost::python::import("gym");
        auto gym_namespace = gym_module.attr("__dict__");

        std::cout<<"Module name "<<boost::python::extract<const char*>(gym_namespace["__name__"])<<std::endl;

        std::cout<<"Creating the environment..."<<std::endl;

        // create an environment
        auto ignored = boost::python::exec("import gym \n"
                                           "world = gym.make('FrozenLake-v0', is_slippery=True) \n"
                                           "world = world.unwrapped", gym_namespace);

        // get the created world
        auto world =  boost::python::extract<boost::python::api::object>(gym_namespace["world"]);

        auto world_dict = boost::python::extract<boost::python::dict>(world().attr("__dict__"));

        // uncomment this to see the attributes
        /*auto keys = boost::python::list(world_dict().keys());

        for(auto i=0; i<boost::python::len(keys); ++i){

            std::cout<<boost::python::extract<std::string>(boost::python::object(keys[i]))()<<std::endl;;
        }*/

        auto observation_space = boost::python::extract<boost::python::api::object>(world_dict()["observation_space"]);
        std::cout<<"Number of states "<<boost::python::extract<int>(observation_space().attr("__dict__")["n"])<<std::endl;

        auto action_space = boost::python::extract<boost::python::api::object>(world_dict()["action_space"]);
        std::cout<<"Number of actions "<<boost::python::extract<int>(action_space().attr("__dict__")["n"])<<std::endl;

        // create an environment
        boost::python::exec("observation = world.reset()", gym_namespace);

        // the observation
        auto observation =  boost::python::extract<int>(gym_namespace["observation"]);
        std::cout<<"Observation after reset="<<observation<<std::endl;

    }
    catch(boost::python::error_already_set const &)
    {
        PyErr_Print();
    }

    std::cout<<"Finilize..."<<std::endl;
    return 0;
}
