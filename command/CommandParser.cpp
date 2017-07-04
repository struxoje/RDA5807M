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
#include "Command.hpp"
#include "CommandParser.hpp"
#include "RadioResult.hpp"
#include "RDA5807M.hpp"
#include "RDA5807MWrapper.hpp"

// Static initialization
const Command<RDA5807M::StatusResult> CommandParser::statusResultCommands[] =
    {
        Command<RDA5807M::StatusResult> { "FREQ", &RDA5807MWrapper::setFrequency },
        Command<RDA5807M::StatusResult> { "VOL", &RDA5807MWrapper::setVolume },
        Command<RDA5807M::StatusResult> { "STATUS", &RDA5807MWrapper::printStatus },
        Command<RDA5807M::StatusResult> { "MUTE", &RDA5807MWrapper::setMute },
        Command<RDA5807M::StatusResult> { "BASSBOOST", &RDA5807MWrapper::setBassBoost }
    };
const std::regex CommandParser::CMD_REGEX { "^([A-Z]+){1}=([0-9]+)"};

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

    for (size_t idx = 0; idx < sizeof(statusResultCommands) / sizeof(Command<RDA5807M::StatusResult> ); ++idx)
    {
        Command<RDA5807M::StatusResult> statusResultCmd = statusResultCommands[idx];
        if (cmd.compare(statusResultCmd.getCommand()) == 0)
        {
            std::cout << "Executing: " << cmd << "; Param: " << param << std::endl;
            return RDA5807M::statusResultToString(statusResultCmd.exec(param, radioWrapper).getResult());
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
