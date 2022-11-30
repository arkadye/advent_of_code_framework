#include "advent9.h"
#include "../advent/advent_utils.h"

#define ENABLE_DAY9DBG 1
#ifdef NDEBUG
#define DAY9DBG 0
#else
#define DAY9DBG ENABLE_DAY9DBG
#endif

#if DAY9DBG
	#include <iostream>
#endif

namespace
{
#if DAY9DBG
	std::ostream & log = std::cout;
#else
	struct {	template <typename T> auto& operator<<(const T&) const noexcept { return *this; } } log;
#endif
}

namespace
{
	int solve_p1(std::istream& input)
	{
		return 0;
	}
}

namespace
{
	int solve_p2(std::istream& input)
	{
		return 0;
	}
}

ResultType advent_nine_p1()
{
	auto input = advent::open_puzzle_input(9);
	return solve_p1(input);
}

ResultType advent_nine_p2()
{
	auto input = advent::open_puzzle_input(9);
	return solve_p2(input);
}

#undef DAY9DBG
#undef ENABLE_DAY9DBG