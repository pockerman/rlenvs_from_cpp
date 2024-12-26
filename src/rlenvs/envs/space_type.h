#ifndef SPACE_TYPE_H
#define SPACE_TYPE_H

#include "rlenvs/rlenvs_types_v2.h"
#include <random>
#include <vector>
#include <type_traits>

namespace rlenvscpp {
namespace envs{

template<uint_t SpaceSize>
struct DiscreteSpace
{

    ///
    /// \brief item_t
    ///
    typedef uint_t space_item_type;


    ///
    /// \brief size
    ///
    static constexpr uint_t size = SpaceSize;

    ///
    /// \brief sample
    /// \return
    ///
    static space_item_type sample();

    ///
    /// \brief sample
    /// \param seed
    /// \return
    ///
    static space_item_type sample(uint_t seed);
	
	///
    /// \brief sample
    /// \param seed
    /// \return
    ///
    static std::vector<space_item_type> sample(uint_t seed, uint_t size);
};

template<uint_t SpaceSize>
typename DiscreteSpace<SpaceSize>::space_item_type
DiscreteSpace<SpaceSize>::sample(){

    std::uniform_int_distribution<> dist(0, SpaceSize - 1);
    std::random_device rd;
    std::mt19937 gen(rd());
    return dist(gen);
}

template<uint_t SpaceSize>
typename DiscreteSpace<SpaceSize>::space_item_type
DiscreteSpace<SpaceSize>::sample(uint_t seed){

    std::uniform_int_distribution<> dist(0, SpaceSize - 1);
    std::mt19937 gen(seed);
    return dist(gen);
}

template<uint_t SpaceSize>
std::vector<typename DiscreteSpace<SpaceSize>::space_item_type>
DiscreteSpace<SpaceSize>::sample(uint_t seed, uint_t size){

	std::vector<typename DiscreteSpace<SpaceSize>::space_item_type> vals_;
	vals_.reserve(size);
	
	std::uniform_int_distribution<> dist(0, SpaceSize - 1);
    std::mt19937 gen(seed);
	for(uint_t i=0; i<size; ++i){
		
		vals_.push_back(dist(gen));
	}
	
    return vals_;
}

template<uint_t SpaceSize, typename ItemType = std::vector<real_t>>
struct ContinuousSpace{
	
	///
    /// \brief item_t
    ///
    typedef std::vector<real_t> space_item_type;

    ///
    /// \brief size. The number of members in the space
    ///
    static constexpr uint_t size = SpaceSize;
};

///
/// \brief The ContinuousSpace class.
///
/*template<uint_t SpaceSize>
struct ContinuousSpace<SpaceSize, std::vector<real_t> >
{
    ///
    /// \brief item_t
    ///
    typedef std::vector<real_t> space_item_type;

    ///
    /// \brief size. The number of members in the space
    ///
    static constexpr uint_t size = SpaceSize;

};

*/

template<>
struct ContinuousSpace<1, Null>
{
	
	///
    /// \brief item_t
    ///
    typedef real_t space_item_type;

    ///
    /// \brief size. The number of members in the space
    ///
    static constexpr uint_t size = 1;

};


template<uint_t SpaceSize, typename T=uint_t>
struct DiscreteVectorSpace
{
	static_assert(std::is_integral_v<T> == true && "Integral type is expected");
	
	///
    /// \brief item_t
    ///
    typedef std::vector<T> space_item_type;
	
	
	///
    /// \brief size. The number of members in the space
    ///
    static constexpr uint_t size = SpaceSize;
};


template<uint_t StateSpaceSize, uint_t ActionSpaceSize> 
struct DiscreteEnv{
	
	///
    /// \brief item_t
    ///
    typedef DiscreteSpace<StateSpaceSize> state_space;
	
	///
	/// \brief the State type
	///
	typedef typename state_space::space_item_type state_type;
	
	///
    /// \brief state space size
    ///
    static constexpr uint_t STATE_SPACE_SIZE = StateSpaceSize;


    ///
    /// \brief the action space type
    ///
    typedef DiscreteSpace<ActionSpaceSize> action_space;
	
	///
	/// \brief the Action type
	///
	typedef typename action_space::space_item_type action_type;
	
	
	///
    /// \brief action space size
    ///
    static constexpr uint_t ACTION_SPACE_SIZE = ActionSpaceSize;
	
	
	///
    /// \brief sample
    /// \return
    ///
    static action_type sample_action(){return action_space::sample();}

    ///
    /// \brief sample
    /// \param seed
    /// \return
    ///
    static action_type sample_action(uint_t seed){return action_space::sample();}
};


template<uint_t StateSpaceSize, 
		 uint_t ActionSpaceSize,
		 typename StateSpaceItemType=std::vector<real_t>>
struct ContinousStateDiscreteActionEnv{
	
	///
    /// \brief the state space type
    ///
    typedef ContinuousSpace<StateSpaceSize, StateSpaceItemType> state_space;
	
	///
	/// \brief the State type
	///
	typedef typename state_space::space_item_type state_type;
	
	///
    /// \brief state space size
    ///
    static constexpr uint_t STATE_SPACE_SIZE = StateSpaceSize;
	
	///
    /// \brief the action space type
    ///
    typedef DiscreteSpace<ActionSpaceSize> action_space;
	
	///
	/// \brief the Action type
	///
	typedef typename action_space::space_item_type action_type;
	
	
	///
    /// \brief action space size
    ///
    static constexpr uint_t ACTION_SPACE_SIZE = ActionSpaceSize;
	
	
	///
    /// \brief sample
    /// \return
    ///
    static action_type sample_action(){return action_space::sample();}

    ///
    /// \brief sample
    /// \param seed
    /// \return
    ///
    static action_type sample_action(uint_t seed){return action_space::sample();}
	
};

template<uint_t StateSpaceSize, 
		 uint_t ActionSpaceSize,
		 typename ActionSpaceItemType=std::vector<real_t>>
struct DisceteStateContinuousActionEnv{
	
	///
    /// \brief the action space type
    ///
    typedef DiscreteSpace<StateSpaceSize> state_space;
	
	///
	/// \brief the Action type
	///
	typedef typename state_space::space_item_type state_type;
	
	///
    /// \brief state space size
    ///
    static constexpr uint_t STATE_SPACE_SIZE = StateSpaceSize;
	
	///
    /// \brief the state space type
    ///
    typedef ContinuousSpace<ActionSpaceSize, ActionSpaceItemType> action_space;
	
	///
	/// \brief the State type
	///
	typedef typename action_space::space_item_type action_type;
	
	///
    /// \brief state space size
    ///
    static constexpr uint_t ACTION_SPACE_SIZE = ActionSpaceSize;
	
};


template<uint_t StateSpaceSize, 
		 uint_t ActionSpaceSize,
		 typename StateSpaceItemType = std::vector<real_t>,
		 typename ActionSpaceItemType = std::vector<real_t> >
struct ContinousEnv{
	
	
	///
    /// \brief the state space type
    ///
    typedef ContinuousSpace<StateSpaceSize, StateSpaceItemType> state_space;
	
	///
	/// \brief the State type
	///
	typedef typename state_space::space_item_type state_type;
	
	///
    /// \brief state space size
    ///
    static constexpr uint_t STATE_SPACE_SIZE = StateSpaceSize;
	
	///
    /// \brief the state space type
    ///
    typedef ContinuousSpace<ActionSpaceSize, ActionSpaceItemType> action_space;
	
	///
	/// \brief the State type
	///
	typedef typename action_space::space_item_type action_type;
	
	///
    /// \brief state space size
    ///
    static constexpr uint_t ACTION_SPACE_SIZE = ActionSpaceSize;
	
};

template<uint_t StateSpaceSize, 
		 uint_t ActionSpaceSize,
		 typename StateSpaceItemType = uint_t>
struct DiscreteVectorStateDiscreteActionEnv
{
	///
    /// \brief the state space type
    ///
    typedef DiscreteVectorSpace<StateSpaceSize, StateSpaceItemType> state_space;
	
	///
	/// \brief the State type
	///
	typedef typename state_space::space_item_type state_type;
	
	///
    /// \brief state space size
    ///
    static constexpr uint_t STATE_SPACE_SIZE = StateSpaceSize;
	
	///
    /// \brief the action space type
    ///
    typedef DiscreteSpace<ActionSpaceSize> action_space;
	
	///
	/// \brief the Action type
	///
	typedef typename action_space::space_item_type action_type;
	
	///
    /// \brief state space size
    ///
    static constexpr uint_t ACTION_SPACE_SIZE = ActionSpaceSize;
};
	
}
}
#endif