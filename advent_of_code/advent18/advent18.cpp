#include "advent18.h"
#include "../advent/advent_utils.h"

#define ENABLE_DAY18DBG 1
#ifdef NDEBUG
#define DAY18DBG 0
#else
#define DAY18DBG ENABLE_DAY18DBG
#endif

#if DAY18DBG
	#include <iostream>
#endif

namespace
{
#if DAY18DBG
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

ResultType advent_eighteen_p1()
{
	auto input = advent::open_puzzle_input(18);
	return solve_p1(input);
}

ResultType advent_eighteen_p2()
{
	auto input = advent::open_puzzle_input(18);
	return solve_p2(input);
}

#undef DAY18DBG
#undef ENABLE_DAY18DBG