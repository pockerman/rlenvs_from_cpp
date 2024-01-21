// SPDX-FileCopyrightText: 2024 <copyright holder> <email>
// SPDX-License-Identifier: Apache-2.0

#ifndef APISERVER_H
#define APISERVER_H


#include <string>
#include <vector>

/**
 * @todo write docs
 */


namespace rlenvs_cpp{

class ApiServerWrapper
{
public:

    ///
    /// \brief Constructor
    ///
    ApiServerWrapper(const std::string& url);

    ///
    ///
    ///
    bool has_gymnasium()const;

    ///
    ///
    ///
    std::vector<std::string> gymnasium_envs()const;

private:

    const std::string url_;


};

}
#endif // APISERVER_H
