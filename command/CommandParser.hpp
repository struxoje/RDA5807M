/*
 * CommandParser.hpp
 *
 *  Created on: Jun 30, 2017
 *      Author: bensherman
 */

#ifndef COMMAND_COMMANDPARSER_HPP_
#define COMMAND_COMMANDPARSER_HPP_

// System includes
#include <cstdint>
#include <vector>

// Project includes
#include "RDA5807M.hpp"
#include "Command.hpp"

class CommandParser
{
public:
    ////////////////////////////////
    // Public interface functions //
    ////////////////////////////////


private:
    /////////////////////////////
    // Private class Constants //
    /////////////////////////////

    /////////////////////////////////
    // Private interface functions //
    /////////////////////////////////

    //////////////////////////////
    // Private member variables //
    //////////////////////////////
	std::vector<Command> commands;
};


#endif /* COMMAND_COMMANDPARSER_HPP_ */
