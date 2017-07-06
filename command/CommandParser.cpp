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
    Command<RDA5807M::StatusResult> { "TUNE", &RDA5807MWrapper::setTune },
    Command<RDA5807M::StatusResult> { "AFCD", &RDA5807MWrapper::setAFCD },
    Command<RDA5807M::StatusResult> { "DEEMPHASIS", &RDA5807MWrapper::setDeEmphasis },
    Command<RDA5807M::StatusResult> { "BAND", &RDA5807MWrapper::setBand },
    Command<RDA5807M::StatusResult> { "CHANNELSPACING", &RDA5807MWrapper::setChannelSpacing },
    Command<RDA5807M::StatusResult> { "SEEKDIR", &RDA5807MWrapper::setSeekDirection },
    Command<RDA5807M::StatusResult> { "SEEKMODE", &RDA5807MWrapper::setSeekMode },
    Command<RDA5807M::StatusResult> { "SOFTBLEND", &RDA5807MWrapper::setSoftBlend }
};

const Command<std::string> CommandParser::STRING_RESULT_COMMANDS[] =
{
    Command<std::string> { "STATUS", &RDA5807MWrapper::getStatusString },
    Command<std::string> { "REGMAP", &RDA5807MWrapper::getRegisterMapString },
    Command<std::string> { "FREQMAP" , &RDA5807MWrapper::generateFreqMap }
};

const Command<uint32_t> CommandParser::UINT32_RESULT_COMMANDS[] =
{
    Command<uint32_t> { "RSSI", &RDA5807MWrapper::getRssi }
};

const size_t CommandParser::STATUS_RESULT_COMMANDS_LIST_LENGTH = sizeof(STATUS_RESULT_COMMANDS) / sizeof(Command<RDA5807M::StatusResult>);
const size_t CommandParser::STRING_RESULT_COMMANDS_LIST_LENGTH = sizeof(STRING_RESULT_COMMANDS) / sizeof(Command<std::string>);
const size_t CommandParser::UINT32_RESULT_COMMANDS_LIST_LENGTH = sizeof(UINT32_RESULT_COMMANDS) / sizeof(Command<uint32_t>);

const std::regex CommandParser::CMD_REGEX { "^([a-zA-Z]+){1}=*([0-9]*)"};
const std::string CommandParser::LIST_CMDS_COMMAND_STRING = "HELP";

/**
 * The command specified by LIST_CMDS_COMMAND_STRING will result in a list of supported
 * functions being printed. If the entered command is invalid, "COMMAND NOT VALID!" is returned.
 * Otherwise, the result of the execution is returned.
 */
std::string CommandParser::execute(std::string& unparsedCommand)
{
    std::string cmd;
    int param;

    bool parseResult = parse(unparsedCommand, cmd, param);
    if (!parseResult)
    {
        return "COMMAND NOT VALID!";
    }

    if (cmd.compare(LIST_CMDS_COMMAND_STRING) == 0)
    {
        return getCommandStringList();
    }

    for (size_t idx = 0; idx < STATUS_RESULT_COMMANDS_LIST_LENGTH; ++idx)
    {
        Command<RDA5807M::StatusResult> statusResultCmd = STATUS_RESULT_COMMANDS[idx];
        if (cmd.compare(statusResultCmd.getCommandString()) == 0)
        {
            std::cout << "Executing: " << cmd << "(" << param << ")" << std::endl;
            return RDA5807M::statusResultToString(statusResultCmd.exec(param, radioWrapper).getResult());
        }
    }

    for (size_t idx = 0; idx < STRING_RESULT_COMMANDS_LIST_LENGTH; ++idx)
    {
        Command<std::string> stringResultCmd = STRING_RESULT_COMMANDS[idx];
        if (cmd.compare(stringResultCmd.getCommandString()) == 0)
        {
            std::cout << "Executing: " << cmd << "(" << param << ")" << std::endl;
            return stringResultCmd.exec(param, radioWrapper).getResult();
        }
    }

    for (size_t idx = 0; idx < UINT32_RESULT_COMMANDS_LIST_LENGTH; ++idx)
    {
        Command<uint32_t> uintResultCmd = UINT32_RESULT_COMMANDS[idx];
        if (cmd.compare(uintResultCmd.getCommandString()) == 0)
        {
            std::cout << "Executing: " << cmd << "(" << param << ")" << std::endl;
            return std::to_string(uintResultCmd.exec(param, radioWrapper).getResult());
        }
    }

    return "COMMAND NOT VALID!";
}

/**
 * Returns a list of commands supported by this interpreter.
 */
std::string CommandParser::getCommandStringList()
{
    std::string cmdList = "SUPPORTED COMMANDS:\n";

    cmdList.append("\nRETURN STATUSES: \n");
    for (size_t idx = 0; idx < STATUS_RESULT_COMMANDS_LIST_LENGTH; ++idx)
    {
        cmdList.append(STATUS_RESULT_COMMANDS[idx].getCommandString());
        cmdList.append("\n");
    }

    cmdList.append("\nRETURN STRINGS: \n");
    for (size_t idx = 0; idx < STRING_RESULT_COMMANDS_LIST_LENGTH; ++idx)
    {
        cmdList.append(STRING_RESULT_COMMANDS[idx].getCommandString());
        cmdList.append("\n");
    }

    cmdList.append("\nRETURN UINT32: \n");
    for (size_t idx = 0; idx < UINT32_RESULT_COMMANDS_LIST_LENGTH; ++idx)
    {
        cmdList.append(UINT32_RESULT_COMMANDS[idx].getCommandString());
        cmdList.append("\n");
    }

    return cmdList;
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
