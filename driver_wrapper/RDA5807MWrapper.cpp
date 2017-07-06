/*
 * RDA5807MWrapper.cpp
 *
 *  Created on: Jul 1, 2017
 *      Author: bensherman
 */

// System includes
#include <string>
#include <unistd.h>

// Project includes
#include "RadioResult.hpp"
#include "RDA5807M.hpp"
#include "RDA5807MWrapper.hpp"
#include "Util.hpp"

/**
 * Sets the radio frequency. The frequency is to be provided as an integer.
 * For example: 104.3MHz is passed as 1043
 * The minimum frequency is 87 MHz, and the maximum is 108MHz; US/European bands
 * are assumed.
 */
RadioResult<RDA5807M::StatusResult> RDA5807MWrapper::setFrequency(int freq)
{
    // Frequency ranges assume US/Europe band selection
    if (freq < 870)
    {
        return RadioResult<RDA5807M::StatusResult> { RDA5807M::StatusResult::BELOW_MIN };
    }
    else if (freq > 1080)
    {
        return RadioResult<RDA5807M::StatusResult> { RDA5807M::StatusResult::ABOVE_MAX };
    }

    radio.setChannel(static_cast<uint16_t>(freq), false);
    return radio.setTune(true);
}

/**
 * Sets the radio volume. The minimum volume is 0, and the maximum volume
 * is 15
 */
RadioResult<RDA5807M::StatusResult> RDA5807MWrapper::setVolume(int vol)
{
    if (vol > RDA5807M::MAX_VOLUME)
    {
        return RadioResult<RDA5807M::StatusResult> { RDA5807M::StatusResult::ABOVE_MAX };
    }

    return radio.setVolume(static_cast<uint8_t>(vol));
}

/*
 * Mute enabled if muteEnable=1, disable mute if muteEnable=0
 */
RadioResult<RDA5807M::StatusResult> RDA5807MWrapper::setMute(int muteEnable)
{
    return radio.setMute(Util::boolFromInteger(muteEnable));
}

/**
 * Bass boost enabled if bassBoostEnable=1, disable bass boost if bassBoostEnable=0
 */
RadioResult<RDA5807M::StatusResult> RDA5807MWrapper::setBassBoost(int bassBoostEnable)
{
    return radio.setBassBoost(Util::boolFromInteger(bassBoostEnable));
}

/**
 * Reinitializes the radio when radioEnable == 1
 * Disables the radio when radioEnable == 0
 */
RadioResult<RDA5807M::StatusResult> RDA5807MWrapper::setRadioEnableState(int radioEnable)
{
    bool isEnableRequested = Util::boolFromInteger(radioEnable);

    if (isEnableRequested)
    {
        radio.reset();
    }

    return radio.setEnabled(isEnableRequested);
}

RadioResult<std::string> RDA5807MWrapper::getStatusString(int UNUSED)
{
    (void) UNUSED;

    radio.readDeviceRegistersAndStoreLocally();
    return radio.getStatusString();
}

RadioResult<std::string> RDA5807MWrapper::getRegisterMapString(int UNUSED)
{
    (void) UNUSED;

    return radio.getRegisterMap();
}

RadioResult<RDA5807M::StatusResult> RDA5807MWrapper::setHighImpedanceOutput(int highImpedanceOutputEnable)
{
    return radio.setHighImpedanceOutput(Util::boolFromInteger(highImpedanceOutputEnable));
}

RadioResult<RDA5807M::StatusResult> RDA5807MWrapper::setStereo(int stereoEnable)
{
    return radio.setStereo(Util::boolFromInteger(stereoEnable));
}

RadioResult<RDA5807M::StatusResult> RDA5807MWrapper::setSeek(int seekEnable)
{
    return radio.setSeek(Util::boolFromInteger(seekEnable));
}

RadioResult<RDA5807M::StatusResult> RDA5807MWrapper::setRDS(int rdsEnable)
{
    return radio.setRDSMode(Util::boolFromInteger(rdsEnable));
}

RadioResult<RDA5807M::StatusResult> RDA5807MWrapper::setNewMethod(int newMethodEnable)
{
    return radio.setNewMethod(Util::boolFromInteger(newMethodEnable));
}

RadioResult<RDA5807M::StatusResult> RDA5807MWrapper::setSoftReset(int softResetEnable)
{
    return radio.setSoftReset(Util::boolFromInteger(softResetEnable));
}

RadioResult<RDA5807M::StatusResult> RDA5807MWrapper::setSoftMute(int softMuteEnable)
{
    return radio.setSoftMute(Util::boolFromInteger(softMuteEnable));
}

RadioResult<RDA5807M::StatusResult> RDA5807MWrapper::setTune(int tuneEnable)
{
    return radio.setTune(Util::boolFromInteger(tuneEnable));
}

RadioResult<std::string> RDA5807MWrapper::generateFreqMap(int UNUSED)
{
    (void) UNUSED;

    std::string results{""};
    radio.setMute(true);
    for (int freq = 880; freq <= 1080; freq+=1)
    {
        setFrequency(freq);
        usleep(125*MICROS_IN_MILLIS);

        // Generate freq bars
        uint8_t rssi = radio.getRssi();
        char barBuff[129] = {0};
        std::memset(barBuff, '|', rssi);
        barBuff[rssi] = '\0';

        char fullBuff[150] = {0};
        std::sprintf(fullBuff, "Freq: %u\tRSSI: %s\n", freq, barBuff);
        results.append(fullBuff);
    }
    return results;
}

RadioResult<RDA5807M::StatusResult> RDA5807MWrapper::setAFCD(int afcdEnable)
{
    return radio.setAFCD(Util::boolFromInteger(afcdEnable));
}

RadioResult<RDA5807M::StatusResult> RDA5807MWrapper::setDeEmphasis(int deEmphasisSelector)
{
    return radio.setDeEmphasis(static_cast<RDA5807M::DeEmphasis>(deEmphasisSelector));
}

RadioResult<RDA5807M::StatusResult> RDA5807MWrapper::setBand(int bandSelector)
{
    return radio.setBand(static_cast<RDA5807M::Band>(bandSelector));
}

RadioResult<RDA5807M::StatusResult> RDA5807MWrapper::setChannelSpacing(int channelSpacingSelector)
{
    return radio.setChannelSpacing(static_cast<RDA5807M::ChannelSpacing>(channelSpacingSelector));
}

RadioResult<RDA5807M::StatusResult> RDA5807MWrapper::setSeekDirection(int seekDirSelector)
{
    return radio.setSeekDirection(static_cast<RDA5807M::SeekDirection>(seekDirSelector));
}

RadioResult<RDA5807M::StatusResult> RDA5807MWrapper::setSeekMode(int seekModeSelector)
{
    return radio.setSeekMode(static_cast<RDA5807M::SeekMode>(seekModeSelector));
}

RadioResult<RDA5807M::StatusResult> RDA5807MWrapper::setSoftBlend(int softBlendEnable)
{
    return radio.setSoftBlend(Util::boolFromInteger(softBlendEnable));
}

RadioResult<uint32_t> RDA5807MWrapper::getRssi(int UNUSED)
{
    (void) UNUSED;

    return radio.getRssi();
}

