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

    RadioResult<T> exec(int param, RDA5807MWrapper& wrapRef)
    {
        return (wrapRef.*cmdFunc)(param);
    }

    std::string getCommand()
    {
        return command;
    }

private:
    //////////////////////////////
    // Private member variables //
    //////////////////////////////
    std::string command;
    WrapperFunction cmdFunc;

};

#endif /* COMMAND_COMMAND_HPP_ */
