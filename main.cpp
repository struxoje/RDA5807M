/**************************************************
 * main.cpp
 * Author: Ben Sherman
 *************************************************/

// Avoid enabling hiz mode. Both cause RSSI to bug out
// System includes
#include <iostream>
#include <signal.h>
#include <unistd.h>

// Project includes
#include "CommandParser.hpp"
#include "RDA5807M.hpp"
#include "RDA5807MWrapper.hpp"

RDA5807M radio;

void die(int unused)
{
    radio.setVolume(0x00);
    radio.writeRegisterToDevice(RDA5807M::Register::REG_0x05);

    radio.setEnabled(false);
    radio.writeRegisterToDevice(RDA5807M::Register::REG_0x02);
    exit(0);
}

int main()
{
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = die;
    sigfillset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);

    sleep(1);

    RDA5807MWrapper wrapper { radio };
    CommandParser parser { wrapper };

    radio.setChannel(949);
    radio.setVolume(0x00);
    radio.setTune(true);
    radio.setMute(false);
    radio.writeAllRegistersToDevice();

    while (true) {
        std::cout << "Enter Command: " << std::endl;
        std::string line = "";
        std::getline(std::cin, line);
        std::cout << "Entered: " << line << std::endl;
        std::string result = parser.execute(line);
        std::cout << "Exec Result: " << result << std::endl;
        std::cout << "\n\n" << std::endl;
    }
}
