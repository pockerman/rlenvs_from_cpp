#ifndef CART_POLE_H
#define CART_POLE_H

#include "gymfcpp/gymfcpp_types.h"
#include "gymfcpp/data_wrapper.h"


#include <boost/noncopyable.hpp>

#include <string>
#include <vector>
#include <array>
#include <tuple>

namespace gymfcpp{

/// Forward declaration
template<typename StateTp> class TimeStep;

///
/// \brief The CartPole class Interface for CartPole environment
///
class CartPole: private boost::noncopyable
{

public:

    ///
    /// \brief action_t. The type of the action
    ///
    typedef uint_t action_t;

    ///
    /// \brief state_t. The type of the state
    ///
    typedef std::vector<real_t> state_t;

    ///
    /// \brief time_step_t. The type of the time step
    ///
    typedef TimeStep<state_t> time_step_t;

    ///
    /// \brief name
    ///
    static  std::string name;

    ///
    /// \brief py_env_name. The name of the environment in Python REPL
    ///
    static std::string py_env_name;

    ///
    /// \brief py_step_result_name. The name of the result when stepping in the Python REPL
    ///
    static std::string py_step_result_name;

    ///
    /// \brief py_step_reset_result_name
    ///
    static std::string py_reset_result_name;

    ///
    /// \brief The class Screen. Wrapper to the screen captured when calling render
    ///
    class Screen;

    ///
    /// \brief CartPole. Constructor
    ///
    CartPole(const std::string& version, obj_t gym_namespace, bool do_create=true);

    ///
    /// \brief make. Build the environment
    ///
    void make();

    ///
    /// \brief n_actions. Returns the number of actions
    ///
    uint_t n_actions()const;

    ///
    /// \brief version
    ///
    std::string_view version()const noexcept{return data_.v;}

    ///
    /// \brief is_created
    /// \return
    ///
    bool is_created()const noexcept{return data_.is_created;}

    ///
    /// \brief reset
    /// \return
    ///
    time_step_t reset();

    ///
    /// \brief step
    ///
    time_step_t step(action_t action);

    ///
    /// \brief the screen
    ///
    Screen get_screen()const;

    ///
    /// \brief render
    ///
    void render();

private:


    ///
    /// \brief data_
    ///
    detail::data_wrapper<time_step_t> data_;

    /// cached data

    ///
    /// \brief number of actions
    ///
    mutable uint_t n_actions_;

};


class CartPole::Screen
{
public:

    typedef std::vector<std::vector<std::vector<real_t>>> screen_vector_t;

    ///
    ///
    ///
    Screen()=default;

    ///
    /// \brief Screen
    ///
    Screen(obj_t screen, std::array<uint_t, 3>&& shp);

    ///
    /// \brief shape. The shape of screen.
    ///
    std::array<uint_t, 3> shape()const noexcept{return shape_;};

    ///
    /// \brief get_as_vector. Returns the screen as floats
    ///
    const std::vector<std::vector<std::vector<real_t>>>& get_as_vector()const;
    
    ///
    /// \brief is_valid. Returns true if the computed screen is valid
    ///
    bool is_valid()const noexcept{return is_valid_screen_;}
    
    ///
    /// \brief invalidate. Invalidate the screen.
    ///
    void invalidate() noexcept;

private:

    obj_t screen_;
    std::array<uint_t, 3> shape_;
    bool is_valid_screen_{false};
    mutable std::vector<std::vector<std::vector<real_t>>> screen_vec_;

};

inline
bool operator==(const CartPole::Screen& screen1, const CartPole::Screen& screen2){
	auto shape_1 = screen1.shape();
	auto shape_2 = screen2.shape();
	return (shape_1[0] == shape_2[0]) &&  (shape_1[1] == shape_2[1]) &&  (shape_1[2] == shape_2[2]);
}

inline
bool operator!=(const CartPole::Screen& screen1, const CartPole::Screen& screen2){
	return !(screen1 == screen2); 
}


inline
CartPole::Screen operator-(const CartPole::Screen& screen1, const CartPole::Screen& screen2){
}


}

#endif // CART_POLE_H
