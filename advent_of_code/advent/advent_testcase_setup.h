#pragma once

#include <functional>
#include <string>
#include <string_view>
#include <optional>
#include <variant>

using TestFunc = std::function<ResultType()>;
using TestFuncWithArg = std::function<ResultType(std::istream&)>;

struct TestExecutable
{
	TestFunc func;
	ResultType execute() { return func(); }
	explicit TestExecutable(TestFunc func_) : func{func_}{}
};

struct TestWithArgExecutable
{
	TestFuncWithArg func;
	std::string arg;
	ResultType execute();
	TestWithArgExecutable(TestFuncWithArg func_, std::string arg_) : func(func_) , arg(std::move(arg_)){}
};

using Test = std::variant<TestExecutable,TestWithArgExecutable>;

// This describes a test to run.
struct verification_test
{
	std::string name;
	Test test_func;
	std::optional<std::string> expected_result;
	verification_test(std::string name_, TestFunc func) : verification_test{ std::move(name_), func, std::nullopt }{}
	verification_test(std::string name_, TestFunc func, std::string result) : verification_test{std::move(name_), func, std::optional<std::string>{std::move(result)} }{}
	verification_test(std::string name_, TestFuncWithArg func, std::string arg) : verification_test{ std::move(name_), func, std::move(arg), std::nullopt } {}
	verification_test(std::string name_, TestFuncWithArg func, std::string arg, std::string result) : verification_test{ std::move(name_), func, std::move(arg), std::optional<std::string>{std::move(result)} } {}
private:
	verification_test(std::string n, TestFunc t, std::optional<std::string> r) : name(std::move(n)),test_func(TestExecutable{t}),expected_result(std::move(r)){} 
	verification_test(std::string n, TestFuncWithArg t, std::string arg, std::optional<std::string> r) : name(std::move(n)),test_func(TestWithArgExecutable{t , std::move(arg)}),expected_result(std::move(r)){} 
};

// A type to use to indicate the result is not known yet. Using this in a verification test
// will run the test and report the result, but will count as neither pass nor failure.
struct Dummy {};
static constexpr Dummy dummy;

verification_test make_test(std::string name, TestFunc func, int64_t result);
verification_test make_test(std::string name, TestFunc func, std::string result);
verification_test make_test(std::string name, TestFunc func, Dummy);

verification_test make_test(std::string name, TestFuncWithArg func, int64_t result, std::string arg);
verification_test make_test(std::string name, TestFuncWithArg func, std::string result, std::string arg);
verification_test make_test(std::string name, TestFuncWithArg func, Dummy, std::string arg);

#define ARG(func_name) std::string{ #func_name },func_name
#define ARG_WITH_PARAM(func_name,param) std::string{ #func_name "("  #param ")"  }, func_name
#define TESTCASE(func_name,expected_result) make_test(ARG(func_name),expected_result)
#define TESTCASE_WITH_ARG(func_name,arg,expected_result) make_test(ARG_WITH_PARAM(func_name,arg),expected_result,arg)
#define FUNC_NAME(day_num,part_num) advent_ ## day_num ## _p ## part_num
#define TEST_DECL(day_num,part_num,expected_result) TESTCASE(FUNC_NAME(day_num,part_num),expected_result)
#define DAY(day_num,part1_result,part2_result) \
	TEST_DECL(day_num,1,part1_result), \
	TEST_DECL(day_num,2,part2_result)