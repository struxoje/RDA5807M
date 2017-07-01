/**************************************************
 * Util.cpp
 * Author: Ben Sherman
 *************************************************/

// System includes
#include <cstdint>

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


