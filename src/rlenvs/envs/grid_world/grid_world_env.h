// SPDX-FileCopyrightText: 2024 <copyright holder> <email>
// SPDX-License-Identifier: Apache-2.0
#ifndef GRID_WORLD_ENV_H
#define GRID_WORLD_ENV_H

/**
  * Implements the Gridworld environment from the
  * book Deep Reinforcement Learning in Action by Manning publications.
  * You can find the original environment here:
  * https://github.com/DeepReinforcementLearning/DeepReinforcementLearningInAction
  * */

#include "rlenvs/rlenvscpp_config.h"
#include "rlenvs/rlenvs_types_v2.h"
#include "rlenvs/rlenvs_consts.h"
#include "rlenvs/discrete_space.h"
#include "rlenvs/time_step.h"
#include "rlenvs/envs/synchronized_env_mixin.h"
#include "rlenvs/extern/enum.h"

#ifdef RLENVSCPP_DEBUG
#include <cassert>
#endif

#include <boost/noncopyable.hpp>
#include <vector>
#include <string>
#include <utility>
#include <unordered_map>
#include <map>
#include <any>
#include <iostream>

///
/// Different namespace so that we differentiate
/// from OpenAI-Gym environment
///
namespace rlenvs_cpp{

// still we may want to use some utilities
//using namespace gymfcpp;

namespace envs{
namespace grid_world{

///
/// \brief The enum describing how to initialize the world
///
BETTER_ENUM(GridWorldInitType, int, STATIC=0, RANDOM=1, PLAYER=2, INVALID_TYPE=4);

/**
 * @brief Enumeration describing the action type
 */
BETTER_ENUM(GridWorldActionType, int, UP=0, DOWN=1, LEFT=2, RIGHT=3, INVALID_TYPE=4);




///
/// \brief to_string.  Returns the RenderModeType to its stringrepresentation
/// \param type The RenderModeType to convert
/// \return std::string
inline
std::string to_string(GridWorldInitType type){return type._to_string();}


GridWorldInitType
from_string(const std::string& gw_init_type);


namespace detail{


    // models a position on the board
    typedef std::pair<uint_t,  uint_t> board_position;


    /**
     * @brief Test if two positions are equal
     */
    bool operator==(const board_position& p1, const board_position& p2);

    /**
     * @brief Add two positions and return their result
     */
    board_position operator+(const board_position& p1, const board_position& p2);

    /**
     * @brief Returns the max component of a position
     */
    uint_t max(const board_position& p);

    /**
     * @brief Returns the min component of a position
     */
    uint_t min(const board_position& p);


    /**
     * @brief Array specifying the state of the board
     */
    typedef std::vector<std::vector<std::vector<real_t>>> board_state_type;

    ///
    /// \brief The BoardComponentType enum
    ///
    enum board_component_type{PLAYER=0, GOAL=1, PIT=2, WALL=3};

    ///
    /// \brief The MoveType enum
    ///
    enum board_move_type{VALID=0, INVALID=1, LOST_GAME=2};

    ///
    /// \brief The BoardPiece struct
    ///
    struct board_piece
    {
        ///
        ///
        ///
        std::string name;
        ///
        /// n ASCII character to display on the board
        ///
        std::string code;

        ///
        /// \brief pos 2-tuple e.g. (1,4)
        ///
        board_position pos;

        ///
        /// \brief BoardPiece
        /// \param name_
        /// \param code_
        /// \param pos_
        ///
        board_piece(std::string name_, std::string code_, board_position pos_)
            :
              name(name_),
              code(code_),
              pos(pos_)
        {}

        ///
        /// \brief Default constructor
        ///
        board_piece()=default;
    };

    ///
    /// \brief The BoardPiece struct
    ///
    /*struct BoardMask
    {
        ///
        ///
        ///
        std::string name;
        ///
        /// n ASCII character to display on the board
        ///
        std::string code;

        ///
        /// \brief pos 2-tuple e.g. (1,4)
        ///
        Position pos;

        ///
        /// \brief BoardPiece
        /// \param name_
        /// \param code_
        /// \param pos_
        ///
        BoardMask(std::string name_, std::string code_, Position pos_)
            :
              name(name_),
              code(code_),
              pos(pos_)
        {}
    };*/

    using board_mask = board_piece;
     ///
    /// Represents the board
    ///
    struct board
    {
        uint_t board_size;
        std::map<board_component_type, board_piece> components;
        std::map<std::string, board_mask> masks;

        /**
         * @brief initialize the board
         */
        void init_board(uint_t board_s);

        ///
        /// \brief move_piece Move the pice to the given position
        /// \param piece
        /// \param pos
        ///
        void move_piece(board_component_type piece, board_position pos);

        ///
        /// \brief get_state. Returns the state of the board
        /// \return
        ///
        board_state_type get_state()const;

        /**
         * @brief Get the reward the board currently returns depending
         * on the position of the player
         */
        real_t get_reward()const;

        ///
        /// \brief close
        ///
        void close();

        ///
        /// \brief build_static_mode
        ///
        void build_static_mode();

        ///
        /// \brief build_random_mode
        ///
        void build_random_mode();

        ///
        /// \brief build_player_mode
        ///
        void build_player_mode();

        /**
         * @brief check if the given move is valid and
         * change the position of the player
         */
        void check_move(uint_t row, uint_t col);


        ///
        /// \brief validate_move_
        /// \param piece
        /// \param row
        /// \param col
        ///
        board_move_type validate_move(board_component_type piece, board_position pos);
    };


}


///
/// The Gridworld class models a square board. There are three ways to initialize the board.
/// - static
/// - random
/// - player
/// See the GridworldInitType enumeration.
/// Static initialization means that the objects on the board are initialized at the same predetermined locations.
/// Player initialization means that the player is initialized at a random position on the board.
/// Random initialization means that all the objects are placed randomly
///
template<uint_t side_size_>
class Gridworld final: private boost::noncopyable, protected synchronized_env_mixin
{
public:

    static_assert (side_size_ >= 4, "The side size should be greater than or equal to 4");

    ///
    /// \brief name
    ///
    static  const std::string name;

    ///
    /// \brief n_components
    ///
    static const uint_t n_components;

    ///
    /// \brief side_size
    ///
    static const uint_t side_size;

    /**
     * @brief Convert the action index to a valid FrozenLakeActionsEnum
     *
     * */
    static GridWorldActionType action_from_int(uint_t aidx);

    /**
     * @brief The type of the action space
     */
    typedef DiscreteSpace<side_size_> action_space_type;

    /**
     * @brief The state space type. The state space
     *
     *
     */
    typedef DiscreteSpace<side_size_ * side_size_> state_space_type;

    /**
     * @brief The action type
     *
     */
    typedef typename action_space_type::item_t action_type;

     /**
     * @brief The state_type
     *
     */
    typedef typename state_space_type::item_t state_type;


    /**
     * @brief The type of the time step
     *
     */
    typedef TimeStep<state_type> time_step_type;

    /**
     * @brief The state type as this is returned from the underlying board
     */
    typedef std::vector<std::vector<std::vector<real_t>>> raw_state_type;


    ///
    /// \brief Constructor
    ///
    Gridworld();

    /*
     * Expose functionality
     *
     */
    using synchronized_env_mixin::sync;

    ///
    /// \brief version
    /// \return
    ///
    std::string version()const noexcept{return version_;}

    ///
    /// \brief init_type
    /// \return
    ///
    GridWorldInitType init_type()const noexcept{return init_mode_;}

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
    uint_t n_states()const noexcept{ return side_size_ * side_size_; }

    ///
    /// \brief n_actions. Returns the number of actions
    ///
    uint_t n_actions()const noexcept{return action_space_type::size;}

    ///
    /// \brief step
    /// \param action
    /// \return
    ///
    time_step_type step(action_type action);

    /**
     * @brief Step in the environment
     *
     */
    time_step_type step(GridWorldActionType action);

    ///
    /// \brief close
    ///
    void close();

    ///
    /// \brief reset the environment
    ///
    time_step_type reset();

    ///
    /// \brief get_reward
    /// \return
    ///
    real_t get_reward()const;

    ///
    /// \brief get_observation
    /// \return
    ///
    raw_state_type get_raw_observation()const;

    ///
    /// \brief has_random_state
    /// \return
    ///
    bool has_random_state()const noexcept{return randomize_state_;}

    ///
    /// \brief seed
    /// \return
    ///
    uint_t seed()const noexcept{return seed_;}

    ///
    /// \brief noise_factor
    /// \return
    ///
    real_t noise_factor()const noexcept{return noise_factor_;}

private:

    ///
    /// \brief version_
    ///
    std::string version_;

    ///
    /// \brief init_mode_
    ///
    GridWorldInitType init_mode_;

    ///
    /// \brief randomize_state_
    ///
    bool randomize_state_;

    ///
    /// \brief seed_
    ///
    uint_t seed_;

    ///
    /// \brief noise_factor_
    ///
    real_t noise_factor_;

    ///
    /// \brief is_created_
    ///
    bool is_created_;




    ///
    /// \brief init_board_
    ///
    void init_board_();


    ///
    /// \brief board_
    ///
    detail::board board_;
};

template<uint_t side_size>
const std::string Gridworld<side_size>::name = "Gridworld";

template<uint_t side_size_>
const uint_t Gridworld<side_size_>::side_size = side_size_;

template<uint_t side_size_>
const uint_t Gridworld<side_size_>:: n_components = 4;

template<uint_t side_size>
GridWorldActionType
Gridworld<side_size>::action_from_int(uint_t aidx){

    switch(aidx){
        case 0:
           return rlenvs_cpp::envs::grid_world::GridWorldActionType::UP;
        case 1:
            return rlenvs_cpp::envs::grid_world::GridWorldActionType::DOWN;
        case 2:
            return rlenvs_cpp::envs::grid_world::GridWorldActionType::LEFT;
        case 3:
            return rlenvs_cpp::envs::grid_world::GridWorldActionType::RIGHT;
    }

    return rlenvs_cpp::envs::grid_world::GridWorldActionType::INVALID_TYPE;

}


template<uint_t side_size>
Gridworld<side_size>::Gridworld()
    :
      version_(INVALID_STR),
      init_mode_(GridWorldInitType::INVALID_TYPE),
      randomize_state_(false),
      seed_(0),
      noise_factor_(0.0),
      is_created_(false)
{}


template<uint_t side_size>
void
Gridworld<side_size>::make(const std::string& version,
                           const std::unordered_map<std::string, std::any>& options){

    if(is_created()){
        return;
    }


    // find the mode
    auto mode = options.find("mode");

    if(mode != options.end()){
       init_mode_ = from_string(std::any_cast<std::string>(mode->second));
    }
    else{
       init_mode_ = GridWorldInitType::STATIC;
    }

    auto seed = options.find("seed");
    if(seed != options.end()){
        seed_ = std::any_cast<uint_t>(seed->second);
    }

    auto noise_factor = options.find("noise_factor");
    if(noise_factor != options.end()){
        noise_factor_ = std::any_cast<real_t>(noise_factor->second);
    }

    auto randomize_state = options.find("randomize_state");
    if(randomize_state != options.end()){
        randomize_state_ = std::any_cast<bool>(randomize_state->second);
    }

    // initialize the board
    init_board_();
    switch (init_mode_) {

        case GridWorldInitType::STATIC:
        {
            build_static_mode_();
            break;
        }
        case GridWorldInitType::RANDOM:
        {
            build_random_mode_();
            break;
        }
        case GridWorldInitType::PLAYER:
        {
            build_player_mode_();
            break;
        }
#ifdef RLENVSCPP_DEBUG
        default:
        {
            assert(false && "Invalid initialization mode");
        }
#endif

    }

    // set the version and set the board
    // to created
    is_created_ = true;
    version_ = version;
}

template<uint_t side_size>
typename Gridworld<side_size>::time_step_type
Gridworld<side_size>::step(action_type action){

        auto action_enum = Gridworld<side_size>::action_from_int(action);
        return step(action_enum);
}


template<uint_t side_size>
typename Gridworld<side_size>::time_step_type
Gridworld<side_size>::step(GridWorldActionType action){

    switch( action ){
        case GridWorldActionType::UP:
            {
                // move up
                 check_move_(-1, 0);
                 break;
            }
        case GridWorldActionType::DOWN:
            {
                //down
                check_move_(1, 0);
                break;
            }
        case GridWorldActionType::LEFT:
            {
                // left
                 check_move_(0, -1);
                 break;
            }
        case GridWorldActionType::RIGHT:
            {
                // right
                check_move_(0, 1);
                break;
            }
#ifdef RLENVSCPP_DEBUG
            default:
            {
                assert(false && "Invalid move");
            }
#endif

        }

        // what is the next state
        /*auto reward = get_reward();

        // get the observation
        auto obs = to_1d_from_3d<raw_state_type, real_t>(n_components, side_size, side_size, get_raw_observation());

        if(has_random_state()){

            // add uniform noise and scale
            add_uniform_noise(obs, this->seed(), noise_factor());
        }*/

        auto reward = get_reward();
        auto obs = get_raw_observation();
        auto step_type = reward == -1.0 ? TimeStepTp::LAST : TimeStepTp::MID;
        auto time_step = time_step_type(step_type, reward, obs);
        return time_step;
}

template<uint_t side_size>
typename Gridworld<side_size>::time_step_type
Gridworld<side_size>::reset(){

    // reinitialize the board
    init_board_();
    auto raw_observation = get_raw_observation();
    auto obs = to_1d_from_3d<raw_state_type, real_t>(n_components, side_size, side_size, get_raw_observation());

    /*if(has_random_state()){

        // add uniform noise and scale
        add_uniform_noise(obs, this->seed(), noise_factor());
    }*/

    auto reward = get_reward();
    auto time_step = time_step_type(TimeStepTp::FIRST, reward, obs);
    return time_step;
}

template<uint_t side_size>
real_t
Gridworld<side_size>::get_reward()const{
    return board_.get_reward();
}

template<uint_t side_size>
typename Gridworld<side_size>::raw_state_type
Gridworld<side_size>::get_raw_observation()const{

#ifdef GYMFCPP_DEBUG
    if(!is_created_){
        assert(false && "Environment has not been created. Have you called make?");
    }
#endif
    return board_.get_observation();
}


template<uint_t side_size>
void
Gridworld<side_size>::close(){
    board_.close();
}

}// grid_world
}// envs
}// rlenvs_cpp

#endif // GRID_WORLD_ENV_H
