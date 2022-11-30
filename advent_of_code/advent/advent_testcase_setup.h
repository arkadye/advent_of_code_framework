#pragma once

#include <functional>
#include <string>

using TestFunc = std::function<ResultType()>;

// This describes a test to run.
struct verification_test
{
	std::string name;
	TestFunc test_func;
	std::string expected_result;
	bool result_known;
};

// A type to use to indicate the result is not known yet. Using this in a verification test
// will run the test and report the result, but will count as neither pass nor failure.
struct Dummy {};

verification_test make_test(std::string name, TestFunc func, int64_t result);
verification_test make_test(std::string name, TestFunc func, std::string result);
verification_test make_test(std::string name, TestFunc func, Dummy);
verification_test make_test(std::string name, Dummy, Dummy);

#define ARG(func_name) std::string{ #func_name },func_name
#define TESTCASE(func_name,expected_result) make_test(ARG(func_name),expected_result)
#define FUNC_NAME(day_num,part_num) advent_ ## day_num ## _p ## part_num
#define TEST_DECL(day_num,part_num,expected_result) TESTCASE(FUNC_NAME(day_num,part_num),expected_result)
#define DAY(day_num,part1_result,part2_result) \
	TEST_DECL(day_num,1,part1_result), \
	TEST_DECL(day_num,2,part2_result)