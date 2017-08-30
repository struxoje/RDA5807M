/**************************************************
 * RDA5807M.hpp - Header for RDA5807M driver
 * Author: Ben Sherman
 *************************************************/

#ifndef RDA5807M_HPP
#define RDA5807M_HPP

// System includes
#include <cstdint>

// Project includes
#include "mraa.hpp"

class RDA5807M
{
public:
    /////////////////////
    // Class Constants //
    /////////////////////
    // Limits
    static const uint8_t MAX_VOLUME = 0xFF;
    static const uint8_t RSSI_MAX = 0x7F;

    //////////////////////
    // Enum Definitions //
    //////////////////////
    enum Register
    {
        REG_0x00 = 0x00,
        REG_0x01 = 0x01,
        REG_0x02 = 0x02,
        REG_0x03 = 0x03,
        REG_0x04 = 0x04,
        REG_0x05 = 0x05,
        REG_0x06 = 0x06,
        REG_0x07 = 0x07,
        REG_0x08 = 0x08,
        REG_0x09 = 0x09,
        REG_0x0A = 0x0A,
        REG_0x0B = 0x0B,
        BLOCK_A = 0x0C,
        BLOCK_B = 0x0D,
        BLOCK_C = 0x0E,
        BLOCK_D = 0x0F
    };

    enum SeekDirection {SEEK_UP = 0, SEEK_DOWN = 1};
    enum SeekMode {WRAP_AT_LIMIT = 0, STOP_AT_LIMIT = 1};
    enum Band {US_EUR = 0, JAP = 1, WORLD_WIDE = 2, EAST_EUROPE= 3};
    enum ChannelSpacing {ONE_HUND_KHZ = 0, TWO_HUND_KHZ = 1, FIFTY_KHZ = 2, TWENTY_FIVE_KHZ = 3};
    enum DeEmphasis {SEVENTY_FIVE_US = 0, FIFTY_US = 1};
    enum RdsBlockErrors {ZERO_ERRORS = 0, ONE_TO_TWO_ERRORS = 1, THREE_TO_FIVE_ERRORS = 2, SIX_OR_MORE_ERRORS = 3};

    enum class StatusResult
    {
        SUCCESS = 0, ABOVE_MAX = 1, BELOW_MIN = 2, GENERAL_FAILURE = 3, I2C_FAILURE = 4
    };

    ////////////////////////////////
    // Public interface functions //
    ////////////////////////////////

    RDA5807M();

    void reset();

    // Updates the local register map, but DOES NOT WRITE the changes
    // to the device
    void setRegister(Register regNum, uint16_t value, uint16_t mask = 0xFF);

    StatusResult writeRegisterToDevice(Register reg);

    StatusResult writeAllRegistersToDevice();

    void readDeviceRegistersAndStoreLocally();

    void readAndStoreSingleRegisterFromDevice(Register reg);

    uint16_t readRegisterFromDevice(Register reg);

    uint16_t getLocalRegisterContent(Register reg);

    // FUNCTIONS USED TO ENABLE/DISABLE RADIO MODES
    StatusResult setMute(bool muteEnable, bool writeResultToDevice = true);

    StatusResult setHighImpedanceOutput(bool highImpedanceEnable, bool writeResultToDevice = true);

    StatusResult setStereo(bool stereoEnable, bool writeResultToDevice = true);

    StatusResult setBassBoost(bool bassBoostEnable, bool writeResultToDevice = true);

    StatusResult setSeekDirection(SeekDirection seekDirection, bool writeResultToDevice = true);

    StatusResult setSeek(bool seekEnable, bool writeResultToDevice = true);

    StatusResult setSeekMode(SeekMode seekMode, bool writeResultToDevice = true);

    StatusResult setRdsMode(bool rdsEnable, bool writeResultToDevice = true);

    StatusResult setNewMethod(bool newMethodEnable, bool writeResultToDevice = true);

    StatusResult setSoftReset(bool softResetEnable, bool writeResultToDevice = true);

    StatusResult setEnabled(bool enable, bool writeResultToDevice = true);

    StatusResult setChannel(uint16_t channel, bool writeResultToDevice = true);

    StatusResult setTune(bool enable, bool writeResultToDevice = true);

    StatusResult setBand(Band band, bool writeResultToDevice = true);

    StatusResult setChannelSpacing(ChannelSpacing spacing, bool writeResultToDevice = true);

    StatusResult setVolume(uint8_t volume, bool writeResultToDevice = true);

    StatusResult setDeEmphasis(DeEmphasis de, bool writeResultToDevice = true);

    StatusResult setAFCD(bool afcdEnable, bool writeResultToDevice = true);

    StatusResult setSoftMute(bool softMuteEnable, bool writeResultToDevice = true);

    StatusResult setSoftBlend(bool softBlendEnable, bool writeResultToDevice = true);

    bool readAndStoreRegFromDeviceAndReturnFlag(Register reg, uint16_t mask);

    bool isRdsReady();

    bool isStcComplete();

    bool didSeekFail();

    bool isRdsDecoderSynchronized();

    bool hasBlkEBeenFound();

    bool isStereoEnabled();

    uint16_t getReadChannel();

    uint8_t getRssi();

    bool isFmTrue();

    bool isFmReady();

    // RDS functions
    uint16_t getRdsPiCode(bool readRegisterFromDevice=false);
    uint8_t getRdsGroupTypeCode(bool readRegisterFromDevice=false);
    uint8_t getRdsVersionCode(bool readRegisterFromDevice=false);
    uint8_t getRdsTrafficProgramIdCode(bool readRegisterFromDevice=false);
    uint8_t getRdsProgramTypeCode(bool readRegisterFromDevice=false);
    RdsBlockErrors getRdsErrorsForBlock(Register block);

    std::string getRegisterMap();

    static std::string statusResultToString(StatusResult toConvert);
    static std::string rdsBlockErrorToString(RdsBlockErrors toConvert);

    Band getBand();
    uint16_t getBandMinumumFrequency();
    uint16_t getBandMaximumFrequency();

private:
    /////////////////////////////
    // Private class Constants //
    /////////////////////////////

    // Index of the first writable register
    static const uint8_t WRITE_REGISTER_BASE_IDX = 0x02;

    // Index of the last writable register
    static const uint8_t WRITE_REGISTER_MAX_IDX = 0x07;

    // Index of the first readable register
    static const uint8_t READ_REG_BASE_IDX = 0x0A;

    // Index of the last reasdable register
    static const uint8_t READ_REG_MAX_IDX = 0x0F;

    // Default values for the register map
    static const uint16_t REGISTER_MAP_DEFAULT_STATE[0x10];

    // This driver makes use of the random access I2C mode, which
    // isn't documented well for this chip. Nonetheless, it's less
    // of a pain to use than the sequential access mode.
    static const uint8_t SEQUENTIAL_ACCESS_I2C_MODE_ADDR = 0x10;
    static const uint8_t RANDOM_ACCESS_I2C_MODE_ADDR = 0x11;

    // Number of bytes in the register map
    static const uint16_t REGISTER_MAP_SIZE_BYTES = sizeof(REGISTER_MAP_DEFAULT_STATE);

    // Number of registers in the register map
    static const uint16_t REGISTER_MAP_SIZE_REGISTERS = sizeof(REGISTER_MAP_DEFAULT_STATE) / sizeof(uint16_t);

    // Band selection constants
    static const uint8_t US_EUR_BAND_SELECT = 0x00;

    static const uint8_t JAPAN_BAND_SELECT = 0x01;

    static const uint8_t WORLD_WIDE_BAND_SELECT = 0x02;

    static const uint8_t EAST_EUR_BAND_SELECT = 0x03;

    // Channel spacing constants
    static const uint8_t CHANNEL_SPACE_100KHZ = 0x00;

    static const uint8_t CHANNEL_SPACE_200KHZ = 0x01;

    static const uint8_t CHANNEL_SPACE_50KHZ = 0x02;

    static const uint8_t CHANNEL_SPACE_25KHZ = 0x03;

    // Deemphasis
    static const uint8_t DEEMP_75_US = 0x00;
    static const uint8_t DEEMP_50_US = 0x01;

    // String values for the StatusResult enum
    static const std::string STATUSRESULT_TO_STRING[];

    static const std::string BLOCK_ERRORS_TO_STRING[];

    // The indices to this array are band selection IDs 0-3
    // The values of this array represent frequencies in MHz,
    // where 870 = 87.0 MHz
    static const uint16_t FREQUENCY_RANGE_MIN[];
    static const uint16_t FREQUENCY_RANGE_MAX[];

    /////////////////////////////////
    // Private interface functions //
    /////////////////////////////////
    void init();
    StatusResult setI2cAddress(uint8_t addr);
    StatusResult conditionallyWriteRegisterToDevice(Register regToWrite, bool shouldWrite);

    //////////////////////////////
    // Private member variables //
    //////////////////////////////

    // The register map of the device. The first register is 0x00
    // and the last is 0x0F. Each register is two bytes (16 bits)
    uint16_t registers[0x0F];

    // The current band (freq range)
    Band band;

    // The I2c interface used to talk to the radio
    mraa::I2c i2cInterface;

};

#endif  // ifndef RDA5807M_HPP
