/*
 * Command.hpp
 *
 *  Created on: Jun 30, 2017
 *      Author: bensherman
 */

#ifndef COMMAND_COMMAND_HPP_
#define COMMAND_COMMAND_HPP_

// Stdlib includes
#include <cstring>

// Project includes
#include "RadioResult.hpp"
#include "RDA5807M.hpp"
#include "RDA5807MWrapper.hpp"

template<typename T>
class Command
{
public:

    // Alias declarations to the rescue!
    using WrapperFunction = RadioResult<T> (RDA5807MWrapper::*)(int);

    ////////////////////////////////
    // Public interface functions //
    ////////////////////////////////
    Command(std::string commandParam, WrapperFunction cmdFuncParam) :
            command(commandParam), cmdFunc(cmdFuncParam) {};

    RadioResult<T> exec(int param, RDA5807MWrapper& wrapRef) const
    {
        return (wrapRef.*cmdFunc)(param);
    }

    std::string getCommand() const
    {
        return command;
    }

private:
    //////////////////////////////
    // Private member variables //
    //////////////////////////////
    const std::string command;
    const WrapperFunction cmdFunc;

};

#endif /* COMMAND_COMMAND_HPP_ */
