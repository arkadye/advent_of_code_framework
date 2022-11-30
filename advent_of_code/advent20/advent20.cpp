#include "advent20.h"
#include "../advent/advent_utils.h"

#define ENABLE_DAY20DBG 1
#ifdef NDEBUG
#define DAY20DBG 0
#else
#define DAY20DBG ENABLE_DAY20DBG
#endif

#if DAY20DBG
	#include <iostream>
#endif

namespace
{
#if DAY20DBG
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

ResultType advent_twenty_p1()
{
	auto input = advent::open_puzzle_input(20);
	return solve_p1(input);
}

ResultType advent_twenty_p2()
{
	auto input = advent::open_puzzle_input(20);
	return solve_p2(input);
}

#undef DAY20DBG
#undef ENABLE_DAY20DBG