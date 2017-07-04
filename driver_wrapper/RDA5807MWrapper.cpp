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

    radio.setChannel(static_cast<uint16_t>(freq));
    radio.setTune(true);
    return RadioResult<RDA5807M::StatusResult> { RDA5807M::StatusResult::SUCCESS };;
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

    radio.setVolume(static_cast<uint8_t>(vol));

    return RadioResult<RDA5807M::StatusResult> { RDA5807M::StatusResult::SUCCESS };
}

RadioResult<RDA5807M::StatusResult> RDA5807MWrapper::printStatus(int UNUSED)
{
    radio.readDeviceRegistersAndStoreLocally();
    radio.printStatus();
    return RadioResult<RDA5807M::StatusResult> { RDA5807M::StatusResult::SUCCESS };
}

/*
 * Mute enabled if muteEnable=1, disable mute if muteEnable=0
 */
RadioResult<RDA5807M::StatusResult> RDA5807MWrapper::setMute(int muteEnable)
{
    radio.setMute(Util::boolFromInteger(muteEnable));
    return RadioResult<RDA5807M::StatusResult> { RDA5807M::StatusResult::SUCCESS };
}

/**
 * Bass boost enabled if bassBoostEnable=1, disable bass boost if bassBoostEnable=0
 */
RadioResult<RDA5807M::StatusResult> RDA5807MWrapper::setBassBoost(int bassBoostEnable)
{
    radio.setBassBoost(Util::boolFromInteger(bassBoostEnable));
    return RadioResult<RDA5807M::StatusResult> { RDA5807M::StatusResult::SUCCESS };
}


