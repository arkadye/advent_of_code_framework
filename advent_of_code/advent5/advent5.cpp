#include "advent5.h"
#include "../advent/advent_utils.h"

#define ENABLE_DAY5DBG 1
#ifdef NDEBUG
#define DAY5DBG 0
#else
#define DAY5DBG ENABLE_DAY5DBG
#endif

#if DAY5DBG
	#include <iostream>
#endif

namespace
{
#if DAY5DBG
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

ResultType advent_five_p1()
{
	auto input = advent::open_puzzle_input(5);
	return solve_p1(input);
}

ResultType advent_five_p2()
{
	auto input = advent::open_puzzle_input(5);
	return solve_p2(input);
}

#undef DAY5DBG
#undef ENABLE_DAY5DBG