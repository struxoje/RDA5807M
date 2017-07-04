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
    ////////////////////////////////
    // Public interface functions //
    ////////////////////////////////
    RDA5807MWrapper(RDA5807M& radioParam) : radio(radioParam) { };

    RadioResult<RDA5807M::StatusResult> setFrequency(int freq);
    RadioResult<RDA5807M::StatusResult> setVolume(int vol);
    RadioResult<RDA5807M::StatusResult> printStatus(int UNUSED);
    RadioResult<RDA5807M::StatusResult> setMute(int muteEnable);
    RadioResult<RDA5807M::StatusResult> setBassBoost(int bassBoostEnable);

private:
    ///////////////////////////
    // Private Class Members //
    ///////////////////////////
    RDA5807M& radio;
};

#endif /* DRIVER_WRAPPER_RDA5807MWRAPPER_HPP_ */
