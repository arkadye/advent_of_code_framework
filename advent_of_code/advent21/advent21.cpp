#include "advent21.h"
#include "../advent/advent_utils.h"

#define ENABLE_DAY21DBG 1
#ifdef NDEBUG
#define DAY21DBG 0
#else
#define DAY21DBG ENABLE_DAY21DBG
#endif

#if DAY21DBG
	#include <iostream>
#endif

namespace
{
#if DAY21DBG
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

ResultType advent_twentyone_p1()
{
	auto input = advent::open_puzzle_input(21);
	return solve_p1(input);
}

ResultType advent_twentyone_p2()
{
	auto input = advent::open_puzzle_input(21);
	return solve_p2(input);
}

#undef DAY21DBG
#undef ENABLE_DAY21DBG