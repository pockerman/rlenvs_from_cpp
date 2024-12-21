#ifndef CONNECT2_ENV_H
#define CONNECT2_ENV_H

#include "rlenvs/rlenvscpp_config.h"
#include "rlenvs/rlenvs_types_v2.h"
#include "rlenvs/discrete_space.h"
#include "rlenvs/time_step.h"
#include "rlenvs/envs/synchronized_env_mixin.h"

#include <boost/noncopyable.hpp>
#include <vector>
#include <string>
#include <unordered_map>

namespace rlenvs_cpp{
namespace envs{
namespace connect2{

///
/// \brief Implementation of Connect2 environment from https://github.com/JoshVarty/AlphaZeroSimple
/// Initially the environment has all its positions set to zero. When a player makes
/// a move then the position corresponding to this move 
///	
class Connect2 final: private boost::noncopyable, protected synchronized_env_mixin{
	
public:
	
	///
    /// \brief name
    ///
    static  const std::string name;
	
	///
    /// \brief The type of the action space
	///
    typedef DiscreteSpace<4> action_space_type;
	
	///
	/// \brief The type of the action
	///
	typedef typename action_space_type::item_t action_type;
	
	///
	/// \brief The state type
	///
	typedef std::vector<int> state_type;
	
	///
	/// \brief The type of the time step
	///
    typedef TimeStep<state_type> time_step_type;
	
	///
    /// \brief Constructor
    ///
    Connect2();

    ///
	/// \brief Expose functionality. Use the default as nothing to
	/// synchronize
	///
    using synchronized_env_mixin::sync;

    ///
    /// \brief version
    /// \return
    ///
    std::string version()const noexcept{return version_;}

    ///
    /// \brief is_created
    /// \return
    ///
    bool is_created()const noexcept{return is_created_;}
	
    ///
    /// \brief make. Builds the environment. Optionally we can choose if the
    /// environment will be slippery
    ///
    void make(const std::string& version,
              const std::unordered_map<std::string, std::any>& options);

    ///
    /// \brief n_states. Returns the number of states
    ///
    uint_t n_states()const noexcept{ return 53; }

    ///
    /// \brief n_actions. Returns the number of actions
    ///
    uint_t n_actions()const noexcept{return action_space_type::size;}

    ///
    /// \brief step. Move in the environment with the given action
	/// This function always moves player_1
    /// \param action
    /// \return
    ///
    time_step_type step(action_type action);
		
	///
	/// \brief Make a move for the player with the given id
	///
	time_step_type move(const uint_t pid, action_type action);

	///
	/// \brief Returns true if the player wins
	///
	bool is_win(uint_t player)const noexcept;
	
	///
	/// \brief Returns true if there are still legal moves to do
	///
	bool has_legal_moves()const noexcept;

    ///
    /// \brief close
    ///
    void close();

    ///
    /// \brief reset the environment
    ///
    time_step_type reset();
	
	///
	/// \brief Get the valid moves
	///
	std::vector<int> get_valid_moves()const;

private:
	
	bool is_created_;
	
	///
	/// \brief Version of the environment
	///
	std::string version_;
	
	///
	/// \brief The discount factor
	///
	real_t discount_;
	
	///
	/// \brief The id of player 1
	///
	const uint_t player_id_1_{1};
	
	///
	/// \brief The id of player 2
	///
	const uint_t player_id_2_{2};
	
	///
	/// \brief The wining value for a player
	///
	const uint_t win_val_{2};
		
	///
	/// \brief The representation of the board
	///
	std::vector<int> board_;
	
	///
	/// \brief Flag indicating if the game is finished
	///
	bool is_finished_{false};
	
};

inline
void 
Connect2::close(){
	board_ = std::vector<int> ();
	is_created_ = false;
}
		
}
}
}

#endif