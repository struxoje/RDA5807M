/*
 * CommandParser.cpp
 *
 *  Created on: Jun 30, 2017
 *      Author: Bennett Sherman
 */

//System includes
#include <iostream>
#include <regex>
#include <string>
#include <stdexcept>

// Project includes
#include "Command.hpp"
#include "CommandParser.hpp"
#include "TcpServer.hpp"
#include "RDA5807MWrapper.hpp"

// Static initialization
const Command<TcpServer> CommandParser::SERVER_CMDS[]
{
    Command<TcpServer> { "SVR_INFO", &TcpServer::getAddressInfoHandler, "Gets the server's networking information"},
    Command<TcpServer> { "CLIENTS_INFO", &TcpServer::getClientsInfoHandler, "Gets the IP and port #s of connected clients"}
};

const Command<RDA5807MWrapper> CommandParser::RADIO_CMDS[] {
    Command<RDA5807MWrapper> { "FREQ", &RDA5807MWrapper::setFrequency, "Enter freq in as an integer"},
    Command<RDA5807MWrapper> { "VOL", &RDA5807MWrapper::setVolume, "Enter volume from 0 to 15"},
    Command<RDA5807MWrapper> { "MUTE", &RDA5807MWrapper::setMute, "1 to mute, 0 to unmute"},
    Command<RDA5807MWrapper> { "BASSBOOST", &RDA5807MWrapper::setBassBoost, "1 to enable bass boost, 0 to disable"},
    Command<RDA5807MWrapper> { "ENABLE", &RDA5807MWrapper::setRadioEnableState, "1 to enable/reset radio, 0 to disable"},
    Command<RDA5807MWrapper> { "HIGHIMPEDANCEOUTPUT", &RDA5807MWrapper::setHighImpedanceOutput, "1 to enable HiZ output, 0 to disable. Unstable - avoid"},
    Command<RDA5807MWrapper> { "STEREO", &RDA5807MWrapper::setStereo, "1 for stereo, 0 for mono"},
    Command<RDA5807MWrapper> { "SEEK", &RDA5807MWrapper::setSeek, "1 to enable seeking, 0 to disable. If 1, other commands may cause seeking, so be sure to set back to 0"},
    Command<RDA5807MWrapper> { "RDS", &RDA5807MWrapper::setRDS, "1 to enable RDS, 0 to disable"},
    Command<RDA5807MWrapper> { "NEWMETHOD", &RDA5807MWrapper::setNewMethod, "1 to enable new demodulate method, 0 to disable. Strongly recommend using 1 permanently"},
    Command<RDA5807MWrapper> { "SOFTRESET", &RDA5807MWrapper::setSoftReset, "Setting to 1 causes a soft reset"},
    Command<RDA5807MWrapper> { "SOFTMUTE", &RDA5807MWrapper::setSoftMute, "1 to enable softmute, 0 to disable"},
    Command<RDA5807MWrapper> { "TUNE", &RDA5807MWrapper::setTune, "1 to tune. Resets to 0 automatically after tune complete"},
    Command<RDA5807MWrapper> { "AFCD", &RDA5807MWrapper::setAFCD, "1 toe enable automatic frequency control (AFC), 0 tod disable"},
    Command<RDA5807MWrapper> { "DEEMPHASIS", &RDA5807MWrapper::setDeEmphasis, "SEVENTY_FIVE_US = 0, FIFTY_US = 1"},
    Command<RDA5807MWrapper> { "BAND", &RDA5807MWrapper::setBand, "US_EUR = 0, JAP = 1, WORLD_WIDE = 2, EAST_EUROPE= 3"},
    Command<RDA5807MWrapper> { "CHANNELSPACING", &RDA5807MWrapper::setChannelSpacing, "E_HUND_KHZ = 0, TWO_HUND_KHZ = 1, FIFTY_KHZ = 2, TWENTY_FIVE_KHZ = 3"},
    Command<RDA5807MWrapper> { "SEEKDIR", &RDA5807MWrapper::setSeekDirection, "SEEK_UP = 0, SEEK_DOWN = 1"},
    Command<RDA5807MWrapper> { "SEEKMODE", &RDA5807MWrapper::setSeekMode, "WRAP_AT_LIMIT = 0, STOP_AT_LIMIT = 1"},
    Command<RDA5807MWrapper> { "SOFTBLEND", &RDA5807MWrapper::setSoftBlend, "1 to enable soft blend, 0 to disable" },
    Command<RDA5807MWrapper> { "UPDATELOCALREGS", &RDA5807MWrapper::updateLocalRegisterMapFromDevice, "No param. Updates local regmap with regs from device"},
    Command<RDA5807MWrapper> { "STATUS", &RDA5807MWrapper::getStatusString, "No param. Prints status info"},
    Command<RDA5807MWrapper> { "REGMAP", &RDA5807MWrapper::getRegisterMapString, "No param. Prints local register map"},
    Command<RDA5807MWrapper> { "FREQMAP" , &RDA5807MWrapper::generateFreqMap, "Prints dotplot of freqs and their RSSI. No param for short search. Param=1 shows RDS support (takes a long time)"},
    Command<RDA5807MWrapper> { "RDSINFO" , &RDA5807MWrapper::getRdsInfoString, "No param. Prints RDS information"},
    Command<RDA5807MWrapper> { "GETREGFROMLOCALMAP", &RDA5807MWrapper::getLocalCopyOfReg, "Returns the local copy of the register addressed by the param (in hex)"},
    Command<RDA5807MWrapper> { "SNOOPRDSGROUP2", &RDA5807MWrapper::snoopRdsGroupTwo, "Snoops RDS group 2 for param (in ms) milliseconds at 10ms intervals"},
    Command<RDA5807MWrapper> { "RSSI", &RDA5807MWrapper::getRssi, "No param. Prints the RSSI"},
    Command<RDA5807MWrapper> { "RDSPI", &RDA5807MWrapper::getRdsPiCode, "No param. Prints RDS program identification code"},
    Command<RDA5807MWrapper> { "RDSGROUPTYPE", &RDA5807MWrapper::getRdsGroupTypeCode, "No param. Prints RDS group type"},
    Command<RDA5807MWrapper> { "RDSVERSION", &RDA5807MWrapper::getRdsVersionCode, "No param. Prints RDS version code"},
    Command<RDA5807MWrapper> { "RDSTRAFPROGRAMID", &RDA5807MWrapper::getRdsTrafficProgramIdCode, "No param. Prints RDS traffic ID code"},
    Command<RDA5807MWrapper> { "RDSPROGRAMTYPE", &RDA5807MWrapper::getRdsProgramTypeCode, "No param. Prints RDS program type code"}
};

const size_t CommandParser::SERVER_CMDS_LIST_LENGTH = sizeof(SERVER_CMDS) / sizeof(Command<TcpServer>);
const size_t CommandParser::RADIO_CMDS_LIST_LENGTH = sizeof(RADIO_CMDS)/sizeof(Command<RDA5807MWrapper>);


const std::string CommandParser::FUNCTION_AND_PARAM_SEPARATOR = "=";
const std::regex CommandParser::CMD_REGEX { "^([A-Z0-9_]+)" +
                                FUNCTION_AND_PARAM_SEPARATOR +
                                "?([-]?[0-9a-zA-Z:]*\\.?[0-9a-zA-Z:]*)"};
const std::string CommandParser::LIST_CMDS_COMMAND_STRING {"HELP"};
const std::string CommandParser::INVALID_SYNTAX_STRING {"~INVALID COMMAND SYNTAX"};
const std::string CommandParser::NO_SUCH_COMMAND_EXISTS_STRING {"~NO SUCH COMMAND EXISTS"};
const std::string CommandParser::EXEC_ERROR_BASE_STRING {"~ERROR"};
const std::string CommandParser::EXECUTION_OK_STRING {"~OK"};
const std::string CommandParser::UNUSED_PARAM_VALUE = "0";

CommandParser::CommandParser(const TcpServerSharedPtr& tcpServer, const RDA5807MWrapperSharedPtr& radioWrapper):
    tcpServer(tcpServer), radioWrapper(radioWrapper)
{

}

/**
 * The command specified by LIST_CMDS_COMMAND_STRING will result in a list of supported
 * functions being printed. If the command was invalid or there was a parsing error, a
 * string reporting the error will be returned. If the parse operation was successful,
 * the result of the execution will be returned.
 *
 * TODO: Throw exceptions for bad commands
 */
std::string CommandParser::execute(const std::string& unparsedCommand)
{
    std::string cmd;
    std::string param;

    bool parseResult = parse(unparsedCommand, cmd, param);
    if (!parseResult)
    {
        return INVALID_SYNTAX_STRING;
    }

    if (cmd.compare(LIST_CMDS_COMMAND_STRING) == 0)
    {
        return EXECUTION_OK_STRING + getCommandStringList();
    }

    // Attempt to find and execute the specified command
    try
    {
        std::string funcUpdatableString = EXECUTION_OK_STRING;

        // Radio commands
        for (size_t idx = 0; idx < RADIO_CMDS_LIST_LENGTH; ++idx)
        {
            Command<RDA5807MWrapper>radioCmd = RADIO_CMDS[idx];
            if (cmd.compare(radioCmd.getCommandString()) == 0)
            {
                std::cout << "Executing: " << cmd << "(" << param << ")" << std::endl;
                radioCmd.exec(param, &funcUpdatableString, *radioWrapper);
                return funcUpdatableString;
            }
        }

        // Server commands
        for (size_t idx = 0; idx < SERVER_CMDS_LIST_LENGTH; ++idx)
        {
            Command<TcpServer>serverCmd = SERVER_CMDS[idx];
            if (cmd.compare(serverCmd.getCommandString()) == 0)
            {
                std::cout << "Executing: " << cmd << "(" << param << ")" << std::endl;
                serverCmd.exec(param, &funcUpdatableString, *tcpServer);
                return funcUpdatableString;
            }
        }
    }
    catch (const std::exception& err)
    {
        return EXEC_ERROR_BASE_STRING + "; " + err.what();
    }

    return NO_SUCH_COMMAND_EXISTS_STRING;
}

/**
 * Returns a list of commands supported by this interpreter.
 */
std::string CommandParser::getCommandStringList() const
{
    std::string cmdList = "\nSUPPORTED COMMANDS:\n";

    cmdList.append("RADIO COMMANDS: \n");
    for (size_t idx = 0; idx < RADIO_CMDS_LIST_LENGTH; ++idx)
    {
        cmdList.append(RADIO_CMDS[idx].getCommandString());
        cmdList.append(" - ");
        cmdList.append(RADIO_CMDS[idx].getCommandDescription());
        cmdList.append("\n");
    }

    cmdList.append("\nSERVER COMMANDS: \n");
    for (size_t idx = 0; idx < SERVER_CMDS_LIST_LENGTH; ++idx)
    {
        cmdList.append(SERVER_CMDS[idx].getCommandString());
        cmdList.append(" - ");
        cmdList.append(SERVER_CMDS[idx].getCommandDescription());
        cmdList.append("\n");
    }

    return cmdList;
}

/**
 * Commands with params are of the form:
 * <COMMAND>=<VAL>, where <VAL> is a positive or negative number
 * (decimal allowed), or is a string.
 *
 * Commands with no params are of the form:
 * <COMMAND>
 * and " " is used as the value
 *
 * Returns true if a valid command is found, false otherwise
 */
bool CommandParser::parse(const std::string& unparsedCommand, std::string& command, std::string& param) const
{
    // Find the command and value if they exist
    std::smatch matches;

    std::regex_search(unparsedCommand, matches, CMD_REGEX);

    if (matches.size() != 3)
    {
        return false;
    }

    // matches[0] should be the whole string, minus any trailing whitespace
    if (unparsedCommand.substr(0, unparsedCommand.find_first_of("\r\n")).
            compare(matches[0].str()) != 0)
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
        param = matches[2].str();
    }

    return true;
}


