#include "gymfcpp/tiled_cart_pole_env.h"

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


void
TiledCartPole::create_bins(){

    if(state_idx_ == 4){
         _build_theta_velocity_space();
         _build_pole_theta_space();
         _build_cart_position_space();
         _build_cart_velocity_space();
    }
    else if(state_idx_ == 2 ){
        _build_theta_velocity_space();
    }
    else if( state_idx_ == 0 ){
        _build_cart_position_space();
    }
    else if( state_idx_ == 1){
        _build_cart_velocity_space();
    }
    else if( state_idx_ == 3){
        _build_theta_velocity_space();
    }

}

void
TiledCartPole::_build_theta_velocity_space(){

    auto low = boundaries_.cart_pole_theta_vel.first;
    auto high = boundaries_.cart_pole_theta_vel.second;
    pole_theta_space_; //= np.linspace(low, high, self.n_states)

}

void
TiledCartPole::_build_cart_position_space(){

}

void
TiledCartPole::_build_cart_velocity_space(){

}

void
TiledCartPole::_build_theta_velocity_space(){

}


}
