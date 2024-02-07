/***********************************************************************************************************************
** Simple Qt DiceRoller Program
** diceroll.cpp
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
#include "diceroll.hpp"

#include <random>
#include <chrono>
#include <numeric>

class RollRandomGenerator
{
	std::mt19937_64 rnd;

public:
	RollRandomGenerator() : rnd(std::chrono::high_resolution_clock::now().time_since_epoch().count()) {}

	uint16_t roll(uint16_t face)
	{
		return (rnd() % face) + 1;
	}
} gen;

int32_t privRollTotal(DiceRoll const &self, uint16_t roll)
{
	return (roll + self.addDie) * self.multDie;
}

DiceRoll::DiceRoll(uint8_t amount, uint16_t face, int32_t perDieAdd,
                   int32_t perDieMult, int32_t rollAdd, int32_t rollMult) :
    addDie(perDieAdd),
    multDie(perDieMult),
    addRoll(rollAdd),
    multRoll(rollMult),
    faces(face),
    numberOfDice(amount)
{
	while (amount--)
	{
		rolls.emplace_back(gen.roll(face));
	}
}

uint16_t DiceRoll::rollAt(uint8_t index) const
{
	static const uint16_t deadRoll = 0;
	if (numberOfDice <= index)
	{
		return deadRoll;
	}

	return rolls[index];
}

int32_t DiceRoll::rollTotal(uint8_t index) const
{
	return privRollTotal(*this, rollAt(index));
}

int64_t DiceRoll::total() const
{
	auto rollAdder = [this](int64_t acc, uint16_t roll) { return acc + privRollTotal(*this, roll); };
	return (std::accumulate(rolls.begin(), rolls.end(), int64_t(0), rollAdder) + addRoll) * multRoll;
}
