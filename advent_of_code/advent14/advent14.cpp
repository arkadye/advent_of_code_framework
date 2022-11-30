#include "advent14.h"
#include "../advent/advent_utils.h"

#define ENABLE_DAY14DBG 1
#ifdef NDEBUG
#define DAY14DBG 0
#else
#define DAY14DBG ENABLE_DAY14DBG
#endif

#if DAY14DBG
	#include <iostream>
#endif

namespace
{
#if DAY14DBG
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

ResultType advent_fourteen_p1()
{
	auto input = advent::open_puzzle_input(14);
	return solve_p1(input);
}

ResultType advent_fourteen_p2()
{
	auto input = advent::open_puzzle_input(14);
	return solve_p2(input);
}

#undef DAY14DBG
#undef ENABLE_DAY14DBG