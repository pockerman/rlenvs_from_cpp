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
/// \brief The RenderModeType enum
///
BETTER_ENUM(GridWorldInitType, char, STATIC=0, RANDOM=1, PLAYER=2, INVALID_TYPE=4);


///
/// \brief to_string.  Returns the RenderModeType to its stringrepresentation
/// \param type The RenderModeType to convert
/// \return std::string
inline
std::string to_string(GridWorldInitType type){return type._to_string();}


GridWorldInitType
from_string(const std::string& gw_init_type);


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


     typedef std::vector<std::vector<std::vector<real_t>>> raw_state_type;
     //typedef std::vector<real_t> state_type;


    ///
    /// \brief n_components
    ///
    static const uint_t n_components = 4;

    ///
    /// \brief side_size
    ///
    static const uint_t side_size = side_size_;

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
    /// \brief The MoveType enum
    ///
    enum MoveType{VALID=0, INVALID=1, LOST_GAME=2};

    ///
    /// \brief The BoardComponentType enum
    ///
    enum BoardComponentType{PLAYER=0, GOAL=1, PIT=2, WALL=3};

    typedef std::pair<uint_t,  uint_t> Position;

    ///
    /// \brief init_board_
    ///
    void init_board_();

    ///
    /// \brief build_static_mode_
    ///
    void build_static_mode_();

    ///
    /// \brief build_random_mode_
    ///
    void build_random_mode_();

    ///
    /// \brief build_player_mode_
    ///
    void build_player_mode_();

    ///
    /// \brief check_move_
    /// \param row
    /// \param col
    ///
    void check_move_(uint_t row, uint_t col);

    ///
    /// \brief validate_move_
    /// \param piece
    /// \param row
    /// \param col
    ///
    MoveType validate_move_(BoardComponentType piece, Position pos);

    ///
    /// \brief The BoardPiece struct
    ///
    struct BoardPiece
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
        BoardPiece(std::string name_, std::string code_, Position pos_)
            :
              name(name_),
              code(code_),
              pos(pos_)
        {}

        ///
        /// \brief Default constructor
        ///
        BoardPiece()=default;
    };

    ///
    /// \brief The BoardPiece struct
    ///
    struct BoardMask
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
    };

    ///
    /// Represents the board
    ///
    struct Board
    {
        uint_t size;
        std::map<BoardComponentType, BoardPiece> components;
        std::map<std::string, BoardMask> masks;

        ///
        /// \brief move_piece Move the pice to the given position
        /// \param piece
        /// \param pos
        ///
        void move_piece(BoardComponentType piece, Position pos);

        ///
        /// \brief get_observation
        /// \return
        ///
        raw_state_type get_observation()const;

        ///
        /// \brief close
        ///
        void close();
    };

    ///
    /// \brief board_
    ///
    Board board_;
};

template<uint_t side_size>
const std::string Gridworld<side_size>::name = "Gridworld";


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

        switch( action ){
            case 0:
            {
                // move up
                 check_move_(-1, 0);
                 break;
            }
            case 1:
            {
                //down
                check_move_(1, 0);
                break;
            }
            case 2:
            {
                // left
                 check_move_(0, -1);
                 break;
            }
            case 3:
            {
                // right
                check_move_(0, 1);
                break;
            }
#ifdef GYMFCPP_DEBUG
            default:
            {
                assert(false && "Invalid move");
            }
#endif

        }

        // get the observation
        auto obs = to_1d_from_3d<raw_state_type, real_t>(n_components, side_size, side_size, get_raw_observation());

        if(has_random_state()){

            // add uniform noise and scale
            add_uniform_noise(obs, this->seed(), noise_factor());
        }

        auto reward = get_reward();
        auto step_type = reward == -1.0 ? TimeStepTp::LAST : TimeStepTp::MID;
        auto time_step = time_step_type(step_type, reward, obs);

        return time_step;
}

template<uint_t side_size>
typename Gridworld<side_size>::time_step_type
Gridworld<side_size>::reset(){

    // reinitialize the board
    init_board_();
    auto obs = to_1d_from_3d<raw_state_type, real_t>(n_components, side_size, side_size, get_raw_observation());

    if(has_random_state()){

        // add uniform noise and scale
        add_uniform_noise(obs, this->seed(), noise_factor());
    }

    auto reward = get_reward();
    auto time_step = time_step_type(TimeStepTp::FIRST, reward, obs);
    return time_step;
}

template<uint_t side_size>
real_t
Gridworld<side_size>::get_reward()const{

    auto player_pos = board_.components.find(BoardComponentType::PLAYER)->second.pos;
    auto pit_pos = board_.components.find(BoardComponentType::PIT)->second.pos;
    auto goal_pos = board_.components.find(BoardComponentType::GOAL)->second.pos;
    if (player_pos == pit_pos){
        return -10.;
    }
    else if (player_pos == goal_pos){
        return 10.0;
    }

    return -1.0;
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

template<uint_t side_size>
void
Gridworld<side_size>::init_board_(){

    board_.size = side_size;
    board_.components[BoardComponentType::PLAYER] = BoardPiece("Player", "P", std::make_pair(0, 0));
    board_.components[BoardComponentType::GOAL] = BoardPiece("Goal", "G", std::make_pair(1, 0));
    board_.components[BoardComponentType::PIT] = BoardPiece("Pit", "-", std::make_pair(2, 0));
    board_.components[BoardComponentType::WALL] = BoardPiece("Wall", "W", std::make_pair(3, 0));
}


template<uint_t side_size>
typename Gridworld<side_size>::MoveType
Gridworld<side_size>::validate_move_(BoardComponentType piece, Position pos){

    // 0 is valid
    //auto outcome = 0 #0 is valid, 1 invalid, 2 lost game
    auto outcome = Gridworld<side_size>::MoveType::VALID;

    // get position of pit
    auto pit_pos = board_.components[BoardComponentType::PIT].pos;
    auto wall_pos = board_.components[BoardComponentType::WALL].pos;

    auto new_pos = board_.components[piece].pos + pos;

     if (new_pos == wall_pos){
         //1 //block move, player can't move to wall
         outcome = Gridworld<side_size>::MoveType::INVALID;
     }
     else if( max(new_pos) > (board_.size - 1 )){
        // #if outside bounds of board
         outcome = Gridworld<side_size>::MoveType::INVALID;
    }
    else if( min(new_pos) < 0){
        // #if outside bounds
         outcome = Gridworld<side_size>::MoveType::INVALID;
    }
    else if( new_pos == pit_pos){
         outcome = Gridworld<side_size>::MoveType::LOST_GAME;
    }

    return outcome;
}

template<uint_t side_size>
void
Gridworld<side_size>::check_move_(uint_t row, uint_t col){

    auto position = std::make_pair(row, col);
    auto move_type = validate_move_(BoardComponentType::PLAYER, position);

    if( move_type == MoveType::VALID || move_type == MoveType::LOST_GAME){

        auto new_pos = board_.components[BoardComponentType::PLAYER].pos + position;
        board_.move_piece(BoardComponentType::PLAYER, new_pos);
    }

}

template<uint_t side_size>
void
Gridworld<side_size>::build_static_mode_(){

    // Row, Column
    board_.components[BoardComponentType::PLAYER].pos = std::make_pair(0,3);
    board_.components[BoardComponentType::GOAL].pos = std::make_pair(0,0);
    board_.components[BoardComponentType::PIT].pos = std::make_pair(0,1);
    board_.components[BoardComponentType::WALL].pos = std::make_pair(1,1);

}

template<uint_t side_size>
void
Gridworld<side_size>::build_random_mode_(){

}

template<uint_t side_size>
void
Gridworld<side_size>:: build_player_mode_(){

}

// Board inner struct

template<uint_t side_size>
void
Gridworld<side_size>::Board::close(){

    components.clear();
    masks.clear();
}

template<uint_t side_size>
typename Gridworld<side_size>::raw_state_type
Gridworld<side_size>::Board::get_observation()const{

    // initialize the data struct the
    // represents the state
    auto num_pieces = components.size() + masks.size();
    raw_state_type board(num_pieces);

    for(uint_t i=0; i<num_pieces; ++i){
        board[i].resize(size);

        for(uint_t j=0; j<size; ++j){
            board[i][j].resize(size);

            for(uint_t k=0; k<size; ++k){
                board[i][j][k] = 0;
            }
        }
    }

    auto layer = 0;

    // check if we can move the piece
    auto comp_begin = components.begin();
    auto comp_end = components.end();

    for(; comp_begin != comp_end; ++comp_begin){

        auto position = comp_begin->second.pos;
        board[layer][position.first][position.second] = 1;
        layer +=1;
    }

    return board;

    // initialize the data struct the
    // represents the state
    /* displ_board = np.zeros((num_pieces, self.size, self.size), dtype=np.uint8)
            layer = 0
            for name, piece in self.components.items():
                pos = (layer,) + piece.pos
                displ_board[pos] = 1
                layer += 1*/


}

template<uint_t side_size>
void
Gridworld<side_size>::Board::move_piece(BoardComponentType piece, Position pos){

    auto move = true;

    // check if we can move the piece
    //auto mask_begin = masks.begin();
    //auto mask_end = masks.end();

    /*for(; mask_begin != mask_end; ++mask_begin){
        position =
    }*/


    //        for _, mask in self.masks.items():
    //            if pos in zip_positions2d(mask.get_positions()):
    //                move = False
    if( move){
        components[piece].pos = pos;
    }

}

}// grid_world
}// envs
}// rlenvs_cpp

#endif // GRID_WORLD_ENV_H
