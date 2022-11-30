#include "advent24.h"
#include "../advent/advent_utils.h"

#define ENABLE_DAY24DBG 1
#ifdef NDEBUG
#define DAY24DBG 0
#else
#define DAY24DBG ENABLE_DAY24DBG
#endif

#if DAY24DBG
	#include <iostream>
#endif

namespace
{
#if DAY24DBG
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

ResultType advent_twentyfour_p1()
{
	auto input = advent::open_puzzle_input(24);
	return solve_p1(input);
}

ResultType advent_twentyfour_p2()
{
	auto input = advent::open_puzzle_input(24);
	return solve_p2(input);
}

#undef DAY24DBG
#undef ENABLE_DAY24DBG