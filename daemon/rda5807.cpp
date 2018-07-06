/*
 * rda5807.cpp
 *
 *  Created on: Sep 2, 2017
 *      Author: Bennett Sherman
 */

// System Includes
#include <string>
#include <iostream>
#include <csignal>
#include <string.h>
#include <stdexcept>

// Project Includes
#include "ArgParser.hpp"
#include "CommandParser.hpp"
#include "TcpServer.hpp"
#include "RDA5807MWrapper.hpp"

static TcpServerSharedPtr tcpServerPtr = nullptr;
static RDA5807MWrapperSharedPtr radioWrapperSharedPtr = nullptr;

void terminationSignalHandler(int sigNum)
{
    std::cout << "Received signal: " << strsignal(sigNum) << "(#" << sigNum <<
              ")" << std::endl;

    // If exceptions aren't caught when killing the server, std::terminate()
    // will get called recursively.
    if (tcpServerPtr != nullptr)
    {
        try
        {
           tcpServerPtr->terminate();
        }
        // We expect an invalid_argument to be thrown when calling getInstance() in
        // the event that the server wasn't instantiated prior. We want to ignore
        // such errors.
        catch (std::invalid_argument& invalidArgErr)
        {
        }
        catch (std::exception& genErr)
        {
            std::cerr << "Caught exception when trying to kill TcpServer; Message: "
                      << genErr.what() << std::endl;
        }
    }

    if (radioWrapperSharedPtr != nullptr) {
        RDA5807M& radio = radioWrapperSharedPtr->getRadio();
        radio.setVolume(0x00);
        radio.writeRegisterToDevice(RDA5807M::Register::REG_0x05);

        radio.setEnabled(false);
        radio.writeRegisterToDevice(RDA5807M::Register::REG_0x02);
    }

    std::cout << "Exiting!" << std::endl;
    exit(EXIT_SUCCESS);
}

int main(int argc, const char *argv[])
{
    // Execute terminationSignalHandler() in the event that any of the signals
    // below are caught
    signal(SIGTERM, terminationSignalHandler);
    signal(SIGABRT, terminationSignalHandler);
    signal(SIGINT, terminationSignalHandler);
    signal(SIGKILL, terminationSignalHandler);

    // Parse and apply the command line arguments
    ArgParser argParser{};
    argParser.parse(argc, argv);

    // Have the server wait for, accept, and process connections
    tcpServerPtr = argParser.buildTcpServer();
    radioWrapperSharedPtr = argParser.buildRadioWrapper();
    CommandParser cmdParser(tcpServerPtr, radioWrapperSharedPtr);

    tcpServerPtr->run(cmdParser);
}
