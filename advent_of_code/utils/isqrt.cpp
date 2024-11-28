#include "isqrt.h"

#include "advent/advent_assert.h"

namespace
{
	uint64_t isqrt_implementation(uint64_t guess_base, uint64_t input) noexcept
	{
		AdventCheck(input > 0);
		uint64_t guess = 1;
		while (true)
		{
			auto sqr = [](uint64_t x) -> uint64_t {return x * x; };
			const uint64_t trial_result = guess_base + guess;
			const uint64_t trial_square = sqr(trial_result);
			if (trial_square == input) // Perfect match!
			{
				return trial_result;
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
				return isqrt_implementation(guess_base + guess / 2, input);
			}
		}
	}
}

// Return the square root of an integer. If not exact, this is rounded down.
// Guaranteed to have no floating point inaccuracies.
uint64_t utils::isqrt(uint64_t input) noexcept
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

int64_t utils::isqrt(int64_t input) noexcept
{
	AdventCheck(input >= 0);
	return static_cast<int64_t>(isqrt(static_cast<uint64_t>(input)));
}

bool utils::is_square(uint64_t input) noexcept
{
	const uint64_t root = utils::isqrt(input);
	return input == (root * root);
}

bool utils::is_square(int64_t input) noexcept
{
	if(input < 0) return false;
	return is_square(static_cast<uint64_t>(input));
}