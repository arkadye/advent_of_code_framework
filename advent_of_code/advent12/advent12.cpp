#include "advent12.h"
#include "../advent/advent_utils.h"

#define ENABLE_DAY12DBG 1
#ifdef NDEBUG
#define DAY12DBG 0
#else
#define DAY12DBG ENABLE_DAY12DBG
#endif

#if DAY12DBG
	#include <iostream>
#endif

namespace
{
#if DAY12DBG
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

ResultType advent_twelve_p1()
{
	auto input = advent::open_puzzle_input(12);
	return solve_p1(input);
}

ResultType advent_twelve_p2()
{
	auto input = advent::open_puzzle_input(12);
	return solve_p2(input);
}

#undef DAY12DBG
#undef ENABLE_DAY12DBG