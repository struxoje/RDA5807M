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
#include <memory>

// Project Includes
#include "RDA5807M.hpp"

class RDA5807MWrapper;
using RDA5807MWrapperSharedPtr = std::shared_ptr<RDA5807MWrapper>;

class RDA5807MWrapper
{
public:
    ////////////////////////////////
    // Public interface functions //
    ////////////////////////////////
    RDA5807MWrapper() : radio(RDA5807M{}) { };

    // RDA5807M::StatusResult-returning functions
    void setFrequency(const std::string& freqStr, std::string* updatableStr);
    void setVolume(const std::string& volStr, std::string* updatableStr);
    void setMute(const std::string& muteEnable, std::string* updatableStr);
    void setBassBoost(const std::string& bassBoostEnableStr, std::string* updatableStr);
    void setRadioEnableState(const std::string& enableStr, std::string* updatableStr);
    void setHighImpedanceOutput(const std::string& highImpedanceOutputEnable, std::string* updatableStr);
    void setStereo(const std::string& stereoEnable, std::string* updatableStr);
    void setSeek(const std::string& seekEnable, std::string* updatableStr);
    void setRDS(const std::string& rdsEnable, std::string* updatableStr);
    void setNewMethod(const std::string& newMethodEnable, std::string* updatableStr);
    void setSoftReset(const std::string& softResetEnable, std::string* updatableStr);
    void setSoftMute(const std::string& softMuteEnable, std::string* updatableStr);
    void setTune(const std::string& tuneEnable, std::string* updatableStr);
    void setAFCD(const std::string& afcdEnable, std::string* updatableStr);
    void setDeEmphasis(const std::string& deEmphasisSelectorStr, std::string* updatableStr);
    void setBand(const std::string& bandSelectorStr, std::string* updatableStr);
    void setChannelSpacing(const std::string& channelSpacingSelectorStr, std::string* updatableStr);
    void setSeekDirection(const std::string& seekDirSelectorStr, std::string* updatableStr);
    void setSeekMode(const std::string& seekModeSelectorStr, std::string* updatableStr);
    void setSoftBlend(const std::string& softBlendEnable, std::string* updatableStr);
    void updateLocalRegisterMapFromDevice(const std::string& param, std::string* updatableStr);

    void getStatusString(const std::string& param, std::string* updatableStr);
    void getRegisterMapString(const std::string& param, std::string* updatableStr);
    void generateFreqMap(const std::string& lengthStr, std::string* updatableStr);
    void getRdsInfoString(const std::string& param, std::string* updatableStr);
    void getLocalCopyOfReg(const std::string& regStr, std::string* updatableStr);
    void snoopRdsGroupTwo(const std::string& millisecondsStr, std::string* updatableStr);

    void getRssi(const std::string& param, std::string* updatableStr);

    /**
     * Use the UPDATELOCALREGS command before calling to pull in new
     * RDS data. Note that if UPDATELOCALREGS is called between queries
     * for RDS data, it'll probably be the case that the RDS data will
     * change
     */
    void getRdsPiCode(const std::string& param, std::string* updatableStr);
    void getRdsGroupTypeCode(const std::string& param, std::string* updatableStr);
    void getRdsVersionCode(const std::string& param, std::string* updatableStr);
    void getRdsTrafficProgramIdCode(const std::string& param, std::string* updatable);
    void getRdsProgramTypeCode(const std::string& param, std::string* updatableStr);


    RDA5807M& getRadio();

    class Builder
    {
    public:
        Builder();
//        Builder& withRda5807m(RDA5807M&& rda5807m);
        RDA5807MWrapperSharedPtr build();

    private:
//        RDA5807M rda5807m;
    };

private:

    static void validateStatusResult(RDA5807M::StatusResult result);
    static void outOfRangeErrorHelper(const std::string& quantity,
                                        uint32_t max, uint32_t min);

    static const int MICROS_IN_MILLIS = 1000;

    RDA5807M radio;

};

#endif /* DRIVER_WRAPPER_RDA5807MWRAPPER_HPP_ */
