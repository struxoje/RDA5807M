/**************************************************
 * main.cpp
 * Author: Ben Sherman
 *************************************************/

// System includes
#include <iostream>
#include <signal.h>
#include <unistd.h>

// Project includes
#include "CommandParser.hpp"
#include "RDA5807M.hpp"
#include "RDA5807MWrapper.hpp"

RDA5807M radio;

void die(int UNUSED)
{
    (void) UNUSED;

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

    while (true) {
        std::cout << "Enter Command: " << std::flush;
        std::string line = "";
        std::getline(std::cin, line);
        std::string result = parser.execute(line);
        std::cout << "Exec Result: " << result << std::endl;
        std::cout << "\n" << std::endl;
    }
}
