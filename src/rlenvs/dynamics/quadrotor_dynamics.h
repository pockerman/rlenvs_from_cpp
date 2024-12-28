#ifndef QUADROTOR_DYNAMICS_H
#define QUADROTOR_DYNAMICS_H

#include "rlenvs/rlenvs_types_v2.h"
#include "rlenvs/dynamics/system_state.h"
#include "rlenvs/dynamics/motion_model_base.h"
#include "rlenvs/dynamics/dynamics_matrix_descriptor.h"

#include <any>

namespace rlenvscpp {
namespace dynamics {
	
	
struct QuadrotorDynamicsConfig
{
	
	///
	/// \brief mass
	///
	real_t mass;
	
	///
	/// \brief the arm length
	///
	real_t l;
	
	///
	/// \brief Proportionality constant
	///
	real_t k_1;
	
	///
	/// \brief flag indicating if gravity is used
	///
	bool use_gravity{true};
};

///
/// \brief The QuadrotorDynamics class. Implements quadrotor dynamics
/// The implementation of this class follows the System Modeling
/// section at https://wilselby.com/research/arducopter/modeling/
/// The quadrotor is assumed to have four rotors 
/// Each rotor consists of a brushless DC motor and rotor with a fixed pitch.
/// It is assumed that the quadrotor can move in 6 degrees of freedom; 3-translational and 3-rotational, 
/// The motion of the quadrotor is controlled via 4 inputs namely the speeds of the 4 motors.
/// The quadrotor model that this class implements assumes the following:
///
/// - The quadrotor structure is rigid and symmetrical with a center of mass aligned with the center of the body frame of the vehicle.
/// - The thrust and drag of each motor is proportional to the square of the motor velocity.
/// - The propellers are considered to be rigid and therefore blade flapping is negligible (deformation of propeller blades due to high velocities and flexible material).
/// - The Earth is flat and non-rotating (difference of gravity by altitude or the spin of the earth is negligible)
/// - Surrounding fluid velocities (wind) are negligible.
/// - Ground effect is negligible
///
class QuadrotorDynamics: public MotionModelDynamicsBase<SysState<12>,
        DynamicsMatrixDescriptor, std::map<std::string, boost::any>>
{
public:

    ///
    /// \brief The type of the state handled by this dynamics object
    ///
    typedef MotionModelDynamicsBase<SysState<12>,
                            DynamicsMatrixDescriptor, 
							std::map<std::string, std::any>>::state_t state_t;

    ///
    /// \brief input_t The type of the input for solving the dynamics
    ///
    typedef MotionModelDynamicsBase<SysState<12>,
                            DynamicsMatrixDescriptor,
                            std::map<std::string, std::any>>::input_t input_t;

    ///
    /// \brief matrix_t Matrix type that describes the dynamics
    ///
    typedef MotionModelDynamicsBase<SysState<12>,
                            DynamicsMatrixDescriptor,
                            std::map<std::string, std::any>>::matrix_t matrix_t;

    ///
    /// \brief vector_t
    ///
    typedef MotionModelDynamicsBase<SysState<12>,
                            DynamicsMatrixDescriptor,
                            std::map<std::string, std::any>>::vector_t vector_t;

    ///
    /// \brief QuadrotorDynamics Constructor. 
	/// Specify the mass and the arm length of the
	/// simulated quadrotor. It is assumed that
	/// the mass of the quadrotor remains fixed
    ///
    QuadrotorDynamics(QuadrotorDynamicsConfig config );

    ///
    /// \brief Evaluate the new state using the given input
    /// it also updates the various matrices if needed
    ///
    virtual state_t& evaluate(const input_t& input )override;

    ///
    /// \brief Integrate the new state. It also uses error terms
    ///
    void integrate(const input_t& input);
	
	///
	/// \brief Implements the translational dynamics.
	/// It accepts the motors angular velocities.
	/// Notice that this model will squared and input
	/// velocities
	///
	void translational_dynamics(const RealVec& motor_w);
	
private:
	
	QuadrotorDynamicsConfig config_;


};

}

}

#endif // QUADROTOR_DYNAMICS_H
