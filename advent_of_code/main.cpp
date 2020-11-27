#include "advent/advent_of_code.h"

#include <iostream>
#include <thread>
#include <vector>
#include <atomic>

int main()
{
	// Use the filter to only run certain tests.
	// This uses some magic to test against the name of the function,
	// so putting "eighteen" as the argument will only run advent_eighteen_p1()
	// and advent_eighteen_p2() (as well as any other test functions with "eighteen"
	// in the function name.
	// Leave blank to run everything.
	verify_all("");

	return 0;
}