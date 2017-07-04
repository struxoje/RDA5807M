/*
 * RDA5807MWrapper.cpp
 *
 *  Created on: Jul 1, 2017
 *      Author: bensherman
 */

// System includes
#include <string>

// Project includes
#include "RadioResult.hpp"
#include "RDA5807M.hpp"
#include "RDA5807MWrapper.hpp"
#include "Util.hpp"

// Static Initialization
const std::string RDA5807MWrapper::RESULT_TO_STRING[] = { "SUCCESS", "ABOVE_MAX", "BELOW_MIN", "GENERAL_FAILURE" };

/**
 * Returns the string value of the result parameter
 */
std::string RDA5807MWrapper::resultToString(StatusResult toConvert)
{
    return RESULT_TO_STRING[static_cast<unsigned int>(toConvert)];
}

/**
 * Sets the radio frequency. The frequency is to be provided as an integer.
 * For example: 104.3MHz is passed as 1043
 * The minimum frequency is 87 MHz, and the maximum is 108MHz; US/European bands
 * are assumed.
 */
RadioResult<RDA5807MWrapper::StatusResult> RDA5807MWrapper::setFrequency(int freq)
{
    // Frequency ranges assume US/Europe band selection
    if (freq < 870)
    {
        return RadioResult<StatusResult> { StatusResult::BELOW_MIN };
    }
    else if (freq > 1080)
    {
        return RadioResult<StatusResult> { StatusResult::ABOVE_MAX };
    }

    radio.setChannel(static_cast<uint16_t>(freq));
    radio.setTune(true);
    return RadioResult<StatusResult> { StatusResult::SUCCESS };;
}

/**
 * Sets the radio volume. The minimum volume is 0, and the maximum volume
 * is 15
 */
RadioResult<RDA5807MWrapper::StatusResult> RDA5807MWrapper::setVolume(int vol)
{
    if (vol > RDA5807M::MAX_VOLUME)
    {
        return RadioResult<StatusResult> { StatusResult::ABOVE_MAX };
    }

    radio.setVolume(static_cast<uint8_t>(vol));

    return RadioResult<StatusResult> { StatusResult::SUCCESS };
}

RadioResult<RDA5807MWrapper::StatusResult> RDA5807MWrapper::printStatus(int UNUSED)
{
    radio.readDeviceRegistersAndStoreLocally();
    radio.printStatus();
    return RadioResult<StatusResult> { StatusResult::SUCCESS };
}

/*
 * Mute enabled if muteEnable=1, disable mute if muteEnable=0
 */
RadioResult<RDA5807MWrapper::StatusResult> RDA5807MWrapper::setMute(int muteEnable)
{
    radio.setMute(Util::boolFromInteger(muteEnable));
    return RadioResult<StatusResult> { StatusResult::SUCCESS };
}

/**
 * Bass boost enabled if bassBoostEnable=1, disable bass boost if bassBoostEnable=0
 */
RadioResult<RDA5807MWrapper::StatusResult> RDA5807MWrapper::setBassBoost(int bassBoostEnable)
{
    radio.setBassBoost(Util::boolFromInteger(bassBoostEnable));
    return RadioResult<StatusResult> { StatusResult::SUCCESS };
}


