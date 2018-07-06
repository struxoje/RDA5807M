/**************************************************
 * Util.hpp
 * Author: Ben Sherman
 *************************************************/

#ifndef UTIL_HPP
#define UTIL_HPP

// System includes
#include <cstdint>

// Project includes
//<none>

namespace Util
{
    uint16_t valueFromReg(uint16_t regContent, uint16_t mask);
    uint16_t boolToInteger(bool boolean);
    bool boolFromInteger(int val);
    bool boolFromString(const std::string& str);
    uint16_t stringToUInt16(const std::string& str);
    uint8_t stringToUInt8(const std::string& str);
};

#endif	// ifndef UTIL_HPP
