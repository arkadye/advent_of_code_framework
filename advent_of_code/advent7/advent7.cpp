#include "advent7.h"
#include "../advent/advent_utils.h"

#define ENABLE_DAY7DBG 1
#ifdef NDEBUG
#define DAY7DBG 0
#else
#define DAY7DBG ENABLE_DAY7DBG
#endif

#if DAY7DBG
	#include <iostream>
#endif

namespace
{
#if DAY7DBG
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

ResultType advent_seven_p1()
{
	auto input = advent::open_puzzle_input(7);
	return solve_p1(input);
}

ResultType advent_seven_p2()
{
	auto input = advent::open_puzzle_input(7);
	return solve_p2(input);
}

#undef DAY7DBG
#undef ENABLE_DAY7DBG