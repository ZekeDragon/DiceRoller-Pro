/***********************************************************************************************************************
** Simple Qt DiceRoller Program
** diceroll.hpp
** Copyright (C) 2024 Ezekiel Oruven
**
** Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
** documentation files (the "Software"), to deal in the Software without restriction, including without limitation the
** rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
** permit persons to whom the Software is furnished to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
** Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
** WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
** COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
** OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
***********************************************************************************************************************/
#pragma once

#include <cstdint>
#include <vector>

class DiceRoll
{
	std::vector<uint16_t> rolls;

public:
	DiceRoll(uint8_t amount, uint16_t face, int32_t perDieAdd, int32_t perDieMult, int32_t rollAdd, int32_t rollMult);

	const int32_t addDie, multDie, addRoll, multRoll;
	const uint16_t faces;
	const uint8_t numberOfDice;

	uint16_t rollAt(uint8_t index) const;
	int32_t rollTotal(uint8_t index) const;
	int64_t total() const;
};