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
RDA5807M::StatusResult RDA5807MWrapper::setFrequency(int freq)
{
    // Frequency ranges assume US/Europe band selection
    if (freq < 870)
    {
        return RDA5807M::StatusResult { RDA5807M::StatusResult::BELOW_MIN };
    }
    else if (freq > 1080)
    {
        return RDA5807M::StatusResult { RDA5807M::StatusResult::ABOVE_MAX };
    }

    radio.setChannel(static_cast<uint16_t>(freq), false);
    return radio.setTune(true);
}

/**
 * Sets the radio volume. The minimum volume is 0, and the maximum volume
 * is 15
 */
RDA5807M::StatusResult RDA5807MWrapper::setVolume(int vol)
{
    if (vol > RDA5807M::MAX_VOLUME)
    {
        return RDA5807M::StatusResult { RDA5807M::StatusResult::ABOVE_MAX };
    }

    return radio.setVolume(static_cast<uint8_t>(vol));
}

/*
 * Mute enabled if muteEnable=1, disable mute if muteEnable=0
 */
RDA5807M::StatusResult RDA5807MWrapper::setMute(int muteEnable)
{
    return radio.setMute(Util::boolFromInteger(muteEnable));
}

/**
 * Bass boost enabled if bassBoostEnable=1, disable bass boost if bassBoostEnable=0
 */
RDA5807M::StatusResult RDA5807MWrapper::setBassBoost(int bassBoostEnable)
{
    return radio.setBassBoost(Util::boolFromInteger(bassBoostEnable));
}

/**
 * Reinitializes the radio when radioEnable == 1
 * Disables the radio when radioEnable == 0
 */
RDA5807M::StatusResult RDA5807MWrapper::setRadioEnableState(int radioEnable)
{
    bool isEnableRequested = Util::boolFromInteger(radioEnable);

    if (isEnableRequested)
    {
        radio.reset();
    }

    return radio.setEnabled(isEnableRequested);
}

std::string RDA5807MWrapper::getStatusString(int UNUSED)
{
    (void) UNUSED;

    radio.readDeviceRegistersAndStoreLocally();

    std::string status{""};
    char buffer[100] = {0};

    std::sprintf(buffer, "New RDS/RBDS group ready?: %s\n", radio.isRdsReady() ? "Yes" : "No");
    status.append(buffer);

    std::sprintf(buffer, "Seek/Tune complete: %s\n", radio.isStcComplete() ? "Complete" : "Not Complete");
    status.append(buffer);

    std::sprintf(buffer, "Seek result: %s\n", radio.didSeekFail() ? "Successful" : "Failure");
    status.append(buffer);

    std::sprintf(buffer, "RDS Sync'd?: %s\n", radio.isRdsDecoderSynchronized() ? "Synchronized" : "Not synchronized");
    status.append(buffer);

    std::sprintf(buffer, "Has Block E been found?: %s\n", radio.hasBlkEBeenFound() ? "Yes" : "No");
    status.append(buffer);

    std::sprintf(buffer, "Audio type: %s\n", radio.isStereoEnabled() ? "Stereo" : "Mono");
    status.append(buffer);

    std::sprintf(buffer, "Read channel: %u\n", radio.getReadChannel());
    status.append(buffer);

    std::sprintf(buffer, "RSSI: 0x%02x\n", radio.getRssi());
    status.append(buffer);

    std::sprintf(buffer, "Is this freq a station?: %s\n", radio.isFmTrue() ? "Yes" : "No");
    status.append(buffer);

    std::sprintf(buffer, "FM Ready?: %s\n", radio.isFmReady() ? "Yes" : "No");
    status.append(buffer);

    return status;
}

std::string RDA5807MWrapper::getRegisterMapString(int UNUSED)
{
    (void) UNUSED;

    return radio.getRegisterMap();
}

RDA5807M::StatusResult RDA5807MWrapper::setHighImpedanceOutput(int highImpedanceOutputEnable)
{
    return radio.setHighImpedanceOutput(Util::boolFromInteger(highImpedanceOutputEnable));
}

RDA5807M::StatusResult RDA5807MWrapper::setStereo(int stereoEnable)
{
    return radio.setStereo(Util::boolFromInteger(stereoEnable));
}

RDA5807M::StatusResult RDA5807MWrapper::setSeek(int seekEnable)
{
    return radio.setSeek(Util::boolFromInteger(seekEnable));
}

RDA5807M::StatusResult RDA5807MWrapper::setRDS(int rdsEnable)
{
    return radio.setRdsMode(Util::boolFromInteger(rdsEnable));
}

RDA5807M::StatusResult RDA5807MWrapper::setNewMethod(int newMethodEnable)
{
    return radio.setNewMethod(Util::boolFromInteger(newMethodEnable));
}

RDA5807M::StatusResult RDA5807MWrapper::setSoftReset(int softResetEnable)
{
    return radio.setSoftReset(Util::boolFromInteger(softResetEnable));
}

RDA5807M::StatusResult RDA5807MWrapper::setSoftMute(int softMuteEnable)
{
    return radio.setSoftMute(Util::boolFromInteger(softMuteEnable));
}

RDA5807M::StatusResult RDA5807MWrapper::setTune(int tuneEnable)
{
    return radio.setTune(Util::boolFromInteger(tuneEnable));
}

/**
 * If the param is 1, then the function will
 * search for RDS (long mode). Otherwise, no RDS info is shown.
 */
std::string RDA5807MWrapper::generateFreqMap(int length)
{

    std::string results{""};
    radio.setMute(true);
    for (int freq = 880; freq <= 1080; freq+=1)
    {
        setFrequency(freq);
        if (length == 1)
        {
            usleep(1000*MICROS_IN_MILLIS);
        }
        else
        {
            usleep(120*MICROS_IN_MILLIS);
        }

        // Generate freq bars
        uint8_t rssi = radio.getRssi();
        char barBuff[129] = {0};
        std::memset(barBuff, '|', rssi);
        barBuff[rssi] = '\0';

        char fullBuff[160] = {0};
        if (length == 1)
        {
            std::sprintf(fullBuff, "Freq: %04u  RDS: %s  RSSI(%03u): %s\n", freq, radio.isRdsDecoderSynchronized() ? "Y" : "N", rssi, barBuff);
        }
        else
        {
            std::sprintf(fullBuff, "Freq: %04u RSSI(%03u): %s\n", freq, rssi, barBuff);
        }
        results.append(fullBuff);
    }
    return results;
}

RDA5807M::StatusResult RDA5807MWrapper::setAFCD(int afcdEnable)
{
    return radio.setAFCD(Util::boolFromInteger(afcdEnable));
}

RDA5807M::StatusResult RDA5807MWrapper::setDeEmphasis(int deEmphasisSelector)
{
    return radio.setDeEmphasis(static_cast<RDA5807M::DeEmphasis>(deEmphasisSelector));
}

RDA5807M::StatusResult RDA5807MWrapper::setBand(int bandSelector)
{
    return radio.setBand(static_cast<RDA5807M::Band>(bandSelector));
}

RDA5807M::StatusResult RDA5807MWrapper::setChannelSpacing(int channelSpacingSelector)
{
    return radio.setChannelSpacing(static_cast<RDA5807M::ChannelSpacing>(channelSpacingSelector));
}

RDA5807M::StatusResult RDA5807MWrapper::setSeekDirection(int seekDirSelector)
{
    return radio.setSeekDirection(static_cast<RDA5807M::SeekDirection>(seekDirSelector));
}

RDA5807M::StatusResult RDA5807MWrapper::setSeekMode(int seekModeSelector)
{
    return radio.setSeekMode(static_cast<RDA5807M::SeekMode>(seekModeSelector));
}

RDA5807M::StatusResult RDA5807MWrapper::setSoftBlend(int softBlendEnable)
{
    return radio.setSoftBlend(Util::boolFromInteger(softBlendEnable));
}

uint32_t RDA5807MWrapper::getRssi(int UNUSED)
{
    (void) UNUSED;

    return radio.getRssi();
}

uint32_t RDA5807MWrapper::getRdsPiCode(int UNUSED)
{
    (void) UNUSED;
    return radio.getRdsPiCode();
}

uint32_t RDA5807MWrapper::getRdsGroupTypeCode(int UNUSED)
{
    (void) UNUSED;
    return radio.getRdsGroupTypeCode();
}

uint32_t RDA5807MWrapper::getRdsVersionCode(int UNUSED)
{
    (void) UNUSED;
    return radio.getRdsVersionCode();
}

uint32_t RDA5807MWrapper::getRdsTrafficProgramIdCode(int UNUSED)
{
    (void) UNUSED;
    return radio.getRdsTrafficProgramIdCode();
}

uint32_t RDA5807MWrapper::getRdsProgramTypeCode(int UNUSED)
{
    (void) UNUSED;
    return radio.getRdsProgramTypeCode();
}

RDA5807M::StatusResult RDA5807MWrapper::updateLocalRegisterMapFromDevice(int UNUSED)
{
    (void) UNUSED;
    radio.readDeviceRegistersAndStoreLocally();
    return RDA5807M::StatusResult::SUCCESS;
}

std::string RDA5807MWrapper::getRdsInfoString(int UNUSED)
{
    (void) UNUSED;

    (void) UNUSED;

    radio.readDeviceRegistersAndStoreLocally();

    std::string status{""};
    char buffer[150] = {0};

    std::sprintf(buffer, "New RDS/RBDS Group Ready?: %s\n", radio.isRdsReady() ? "Yes" : "No");
    status.append(buffer);

    std::sprintf(buffer, "RDS Decoder Synchronized?: %s\n", radio.isRdsDecoderSynchronized() ? "Yes" : "No");
    status.append(buffer);

    std::sprintf(buffer, "Block E Found?: %s\n", radio.hasBlkEBeenFound() ? "Yes" : "No");
    status.append(buffer);

    std::sprintf(buffer, "Program ID Code: 0x%04x\n", radio.getRdsTrafficProgramIdCode());
    status.append(buffer);

    std::sprintf(buffer, "Group Type: %02u%s\n", radio.getRdsGroupTypeCode(), radio.getRdsVersionCode() ? "B" : "A");
    status.append(buffer);

    std::sprintf(buffer, "Traffic Program?: %s\n", radio.getRdsTrafficProgramIdCode() ? "Yes" : "No");
    status.append(buffer);

    std::sprintf(buffer, "Program Type?: %02u\n", radio.getRdsProgramTypeCode());
    status.append(buffer);

    std::sprintf(buffer, "Block A Register: 0x%04x\n", radio.getLocalRegisterContent(RDA5807M::Register::BLOCK_A));
    status.append(buffer);

    std::sprintf(buffer, "Block B Register: 0x%04x\n", radio.getLocalRegisterContent(RDA5807M::Register::BLOCK_B));
    status.append(buffer);

    std::sprintf(buffer, "Block C Register: 0x%04x (%c%c)\n", radio.getLocalRegisterContent(RDA5807M::Register::BLOCK_C),
                Util::valueFromReg(radio.getLocalRegisterContent(RDA5807M::Register::BLOCK_C), UINT16_UPPER_BYTE),
                Util::valueFromReg(radio.getLocalRegisterContent(RDA5807M::Register::BLOCK_C), UINT16_LOWER_BYTE));
    status.append(buffer);

    std::sprintf(buffer, "Block D Register: 0x%04x (%c%c)\n", radio.getLocalRegisterContent(RDA5807M::Register::BLOCK_D),
            Util::valueFromReg(radio.getLocalRegisterContent(RDA5807M::Register::BLOCK_D), UINT16_UPPER_BYTE),
            Util::valueFromReg(radio.getLocalRegisterContent(RDA5807M::Register::BLOCK_D), UINT16_LOWER_BYTE));
    status.append(buffer);
    return status;

}

std::string RDA5807MWrapper::getLocalCopyOfReg(int reg)
{
    char buff[10] = {0};
    std::sprintf(buff, "0x%04x", radio.getLocalRegisterContent(static_cast<RDA5807M::Register>(reg)));
    return buff;
}

