#include "advent8.h"
#include "advent/advent_utils.h"

#ifdef FORCE_DAY8DBG
#define DAY8DBG 1
#else
#define ENABLE_DAY8DBG 1
#ifdef NDEBUG
#define DAY8DBG 0
#else
#define DAY8DBG ENABLE_DAY8DBG
#endif
#endif

#if DAY8DBG
	#include <iostream>
#endif

namespace
{
#if DAY8DBG
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

ResultType advent_eight_p1()
{
	auto input = advent::open_puzzle_input(8);
	return solve_p1(input);
}

ResultType advent_eight_p2()
{
	auto input = advent::open_puzzle_input(8);
	return solve_p2(input);
}

#undef DAY8DBG
#undef ENABLE_DAY8DBG
