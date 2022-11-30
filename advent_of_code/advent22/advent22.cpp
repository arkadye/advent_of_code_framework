#include "advent22.h"
#include "../advent/advent_utils.h"

#define ENABLE_DAY22DBG 1
#ifdef NDEBUG
#define DAY22DBG 0
#else
#define DAY22DBG ENABLE_DAY22DBG
#endif

#if DAY22DBG
	#include <iostream>
#endif

namespace
{
#if DAY22DBG
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

ResultType advent_twentytwo_p1()
{
	auto input = advent::open_puzzle_input(22);
	return solve_p1(input);
}

ResultType advent_twentytwo_p2()
{
	auto input = advent::open_puzzle_input(22);
	return solve_p2(input);
}

#undef DAY22DBG
#undef ENABLE_DAY22DBG