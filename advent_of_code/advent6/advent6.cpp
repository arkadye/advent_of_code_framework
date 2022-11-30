#include "advent6.h"
#include "../advent/advent_utils.h"

#define ENABLE_DAY6DBG 1
#ifdef NDEBUG
#define DAY6DBG 0
#else
#define DAY6DBG ENABLE_DAY6DBG
#endif

#if DAY6DBG
	#include <iostream>
#endif

namespace
{
#if DAY6DBG
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

ResultType advent_six_p1()
{
	auto input = advent::open_puzzle_input(6);
	return solve_p1(input);
}

ResultType advent_six_p2()
{
	auto input = advent::open_puzzle_input(6);
	return solve_p2(input);
}

#undef DAY6DBG
#undef ENABLE_DAY6DBG