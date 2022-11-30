#include "isqrt.h"

#include <cassert>
namespace
{
	uint32_t isqrt_implementation(uint32_t guess_base, uint32_t input)
	{
		assert(input > 0);
		uint64_t guess = 1;
		while (true)
		{
			auto sqr = [](uint64_t x) -> uint64_t {return x * x; };
			const uint64_t trial_result = guess_base + guess;
			const uint64_t trial_square = sqr(trial_result);
			if (trial_square == input) // Perfect match!
			{
				return static_cast<int32_t>(trial_result);
			}
			else if (trial_square < input) // Too small. Try a bigger number.
			{
				guess *= 2;
			}
			else if (guess == 1) // We have an imperfect result: guess_base is too small, but guess_base+1 is too big
			{
				return guess_base;
			}
			else // guess > 1 && trial_result is too big.
			{
				// Roll the guess back a stage (div by 2) and restart with a better base.
				return isqrt_implementation(guess_base + static_cast<uint32_t>(guess / 2), input);
			}
		}
	}
}

// Return the square root of an integer. If not exact, this is rounded down.
// Guaranteed to have no floating point inaccuracies.
uint32_t utils::isqrt(uint32_t input)
{
	switch (input)
	{
	default:
		return isqrt_implementation(0, input);
	case 0:
		return 0;
		// Add more common cases here if optimisation is needed.
	}
}

int32_t utils::isqrt(int32_t input)
{
	assert(input >= 0);
	return static_cast<int32_t>(isqrt(static_cast<uint32_t>(input)));
}