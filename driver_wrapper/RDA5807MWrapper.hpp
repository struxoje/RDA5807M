/*
 * RDA5807MWrapper.hpp
 *
 *  Created on: Jul 1, 2017
 *      Author: bensherman
 */

#ifndef DRIVER_WRAPPER_RDA5807MWRAPPER_HPP_
#define DRIVER_WRAPPER_RDA5807MWRAPPER_HPP_

// System Includes
#include <cstdint>
#include <string>

// Project Includes
#include "RadioResult.hpp"
#include "RDA5807M.hpp"

class RDA5807MWrapper
{
public:
    //////////////////////
    // Enum Definitions //
    //////////////////////
    enum class StatusResult
    {
        SUCCESS = 0, ABOVE_MAX = 1, BELOW_MIN = 2, GENERAL_FAILURE = 3
    };

    ////////////////////////////////
    // Public interface functions //
    ////////////////////////////////
    RDA5807MWrapper(RDA5807M& radioParam) : radio(radioParam) { };

    static std::string resultToString(StatusResult toConvert);
    RadioResult<StatusResult> setFrequency(int freq);
    RadioResult<StatusResult> setVolume(int vol);
    RadioResult<StatusResult> printStatus(int UNUSED);
    RadioResult<StatusResult> setMute(int mute);

private:
    ////////////////////////////////////
    // Private Static Class Constants //
    ////////////////////////////////////
    static const std::string RESULT_TO_STRING[];

    ///////////////////////////
    // Private Class Members //
    ///////////////////////////
    RDA5807M& radio;
};

#endif /* DRIVER_WRAPPER_RDA5807MWRAPPER_HPP_ */
