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
    RadioResult<RDA5807M::StatusResult> setMute(int muteEnable);
    RadioResult<RDA5807M::StatusResult> setBassBoost(int bassBoostEnable);
    RadioResult<RDA5807M::StatusResult> setRadioEnableState(int radioEnable);
    RadioResult<RDA5807M::StatusResult> setHighImpedanceOutput(int highImpedanceOutputEnable);
    RadioResult<RDA5807M::StatusResult> setStereo(int stereoEnable);
    RadioResult<RDA5807M::StatusResult> setSeek(int seekEnable);
    RadioResult<RDA5807M::StatusResult> setRDS(int rdsEnable);
    RadioResult<RDA5807M::StatusResult> setNewMethod(int newMethodEnable);
    RadioResult<RDA5807M::StatusResult> setSoftReset(int softResetEnable);
    RadioResult<RDA5807M::StatusResult> setSoftMute(int softMuteEnable);

    RadioResult<std::string> getStatusString(int UNUSED);
    RadioResult<std::string> getRegisterMapString(int UNUSED);


private:
    ///////////////////////////
    // Private Class Members //
    ///////////////////////////
    RDA5807M& radio;
};

#endif /* DRIVER_WRAPPER_RDA5807MWRAPPER_HPP_ */
