/*
 * RadioResult.hpp
 *
 *  Created on: Jul 1, 2017
 *      Author: bensherman
 */

#ifndef DRIVER_WRAPPER_RADIORESULT_HPP_
#define DRIVER_WRAPPER_RADIORESULT_HPP_

template<typename T>
class RadioResult {
public:
    ////////////////////////////////
    // Public interface functions //
    ////////////////////////////////
    RadioResult(T resultParam) : result(resultParam){};
    T getResult() { return result; }

private:
    /////////////////////////////
    // Private Class Constants //
    /////////////////////////////
    T result;
};

#endif /* DRIVER_WRAPPER_RADIORESULT_HPP_ */
