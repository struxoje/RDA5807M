/*
 * CommandParser.hpp
 *
 *  Created on: Jun 30, 2017
 *      Author: Bennett Sherman
 */

#ifndef COMMAND_COMMANDPARSER_HPP_
#define COMMAND_COMMANDPARSER_HPP_

// System includes
#include <cstdint>
#include <regex>
#include <string>
#include <memory>
#include <unordered_map>

// Project includes
#include "Command.hpp"
#include "RDA5807MWrapper.hpp"

// Forward declarations
class TcpServer;
using TcpServerSharedPtr = std::shared_ptr<TcpServer>;
using RDA5807MWrapperSharedPtr = std::shared_ptr<RDA5807MWrapper>;

class CommandParser
{
public:
    ////////////////////////////////
    // Public interface functions //
    ////////////////////////////////
    std::string execute(const std::string& unparsedCommand);

    CommandParser(const TcpServerSharedPtr& tcpServerPtr, const RDA5807MWrapperSharedPtr& radioWrapper);

private:
    const TcpServerSharedPtr tcpServer;
    const RDA5807MWrapperSharedPtr radioWrapper;

    bool parse(const std::string& unparsedCommand, std::string& command, std::string& param) const;
    std::string getCommandStringList() const;

    /////////////////////////////
    // Private class Constants //
    /////////////////////////////
    static const std::unordered_map<std::string, Command<TcpServer>> SERVER_CMDS;
    static const std::unordered_map<std::string, Command<RDA5807MWrapper>> RADIO_CMDS;

    // The regex string used to parse commands
    static const std::regex CMD_REGEX;

    // The character used to separate functions from parameters
    static const std::string FUNCTION_AND_PARAM_SEPARATOR;

    // When no parameter is specified for a command, this value is used
    static const std::string UNUSED_PARAM_VALUE;

    // When this command is entered, a list of commands is returned
    static const std::string LIST_CMDS_COMMAND_STRING;

    // Constant strings representing different parse/execution results
    static const std::string NO_SUCH_COMMAND_EXISTS_STRING;
    static const std::string EXEC_ERROR_BASE_STRING;
    static const std::string EXECUTION_OK_STRING;
    static const std::string INVALID_SYNTAX_STRING;

};

#endif /* COMMAND_COMMANDPARSER_HPP_ */
