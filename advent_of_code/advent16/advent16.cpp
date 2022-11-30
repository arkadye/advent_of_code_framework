#include "advent16.h"
#include "../advent/advent_utils.h"

#define ENABLE_DAY16DBG 1
#ifdef NDEBUG
#define DAY16DBG 0
#else
#define DAY16DBG ENABLE_DAY16DBG
#endif

#if DAY16DBG
	#include <iostream>
#endif

namespace
{
#if DAY16DBG
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

ResultType advent_sixteen_p1()
{
	auto input = advent::open_puzzle_input(16);
	return solve_p1(input);
}

ResultType advent_sixteen_p2()
{
	auto input = advent::open_puzzle_input(16);
	return solve_p2(input);
}

#undef DAY16DBG
#undef ENABLE_DAY16DBG