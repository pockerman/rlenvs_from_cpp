#include "gymfcpp/tiled_cart_pole_env.h"
#include "gymfcpp/numpy_cpp_utils.h"
#include "gymfcpp/time_step_type.h"
#include "gymfcpp/time_step.h"

namespace gymfcpp
{

TiledCartPole::TiledCartPole(const std::string version, obj_t main_namespace, uint_t n_states,
                             uint_t state_idx, const TiledCartPoleBoundaries& boundaries)
    :
      TiledEnviromentBase<CartPole>(version, main_namespace, n_states),
      state_idx_(state_idx),
      boundaries_(boundaries),
      pole_theta_space_(),
      pole_theta_velocity_space_(),
      cart_pos_space_(),
      cart_vel_space_()
{}

TiledCartPole::time_step_type
TiledCartPole::reset(){

    auto step = env_.reset();

    auto discrete_step = get_state(step.observation());

    current_state_ = time_step_type(TimeStepTp::FIRST, step.reward(), discrete_step);
    return current_state_;
}

TiledCartPole::time_step_type
TiledCartPole::step(const action_type& action){

    auto step = env_.step(action);

    auto discrete_step = get_state(step.observation());

    current_state_ = time_step_type(step.type(), step.reward(), discrete_step);
    return current_state_;
}


TiledCartPole::state_type
TiledCartPole::get_state(const CartPole::state_type& obs)const{

    auto cart_x = obs[0];
    auto cart_x_dot = obs[1];
    auto cart_theta = obs[2];
    auto cart_theta_dot = obs[3];

    if(state_idx_ == 4){
        auto cart_x_int = digitize(cart_x, cart_pos_space_);
        auto cart_x_dot_int = digitize(cart_x_dot, cart_vel_space_);
        auto cart_theta_int = digitize(cart_theta, pole_theta_space_);
        auto cart_theta_dot_int = digitize(cart_theta_dot, pole_theta_velocity_space_);

        return {cart_x_int, cart_x_dot_int, cart_theta_int, cart_theta_dot_int};
    }
    else if (state_idx_ == 0 ){
        return {digitize(cart_x, cart_pos_space_)};
    }
    else if (state_idx_ == 1){
        return {digitize(cart_x_dot, cart_vel_space_)};
    }
    else if( state_idx_ == 2 ){
        return {digitize(cart_theta, pole_theta_space_)};
    }
    else if (state_idx_ == 3){
        return {digitize(cart_theta_dot, pole_theta_velocity_space_)};
    }


}

void
TiledCartPole::create_bins(){

    if(state_idx_ == 4){
         _build_pole_theta_velocity_space();
         _build_pole_theta_space();
         _build_cart_position_space();
         _build_cart_velocity_space();
    }
    else if(state_idx_ == 2 ){
        _build_pole_theta_velocity_space();
    }
    else if( state_idx_ == 0 ){
        _build_cart_position_space();
    }
    else if( state_idx_ == 1){
        _build_cart_velocity_space();
    }
    else if( state_idx_ == 3){
        _build_pole_theta_velocity_space();
    }
}

void
TiledCartPole::_build_cart_position_space(){

    auto low = boundaries_.cart_pole_pos.first;
    auto high = boundaries_.cart_pole_pos.second;
    cart_pos_space_ = linspace(low, high, this->n_states());
}

void
TiledCartPole::_build_cart_velocity_space(){

    auto low = boundaries_.cart_pole_vel.first;
    auto high = boundaries_.cart_pole_vel.second;
    cart_vel_space_ = linspace(low, high, this->n_states());
}

void
TiledCartPole::_build_pole_theta_space(){

    auto low = boundaries_.cart_pole_theta.first;
    auto high = boundaries_.cart_pole_theta.second;
    pole_theta_space_ = linspace(low, high, this->n_states());
}

void
TiledCartPole::_build_pole_theta_velocity_space(){

    auto low = boundaries_.cart_pole_theta_vel.first;
    auto high = boundaries_.cart_pole_theta_vel.second;
    pole_theta_velocity_space_ = linspace(low, high, this->n_states());
}


}
