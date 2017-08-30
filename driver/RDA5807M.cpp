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

const std::string RDA5807M::STATUSRESULT_TO_STRING[] = { "SUCCESS", "ABOVE_MAX", "BELOW_MIN", "GENERAL_FAILURE", "I2C_FAILURE" };
const std::string RDA5807M::BLOCK_ERRORS_TO_STRING[] = { "ZERO_ERRORS", "ONE_TO_TWO_ERRORS", "THREE_TO_FIVE_ERRORS", "SIX_OR_MORE_ERRORS"};
const uint16_t RDA5807M::FREQUENCY_RANGE_MIN[] = {870, 760, 760, 650};
const uint16_t RDA5807M::FREQUENCY_RANGE_MAX[] = {1080, 910, 1080, 760};

RDA5807M::RDA5807M() : band(US_EUR), i2cInterface(mraa::I2c(0, true))
{
    // Reset the local register map
    std::memcpy(registers, REGISTER_MAP_DEFAULT_STATE, REGISTER_MAP_SIZE_BYTES);

    // Set the i2c addr
    StatusResult result = setI2cAddress(RANDOM_ACCESS_I2C_MODE_ADDR);

    std::cout << "I2C address set result: " << (result == StatusResult::SUCCESS ? "OK!" : "Error!") << std::endl;

    init();
}

/**
 * Returns the string value of the result parameter
 */
std::string RDA5807M::statusResultToString(StatusResult toConvert)
{
    return STATUSRESULT_TO_STRING[static_cast<unsigned int>(toConvert)];
}

/**
 * Returns the string value of the result parameter
 */
std::string RDA5807M::rdsBlockErrorToString(RdsBlockErrors toConvert)
{
    return BLOCK_ERRORS_TO_STRING[toConvert];
}

/**
 * Resets the radio to a known state.
 */
void RDA5807M::reset()
{
    init();
}

/**
 * Initializes the radio to a known state. This is done implicitly when an RDA5807M instance
 * is constructed.
 */
void RDA5807M::init()
{
    std::memcpy(registers, REGISTER_MAP_DEFAULT_STATE, REGISTER_MAP_SIZE_BYTES);
    setMute(true, false);
    setHighImpedanceOutput(false, false);
    setRdsMode(true, false);
    setSoftMute(false, false);
    setStereo(true, false);
    setNewMethod(true, false); // KEEP ME ENABLED! Using new method offers a drastic performance reception improvement
    setVolume(0x00, false);
    setChannelSpacing(RDA5807M::ChannelSpacing::ONE_HUND_KHZ, false);
    setBand(band, false);
    setTune(true, false);
    setEnabled(true, false);

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

    // Voila!
    registers[regNum] = regTemp;
}

RDA5807M::StatusResult RDA5807M::setI2cAddress(uint8_t addr)
{
    if (mraa::Result::SUCCESS != i2cInterface.address(addr))
    {
        return StatusResult::I2C_FAILURE;
    }
    else
    {
        return StatusResult::SUCCESS;
    }
}

RDA5807M::StatusResult RDA5807M::writeRegisterToDevice(Register reg)
{
    uint8_t dataToWrite[3] = { static_cast<uint8_t>(reg), static_cast<uint8_t>(registers[reg] >> 8),
            static_cast<uint8_t>(registers[reg]) };

//    std::printf("Writing: {reg: 0x%02x, upper: 0x%02x, lower: 0x%02x}\n", dataToWrite[0], dataToWrite[1], dataToWrite[2]);

    mraa::Result result = i2cInterface.write(&dataToWrite[0], 3);

//    std::cout << "\tMRAA Result: " << result << std::endl;

    if (result == mraa::Result::SUCCESS)
    {
//        std::cout << "\tWrite successful" << std::endl;
        return StatusResult::SUCCESS;
    }
    else
    {
        std::cout << "\tWrite failed" << std::endl;
        return StatusResult::I2C_FAILURE;
    }
}

RDA5807M::StatusResult RDA5807M::writeAllRegistersToDevice()
{
    StatusResult res = StatusResult::SUCCESS;

    for (uint8_t regIdx = WRITE_REGISTER_BASE_IDX; regIdx <= WRITE_REGISTER_MAX_IDX; ++regIdx)
    {
        if (StatusResult::I2C_FAILURE == writeRegisterToDevice(static_cast<Register>(regIdx)))
        {
            res = StatusResult::I2C_FAILURE;
        }
    }
    return res;
}

/**
 * Returns the content of the specified register from the device
 */
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

/**
 * Reads a single register from the device and updates its value in the local register map
 */
void RDA5807M::readAndStoreSingleRegisterFromDevice(Register reg)
{
    registers[reg] = readRegisterFromDevice(static_cast<Register>(reg));
}

void RDA5807M::readDeviceRegistersAndStoreLocally()
{
    registers[REG_0x00] = readRegisterFromDevice(static_cast<Register>(REG_0x00));

    for (int regIdx = READ_REG_BASE_IDX; regIdx <= READ_REG_MAX_IDX; ++regIdx)
    {
        registers[regIdx] = readRegisterFromDevice(static_cast<Register>(regIdx));
    }
}

/**
 * Generates a fancy-formatted version of the register map
 */
std::string RDA5807M::getRegisterMap()
{
    char buffer[50] = {0};
    std::string regMap{" _______________\n"};

    regMap.append("| REG  | VALUE  |\n");

    for (uint16_t regIdx = 0; regIdx < REGISTER_MAP_SIZE_REGISTERS; ++regIdx)
    {
        std::sprintf(buffer,"| 0x%02x | 0x%04x |\n", regIdx, registers[regIdx]);
        regMap.append("|---------------|\n");
        regMap.append(buffer);
    }
    regMap.append("|===============|");
    return regMap;
}

/**
 * Enables mute if muteEnable is true, disables mute if muteEnable is false
 */
RDA5807M::StatusResult RDA5807M::setMute(bool muteEnable, bool writeResultToDevice)
{
    setRegister(REG_0x02, Util::boolToInteger(!muteEnable), DMUTE);

    return conditionallyWriteRegisterToDevice(REG_0x02, writeResultToDevice);
}

/**
 * Enables high impedance output mode if highImpedanceEnable is true,
 * enables normal operation if highImpedanceEmable is false.
 */
RDA5807M::StatusResult RDA5807M::setHighImpedanceOutput(bool highImpedanceEnable, bool writeResultToDevice)
{
    setRegister(REG_0x02, Util::boolToInteger(!highImpedanceEnable), DHIZ);

    return conditionallyWriteRegisterToDevice(REG_0x02, writeResultToDevice);
}

/**
 * Enables stereo mode if stereoEnable is true, forces mono mode if
 * stereoEnable is false.
 */
RDA5807M::StatusResult RDA5807M::setStereo(bool stereoEnable, bool writeResultToDevice)
{
    setRegister(REG_0x02, Util::boolToInteger(!stereoEnable), DMONO);

    return conditionallyWriteRegisterToDevice(REG_0x02, writeResultToDevice);
}

/**
 * Enables bass bost if bassBostEnabe is true, disables bass boost if
 * bassBostEnable is false
 */
RDA5807M::StatusResult RDA5807M::setBassBoost(bool bassBoostEnable, bool writeResultToDevice)
{
    setRegister(REG_0x02, Util::boolToInteger(bassBoostEnable), DBASS);

    return conditionallyWriteRegisterToDevice(REG_0x02, writeResultToDevice);
}

/**
 * Sets the seek direction to up if seekDirection is SEEK_UP,
 * and sets the seek direction to down if seekDirection is SEEK_DOWN.
 */
RDA5807M::StatusResult RDA5807M::setSeekDirection(SeekDirection seekDirection, bool writeResultToDevice)
{
    if (seekDirection == SeekDirection::SEEK_DOWN)
    {
        setRegister(REG_0x02, 0x00, SEEKUP);
    }
    else
    {
        setRegister(REG_0x02, 0xFF, SEEKUP);
    }

    return conditionallyWriteRegisterToDevice(REG_0x02, writeResultToDevice);
}

/**
 * Enables seek mode if seekEnable is true, and disables seek mode if seekEnable
 * is false.
 */
RDA5807M::StatusResult RDA5807M::setSeek(bool seekEnable, bool writeResultToDevice)
{
    setRegister(REG_0x02, Util::boolToInteger(seekEnable), SEEK);

    return conditionallyWriteRegisterToDevice(REG_0x02, writeResultToDevice);
}

/**
 * If seekMode is WRAP_AT_LIMIT, when the seek operation reaches the upper or
 * lower band limit, the frequency will roll over to the opposite limit.
 * If seekMode is STOP_AT_LIMIT, when the seek operation reaches a band limit,
 * seeking will stop.
 */
RDA5807M::StatusResult RDA5807M::setSeekMode(SeekMode seekMode, bool writeResultToDevice)
{
    if (seekMode == SeekMode::STOP_AT_LIMIT)
    {
        setRegister(REG_0x02, 0xFF, SKMODE);
    }
    else
    {
        setRegister(REG_0x02, 0x00, SKMODE);
    }

    return conditionallyWriteRegisterToDevice(REG_0x02, writeResultToDevice);
}

/**
 * Enables RDS/RBDS if rdsEnable is true. Disables RDS/RBDS if rdsEnable
 * is false.
 */
RDA5807M::StatusResult RDA5807M::setRdsMode(bool rdsEnable, bool writeResultToDevice)
{
    setRegister(REG_0x02, Util::boolToInteger(rdsEnable), RDS_EN);

    return conditionallyWriteRegisterToDevice(REG_0x02, writeResultToDevice);
}

/**
 * As per the manual, using the "new method" can "improve the receive sensitiity
 * by about 1dB." If newMethodEnable is true, the new method is enabled, and if
 * newMethodEnabled is false, the new method is disabled.
 */
RDA5807M::StatusResult RDA5807M::setNewMethod(bool newMethodEnable, bool writeResultToDevice)
{
    setRegister(REG_0x02, Util::boolToInteger(newMethodEnable), NEW_METHOD);

    return conditionallyWriteRegisterToDevice(REG_0x02, writeResultToDevice);
}

/**
 * Puts the radio in soft reset mode if softResetEnable is true,
 * removes the radio from soft reset mode if softResetEnable is false.
 */
RDA5807M::StatusResult RDA5807M::setSoftReset(bool softResetEnable, bool writeResultToDevice)
{
    setRegister(REG_0x02, Util::boolToInteger(softResetEnable), SOFT_RESET);

    return conditionallyWriteRegisterToDevice(REG_0x02, writeResultToDevice);
}

/**
 * Enables powerup of the radio if enable is true, turns radio power off
 * if enable is false.
 */
RDA5807M::StatusResult RDA5807M::setEnabled(bool enable, bool writeResultToDevice)
{
    setRegister(REG_0x02, Util::boolToInteger(enable), ENABLE);

    return conditionallyWriteRegisterToDevice(REG_0x02, writeResultToDevice);
}

// TODO this depends on channel spacing!
// We assume 100KHz spacing and US band currently, and the channel
// must be in terms of 10f - that is, 985 for 98.5MHz, for example
RDA5807M::StatusResult RDA5807M::setChannel(uint16_t channel, bool writeResultToDevice)
{
    uint16_t chan = channel - FREQUENCY_RANGE_MIN[US_EUR_BAND_SELECT];
    setRegister(REG_0x03, chan, CHAN);

    return conditionallyWriteRegisterToDevice(REG_0x03, writeResultToDevice);
}

RDA5807M::StatusResult RDA5807M::setTune(bool enable, bool writeResultToDevice)
{
    setRegister(REG_0x03, Util::boolToInteger(enable), TUNE);

    return conditionallyWriteRegisterToDevice(REG_0x03, writeResultToDevice);
}

RDA5807M::StatusResult RDA5807M::setBand(Band band, bool writeResultToDevice)
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

    this->band = static_cast<Band>(bandBits);

    return conditionallyWriteRegisterToDevice(REG_0x03, writeResultToDevice);
}

RDA5807M::StatusResult RDA5807M::setChannelSpacing(ChannelSpacing spacing, bool writeResultToDevice)
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

    return conditionallyWriteRegisterToDevice(REG_0x03, writeResultToDevice);
}

RDA5807M::StatusResult RDA5807M::setDeEmphasis(DeEmphasis de, bool writeResultToDevice)
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

    return conditionallyWriteRegisterToDevice(REG_0x04, writeResultToDevice);
}

RDA5807M::StatusResult RDA5807M::setAFCD(bool afcdEnable, bool writeResultToDevice)
{
    setRegister(REG_0x04, Util::boolToInteger(!afcdEnable), AFCD);

    return conditionallyWriteRegisterToDevice(REG_0x04, writeResultToDevice);
}

/**
 * Sets the radio volume. volume = 1111 is maximum, and volume = 0
 * is muted. The volume scale is "logarithmic" as per the manual.
 */
RDA5807M::StatusResult RDA5807M::setVolume(uint8_t volume, bool writeResultToDevice)
{
    setRegister(REG_0x05, volume, VOLUME);

    return conditionallyWriteRegisterToDevice(REG_0x05, writeResultToDevice);
}

RDA5807M::StatusResult RDA5807M::setSoftMute(bool softMuteEnable, bool writeResultToDevice)
{
    setRegister(REG_0x04, Util::boolToInteger(softMuteEnable), SOFTMUTE_EN);

    return conditionallyWriteRegisterToDevice(REG_0x04, writeResultToDevice);
}

bool RDA5807M::readAndStoreRegFromDeviceAndReturnFlag(Register reg, uint16_t mask)
{
    readAndStoreSingleRegisterFromDevice(reg);
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
    return readAndStoreRegFromDeviceAndReturnFlag(REG_0x0A, RDSR);
}

bool RDA5807M::isStcComplete()
{
    return readAndStoreRegFromDeviceAndReturnFlag(REG_0x0A, STC);
}

bool RDA5807M::didSeekFail()
{
    return readAndStoreRegFromDeviceAndReturnFlag(REG_0x0A, SF);
}

bool RDA5807M::isRdsDecoderSynchronized()
{
    return readAndStoreRegFromDeviceAndReturnFlag(REG_0x0A, RDSS);
}

bool RDA5807M::hasBlkEBeenFound()
{
    return readAndStoreRegFromDeviceAndReturnFlag(REG_0x0A, BLK_E);
}

bool RDA5807M::isStereoEnabled()
{
    return readAndStoreRegFromDeviceAndReturnFlag(REG_0x0A, ST);
}

//TODO this depends on channel spacing!
// We assume 100KHz spacing and US band currently, and the channel
// must be in terms of 10f - that is, 985 for 98.5MHz, for example
uint16_t RDA5807M::getReadChannel()
{
    registers[REG_0x0A] = readRegisterFromDevice(REG_0x0A);

    uint16_t readChannel = Util::valueFromReg(registers[REG_0x0A], READCHAN);

    return (readChannel + FREQUENCY_RANGE_MIN[US_EUR_BAND_SELECT]);
}

bool RDA5807M::isFmTrue()
{
    return readAndStoreRegFromDeviceAndReturnFlag(REG_0x0B, FM_TRUE);
}

bool RDA5807M::isFmReady()
{
    return readAndStoreRegFromDeviceAndReturnFlag(REG_0x0B, FM_READY);
}

uint8_t RDA5807M::getRssi()
{
    registers[REG_0x0B] = readRegisterFromDevice(REG_0x0B);
    return static_cast<uint8_t>(Util::valueFromReg(registers[REG_0x0B],
        RSSI));
}

/**
 * A wrapper for writeRegisterToDevice(). Since many functions have the same if(writeResultToDevice) do write;
 * else return SUCCESS; statement, this function is intended to help slim code duplication.
 * Returns the result of writeRegisterToDevice(regToWrite) if shouldWrite is true, and returns
 * SUCCESS if shouldWrite is false.
 */
RDA5807M::StatusResult RDA5807M::conditionallyWriteRegisterToDevice(Register regToWrite, bool shouldWrite)
{
    if (shouldWrite)
    {
        return writeRegisterToDevice(regToWrite);
    }
    else
    {
        return StatusResult::SUCCESS;
    }
}

RDA5807M::StatusResult RDA5807M::setSoftBlend(bool softBlendEnable, bool writeResultToDevice)
{
    setRegister(REG_0x07, Util::boolToInteger(softBlendEnable), SOFTBLEND_EN);

    return conditionallyWriteRegisterToDevice(REG_0x07, writeResultToDevice);
}

uint16_t RDA5807M::getRdsPiCode(bool readRegisterFromDevice)
{
    if (readRegisterFromDevice)
    {
        readAndStoreSingleRegisterFromDevice(BLOCK_A);
    }
    return registers[BLOCK_A];
}

uint8_t RDA5807M::getRdsGroupTypeCode(bool readRegisterFromDevice)
{
    if (readRegisterFromDevice)
    {
        readAndStoreSingleRegisterFromDevice(BLOCK_B);
    }
    return static_cast<uint8_t>(Util::valueFromReg(registers[BLOCK_B], GROUP_TYPE));
}

uint8_t RDA5807M::getRdsVersionCode(bool readRegisterFromDevice)
{
    if (readRegisterFromDevice)
    {
        readAndStoreSingleRegisterFromDevice(BLOCK_B);
    }
    return static_cast<uint8_t>(Util::valueFromReg(registers[BLOCK_B], VERSION_CODE));

}

uint8_t RDA5807M::getRdsTrafficProgramIdCode(bool readRegisterFromDevice)
{
    if (readRegisterFromDevice)
    {
        readAndStoreSingleRegisterFromDevice(BLOCK_B);
    }
    return static_cast<uint8_t>(Util::valueFromReg(registers[BLOCK_B], TRAFFIC_PROGRAM));
}

uint8_t RDA5807M::getRdsProgramTypeCode(bool readRegisterFromDevice)
{
    if (readRegisterFromDevice)
    {
        readAndStoreSingleRegisterFromDevice(BLOCK_B);
    }
    return static_cast<uint8_t>(Util::valueFromReg(registers[BLOCK_B], PROGRAM_TYPE));
}

/**
 * Returns the content of the *LOCALLY STORED* register
 * specified by reg
 */
uint16_t RDA5807M::getLocalRegisterContent(Register reg)
{
    return registers[reg];
}

/**
 * Returns the number of errors on the RDS block specified as the param.
 * DOES NOT FETCH THE REGISTERS FROM THE DEVICE! Only works off of local register copies.
 * Only valid for BLOCK_A or BLOCK_B params
 */
RDA5807M::RdsBlockErrors RDA5807M::getRdsErrorsForBlock(Register block)
{
    if (block == BLOCK_A)
    {
        return static_cast<RdsBlockErrors>(Util::valueFromReg(registers[block], BLERA));
    }
    else if (block == BLOCK_B)
    {
        return static_cast<RdsBlockErrors>(Util::valueFromReg(registers[block], BLERB));
    }
    // Return SIX_OR_MORE_ERRORS in the event that an invalid
    // registers is passed as a param
    else
    {
        return SIX_OR_MORE_ERRORS;
    }
}

/**
 * Returns the currently selected band (stored as an internal member,
 * not read from the radio).
 */
RDA5807M::Band RDA5807M::getBand()
{
    return band;
}

uint16_t RDA5807M::getBandMinumumFrequency()
{
    return FREQUENCY_RANGE_MIN[band];
}

uint16_t RDA5807M::getBandMaximumFrequency()
{
    return FREQUENCY_RANGE_MAX[band];

}


