#include "advent4.h"
#include "../advent/advent_utils.h"

#define ENABLE_DAY4DBG 1
#ifdef NDEBUG
#define DAY4DBG 0
#else
#define DAY4DBG ENABLE_DAY4DBG
#endif

#if DAY4DBG
	#include <iostream>
#endif

namespace
{
#if DAY4DBG
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

ResultType advent_four_p1()
{
	auto input = advent::open_puzzle_input(4);
	return solve_p1(input);
}

ResultType advent_four_p2()
{
	auto input = advent::open_puzzle_input(4);
	return solve_p2(input);
}

#undef DAY4DBG
#undef ENABLE_DAY4DBG