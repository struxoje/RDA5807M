/**************************************************
 * RDA5807M.cpp
 * Author: Ben Sherman
 *************************************************/

// System includes
#include <cstdint>
#include <cstring>
#include <iostream>

// Project includes
#include "mraa.hpp"
#include "RDA5807M.hpp"
#include "RDA5807MRegDefines.hpp"
#include "Util.hpp"

// Static variable initialization
const uint16_t RDA5807M::REGISTER_MAP_DEFAULT_STATE[] = {
        /* Reg 0x00 */0x5800,
        /* Reg 0x01 */0x8000, // HighZ mode is disabled - it caused RSSI problems
        /* Reg 0x02 */0x0000,
        /* Reg 0x03 */0x0000,
        /* Reg 0x04 */0x0200,
        /* Reg 0x05 */0x880F,
        /* Reg 0x06 */0x0000,
        /* Reg 0x07 */0x4202,
        /* Below are read-only regs */
        /* Reg 0x08 */0x0000,
        /* Reg 0x09 */0x0000,
        /* Reg 0x0A */0x0000,
        /* Reg 0x0B */0x0000,
        /* Reg 0x0C */0x0000,
        /* Reg 0x0D */0x0000,
        /* Reg 0x0E */0x0000,
        /* Reg 0x0F */0x0000 };

RDA5807M::RDA5807M() : i2cInterface(mraa::I2c(0, true))
{
    // Reset the local register map
    std::memcpy(registers, REGISTER_MAP_DEFAULT_STATE, REGISTER_MAP_SIZE_BYTES);

    // Set the i2c addr
    bool result = setI2cAddress(RANDOM_ACCESS_I2C_MODE_ADDR);

    std::cout << "I2C address set result: " << (result == true ? "OK!" : "Error!") << std::endl;

    init();
}

void RDA5807M::init()
{
    std::memcpy(registers, REGISTER_MAP_DEFAULT_STATE, REGISTER_MAP_SIZE_BYTES);
    setMute(false);
    setHighImpedanceOutput(false);
    setRDSMode(true);
    setVolume(0x00);
    setChannelSpacing(RDA5807M::ChannelSpacing::ONE_HUND_KHZ);
    setBand(RDA5807M::Band::US_EUR);
    setTune(true);
    setEnabled(true);
    writeAllRegistersToDevice();
}

/*
 * Applies value to the masked region in register regNum. The changes are not
 * pushed to the device by this call.
 */
void RDA5807M::setRegister(Register regNum, uint16_t value, uint16_t mask)
{
    // Align the mask's LSB to bit zero
    uint8_t shiftAmt = 0;
    uint16_t shiftedMask = mask;

    while (((shiftedMask & 0x0001) == 0) && (shiftAmt < 16))
    {
        shiftedMask >>= 1;
        ++shiftAmt;
    }

    // AND the mask with the value in the mask position
    uint16_t maskAndValue = shiftedMask & value;

    // Shift the mask ANDed with value back to the original mask position
    maskAndValue <<= shiftAmt;

    // Create a working copy of the register we intend to modify
    uint16_t regTemp = registers[regNum];

    // ANDing with the negation of mask will set zeroes to the mask region
    // but won't modify any other bits
    regTemp &= ~mask;

    // ORing maskAndValue with regTemp will apply the changes to the masked
    // region in regTemp, but won't impact any other bits in regTemp
    regTemp |= maskAndValue;

    // Viola!
    registers[regNum] = regTemp;
}

bool RDA5807M::setI2cAddress(uint8_t addr)
{
    if (mraa::Result::SUCCESS != i2cInterface.address(addr))
    {
        return false;
    }
    else
    {
        return true;
    }
}

RDA5807M::I2c_Result RDA5807M::writeRegisterToDevice(Register reg)
{
    uint8_t dataToWrite[3] = { static_cast<uint8_t>(reg), static_cast<uint8_t>(registers[reg] >> 8),
            static_cast<uint8_t>(registers[reg]) };

//    std::printf("Writing: {reg: 0x%02x, upper: 0x%02x, lower: 0x%02x}\n", dataToWrite[0], dataToWrite[1], dataToWrite[2]);

    mraa::Result result = i2cInterface.write(&dataToWrite[0], 3);

//    std::cout << "\tMRAA Result: " << result << std::endl;

    if (result == mraa::Result::SUCCESS)
    {
//        std::cout << "\tWrite successful" << std::endl;
        return I2c_Result::SUCCESS;
    }
    else
    {
        std::cout << "\tWrite failed" << std::endl;
        return I2c_Result::FAILED;
    }
}

RDA5807M::I2c_Result RDA5807M::writeAllRegistersToDevice()
{
    I2c_Result res = SUCCESS;

    for (uint8_t regIdx = WRITE_REGISTER_BASE_IDX; regIdx <= WRITE_REGISTER_MAX_IDX; ++regIdx)
    {
        if (I2c_Result::FAILED == writeRegisterToDevice(static_cast<Register>(regIdx)))
        {
            res = FAILED;
        }
    }
    return res;
}

uint16_t RDA5807M::readRegisterFromDevice(Register reg)
{
    uint16_t data = i2cInterface.readWordReg(static_cast<uint8_t>(reg));

    uint8_t dataLow = static_cast<uint8_t>(data >> 8);
    uint8_t dataHigh = static_cast<uint8_t>(data & 0x00FF);

    data = (0xFFFF & dataLow);
    data |= dataHigh << 8;

//    std::printf("Read reg: 0x%02x; Value: 0x%04x\n", reg, data);

    return data;
}

void RDA5807M::readDeviceRegistersAndStoreLocally()
{
    registers[REG_0x00] = readRegisterFromDevice(static_cast<Register>(REG_0x00));

    for (int regIdx = READ_REG_BASE_IDX; regIdx <= READ_REG_MAX_IDX; ++regIdx)
    {
        registers[regIdx] = readRegisterFromDevice(static_cast<Register>(regIdx));
    }
}

void RDA5807M::printRegisterMap()
{
    for (uint16_t regIdx = 0; regIdx < REGISTER_MAP_SIZE_REGISTERS; ++regIdx)
    {
        std::printf("Reg: 0x%02x; Value: 0x%04x\n", regIdx, registers[regIdx]);
    }
}

/**
 * Enables mute if muteEnable is true, disables mute if muteEnable is false
 */
void RDA5807M::setMute(bool muteEnable, bool writeResultToDevice)
{
    setRegister(REG_0x02, Util::boolToInteger(!muteEnable), DMUTE);

    if (writeResultToDevice)
    {
        writeRegisterToDevice(REG_0x02);
    }
}

/**
 * Enables high impedance output mode if highImpedanceEnable is true,
 * enables normal operation if highImpedanceEmable is false.
 */
void RDA5807M::setHighImpedanceOutput(bool highImpedanceEnable, bool writeResultToDevice)
{
    setRegister(REG_0x02, Util::boolToInteger(!highImpedanceEnable), DHIZ);

    if (writeResultToDevice)
    {
        writeRegisterToDevice(REG_0x02);
    }
}

/**
 * Enables stereo mode if stereoEnable is true, forces mono mode if
 * stereoEnable is false.
 */
void RDA5807M::setStereo(bool stereoEnable, bool writeResultToDevice)
{
    setRegister(REG_0x02, Util::boolToInteger(!stereoEnable), DMONO);

    if (writeResultToDevice)
    {
        writeRegisterToDevice(REG_0x02);
    }
}

/**
 * Enables bass bost if bassBostEnabe is true, disables bass boost if
 * bassBostEnable is false
 */
void RDA5807M::setBassBoost(bool bassBoostEnable, bool writeResultToDevice)
{
    setRegister(REG_0x02, Util::boolToInteger(bassBoostEnable), DBASS);

    if (writeResultToDevice)
    {
        writeRegisterToDevice(REG_0x02);
    }
}

/**
 * Sets the seek direction to up if seekDirection is SEEK_UP,
 * and sets the seek direction to down if seekDirection is SEEK_DOWN.
 */
void RDA5807M::setSeekDirection(SeekDirection seekDirection, bool writeResultToDevice)
{
    if (seekDirection == SeekDirection::SEEK_DOWN)
    {
        setRegister(REG_0x02, 0x00, SEEKUP);
    }
    else
    {
        setRegister(REG_0x02, 0xFF, SEEKUP);
    }

    if (writeResultToDevice)
    {
        writeRegisterToDevice(REG_0x02);
    }
}

/**
 * Enables seek mode if seekEnable is true, and disables seek mode if seekEnable
 * is false.
 */
void RDA5807M::setSeek(bool seekEnable, bool writeResultToDevice)
{
    setRegister(REG_0x02, Util::boolToInteger(seekEnable), SEEK);

    if (writeResultToDevice)
    {
        writeRegisterToDevice(REG_0x02);
    }
}

/**
 * If seekMode is WRAP_AT_LIMIT, when the seek operation reaches the upper or
 * lower band limit, the frequency will roll over to the opposite limit.
 * If seekMode is STOP_AT_LIMIT, when the seek operation reaches a band limit,
 * seeking will stop.
 */
void RDA5807M::setSeekMode(SeekMode seekMode, bool writeResultToDevice)
{
    if (seekMode == SeekMode::STOP_AT_LIMIT)
    {
        setRegister(REG_0x02, 0xFF, SKMODE);
    }
    else
    {
        setRegister(REG_0x02, 0x00, SKMODE);
    }

    if (writeResultToDevice)
    {
        writeRegisterToDevice(REG_0x02);
    }
}

/**
 * Enables RDS/RBDS if rdsEnables is true. Disables RDS/RBDS if rdsEnable
 * is false.
 */
void RDA5807M::setRDSMode(bool rdsEnable, bool writeResultToDevice)
{
    setRegister(REG_0x02, Util::boolToInteger(rdsEnable), RDS_EN);

    if (writeResultToDevice)
    {
        writeRegisterToDevice(REG_0x02);
    }
}

/**
 * As per the manual, using the "new method" can "improve the receive sensitiity
 * by about 1dB." If newMethodEnable is true, the new method is enabled, and if
 * newMethodEnabled is false, the new method is disabled.
 */
void RDA5807M::setNewMethod(bool newMethodEnable, bool writeResultToDevice)
{
    setRegister(REG_0x02, Util::boolToInteger(newMethodEnable), NEW_METHOD);

    if (writeResultToDevice)
    {
        writeRegisterToDevice(REG_0x02);
    }
}

/**
 * Puts the radio in soft reset mode if softResetEnable is true,
 * removes the radio from soft reset mode if softResetEnable is false.
 */
void RDA5807M::setSoftReset(bool softResetEnable, bool writeResultToDevice)
{
    setRegister(REG_0x02, Util::boolToInteger(softResetEnable), SOFT_RESET);

    if (writeResultToDevice)
    {
        writeRegisterToDevice(REG_0x02);
    }
}

/**
 * Enables powerup of the radio if enable is true, turns radio power off
 * if enable is false.
 */
void RDA5807M::setEnabled(bool enable, bool writeResultToDevice)
{
    setRegister(REG_0x02, Util::boolToInteger(enable), ENABLE);

    if (writeResultToDevice)
    {
        writeRegisterToDevice(REG_0x02);
    }
}

// TODO this depends on channel spacing!
// We assume 100KHz spacing and US band currently, and the channel
// must be in terms of 10f - that is, 985 for 98.5MHz, for example
void RDA5807M::setChannel(uint16_t channel, bool writeResultToDevice)
{
    uint16_t chan = channel - 870;
    setRegister(REG_0x03, chan, CHAN);

    if (writeResultToDevice)
    {
        writeRegisterToDevice(REG_0x03);
    }
}

void RDA5807M::setTune(bool enable, bool writeResultToDevice)
{
    setRegister(REG_0x03, Util::boolToInteger(enable), TUNE);

    if (writeResultToDevice)
    {
        writeRegisterToDevice(REG_0x03);
    }
}

void RDA5807M::setBand(Band band, bool writeResultToDevice)
{
    uint8_t bandBits = US_EUR_BAND_SELECT;
    switch (band)
    {
        case US_EUR:
            bandBits = US_EUR_BAND_SELECT;
            break;
        case JAP:
            bandBits = JAPAN_BAND_SELECT;
            break;
        case WORLD_WIDE:
            bandBits = WORLD_WIDE_BAND_SELECT;
            break;
        case EAST_EUROPE:
            bandBits = EAST_EUR_BAND_SELECT;
            break;
    }
    setRegister(REG_0x03, bandBits, BAND);

    if (writeResultToDevice)
    {
        writeRegisterToDevice(REG_0x03);
    }
}

void RDA5807M::setChannelSpacing(ChannelSpacing spacing, bool writeResultToDevice)
{
    uint8_t spacingBits = CHANNEL_SPACE_100KHZ;

    switch (spacing)
    {
        case ONE_HUND_KHZ:
            spacingBits = CHANNEL_SPACE_100KHZ;
            break;
        case TWO_HUND_KHZ:
            spacingBits = CHANNEL_SPACE_200KHZ;
            break;
        case FIFTY_KHZ:
            spacingBits = CHANNEL_SPACE_50KHZ;
            break;
        case TWENTY_FIVE_KHZ:
            spacingBits = CHANNEL_SPACE_25KHZ;
            break;
    }
    setRegister(REG_0x03, spacingBits, SPACE);

    if (writeResultToDevice)
    {
        writeRegisterToDevice(REG_0x03);
    }
}

void RDA5807M::setDeEmphasis(DeEmphasis de, bool writeResultToDevice)
{
    uint8_t deemphasisBits = DEEMP_75_US;
    switch (de)
    {
        case SEVENTY_FIVE_US:
            deemphasisBits = DEEMP_75_US;
            break;
        case FIFTY_US:
            deemphasisBits = DEEMP_50_US;
            break;
    }
    setRegister(REG_0x04, deemphasisBits, DE);

    if (writeResultToDevice)
    {
        writeRegisterToDevice(REG_0x03);
    }
}

void RDA5807M::setAFCD(bool afcdEnable, bool writeResultToDevice)
{
    setRegister(REG_0x04, Util::boolToInteger(afcdEnable), AFCD);

    if (writeResultToDevice)
    {
        writeRegisterToDevice(REG_0x04);
    }
}

/**
 * Sets the radio volume. volume = 1111 is maximum, and volume = 0
 * is muted. The volume scale is "logarithmic" as per the manual.
 */
void RDA5807M::setVolume(uint8_t volume, bool writeResultToDevice)
{
    setRegister(REG_0x05, volume, VOLUME);

    if (writeResultToDevice)
    {
        writeRegisterToDevice(REG_0x05);
    }
}

void RDA5807M::setSoftMute(bool softMuteEnable, bool writeResultToDevice)
{
    setRegister(REG_0x04, Util::boolToInteger(softMuteEnable), SOFTMUTE_EN);

    if (writeResultToDevice)
    {
        writeRegisterToDevice(REG_0x04);
    }
}

bool RDA5807M::retrieveUpdateRegAndReturnFlag(Register reg, uint16_t mask)
{
    registers[reg] = readRegisterFromDevice(reg);
    uint16_t maskedValue = Util::valueFromReg(registers[reg], mask);

    if (maskedValue == 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool RDA5807M::isRdsReady()
{
    return retrieveUpdateRegAndReturnFlag(REG_0x0A, RDSR);
}

bool RDA5807M::isStcComplete()
{
    return retrieveUpdateRegAndReturnFlag(REG_0x0A, STC);
}

bool RDA5807M::didSeekFail()
{
    return retrieveUpdateRegAndReturnFlag(REG_0x0A, SF);
}

bool RDA5807M::isRdsDecoderSynchronized()
{
    return retrieveUpdateRegAndReturnFlag(REG_0x0A, RDSS);
}

bool RDA5807M::hasBlkEBeenFound()
{
    return retrieveUpdateRegAndReturnFlag(REG_0x0A, BLK_E);
}

bool RDA5807M::isStereoEnabled()
{
    return retrieveUpdateRegAndReturnFlag(REG_0x0A, ST);
}

//TODO this depends on channel spacing!
// We assume 100KHz spacing and US band currently, and the channel
// must be in terms of 10f - that is, 985 for 98.5MHz, for example
uint16_t RDA5807M::getReadChannel()
{
    registers[REG_0x0A] = readRegisterFromDevice(REG_0x0A);

    uint16_t readChannel = Util::valueFromReg(registers[REG_0x0A], READCHAN);

    return (readChannel + 870);
}

bool RDA5807M::isFmTrue()
{
    return retrieveUpdateRegAndReturnFlag(REG_0x0B, FM_TRUE);
}

bool RDA5807M::isFmReady()
{
    return retrieveUpdateRegAndReturnFlag(REG_0x0B, FM_READY);
}

uint8_t RDA5807M::getRssi()
{
    registers[REG_0x0B] = readRegisterFromDevice(REG_0x0B);
    return static_cast<uint8_t>(Util::valueFromReg(registers[REG_0x0B],
    RSSI));
}

void RDA5807M::printStatus()
{
    std::printf("Read channel: %u\n", getReadChannel());
    std::printf("Seek/Tune complete: %s\n", isStcComplete() ? "Complete" : "Not Complete");
    std::printf("Seek result: %s\n", didSeekFail() ? "Successful" : "Failure");
    std::printf("RDS Sync'd?: %s\n", isRdsDecoderSynchronized() ? "Synchronized" : "Not synchronized");
    std::printf("Has Block E been found?: %s\n", hasBlkEBeenFound() ? "Yes" : "No");
    std::printf("Audio type: %s\n", isStereoEnabled() ? "Stereo" : "Mono");
    std::printf("Read channel: %u\n", getReadChannel());
    std::printf("Rssi: 0x%02x\n", getRssi());
    std::printf("Is this freq a station?: %s\n", isFmTrue() ? "Yes" : "No");
    std::printf("FM Ready?: %s\n", isFmReady() ? "Yes" : "No");
}
