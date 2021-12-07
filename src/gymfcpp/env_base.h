#ifndef ENV_BASE_H
#define ENV_BASE_H

#include "gymfcpp/gymfcpp_types.h"

#include <boost/noncopyable.hpp>
#include <string>

namespace gymfcpp
{

///
/// \brief The EnvBase class
///
class EnvBase: private boost::noncopyable
{
public:

    ///
    /// \brief ~EnvBase
    ///
    virtual ~EnvBase();

    ///
    /// \brief render
    ///
    virtual void render(std::string mode="human");

    ///
    /// \brief close. Close down the python environment
    ///
    virtual void close();

    ///
    /// \brief version
    /// \return
    ///
    std::string_view version()const noexcept{return version_;}

    ///
    /// \brief name
    /// \return
    ///
    std::string_view name()const noexcept{return name_;}

    ///
    /// \brief py_env_name
    /// \return
    ///
    std::string_view py_env_name()const noexcept{return py_env_name_;}

    ///
    /// \brief is_created
    /// \return
    ///
    bool is_created()const noexcept{return is_created_;}


protected:

    ///
    /// \brief EnvBase
    /// \param py_env_name
    /// \param gym_namespace
    /// \param version
    ///
    EnvBase(std::string name, std::string version, std::string py_env_name, obj_t gym_namespace);

    ///
    /// \brief make_created
    ///
    void make_created()noexcept{is_created_ = true;}


private:

    ///
    /// \brief name_
    ///
    std::string name_;

    ///
    /// \brief version_
    ///
    std::string version_;

    ///
    /// \brief py_env_name_
    ///
    std::string py_env_name_;

    ///
    /// \brief gym_namespace_
    ///
    obj_t gym_namespace_;

    ///
    /// \brief is_created_
    ///
    bool is_created_;

};


}

#endif // ENV_BASE_H
