#include "advent3.h"
#include "../advent/advent_utils.h"

#define ENABLE_DAY3DBG 1
#ifdef NDEBUG
#define DAY3DBG 0
#else
#define DAY3DBG ENABLE_DAY3DBG
#endif

#if DAY3DBG
	#include <iostream>
#endif

namespace
{
#if DAY3DBG
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

ResultType advent_three_p1()
{
	auto input = advent::open_puzzle_input(3);
	return solve_p1(input);
}

ResultType advent_three_p2()
{
	auto input = advent::open_puzzle_input(3);
	return solve_p2(input);
}

#undef DAY3DBG
#undef ENABLE_DAY3DBG