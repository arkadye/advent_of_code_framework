#include "../advent/advent1.h"

// Unnamed namespaces are really useful for functions
// specific to this solution. Generally useful stuff should
// go into a utility header. (Some stuff is already there.)
namespace
{
	ResultType solve_p1_general(int input)
	{
		switch (input)
		{
		case 1:
			return "success";
		case 2:
			return "123";
		default:
			return "";
		}
	}
}

ResultType advent_one_testcase_a()
{
	return solve_p1_general(1);
}

ResultType advent_one_testcase_b()
{
	return solve_p1_general(2);
}

ResultType advent_one_p1()
{
	return solve_p1_general(100);
}

ResultType advent_one_p2()
{
	return "";
}