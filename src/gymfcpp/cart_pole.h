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
    /// \brief action_t
    ///
    typedef uint_t action_t;

    ///
    /// \brief state_t
    ///
    typedef std::vector<real_t> state_t;

    ///
    /// \brief time_step_t
    ///
    typedef TimeStep<state_t> time_step_t;

    ///
    /// \brief name
    ///
    static  std::string name;

    ///
    /// \brief py_env_name
    ///
    static std::string py_env_name;

    ///
    /// \brief py_step_result_name
    ///
    static std::string py_step_result_name;

    ///
    /// \brief py_step_reset_result_name
    ///
    static std::string py_reset_result_name;

    ///
    /// \brief The class Screen
    ///
    class Screen;

    ///
    /// \brief CartPole. Constructor
    ///
    CartPole(const std::string& version, obj_t gym_namespace, bool do_create=true);

    ///
    /// \brief make
    /// \param is_slipery
    ///
    void make();

    ///
    /// \brief n_actions
    /// \return
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
    /// \breif shape
    ///
    std::array<uint_t, 3> shape()const noexcept{return shape_;};

    ///
    /// \brief
    ///
    const std::vector<std::vector<std::vector<real_t>>>& get_as_vector()const;
    
    ///
    ///
    ///
    bool is_valid()const noexcept{return is_valid_screen_;}
    
    ///
    ///
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
