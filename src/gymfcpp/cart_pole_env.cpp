#include "gymfcpp/cart_pole_env.h"
#include "gymfcpp/gymfcpp_types.h"
#include "gymfcpp/time_step.h"
#include "gymfcpp/names_generator.h"

#include "gymfcpp/gymfcpp_config.h"

#ifdef USE_PYTOCH
#include <torch/torch.h>
#endif

#include <iostream>


namespace rlenvs_cpp{
namespace gymfcpp{

// static data
std::string CartPoleData::name = "CartPole";


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

CartPoleData::state_type
CartPoleData::extract_state_from_reset(obj_t gym_namespace, std::string py_state_name, std::string result_name){

     std:: string s = py_state_name +   " = " +  result_name + ".tolist()\n";

     boost::python::exec(s.c_str(), gym_namespace);
     auto obs =  boost::python::extract<boost::python::list>(gym_namespace[py_state_name]);

     CartPoleData::state_type state;

     state.reserve(boost::python::len(obs));

     for(auto i=0; i<boost::python::len(obs); ++i){
         state.push_back(boost::python::extract<real_t>(obs()[i]));
     }

     return state;
}

CartPoleData::state_type
CartPoleData::extract_state_from_step(obj_t gym_namespace, std::string py_state_name, std::string result_name){

     std:: string s = py_state_name +   " = " +  result_name + "[0].tolist()\n";

     boost::python::exec(s.c_str(), gym_namespace);
     auto obs_python =  boost::python::extract<boost::python::list>(gym_namespace[py_state_name]);

     /*std::vector<real_t> obs;
     obs.reserve(boost::python::len(obs_python));

     for(auto i=0; i<boost::python::len(obs_python); ++i){
         obs.push_back(boost::python::extract<real_t>(obs_python()[i]));
     }*/

     //return obs;

     return extract_obs(obs_python);
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

#ifdef USE_PYTORCH
torch_tensor_t 
CartPole::Screen::get_as_torch_tensor()const{


	auto options = torch::TensorOptions().dtype(at::kDouble);
    auto tensor = torch::zeros({static_cast<long int>(screen_vec_.size()),
                                static_cast<long int>(screen_vec_[0].size()),
                                static_cast<long int>(screen_vec_[0][0].size())}, options);
  	
  	for (uint_t i = 0; i < screen_vec_.size(); ++i){
  	 for(uint_t j=0; j<screen_vec_[0].size(); ++j)
            tensor.slice(0, i, i+1).slice(0, j, j+1) = torch::from_blob(screen_vec_[i][j].data(),
                                                                        {static_cast<long int>(screen_vec_[i][j].size())}, options);
    	}
  	
	return tensor;

}
#endif

CartPole::CartPole(const std::string& version, obj_t main_namespace, bool do_create)
    :
     EnvMixin<CartPoleData>(version, main_namespace)
{
    this->py_env_name = get_py_env_name(CartPoleData::name);
    this->py_reset_result_name = get_py_reset_rslt_name(CartPoleData::name);
    this->py_step_result_name = get_py_step_rslt_name(CartPoleData::name);
    this->py_state_name = get_py_state_name(CartPoleData::name);

    if(do_create){
        make();
    }
}

CartPole::~CartPole(){
    close();
}

void
CartPole::make(){

    if(is_created){
        return;
    }

    std::string cpp_str = "import gym \n";
    cpp_str += "import numpy as np \n";
    cpp_str += py_env_name + " = gym.make('" + CartPoleData::name +"-" + version + "').unwrapped\n";

    // create an environment
    auto ignored = boost::python::exec(cpp_str.c_str(), gym_namespace);
    env = boost::python::extract<boost::python::api::object>(gym_namespace[CartPole::py_env_name]);
    is_created = true;
}


CartPole::time_step_type
CartPole::step(const action_type action){

#ifdef GYMFCPP_DEBUG
    assert(is_created && "Environment has not been created");
#endif

    if(current_state.last()){
        return reset();
    }

    std::string s = py_step_result_name + " = " + py_env_name +".step("+std::to_string(action)+")\n";
    s += CartPole::py_step_result_name + " = (" + py_step_result_name + "[0].tolist(),";
    s += "float(" + py_step_result_name + "[1]),";
    s += py_step_result_name + "[2])\n";

    boost::python::exec(s.c_str(), gym_namespace);

    // the observation
    auto result =  boost::python::extract<boost::python::tuple>(gym_namespace[py_step_result_name]);

    auto observation = boost::python::extract<boost::python::list>(result()[0]);
    auto obs = extract_obs(observation);
    auto reward = boost::python::extract<real_t>(result()[1]);
    auto done = boost::python::extract<bool>(result()[2]);
    std::unordered_map<std::string, std::any> extra;

    current_state = time_step_type(done ? TimeStepTp::LAST : TimeStepTp::MID, reward(), obs, std::move(extra));
    return current_state;
}

CartPole::Screen
CartPole::get_screen()const{

#ifdef GYMFCPP_DEBUG
    assert(is_created && "Environment has not been created");
#endif

    auto str = "screen = " + py_env_name + ".render(mode='rgb_array').transpose((2, 0, 1))\n";
    str += "_, screen_height, screen_width = screen.shape\n";
    str += "screen_height_scale_1 = int(screen_height * 0.4)\n";
    str += "screen_height_scale_2 = int(screen_height * 0.8)\n";
    str += "screen = screen[:, screen_height_scale_1 : screen_height_scale_2]\n";

    str += "view_width = int(screen_width * 0.6)\n";

    // calculate the cart location
    str += "world_width = " + py_env_name + ".x_threshold * 2\n";
    str += "scale = screen_width / world_width \n";
    str += "cart_location = int(" + py_env_name + ".state[0] * scale + screen_width / 2.0)\n";

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


    // execute the script
    boost::python::exec(str.c_str(), gym_namespace);

    auto shape = boost::python::extract<boost::python::tuple>(gym_namespace["cart_pole_screen_shape"]);
    return Screen(gym_namespace["cart_pole_screen_list"],
    {boost::python::extract<uint_t>(shape()[0]), boost::python::extract<uint_t>(shape()[1]), boost::python::extract<uint_t>(shape()[2])});

}


}
}
