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
        REG_0x0C = 0x0C,
        REG_0x0D = 0x0D,
        REG_0x0E = 0x0E,
        REG_0x0F = 0x0F
    };

    enum SeekDirection {SEEK_UP, SEEK_DOWN};
    enum SeekMode {WRAP_AT_LIMIT, STOP_AT_LIMIT};
    enum I2c_Result {SUCCESS, FAILED};
    enum Band {US_EUR, JAP, WORLD_WIDE, EAST_EUROPE};
    enum ChannelSpacing {ONE_HUND_KHZ, TWO_HUND_KHZ, FIFTY_KHZ, TWENTY_FIVE_KHZ};
    enum DeEmphasis {SEVENTY_FIVE_US, FIFTY_US};

    enum class StatusResult
    {
        SUCCESS = 0, ABOVE_MAX = 1, BELOW_MIN = 2, GENERAL_FAILURE = 3, I2C_FAILURE = 4
    };

    ////////////////////////////////
    // Public interface functions //
    ////////////////////////////////

    RDA5807M();

    // Updates the local register map, but DOES NOT WRITE the changes
    // to the device
    void setRegister(Register regNum, uint16_t value, uint16_t mask = 0xFF);

    I2c_Result writeRegisterToDevice(Register reg);

    I2c_Result writeAllRegistersToDevice();

    void readDeviceRegistersAndStoreLocally();

    uint16_t readRegisterFromDevice(Register reg);

    void printRegisterMap();

    // FUNCTIONS USED TO ENABLE/DISABLE RADIO MODES
    void setMute(bool muteEnable, bool writeResultToDevice = true);

    void setHighImpedanceOutput(bool highImpedanceEnable, bool writeResultToDevice = true);

    void setStereo(bool stereoEnable, bool writeResultToDevice = true);

    void setBassBoost(bool bassBoostEnable, bool writeResultToDevice = true);

    void setSeekDirection(SeekDirection seekDirection, bool writeResultToDevice = true);

    void setSeek(bool seekEnable, bool writeResultToDevice = true);

    void setSeekMode(SeekMode seekMode, bool writeResultToDevice = true);

    void setRDSMode(bool rdsEnable, bool writeResultToDevice = true);

    void setNewMethod(bool newMethodEnable, bool writeResultToDevice = true);

    void setSoftReset(bool softResetEnable, bool writeResultToDevice = true);

    void setEnabled(bool enable, bool writeResultToDevice = true);

    void setChannel(uint16_t channel, bool writeResultToDevice = true);

    void setTune(bool enable, bool writeResultToDevice = true);

    void setBand(Band band, bool writeResultToDevice = true);

    void setChannelSpacing(ChannelSpacing spacing, bool writeResultToDevice = true);

    void setVolume(uint8_t volume, bool writeResultToDevice = true);

    void setDeEmphasis(DeEmphasis de, bool writeResultToDevice = true);

    void setAFCD(bool afcdEnable, bool writeResultToDevice = true);

    void setSoftMute(bool softMuteEnable, bool writeResultToDevice = true);

    bool retrieveUpdateRegAndReturnFlag(Register reg, uint16_t mask);

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

    void printStatus();

    static std::string resultToString(StatusResult toConvert);

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

    // Channel select base freq
    // The indices to this array are band selection IDs 0-3
    // The values of this array represent frequencies in MHz,
    // where 870 = 87.0 MHz
    static const uint16_t CHAN_SELECT_BASES[];

    // String values for the StatusResult enum
    static const std::string RESULT_TO_STRING[];
    /////////////////////////////////
    // Private interface functions //
    /////////////////////////////////
    bool setI2cAddress(uint8_t addr);
    void init();

    //////////////////////////////
    // Private member variables //
    //////////////////////////////

    // The register map of the device. The first register is 0x00
    // and the last is 0x0F. Each register is two bytes (16 bits)
    uint16_t registers[0x0F];

    // The I2c interface used to talk to the radio
    mraa::I2c i2cInterface;

};

#endif  // ifndef RDA5807M_HPP
