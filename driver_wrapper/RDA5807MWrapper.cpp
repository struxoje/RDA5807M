/*
 * RDA5807MWrapper.cpp
 *
 *  Created on: Jul 1, 2017
 *      Author: bensherman
 */

// System includes
#include <cstring>
#include <string>
#include <unistd.h>
#include <stdexcept>

// Project includes
#include "RDA5807M.hpp"
#include "RDA5807MRegDefines.hpp"
#include "RDA5807MWrapper.hpp"
#include "Util.hpp"

/**
 * Sets the radio frequency. The frequency is to be provided as an integer.
 * For example: 104.3MHz is passed as 1043
 * The minimum frequency is 87 MHz, and the maximum is 108MHz; US/European bands
 * are assumed.
 */
void RDA5807MWrapper::setFrequency(const std::string& freqStr, std::string* updatableStr)
{
    (void) updatableStr;

    const uint16_t freq = Util::stringToUInt16(freqStr);

    // Frequency ranges assume US/Europe band selection
    if (freq < radio.getBandMinumumFrequency() ||
        radio.getBandMaximumFrequency() < freq) {
        outOfRangeErrorHelper("Frequency", radio.getBandMaximumFrequency(), radio.getBandMinumumFrequency());
    }
    validateStatusResult(radio.setChannel(freq, false));
    validateStatusResult(radio.setTune(true));
}

/**
 * Sets the radio volume. The minimum volume is 0, and the maximum volume
 * is 15
 */
void RDA5807MWrapper::setVolume(const std::string& volStr, std::string* updatableStr)
{
    (void) updatableStr;

    uint8_t vol;

    try {
        vol = Util::stringToUInt8(volStr);
    } catch (const std::out_of_range& err) {
        outOfRangeErrorHelper("Volume", RDA5807M::MAX_VOLUME, 0);
    }

    validateStatusResult(radio.setVolume(vol));
}

/*
 * Mute enabled if muteEnable=true/1, disable mute if muteEnable=false/0
 */
void RDA5807MWrapper::setMute(const std::string& muteEnable, std::string* updatableStr) {
    (void) updatableStr;

    validateStatusResult(radio.setMute(Util::boolFromString(muteEnable)));
}

/**
 * Bass boost enabled if bassBoostEnable=1/true,
 * disable bass boost if bassBoostEnable=0/false
 */
void RDA5807MWrapper::setBassBoost(const std::string& bassBoostEnableStr, std::string* updatableStr) {
    (void) updatableStr;
    validateStatusResult(radio.setBassBoost(Util::boolFromString(bassBoostEnableStr)));
}

/**
 * Reinitializes the radio when radioEnable == 1/true
 * Disables the radio when radioEnable == 0/false
 */
void RDA5807MWrapper::setRadioEnableState(const std::string& enableStr, std::string* updatableStr)
{
    (void) updatableStr;

    bool isEnableRequested = Util::boolFromString(enableStr);

    if (isEnableRequested)
    {
        radio.reset();
    }

    validateStatusResult(radio.setEnabled(isEnableRequested));
}

void RDA5807MWrapper::getStatusString(const std::string& param, std::string* updatableStr) {
    (void) param;

    radio.readDeviceRegistersAndStoreLocally();

    char buffer[100] = {0};

    std::sprintf(buffer, "New RDS/RBDS group ready?: %s\n", radio.isRdsReady() ? "Yes" : "No");
    updatableStr->append(buffer);

    std::sprintf(buffer, "Seek/Tune complete: %s\n", radio.isStcComplete() ? "Complete" : "Not Complete");
    updatableStr->append(buffer);

    std::sprintf(buffer, "Seek result: %s\n", radio.didSeekFail() ? "Successful" : "Failure");
    updatableStr->append(buffer);

    std::sprintf(buffer, "RDS Sync'd?: %s\n", radio.isRdsDecoderSynchronized() ? "Synchronized" : "Not synchronized");
    updatableStr->append(buffer);

    std::sprintf(buffer, "Has Block E been found?: %s\n", radio.hasBlkEBeenFound() ? "Yes" : "No");
    updatableStr->append(buffer);

    std::sprintf(buffer, "Audio type: %s\n", radio.isStereoEnabled() ? "Stereo" : "Mono");
    updatableStr->append(buffer);

    std::sprintf(buffer, "Read channel: %u\n", radio.getReadChannel());
    updatableStr->append(buffer);

    std::sprintf(buffer, "RSSI: 0x%02x\n", radio.getRssi());
    updatableStr->append(buffer);

    std::sprintf(buffer, "Is this freq a station?: %s\n", radio.isFmTrue() ? "Yes" : "No");
    updatableStr->append(buffer);

    std::sprintf(buffer, "FM Ready?: %s\n", radio.isFmReady() ? "Yes" : "No");
    updatableStr->append(buffer);
}

void RDA5807MWrapper::getRegisterMapString(const std::string& param,
                                           std::string* updatableStr) {
    (void) param;

    updatableStr->append(radio.getRegisterMap());
}

void RDA5807MWrapper::setHighImpedanceOutput(const std::string& highImpedanceOutputEnable, std::string* updatableStr) {

    (void) updatableStr;
    validateStatusResult(radio.setHighImpedanceOutput(Util::boolFromString(highImpedanceOutputEnable)));
}

void RDA5807MWrapper::setStereo(const std::string& stereoEnable, std::string* updatableStr) {

    (void) updatableStr;
    validateStatusResult(radio.setStereo(Util::boolFromString(stereoEnable)));
}

void RDA5807MWrapper::setSeek(const std::string& seekEnable, std::string* updatableStr) {

    (void) updatableStr;
    validateStatusResult(radio.setSeek(Util::boolFromString(seekEnable)));
}

void RDA5807MWrapper::setRDS(const std::string& rdsEnable, std::string* updatableStr) {

    (void) updatableStr;
    validateStatusResult(radio.setRdsMode(Util::boolFromString(rdsEnable)));
}

void RDA5807MWrapper::setNewMethod(const std::string& newMethodEnable, std::string* updatableStr) {

    (void) updatableStr;
    validateStatusResult(radio.setNewMethod(Util::boolFromString(newMethodEnable)));
}

void RDA5807MWrapper::setSoftReset(const std::string& softResetEnable, std::string* updatableStr) {

    (void) updatableStr;
    validateStatusResult(radio.setSoftReset(Util::boolFromString(softResetEnable)));
}

void RDA5807MWrapper::setSoftMute(const std::string& softMuteEnable, std::string* updatableStr) {
    (void) updatableStr;
    validateStatusResult(radio.setSoftMute(Util::boolFromString(softMuteEnable)));
}

void RDA5807MWrapper::setTune(const std::string& tuneEnable, std::string* updatableStr) {

    (void) updatableStr;
    validateStatusResult(radio.setTune(Util::boolFromString(tuneEnable)));
}

/**
 * If the param is 1, then the function will
 * search for RDS (long mode). Otherwise, no RDS info is shown.
 */
void RDA5807MWrapper::generateFreqMap(const std::string& lengthStr, std::string* updatableStr) {

    const uint16_t length = Util::stringToUInt16(lengthStr);

    validateStatusResult(radio.setMute(true));

    for (int freq = 881; freq <= 1081; freq+=2)
    {
        // Set the freq
        validateStatusResult(radio.setChannel(freq, false));
        validateStatusResult(radio.setTune(true));

        if (length == 1) {
            radio.setRdsMode(false);
            radio.setRdsMode(true);
            usleep(1000*MICROS_IN_MILLIS);
        } else {
            usleep(120*MICROS_IN_MILLIS);
        }

        // Generate freq bars
        uint8_t rssi = radio.getRssi();
        char barBuff[129] = {0};
        std::memset(barBuff, '|', rssi);
        barBuff[rssi] = '\0';

        char fullBuff[160] = {0};
        if (length == 1) {
            std::sprintf(fullBuff, "Freq: %04u  RDS: %s  RSSI(%03u): %s\n", freq, radio.isRdsDecoderSynchronized() ? "Y" : "N", rssi, barBuff);
        } else {
            std::sprintf(fullBuff, "Freq: %04u RSSI(%03u): %s\n", freq, rssi, barBuff);
        }
        updatableStr->append(fullBuff);
    }
}

void RDA5807MWrapper::setAFCD(const std::string& afcdEnable, std::string* updatableStr) {

    (void) updatableStr;
    validateStatusResult(radio.setAFCD(Util::boolFromString(afcdEnable)));
}

void RDA5807MWrapper::setDeEmphasis(const std::string& deEmphasisSelectorStr, std::string* updatableStr) {

    (void) updatableStr;

    const uint8_t deEmphasisSelector = Util::stringToUInt8(deEmphasisSelectorStr);
    const auto enumSize = static_cast<uint8_t>(RDA5807M::DeEmphasis::SIZE);

    if (deEmphasisSelector >= enumSize) {
        outOfRangeErrorHelper("Deemphasis", enumSize - 1, 0);
    }
    validateStatusResult(radio.setDeEmphasis(static_cast<RDA5807M::DeEmphasis>(deEmphasisSelector)));
}

void RDA5807MWrapper::setBand(const std::string& bandSelectorStr, std::string* updatableStr) {

    (void) updatableStr;
    const uint8_t bandSelector = Util::stringToUInt8(bandSelectorStr);
    const auto enumSize = static_cast<uint8_t>(RDA5807M::Band::SIZE);

    if (bandSelector >= enumSize) {
        outOfRangeErrorHelper("Band selection", enumSize - 1, 0);
    }

    validateStatusResult(radio.setBand(static_cast<RDA5807M::Band>(bandSelector)));
}

void RDA5807MWrapper::setChannelSpacing(const std::string& channelSpacingSelectorStr, std::string* updatableStr) {

    (void) updatableStr;

    const uint8_t channelSpacingSelector = Util::stringToUInt8(channelSpacingSelectorStr);
    const auto enumSize = static_cast<uint8_t>(RDA5807M::ChannelSpacing::SIZE);

    if (channelSpacingSelector >= enumSize) {
        outOfRangeErrorHelper("Channel spacing", enumSize - 1, 0);
    }
    validateStatusResult(radio.setChannelSpacing(static_cast<RDA5807M::ChannelSpacing>(channelSpacingSelector)));
}

void RDA5807MWrapper::setSeekDirection(const std::string& seekDirSelectorStr, std::string* updatableStr) {

    (void) updatableStr;

    const uint8_t seekDirSelector = Util::stringToUInt8(seekDirSelectorStr);
    const auto enumSize = static_cast<uint8_t>(RDA5807M::SeekDirection::SIZE);

    if (seekDirSelector >= enumSize) {
        outOfRangeErrorHelper("Seek direction", enumSize - 1, 0);
    }

    validateStatusResult(radio.setSeekDirection(static_cast<RDA5807M::SeekDirection>(seekDirSelector)));
}

void RDA5807MWrapper::setSeekMode(const std::string& seekModeSelectorStr, std::string* updatableStr) {

    (void) updatableStr;

    const uint8_t seekModeSelector = Util::stringToUInt8(seekModeSelectorStr);
    const auto enumSize = static_cast<uint8_t>(RDA5807M::SeekMode::SIZE);

    if (seekModeSelector >= enumSize) {
        outOfRangeErrorHelper("Seek mode", enumSize - 1, 0);
    }

    validateStatusResult(radio.setSeekMode(static_cast<RDA5807M::SeekMode>(seekModeSelector)));
}

void RDA5807MWrapper::setSoftBlend(const std::string& softBlendEnable, std::string* updatableStr) {

    (void) updatableStr;
    validateStatusResult(radio.setSoftBlend(Util::boolFromString(softBlendEnable)));
}

void RDA5807MWrapper::getRssi(const std::string& param, std::string* updatableStr) {

    (void) param;
    updatableStr->append(std::to_string(radio.getRssi()));
}

void RDA5807MWrapper::getRdsPiCode(const std::string& param, std::string* updatableStr) {

    (void) param;
    updatableStr->append(std::to_string(radio.getRdsPiCode()));
}

void RDA5807MWrapper::getRdsGroupTypeCode(const std::string& param, std::string* updatableStr) {

    (void) param;
    updatableStr->append(std::to_string(radio.getRdsGroupTypeCode()));
}

void RDA5807MWrapper::getRdsVersionCode(const std::string& param, std::string* updatableStr) {

    (void) param;
    updatableStr->append(std::to_string(radio.getRdsVersionCode()));
}

void RDA5807MWrapper::getRdsTrafficProgramIdCode(const std::string& param, std::string* updatableStr) {

    (void) param;
    updatableStr->append(std::to_string(radio.getRdsTrafficProgramIdCode()));
}

void RDA5807MWrapper::getRdsProgramTypeCode(const std::string& param, std::string* updatableStr) {

    (void) param;
    updatableStr->append(std::to_string(radio.getRdsProgramTypeCode()));
}

void RDA5807MWrapper::updateLocalRegisterMapFromDevice(const std::string& param, std::string* updatableStr) {

    (void) param;
    (void) updatableStr;

    radio.readDeviceRegistersAndStoreLocally();
}

void RDA5807MWrapper::getRdsInfoString(const std::string& param, std::string* updatableStr) {

    (void) param;

    radio.readDeviceRegistersAndStoreLocally();

    char buffer[350] = {0};

    std::sprintf(buffer, "New RDS/RBDS Group Ready?: %s\n", radio.isRdsReady() ? "Yes" : "No");
    updatableStr->append(buffer);

    std::sprintf(buffer, "RDS Decoder Synchronized?: %s\n", radio.isRdsDecoderSynchronized() ? "Yes" : "No");
    updatableStr->append(buffer);

    std::sprintf(buffer, "Block E Found?: %s\n", radio.hasBlkEBeenFound() ? "Yes" : "No");
    updatableStr->append(buffer);

    std::sprintf(buffer, "Program ID Code: 0x%04x\n", radio.getRdsTrafficProgramIdCode());
    updatableStr->append(buffer);

    std::sprintf(buffer, "Group Type: %02u%s\n", radio.getRdsGroupTypeCode(), radio.getRdsVersionCode() ? "B" : "A");
    updatableStr->append(buffer);

    std::sprintf(buffer, "Traffic Program?: %s\n", radio.getRdsTrafficProgramIdCode() ? "Yes" : "No");
    updatableStr->append(buffer);

    std::sprintf(buffer, "Program Type?: %02u\n", radio.getRdsProgramTypeCode());
    updatableStr->append(buffer);

    std::sprintf(buffer, "Block A Register: 0x%04x\n", radio.getLocalRegisterContent(RDA5807M::Register::BLOCK_A));
    updatableStr->append(buffer);

    std::sprintf(buffer, "Errors on Block A: %s\n", radio.rdsBlockErrorToString(radio.getRdsErrorsForBlock(RDA5807M::Register::BLOCK_A)).c_str());
    updatableStr->append(buffer);

    std::sprintf(buffer, "Block B Register: 0x%04x\n", radio.getLocalRegisterContent(RDA5807M::Register::BLOCK_B));
    updatableStr->append(buffer);

    std::sprintf(buffer, "Errors on Block B: %s\n", radio.rdsBlockErrorToString(radio.getRdsErrorsForBlock(RDA5807M::Register::BLOCK_B)).c_str());
    updatableStr->append(buffer);

    std::sprintf(buffer, "Block C Register: 0x%04x (%c%c)\n", radio.getLocalRegisterContent(RDA5807M::Register::BLOCK_C),
                Util::valueFromReg(radio.getLocalRegisterContent(RDA5807M::Register::BLOCK_C), UINT16_UPPER_BYTE),
                Util::valueFromReg(radio.getLocalRegisterContent(RDA5807M::Register::BLOCK_C), UINT16_LOWER_BYTE));
    updatableStr->append(buffer);

    std::sprintf(buffer, "Block D Register: 0x%04x (%c%c)\n", radio.getLocalRegisterContent(RDA5807M::Register::BLOCK_D),
            Util::valueFromReg(radio.getLocalRegisterContent(RDA5807M::Register::BLOCK_D), UINT16_UPPER_BYTE),
            Util::valueFromReg(radio.getLocalRegisterContent(RDA5807M::Register::BLOCK_D), UINT16_LOWER_BYTE));
    updatableStr->append(buffer);

}

void RDA5807MWrapper::getLocalCopyOfReg(const std::string& regStr, std::string* updatableStr) {

    const uint8_t reg = Util::stringToUInt8(regStr);

    char buff[10] = {0};
    std::sprintf(buff, "0x%04x", radio.getLocalRegisterContent(static_cast<RDA5807M::Register>(reg)));
    updatableStr->append(buff);
}

/**
 * Prints the contents block C and D registers when they contain group 2 data.
 * The process runs for ms milliseconds.
 * The RDS registers are queried approximately every 10 ms.
 */
void RDA5807MWrapper::snoopRdsGroupTwo(const std::string& millisecondsStr, std::string* updatableStr) {

    const uint8_t msToWait = 10;
    int numRetries = Util::stringToUInt16(millisecondsStr)/msToWait;

    char charBuff[15] = {};
    std::string strBuff;
    for (int retryIdx = 0; retryIdx < numRetries; ++retryIdx)
    {
        radio.readDeviceRegistersAndStoreLocally();
        if (radio.isRdsDecoderSynchronized() && radio.getRdsGroupTypeCode() == 2)
        {
            sprintf(charBuff, "%c%c%c%c",
                    Util::valueFromReg(radio.getLocalRegisterContent(RDA5807M::Register::BLOCK_C), UINT16_UPPER_BYTE),
                    Util::valueFromReg(radio.getLocalRegisterContent(RDA5807M::Register::BLOCK_C), UINT16_LOWER_BYTE),
                    Util::valueFromReg(radio.getLocalRegisterContent(RDA5807M::Register::BLOCK_D), UINT16_UPPER_BYTE),
                    Util::valueFromReg(radio.getLocalRegisterContent(RDA5807M::Register::BLOCK_D), UINT16_LOWER_BYTE));
            strBuff += charBuff;

            // Toggle RDS. This clears the synchronized flag
            radio.setRdsMode(false);
            radio.setRdsMode(true);
        }
        usleep(msToWait * MICROS_IN_MILLIS);
    }

    updatableStr->append(strBuff);
}

void RDA5807MWrapper::validateStatusResult(RDA5807M::StatusResult result){
    switch(result) {
        case RDA5807M::StatusResult::I2C_FAILURE:
            throw std::runtime_error("I2C error");
        case RDA5807M::StatusResult::GENERAL_FAILURE:
            throw std::runtime_error("Internal error");
        case RDA5807M::StatusResult::ABOVE_MAX:
            throw std::range_error("Parameter value above maximum");
        case RDA5807M::StatusResult::BELOW_MIN:
            throw std::range_error("Parameter value below minimum");
        case RDA5807M::StatusResult::INVALID_PARAMETER:
            throw std::invalid_argument("Invalid parameter value");
        case RDA5807M::StatusResult::SUCCESS:
            return;
        default:
            throw std::runtime_error("Internal error");
    }
}

void RDA5807MWrapper::outOfRangeErrorHelper(const std::string& quantity,
                                                uint32_t max, uint32_t min) {
    throw std::out_of_range(quantity + " must be in range: [" +
                            std::to_string(min) + "," + std::to_string(max) +
                            "]");
}

RDA5807MWrapper::Builder::Builder() {

}

//RDA5807MWrapper::Builder& RDA5807MWrapper::Builder::withRda5807m(RDA5807M&& rda5807m) {
//    this->rda5807m = std::move(rda5807m);
//    return *this;
//}

RDA5807MWrapperSharedPtr RDA5807MWrapper::Builder::build() {
    return RDA5807MWrapperSharedPtr(new RDA5807MWrapper{});
}

RDA5807M& RDA5807MWrapper::getRadio() {
    return radio;
}



