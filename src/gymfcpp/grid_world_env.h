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

#ifdef GYMFCPP_DEBUG
#include <cassert>
#endif

#include <string>
#include <utility>
#include <map>

///
/// Different namespace so that we differentiate
/// from OpenAI-Gym environment
///
namespace rlenvs
{

// still we may want to use some utilities
using namespace gymfcpp;


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
template<uint_t side_size>
class Gridworld final
{
public:

    static_assert (side_size >= 4, "The side size should be greater than 4");

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
    typedef DiscreteSpace<side_size * side_size> state_space_type;

    ///
    /// \brief state_type
    ///
    typedef typename state_space_type::item_t state_type;

    ///
    /// \brief time_step_t. The type of the time step
    ///
    typedef TimeStep<state_type> time_step_type;

    ///
    /// \brief Constructor
    ///
    Gridworld(std::string version, GridworldInitType init_mode, bool create=true);

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
    uint_t n_states()const noexcept{ return side_size * side_size; }

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
    /// \brief render
    ///
    void render();

    ///
    /// \brief close
    ///
    void close();

    ///
    ///
    ///
    void reset();

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
    /// \brief is_created_
    ///
    bool is_created_;

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


    typedef std::pair<uint_t,  uint_t> Position;

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
        ///
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
    ///
    ///
    struct Board
    {
        uint_t size;
        std::map<std::string, BoardPiece> components;
        std::map<std::string, BoardMask> masks;
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
      is_created_(false)
{
    if(create){
        make();
    }
}

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

}

template<uint_t side_size>
void
Gridworld<side_size>::render(){

}

template<uint_t side_size>
void
Gridworld<side_size>::close(){

}

template<uint_t side_size>
void
Gridworld<side_size>::reset(){

}


template<uint_t side_size>
void
Gridworld<side_size>::init_board_(){

    board_.size = side_size;
    board_.components["Player"] = BoardPiece("Player", "P", std::make_pair(0, 0));
    board_.components["Goal"] = BoardPiece("Goal", "G", std::make_pair(1, 0));
    board_.components["Pit"] = BoardPiece("Pit", "-", std::make_pair(2, 0));
    board_.components["Wall"] = BoardPiece("Wall", "W", std::make_pair(3, 0));
}

template<uint_t side_size>
void
Gridworld<side_size>::build_static_mode_(){

    // Row, Column
    board_.components["Player"].pos = std::make_pair(0,3);
    board_.components["Goal"].pos = std::make_pair(0,0);
    board_.components["Pit"].pos = std::make_pair(0,1);
    board_.components["Wall"].pos = std::make_pair(1,1);

}

template<uint_t side_size>
void
Gridworld<side_size>::build_random_mode_(){

}

template<uint_t side_size>
void
Gridworld<side_size>:: build_player_mode_(){

}


}

#endif // GRID_WORLD_ENV_H
