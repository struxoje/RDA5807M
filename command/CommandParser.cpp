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
const Command<RDA5807M::StatusResult> CommandParser::STATUS_RESULT_COMMANDS[] =
{
    Command<RDA5807M::StatusResult> { "FREQ", &RDA5807MWrapper::setFrequency },
    Command<RDA5807M::StatusResult> { "VOL", &RDA5807MWrapper::setVolume },
    Command<RDA5807M::StatusResult> { "MUTE", &RDA5807MWrapper::setMute },
    Command<RDA5807M::StatusResult> { "BASSBOOST", &RDA5807MWrapper::setBassBoost },
    Command<RDA5807M::StatusResult> { "ENABLE", &RDA5807MWrapper::setRadioEnableState },
    Command<RDA5807M::StatusResult> { "HIGHIMPEDANCEOUTPUT", &RDA5807MWrapper::setHighImpedanceOutput },
    Command<RDA5807M::StatusResult> { "STEREO", &RDA5807MWrapper::setStereo },
    Command<RDA5807M::StatusResult> { "SEEK", &RDA5807MWrapper::setSeek },
    Command<RDA5807M::StatusResult> { "RDS", &RDA5807MWrapper::setRDS },
    Command<RDA5807M::StatusResult> { "NEWMETHOD", &RDA5807MWrapper::setNewMethod },
    Command<RDA5807M::StatusResult> { "SOFTRESET", &RDA5807MWrapper::setSoftReset },
    Command<RDA5807M::StatusResult> { "SOFTMUTE", &RDA5807MWrapper::setSoftMute },
};

const Command<std::string> CommandParser::STRING_RESULT_COMMANDS[] =
{
    Command<std::string> { "STATUS", &RDA5807MWrapper::getStatusString },
    Command<std::string> { "REGMAP", &RDA5807MWrapper::getRegisterMapString }
};

const std::regex CommandParser::CMD_REGEX { "^([A-Z]+){1}=*([0-9]*)"};

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

    for (size_t idx = 0; idx < sizeof(STATUS_RESULT_COMMANDS) / sizeof(Command<RDA5807M::StatusResult> ); ++idx)
    {
        Command<RDA5807M::StatusResult> statusResultCmd = STATUS_RESULT_COMMANDS[idx];
        if (cmd.compare(statusResultCmd.getCommand()) == 0)
        {
            std::cout << "Executing: " << cmd << "; Param: " << param << std::endl;
            return RDA5807M::statusResultToString(statusResultCmd.exec(param, radioWrapper).getResult());
        }
    }

    for (size_t idx = 0; idx < sizeof(STRING_RESULT_COMMANDS) / sizeof(Command<std::string> ); ++idx)
    {
        Command<std::string> stringResultCmd = STRING_RESULT_COMMANDS[idx];
        if (cmd.compare(stringResultCmd.getCommand()) == 0)
        {
            std::cout << "Executing: " << cmd << "; Param: " << param << std::endl;
            return stringResultCmd.exec(param, radioWrapper).getResult();
        }
    }

    return "COMMAND NOT FOUND!";
}

/**
 * Commands with params are of the form:
 * <COMMAND>=<VAL>
 *
 * Commands with no params are of the form:
 * <COMMAND>
 * and -1 is used as the value
 *
 * Returns true if a valid command is found, false otherwise
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

    // if matches[2] is not present, it means that this is a no-param command
    if (matches[2].length() == 0)
    {
        param = UNUSED_PARAM_VALUE;
    }
    else
    {
        param = std::stoi(matches[2].str());
    }

    return true;
}
