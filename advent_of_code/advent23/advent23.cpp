#include "advent23.h"
#include "../advent/advent_utils.h"

#define ENABLE_DAY23DBG 1
#ifdef NDEBUG
#define DAY23DBG 0
#else
#define DAY23DBG ENABLE_DAY23DBG
#endif

#if DAY23DBG
	#include <iostream>
#endif

namespace
{
#if DAY23DBG
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

ResultType advent_twentythree_p1()
{
	auto input = advent::open_puzzle_input(23);
	return solve_p1(input);
}

ResultType advent_twentythree_p2()
{
	auto input = advent::open_puzzle_input(23);
	return solve_p2(input);
}

#undef DAY23DBG
#undef ENABLE_DAY23DBG