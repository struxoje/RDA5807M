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
    Command(std::string commandStringParam, WrapperFunction cmdFuncParam) :
            commandString(commandStringParam), cmdFunc(cmdFuncParam) {};

    /**
     * Executes the function pointed to by cmdFunc with the parameter param.
     * wrapRef is used as the object on which cmdFunc is called.
     * The result of the function is returned
     */
    RadioResult<T> exec(int param, RDA5807MWrapper& wrapRef) const
    {
        return (wrapRef.*cmdFunc)(param);
    }

    std::string getCommandString() const
    {
        return commandString;
    }

private:
    //////////////////////////////
    // Private member variables //
    //////////////////////////////
    const std::string commandString;
    const WrapperFunction cmdFunc;

};

#endif /* COMMAND_COMMAND_HPP_ */
