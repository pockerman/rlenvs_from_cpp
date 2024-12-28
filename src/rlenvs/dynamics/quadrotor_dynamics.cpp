#include "rlenvs/dynamics/quadrotor_dynamics.h"
#include "rlenvs/rlenvs_consts.h"
#include "rlenvs/utils/math_utils.h"

namespace rlenvscpp{
namespace dynamics {


QuadrotorDynamics::QuadrotorDynamics()
    :
      MotionModelDynamicsBase<SysState<12>,
                                  DynamicsMatrixDescriptor,
                                  std::map<std::string, boost::any>>()
{}


QuadrotorDynamics::state_t&
QuadrotorDynamics::evaluate(const input_t& input ){

}


void
QuadrotorDynamics::integrate(const QuadrotorDynamics::input_t& input){

}


void 
QuadrotorDynamics::translational_dynamics(const RealVec& motor_w){
	
	const auto mass = config_.mass;
	
	// compute the gravity force
	RealVec fg = RealVec::Zero(3);
	
	if(config_.use_gravity){
		fg[2] = mass * rlenvscpp::consts::maths::G;
	}
	
	// compute the total thurst force
	RealVec ft = RealVec::Zero(3);
	ft[2] = config_.k_1 * rlenvscpp::utils::maths::sum_sqr(motor_w);
}

}
}
