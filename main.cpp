/**************************************************
 * main.cpp
 * Author: Ben Sherman
 *************************************************/

// Avoid enabling hiz mode. Both cause RSSI to bug out
// System includes
#include <unistd.h>
#include <iostream>
#include <signal.h>

// Project includes
#include "RDA5807M.hpp"

RDA5807M radio;

void die(int unused) {
    radio.setVolume(0x00);
    radio.writeRegisterToDevice(RDA5807M::Register::REG_0x05);

    radio.setEnabled(false);
    radio.writeRegisterToDevice(RDA5807M::Register::REG_0x02);
    exit(0);
}

int main() {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = die;
    sigfillset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);

    sleep(1);

    radio.setChannel(949);
    radio.setVolume(0x00);
    radio.setTune(true);
    radio.setMute(false);
    radio.writeAllRegistersToDevice();

    for (uint8_t i = 0; i < 9; i++) {
        radio.setVolume(i);
        radio.writeAllRegistersToDevice();
        sleep(1);
    }

    while (true) {
        radio.setSeek(true);
        radio.readDeviceRegistersAndStoreLocally();
        radio.printStatus();
        sleep(5);
        std::cout << "\n\n\n" << std::endl;
    }

    /*    while (true) {
     for (int i = 870; i < 1080; i++) {
     std::cout << "Setting freq: " << i << std::endl;
     radio.setChannel(i);
     radio.setTune(true);
     radio.writeRegisterToDevice(RDA5807M::Register::REG_0x03);
     usleep(400000);
     radio.printStatus();
     if (radio.getRssi() > 0x10) {
     radio.setMute(false);
     radio.writeRegisterToDevice(RDA5807M::Register::REG_0x02);
     radio.readDeviceRegistersAndStoreLocally();
     radio.printRegisterMap();
     sleep(5);
     } else {
     radio.setMute(true);
     radio.writeRegisterToDevice(RDA5807M::Register::REG_0x02);
     }
     std::printf("\n\n\n");
     }
     }
     */
}
