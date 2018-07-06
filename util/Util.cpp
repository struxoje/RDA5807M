/**************************************************
 * Util.cpp
 * Author: Ben Sherman
 *************************************************/

// System includes
#include <cstdint>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <vector>

// Project includes
#include "Util.hpp"

/**
 * Returns the value of regContent anded with mask.
 * This result is then shifted so that the lower order
 * non-zero bit in mask is moved to the zeroth bit position.
 * For example, if regContent = 0xFF and mask = 0x10, the
 * result would be 0x01. 
 *
 */
uint16_t Util::valueFromReg(uint16_t regContent, uint16_t mask)
{
    uint8_t shiftAmt = 0;
    uint16_t shiftedMask = mask;

    while (((shiftedMask & 0x0001) == 0) && (shiftAmt < 16))
    {
        shiftedMask >>= 1;
        ++shiftAmt;
    }

    return ((regContent & mask) >> shiftAmt);
}

/**
 * Returns 0xFF when boolean is true and 0x00 when boolean is false
 */
uint16_t Util::boolToInteger(bool boolean)
{
    if (boolean)
    {
        return 0xFF;
    }
    else
    {
        return 0x00;
    }
}

/**
 * Return true if val > 0, false otherwise
 */
bool Util::boolFromInteger(int val)
{
    if (val > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

uint16_t Util::stringToUInt16(const std::string& str) {
    int asInt = std::stoi(str);

    if (asInt > UINT16_MAX || asInt < 0) {
        throw std::out_of_range(str + " must be in range [" +
                                std::to_string(UINT16_MAX) + ":0]");
    }

    return static_cast<uint16_t>(asInt);
}

bool Util::boolFromString(const std::string& str) {
    static const std::vector<std::string> VALID_TRUE_VALUES{"1", "true", "on", "yes"};
    static const std::vector<std::string> VALID_FALSE_VALUES{"0", "false", "off", "no"};

    if (std::find(VALID_TRUE_VALUES.begin(), VALID_TRUE_VALUES.end(), str) != VALID_TRUE_VALUES.end()) {
        return true;
    } else if (std::find(VALID_FALSE_VALUES.begin(), VALID_FALSE_VALUES.end(), str) != VALID_FALSE_VALUES.end()) {
        return false;
    }

    throw std::invalid_argument("Booleans must be 0/false/off or 1/true/on");

}

uint8_t Util::stringToUInt8(const std::string& str) {
    int asInt = std::stoi(str);

    if (asInt > UINT8_MAX || asInt < 0) {
        throw std::out_of_range(str + " must be in range [" +
                                std::to_string(UINT8_MAX) + ":0]");
    }

    return static_cast<uint8_t>(asInt);
}


