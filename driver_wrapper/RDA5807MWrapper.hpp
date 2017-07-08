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
#include "RDA5807M.hpp"

class RDA5807MWrapper
{
public:
    ////////////////////////////////
    // Public interface functions //
    ////////////////////////////////
    RDA5807MWrapper(RDA5807M& radioParam) : radio(radioParam) { };

    // RDA5807M::StatusResult-returning functions
    RDA5807M::StatusResult setFrequency(int freq);
    RDA5807M::StatusResult setVolume(int vol);
    RDA5807M::StatusResult setMute(int muteEnable);
    RDA5807M::StatusResult setBassBoost(int bassBoostEnable);
    RDA5807M::StatusResult setRadioEnableState(int radioEnable);
    RDA5807M::StatusResult setHighImpedanceOutput(int highImpedanceOutputEnable);
    RDA5807M::StatusResult setStereo(int stereoEnable);
    RDA5807M::StatusResult setSeek(int seekEnable);
    RDA5807M::StatusResult setRDS(int rdsEnable);
    RDA5807M::StatusResult setNewMethod(int newMethodEnable);
    RDA5807M::StatusResult setSoftReset(int softResetEnable);
    RDA5807M::StatusResult setSoftMute(int softMuteEnable);
    RDA5807M::StatusResult setTune(int tuneEnable);
    RDA5807M::StatusResult setAFCD(int afcdEnable);
    RDA5807M::StatusResult setDeEmphasis(int deEmphasisSelector);
    RDA5807M::StatusResult setBand(int bandSelector);
    RDA5807M::StatusResult setChannelSpacing(int channelSpacingSelector);
    RDA5807M::StatusResult setSeekDirection(int seekDirSelector);
    RDA5807M::StatusResult setSeekMode(int seekModeSelector);
    RDA5807M::StatusResult setSoftBlend(int softBlendEnable);
    RDA5807M::StatusResult updateLocalRegisterMapFromDevice(int UNUSED);

    // std::string-returning functions
    std::string getStatusString(int UNUSED);
    std::string getRegisterMapString(int UNUSED);
    std::string generateFreqMap(int UNUSED);
    std::string getRdsInfoString(int UNUSED);
    std::string getLocalCopyOfReg(int reg);


    // uint32_t-returning functions
    uint32_t getRssi(int UNUSED);

    /**
     * Use the UPDATELOCALREGS command before calling to pull in new
     * RDS data. Note that if UPDATELOCALREGS is called between queries
     * for RDS data, it'll probably be the case that the RDS data will
     * change
     */
    uint32_t getRdsPiCode(int UNUSED);
    uint32_t getRdsGroupTypeCode(int UNUSED);
    uint32_t getRdsVersionCode(int UNUSED);
    uint32_t getRdsTrafficProgramIdCode(int UNUSED);
    uint32_t getRdsProgramTypeCode(int UNUSED);

private:
    /////////////////////////////
    // Private Class Constants //
    /////////////////////////////
    static const int MICROS_IN_MILLIS = 1000;

    ///////////////////////////
    // Private Class Members //
    ///////////////////////////
    RDA5807M& radio;
};

#endif /* DRIVER_WRAPPER_RDA5807MWRAPPER_HPP_ */
