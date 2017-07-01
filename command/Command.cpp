/*
 * Command.cpp
 *
 *  Created on: Jun 30, 2017
 *      Author: bensherman
 */

// Stdlib includes
#include <string>
#include <cstdint>

// Project includes
#include "Command.hpp"
#include "RDA5807M.hpp"

template class Command<bool>;
template class Command<uint16_t>;
template class Command<uint8_t>;

template <typename T>
Command<T>::Command(std::string command, void (RDA5807M::* func)(T))
{
	this->command = command;
	this->func = func;
}

template <typename T>
void Command<T>::exec(RDA5807M& radio, T funcParam)
{
	(radio.*func)(funcParam);
}

