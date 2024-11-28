#include "advent19.h"
#include "advent/advent_utils.h"

#ifdef FORCE_DAY19DBG
#define DAY19DBG 1
#else
#define ENABLE_DAY19DBG 1
#ifdef NDEBUG
#define DAY19DBG 0
#else
#define DAY19DBG ENABLE_DAY19DBG
#endif
#endif

#if DAY19DBG
	#include <iostream>
#endif

namespace
{
#if DAY19DBG
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

ResultType advent_nineteen_p1()
{
	auto input = advent::open_puzzle_input(19);
	return solve_p1(input);
}

ResultType advent_nineteen_p2()
{
	auto input = advent::open_puzzle_input(19);
	return solve_p2(input);
}

#undef DAY19DBG
#undef ENABLE_DAY19DBG
