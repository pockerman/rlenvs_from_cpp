#include "gymfcpp/cart_pole.h"
#include "gymfcpp/gymfcpp_types.h"
#include "gymfcpp/time_step.h"
#include "gymfcpp/config.h"

#include <iostream>

namespace gymfcpp{

// static data
std::string CartPole::name = "CartPole";
std::string CartPole::py_env_name = "py_cart_pole";
std::string CartPole::py_step_result_name = "py_cart_pole_step_rslt";
std::string CartPole::py_reset_result_name = "py_cart_pole_reset_rslt";

namespace  {

template<typename ObsTp>
std::vector<real_t> extract_obs(const ObsTp& observation){

    std::vector<real_t> obs;
    obs.reserve(boost::python::len(observation));

    for(auto i=0; i<boost::python::len(observation); ++i){
        obs.push_back(boost::python::extract<real_t>(observation()[i]));
    }

    return obs;

}
}


CartPole::Screen::Screen(obj_t screen, std::array<uint_t, 3>&& shp)
    :
      screen_(screen),
      shape_(shp),
      is_valid_screen_(true),
      screen_vec_()
{}

void 
CartPole::Screen::invalidate() noexcept{

	screen_ = obj_t();
	
	std::array<uint_t, 3> empty;
	std::swap(shape_, empty);
	
	screen_vector_t empty_vec;
	std::swap(screen_vec_, empty_vec);
	is_valid_screen_ = false;

}


const std::vector<std::vector<std::vector<real_t>>>&
CartPole::Screen::get_as_vector()const{

    if(!screen_vec_.empty()){
        return screen_vec_;
    }

    screen_vec_.resize(shape()[0]);

    for(uint_t i=0; i<shape()[0]; ++i){
        screen_vec_[i].resize(shape()[1]);

        for(uint_t j=0; j<shape()[1]; ++j){
            screen_vec_[i][j].reserve(shape()[2]);
        }
    }

    for(uint_t i=0; i<shape()[0]; ++i){
        for(uint_t j=0; j<shape()[1]; ++j){

            auto screen_data = boost::python::extract<boost::python::list>(screen_[i][j]);
            screen_vec_[i][j] = extract_obs(screen_data);
        }
    }

    return screen_vec_;


}

CartPole::CartPole(const std::string& version, obj_t gym_namespace, bool do_create)
    :
      data_(),
      n_actions_(INVALID_UINT)
{
    data_.v = version;
    data_.gym_namespace = gym_namespace;
    data_.is_created = false;

    if(do_create){
        make();
    }

}


void
CartPole::make(){

    if(data_.is_created){
        return;
    }

    std::string cpp_str = "import gym \n";
    cpp_str += "import numpy as np \n";
    cpp_str += "import torch \n";
    cpp_str += CartPole::py_env_name + " = gym.make('" + CartPole::name +"-" + data_.v + "').unwrapped\n";

    // create an environment
    auto ignored = boost::python::exec(cpp_str.c_str(), data_.gym_namespace);
    data_.world = boost::python::extract<boost::python::api::object>(data_.gym_namespace[CartPole::py_env_name]);
    data_.is_created = true;

}


uint_t
CartPole::n_actions()const{

#ifdef GYMFCPP_DEBUG
    assert(data_.is_created && "Environment has not been created");
#endif

    if(n_actions_ == INVALID_UINT){

        auto world_dict = boost::python::extract<boost::python::dict>(data_.world.attr("__dict__"));
        auto action_space = boost::python::extract<boost::python::api::object>(world_dict()["action_space"]);
        n_actions_ = boost::python::extract<uint64_t>(action_space().attr("__dict__")["n"]);

    }

    return n_actions_;
}

CartPole::time_step_t
CartPole::reset(){

#ifdef GYMFCPP_DEBUG
    assert(data_.is_created && "Environment has not been created");
#endif

    auto cpp_str = std::string(CartPole::py_reset_result_name + " = ");
    cpp_str += CartPole::py_env_name + ".reset().tolist()";

    // reset the python environment
    boost::python::exec(cpp_str.c_str(), data_.gym_namespace);

    // the observation
    auto observation =  boost::python::extract<boost::python::list>(data_.gym_namespace[CartPole::py_reset_result_name]);
    auto obs = extract_obs(observation);

    data_.current_state = time_step_t(TimeStepTp::FIRST, 0.0, obs);
    return data_.current_state;

}


CartPole::time_step_t
CartPole::step(action_t action){

#ifdef GYMFCPP_DEBUG
    assert(data_.is_created && "Environment has not been created");
#endif

    if(data_.current_state.last()){
        return reset();
    }

    std::string s = CartPole::py_step_result_name + " = " + CartPole::py_env_name +".step("+std::to_string(action)+")\n";
    s += CartPole::py_step_result_name + " = (" + CartPole::py_step_result_name + "[0].tolist(),";
    s += "float(" + CartPole::py_step_result_name + "[1]),";
    s += CartPole::py_step_result_name + "[2])\n";


    // create an environment
    boost::python::exec(s.c_str(), data_.gym_namespace);

    // the observation
    auto result =  boost::python::extract<boost::python::tuple>(data_.gym_namespace[CartPole::py_step_result_name]);

    auto observation = boost::python::extract<boost::python::list>(result()[0]);
    auto obs = extract_obs(observation);
    auto reward = boost::python::extract<real_t>(result()[1]);
    auto done = boost::python::extract<bool>(result()[2]);
    std::unordered_map<std::string, std::any> extra;

    data_.current_state = time_step_t(done() ? TimeStepTp::LAST : TimeStepTp::MID, reward(), obs, std::move(extra));
    return data_.current_state;

}

void
CartPole::render(){

#ifdef GYMFCPP_DEBUG
    assert(data_.is_created && "Environment has not been created");
#endif

    auto str = "screen = " + CartPole::py_env_name + ".render(mode='rgb_array')\n";
    boost::python::exec(str.c_str(), data_.gym_namespace);
}

CartPole::Screen
CartPole::get_screen()const{

#ifdef GYMFCPP_DEBUG
    assert(data_.is_created && "Environment has not been created");
#endif

    auto str = "screen = " + CartPole::py_env_name + ".render(mode='rgb_array').transpose((2, 0, 1))\n";
    str += "_, screen_height, screen_width = screen.shape\n";
    str += "screen_height_scale_1 = int(screen_height * 0.4)\n";
    str += "screen_height_scale_2 = int(screen_height * 0.8)\n";
    str += "screen = screen[:, screen_height_scale_1 : screen_height_scale_2]\n";

    str += "view_width = int(screen_width * 0.6)\n";

    // calculate the cart location
    str += "world_width = " + CartPole::py_env_name + ".x_threshold * 2\n";
    str += "scale = screen_width / world_width \n";
    str += "cart_location = int(" + CartPole::py_env_name + ".state[0] * scale + screen_width / 2.0)\n";

    str += "if cart_location < view_width // 2:\n";
    str += "\tslice_range = slice(view_width)\n";
    str += "elif cart_location > (screen_width - view_width // 2):\n";
    str += "\tslice_range = slice(-view_width, None)\n";
    str += "else:\n";
    str += "\tslice_range = slice(cart_location - view_width // 2, cart_location + view_width // 2)\n";

    // # Strip off the edges, so that we have a square image centered on a cart
    str += "screen = screen[:, :, slice_range]\n";

    // Convert to float, rescale, convert to torch tensor
    // (this doesn't require a copy)
    str += "cart_pole_screen = np.ascontiguousarray(screen, dtype=np.float32) / 255\n";
    str += "cart_pole_screen_shape = cart_pole_screen.shape\n";
    str += "cart_pole_screen_list = cart_pole_screen.tolist()\n";
    //str += "cart_pole_screen = torch.from_numpy(screen)\n";

    //std::cout<<str<<std::endl;

    // execute the script
    boost::python::exec(str.c_str(), data_.gym_namespace);

    auto shape = boost::python::extract<boost::python::tuple>(data_.gym_namespace["cart_pole_screen_shape"]);
    return Screen(data_.gym_namespace["cart_pole_screen_list"],
    {boost::python::extract<uint_t>(shape()[0]), boost::python::extract<uint_t>(shape()[1]), boost::python::extract<uint_t>(shape()[2])});

}

}
