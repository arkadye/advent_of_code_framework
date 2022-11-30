#include "advent25.h"
#include "../advent/advent_utils.h"

#define ENABLE_DAY25DBG 1
#ifdef NDEBUG
#define DAY25DBG 0
#else
#define DAY25DBG ENABLE_DAY25DBG
#endif

#if DAY25DBG
	#include <iostream>
#endif

namespace
{
#if DAY25DBG
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
}

ResultType advent_twentyfive_p1()
{
	auto input = advent::open_puzzle_input(25);
	return solve_p1(input);
}

ResultType advent_twentyfive_p2()
{
	auto input = advent::open_puzzle_input(25);
	return "MERRY CHRISTMAS!";
}

#undef DAY25DBG
#undef ENABLE_DAY25DBG