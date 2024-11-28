#include "advent2.h"
#include "advent/advent_utils.h"

#ifdef FORCE_DAY2DBG
#define DAY2DBG 1
#else
#define ENABLE_DAY2DBG 1
#ifdef NDEBUG
#define DAY2DBG 0
#else
#define DAY2DBG ENABLE_DAY2DBG
#endif
#endif

#if DAY2DBG
	#include <iostream>
#endif

namespace
{
#if DAY2DBG
	std::ostream& log = std::cout;
#else
	struct {	template <typename T> auto& operator<<(const T&) const noexcept { return *this; } } log;
#endif
}

namespace
{
	int64_t solve_p1(std::istream& input)
	{
		return 0;
	}
}

namespace
{
	int64_t solve_p2(std::istream& input)
	{
		return 0;
	}
}

ResultType advent_two_p1()
{
	auto input = advent::open_puzzle_input(2);
	return solve_p1(input);
}

ResultType advent_two_p2()
{
	auto input = advent::open_puzzle_input(2);
	return solve_p2(input);
}

#undef DAY2DBG
#undef ENABLE_DAY2DBG
