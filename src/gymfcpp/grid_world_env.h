#ifndef GRID_WORLD_ENV_H
#define GRID_WORLD_ENV_H

/**
  * Implements the Gridworld environment from the
  * book Deep Reinforcement Learning in Action by Manning publications.
  * You can find the original environment here:
  * https://github.com/DeepReinforcementLearning/DeepReinforcementLearningInAction
  * */

#include "gymfcpp/gymfcpp_types.h"
#include "gymfcpp/time_step.h"
#include "gymfcpp/discrete_space.h"
#include "gymfcpp/env_mixin.h"
#include "gymfcpp/extern/enum.h"
#include "gymfcpp/gymfcpp_config.h"
#include "gymfcpp/time_step_type.h"
#include "gymfcpp/numpy_cpp_utils.h"

#ifdef GYMFCPP_DEBUG
#include <cassert>
#endif

#include <vector>
#include <string>
#include <utility>
#include <map>

///
/// Different namespace so that we differentiate
/// from OpenAI-Gym environment
///
namespace rlenvs_cpp
{

// still we may want to use some utilities
//using namespace gymfcpp;

namespace{

uint_t
max(const std::pair<uint_t,  uint_t>& position){
    return position.first > position.second ? position.first : position.second;
}

uint_t
min(const std::pair<uint_t,  uint_t>& position){
    return position.first < position.second ? position.first : position.second;
}

uint_t
get_non_zero_idx(std::pair<uint_t, uint_t>& p){

}

inline
bool operator == (const std::pair<uint_t,  uint_t>& p1, const std::pair<uint_t,  uint_t>& p2){
    return p1.first == p2.first && p1.second == p2.second;
}

inline
std::pair<uint_t, uint_t>
operator + (const std::pair<uint_t,  uint_t>& p1, const std::pair<uint_t,  uint_t>& p2){
    return std::make_pair(p1.first + p2.first, p1.second + p2.second);
}

}


///
/// \brief The RenderModeType enum
///
BETTER_ENUM(GridworldInitType, char, STATIC=0, RANDOM, PLAYER, INVALID_TYPE);


///
/// \brief to_string.  Returns the RenderModeType to its stringrepresentation
/// \param type The RenderModeType to convert
/// \return std::string
inline
std::string to_string(GridworldInitType type){return type._to_string();}



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
class Gridworld final
{
public:

    static_assert (side_size_ >= 4, "The side size should be greater than 4");

    ///
    /// \brief name
    ///
    static  const std::string name;

    ///
    /// \brief action_space_t. The type of the action space
    ///
    typedef DiscreteSpace<4> action_space_type;

    ///
    /// \brief action_t
    ///
    typedef action_space_type::item_t action_type;

    ///
    /// \brief action_space_t. The type of the action space
    ///
    typedef DiscreteSpace<side_size_ * side_size_> state_space_type;

    ///
    /// \brief state_type
    ///
    //typedef typename state_space_type::item_t state_type;

     typedef std::vector<std::vector<std::vector<real_t>>> raw_state_type;
     typedef std::vector<real_t> state_type;

    ///
    /// \brief time_step_t. The type of the time step
    ///
    typedef TimeStep<state_type> time_step_type;

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
    Gridworld(std::string version, GridworldInitType init_mode, bool create=true);

    ///
    /// \brief Constructor. Initializes the world by allowing noise to be
    /// added in the observation vector
    ///
    Gridworld(std::string version, GridworldInitType init_mode, uint_t seed,
              real_t noise_factor, bool create=true);

    ///
    /// \brief version
    /// \return
    ///
    std::string version()const noexcept{return version_;}

    ///
    /// \brief init_type
    /// \return
    ///
    GridworldInitType init_type()const noexcept{return init_mode_;}

    ///
    /// \brief is_created
    /// \return
    ///
    bool is_created()const noexcept{return is_created_;}

    ///
    /// \brief make. Builds the environment. Optionally we can choose if the
    /// environment will be slippery
    ///
    void make();

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
    /// \brief render. Does nothing simply here to respect the contract.
    ///
    void render(){return;}

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
    GridworldInitType init_mode_;

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
Gridworld<side_size>::Gridworld(std::string version, GridworldInitType init_mode, bool create)
    :
      version_(version),
      init_mode_(init_mode),
      randomize_state_(false),
      seed_(0),
      noise_factor_(0.0),
      is_created_(false)
{
    if(create){
        make();
    }
}

template<uint_t side_size>
Gridworld<side_size>::Gridworld(std::string version, GridworldInitType init_mode, uint_t seed,
                                real_t noise_factor, bool create)
    :
      version_(version),
      init_mode_(init_mode),
      randomize_state_(true),
      seed_(seed),
      noise_factor_(noise_factor),
      is_created_(false)
{}

template<uint_t side_size>
void
Gridworld<side_size>::make(){

    if(is_created()){
        return;
    }

    // initialize the board
    init_board_();
    switch (init_mode_) {

        case GridworldInitType::STATIC:
        {
            build_static_mode_();
            break;
        }
        case GridworldInitType::RANDOM:
        {
            build_random_mode_();
            break;
        }
        case GridworldInitType::PLAYER:
        {
            build_player_mode_();
            break;
        }
#ifdef GYMFCPP_DEBUG
        default:
        {
            assert(false && "Invalid initialization mode");
        }
#endif

    }

    is_created_ = true;
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


}

#endif // GRID_WORLD_ENV_H
