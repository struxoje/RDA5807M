/*
 * CommandParser.cpp
 *
 *  Created on: Jun 30, 2017
 *      Author: bensherman
 */

//System includes
#include <iostream>
#include <regex>
#include <string>
#include <utility>

// Project includes
#include "CommandParser.hpp"
#include "Command.hpp"
#include "RDA5807M.hpp"
#include "RDA5807MWrapper.hpp"
#include "RadioResult.hpp"

// Static initialization
const Command<RDA5807MWrapper::StatusResult> CommandParser::statusResultCommands[] =
	{
		Command<RDA5807MWrapper::StatusResult>{"FREQ", &RDA5807MWrapper::setFrequency},
		Command<RDA5807MWrapper::StatusResult>{"VOL", &RDA5807MWrapper::setVolume},
		Command<RDA5807MWrapper::StatusResult>{"STATUS", &RDA5807MWrapper::printStatus},
	};
const std::regex CommandParser::CMD_REGEX{"^([A-Z]+){1}=([0-9]+)"};

std::string CommandParser::execute(std::string& unparsedCommand)
{
	std::string cmd;
	int param;

	bool parseResult = parse(unparsedCommand, cmd, param);
	if (parseResult)
	{
		std::cout << "Parsed command: " << cmd << std::endl;
		std::cout << "Parsed value: " << param << std::endl;

	}

	for (size_t idx = 0; idx < sizeof(statusResultCommands)/sizeof(Command<RDA5807MWrapper::StatusResult>); ++idx)
	{
		Command<RDA5807MWrapper::StatusResult> statusResultCmd = statusResultCommands[idx];
		if (cmd.compare(statusResultCmd.getCommand()) == 0)
		{
			return RDA5807MWrapper::resultToString(statusResultCmd.exec(param, radioWrapper).getResult());
		}
	}
	return "";
}

/**
 * Commands with params are of the form:
 * <COMMAND>=<VAL>
 *
 * Commands with no params are of the form:
 * <COMMAND>=<any number>
 *
 */
bool CommandParser::parse(const std::string& unparsedCommand, std::string& command, int& param)
{
	// Find the command and value if they exist
	std::smatch matches;

	std::regex_match(unparsedCommand, matches, CMD_REGEX);
	if (matches.size() != 3)
	{
		return false;
	}

	// matches[0] should be the whole string
	if (unparsedCommand.compare(matches[0].str()) != 0)
	{
		return false;
	}

	command = matches[1].str();
	param = std::stoi(matches[2].str());

	return true;
}
