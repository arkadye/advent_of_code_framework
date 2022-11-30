#include "advent15.h"
#include "../advent/advent_utils.h"

#define ENABLE_DAY15DBG 1
#ifdef NDEBUG
#define DAY15DBG 0
#else
#define DAY15DBG ENABLE_DAY15DBG
#endif

#if DAY15DBG
	#include <iostream>
#endif

namespace
{
#if DAY15DBG
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

ResultType advent_fifteen_p1()
{
	auto input = advent::open_puzzle_input(15);
	return solve_p1(input);
}

ResultType advent_fifteen_p2()
{
	auto input = advent::open_puzzle_input(15);
	return solve_p2(input);
}

#undef DAY15DBG
#undef ENABLE_DAY15DBG