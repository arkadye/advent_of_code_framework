#include "advent10.h"
#include "../advent/advent_utils.h"

#define ENABLE_DAY10DBG 1
#ifdef NDEBUG
#define DAY10DBG 0
#else
#define DAY10DBG ENABLE_DAY10DBG
#endif

#if DAY10DBG
	#include <iostream>
#endif

namespace
{
#if DAY10DBG
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

ResultType advent_ten_p1()
{
	auto input = advent::open_puzzle_input(10);
	return solve_p1(input);
}

ResultType advent_ten_p2()
{
	auto input = advent::open_puzzle_input(10);
	return solve_p2(input);
}

#undef DAY10DBG
#undef ENABLE_DAY10DBG