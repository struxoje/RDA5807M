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
#include "RDA5807M.hpp"

class Command
{
public:
    ////////////////////////////////
    // Public interface functions //
    ////////////////////////////////
	Command(std::string command, void (RDA5807M::* func)());

	void exec(RDA5807M& radio, T funcParam);

private:
    //////////////////////////////
    // Private member variables //
    //////////////////////////////
	std::string command;
	void (RDA5807M::* func)(T);

};

#endif /* COMMAND_COMMAND_HPP_ */
