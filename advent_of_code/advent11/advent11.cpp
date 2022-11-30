#include "advent11.h"
#include "../advent/advent_utils.h"

#define ENABLE_DAY11DBG 1
#ifdef NDEBUG
#define DAY11DBG 0
#else
#define DAY11DBG ENABLE_DAY11DBG
#endif

#if DAY11DBG
	#include <iostream>
#endif

namespace
{
#if DAY11DBG
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

ResultType advent_eleven_p1()
{
	auto input = advent::open_puzzle_input(11);
	return solve_p1(input);
}

ResultType advent_eleven_p2()
{
	auto input = advent::open_puzzle_input(11);
	return solve_p2(input);
}

#undef DAY11DBG
#undef ENABLE_DAY11DBG