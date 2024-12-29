#include "rlenvs/dynamics/quadrotor_dynamics.h"
#include "rlenvs/rlenvs_consts.h"
#include "rlenvs/utils/math_utils.h"
#include <cmath>

namespace rlenvscpp{
namespace dynamics {


QuadrotorDynamics::QuadrotorDynamics(QuadrotorDynamicsConfig config,
					  SysState<12>& state)
    :
      MotionModelDynamicsBase<SysState<12>,
							  DynamicsMatrixDescriptor,
							  RealVec>(state),
	config_(config)
{
	old_v_ = RealColVec3d::Zero(3);
	v_dot_ = RealColVec3d::Zero(3);
	old_omega_ = RealColVec3d::Zero(3);
	omega_dot_ = RealColVec3d::Zero(3);
	euler_angles_old_ = RealColVec3d::Zero(3);
	euler_dot_ = RealColVec3d::Zero(3);
	
	rotation_mat_ = RealMat3d::Zero();
	euler_mat_ = RealMat3d::Zero();
}


QuadrotorDynamics::state_type&
QuadrotorDynamics::evaluate(const input_type& input ){
	integrate(input);
}


void
QuadrotorDynamics::integrate(const RealVec& motor_w){

	translational_dynamics(motor_w);
	rotational_dynamics(motor_w);
	
	update_euler_angles_();
	update_rotation_matrix_();
	update_position_();
}

void 
QuadrotorDynamics::update_position_(){
	
	const auto dt = config_.dt;
	RealColVec3d p = dt * rotation_mat_ * old_v_ + old_p_;
}

void 
QuadrotorDynamics::update_euler_angles_(){
	
	const auto dt = config_.dt;
	const auto phi = euler_angles_old_[0];
	const auto theta = euler_angles_old_[1];
	const auto psi = euler_angles_old_[2];
	
	euler_mat_(0, 0) = 1.0;
	euler_mat_(0, 1) = std::sin(phi) * std::tan(theta);
	euler_mat_(0, 2) = std::cos(phi) * std::tan(theta);
	
	euler_mat_(1, 0) = 0.0;
	euler_mat_(1, 1) = std::cos(phi);
	euler_mat_(1, 2) = -std::sin(phi);
	
	euler_mat_(2, 0) = 0.0;
	euler_mat_(2, 1) = std::sin(phi) / std::cos(theta);
	euler_mat_(2, 2) = std::cos(phi) / std::cos(theta);
	
	RealColVec3d euler = dt * euler_mat_ * old_omega_ + euler_angles_old_;
	
	euler_dot_ = (euler - euler_angles_old_) / dt;
	euler_angles_old_ = euler;
	
}

void 
QuadrotorDynamics::update_rotation_matrix_(){
	
	const auto phi = euler_angles_old_[0];
	const auto theta = euler_angles_old_[1];
	const auto psi = euler_angles_old_[2];
	
	
	auto ctheta = std::cos(theta);
	auto cpsi = std::cos(psi);
	auto cphi = std::cos(phi);
	
	auto stheta = std::sin(theta);
	auto spsi = std::sin(psi);
	auto sphi = std::sin(phi);
	
	rotation_mat_(0,0) = ctheta * cpsi;
	rotation_mat_(0,1) = spsi * stheta * cpsi - cphi * spsi;
	rotation_mat_(0,2) = cphi * stheta * cpsi + sphi * spsi;
	
	rotation_mat_(1,0) = ctheta * spsi;
	rotation_mat_(1,1) = sphi * stheta * spsi + cphi * cpsi;
	rotation_mat_(1,2) = cphi * stheta * spsi - sphi * cpsi;
	
	rotation_mat_(0,0) = stheta;
	rotation_mat_(0,1) = -sphi * ctheta;
	rotation_mat_(0,2) = -cphi * ctheta;
}

void 
QuadrotorDynamics::rotational_dynamics(const RealVec& motor_w){
	
	const auto l = config_.l;
	const auto k_1 = config_.k_1;
	const auto k_2 = config_.k_2;
	const auto dt = config_.dt;
	
	auto p = old_omega_[0];
	auto q = old_omega_[1];
	auto r = old_omega_[2];
	
	auto Jx = config_.Jx;
	auto Jy = config_.Jy;
	auto Jz = config_.Jz;
	
	RealColVec3d omega_cross_h = RealColVec3d::Zero(3);
	omega_cross_h[0] = ((Jy - Jz) / Jx) * q * r;
	omega_cross_h[1] = ((Jz - Jx) / Jy) * p * r;
	omega_cross_h[2] = ((Jx - Jy) / Jz) * p * q;
	
	// the torque vector
	RealColVec3d tau = RealColVec3d::Zero(3);
	
	// tau_phi = l*K_T*((omeag_4)^2 - (omeag_2)^2) 
	tau[0] = l * k_1* (rlenvscpp::utils::maths::sqr(motor_w[3]) - rlenvscpp::utils::maths::sqr(motor_w[1]));
	tau[0] *= 1.0 / Jx;

	// tau_theta = l*K_T*((omeag_1)^2 - (omeag_3)^2) 
	tau[1] = l * k_1* (rlenvscpp::utils::maths::sqr(motor_w[0]) - rlenvscpp::utils::maths::sqr(motor_w[2]));
	tau[1] = 1.0 / Jy;
	
	// tau_psi = Kd((omega_1)^2 - (omega_2)^2 + (omega_3)^2 - (omega_4)^2) 
	tau[2] = k_2* (rlenvscpp::utils::maths::sqr(motor_w[0]) 
	                   - rlenvscpp::utils::maths::sqr(motor_w[1])
					   + rlenvscpp::utils::maths::sqr(motor_w[2]) 
					   - rlenvscpp::utils::maths::sqr(motor_w[3]));
	tau[2] *= 1.0 / Jz; 
	
	RealColVec3d omega = dt * omega_cross_h + dt * tau - old_omega_;
	
	omega_dot_ = (omega - old_omega_) / dt;
	
	// update the angular velocity
	old_omega_ = omega;
}

void 
QuadrotorDynamics::translational_dynamics(const RealVec& motor_w){
	
	const auto mass = config_.mass;
	const auto dt = config_.dt;
	
	// compute the gravity force
	RealColVec3d fg = RealColVec3d::Zero(3);
	
	if(config_.use_gravity){
		fg[2] = mass * rlenvscpp::consts::maths::G;
	}
	
	// compute the total thurst force
	RealColVec3d ft = RealColVec3d::Zero(3);
	ft[2] = config_.k_1 * rlenvscpp::utils::maths::sum_sqr(motor_w);
	
	// form the vector ωb/i × v
	RealColVec3d  omega_cross_v = old_omega_.cross(old_v_);
	
	// compute the velocity increment
	RealColVec3d v = dt * omega_cross_v  + (1.0 / mass ) * dt *  fg - (1.0 / mass ) * dt * ft - old_v_;
	
	v_dot_ = (v - old_v_) / dt;
	
	// update the old velocity
	old_v_ = v;
}

}
}
