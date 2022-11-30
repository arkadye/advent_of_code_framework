#include "advent1.h"
#include "../advent/advent_utils.h"

#define ENABLE_DAY1DBG 1
#ifdef NDEBUG
#define DAY1DBG 0
#else
#define DAY1DBG ENABLE_DAY1DBG
#endif

#if DAY1DBG
	#include <iostream>
#endif

namespace
{
#if DAY1DBG
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

ResultType advent_one_p1()
{
	auto input = advent::open_puzzle_input(1);
	return solve_p1(input);
}

ResultType advent_one_p2()
{
	auto input = advent::open_puzzle_input(1);
	return solve_p2(input);
}

#undef DAY1DBG
#undef ENABLE_DAY1DBG