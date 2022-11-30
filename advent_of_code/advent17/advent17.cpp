#include "advent17.h"
#include "../advent/advent_utils.h"

#define ENABLE_DAY17DBG 1
#ifdef NDEBUG
#define DAY17DBG 0
#else
#define DAY17DBG ENABLE_DAY17DBG
#endif

#if DAY17DBG
	#include <iostream>
#endif

namespace
{
#if DAY17DBG
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

ResultType advent_seventeen_p1()
{
	auto input = advent::open_puzzle_input(17);
	return solve_p1(input);
}

ResultType advent_seventeen_p2()
{
	auto input = advent::open_puzzle_input(17);
	return solve_p2(input);
}

#undef DAY17DBG
#undef ENABLE_DAY17DBG