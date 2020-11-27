#include <functional>
#include <string>
#include <iostream>
#include <array>
#include <algorithm>
#include <sstream>
#include <chrono>
#include <optional>
#include <iomanip>

#include "../advent/advent_of_code.h"
#include "../advent/advent_headers.h"

using TestFunc = std::function<ResultType()>;

// This describes a test to run.
struct verification_test
{
	std::string name;
	TestFunc test_func;
	ResultType expected_result;
	bool result_known;
};

// A type to use to indicate the result is not known yet. Using this in a verification test
// will run the test and report the result, but will count as neither pass nor failure.
struct Dummy {};

verification_test make_test(std::string name, TestFunc func, int64_t result);
verification_test make_test(std::string name, TestFunc func, ResultType result);
verification_test make_test(std::string name, TestFunc func, Dummy);
verification_test make_test(std::string name, Dummy, Dummy);

// Result a test can give.
enum class test_status : char
{
	pass,
	fail,
	unknown,
	filtered
};

// Full results of a test.
struct test_result
{
	std::string name;
	ResultType result = "";
	ResultType expected = "";
	test_status status = test_status::unknown;
};

template <test_status status>
bool check_result(const test_result& result)
{
	return status == result.status;
}

std::string two_digits(int num)
{
	std::ostringstream oss;
	oss << std::setfill('0') << std::setw(2) << num;
	return oss.str();
}

std::optional<std::string> to_human_readable(long long count, int inner_max, int outer_max, const std::string& suffix)
{
	if (count < inner_max)
		return std::to_string(count) + suffix;

	if (count < (static_cast<decltype(count)>(inner_max) * static_cast<decltype(count)>(outer_max)))
	{
		std::ostringstream oss;
		oss << count / inner_max << ':' << two_digits(count % inner_max) << suffix;
		return oss.str();
	}

	return std::optional<std::string>{};
}

std::string to_human_readable(std::chrono::hours time)
{
	const auto count = time.count();
	std::ostringstream oss;
	oss << count / 24 << " days and " << count % 24 << " hours";
	return oss.str();
}

std::string to_human_readable(std::chrono::minutes time)
{
	const auto res = to_human_readable(time.count(), 60, 24, "s");
	if (res.has_value())
		return res.value();

	const auto h = std::chrono::duration_cast<std::chrono::hours>(time);
	return to_human_readable(h);
}

std::string to_human_readable(std::chrono::seconds time)
{
	const auto res = to_human_readable(time.count(), 60,60,"s");
	if (res.has_value())
		return res.value();

	const auto m = std::chrono::duration_cast<std::chrono::minutes>(time);
	return to_human_readable(m);
}

std::optional<std::string> to_human_readable(long long count, const std::string& suffix_short, const std::string& suffix_long)
{
	if (count < 10'000)
		return std::to_string(count) + suffix_short;
	if (count < 100'000)
	{
		std::ostringstream oss;
		oss << std::setprecision(3) << static_cast<double>(count) / 1000.0 << suffix_long;
		return oss.str();
	}
	return std::optional<std::string>{};
}

std::string to_human_readable(std::chrono::milliseconds time)
{
	const auto res = to_human_readable(time.count(), "ms", "s");
	if (res.has_value())
		return res.value();

	const auto s = std::chrono::duration_cast<std::chrono::seconds>(time);
	return to_human_readable(s);
}

std::string to_human_readable(std::chrono::microseconds time)
{
	const auto res = to_human_readable(time.count(), "us", "ms");
	if (res.has_value())
		return res.value();

	const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(time);
	return to_human_readable(ms);
}

std::string to_human_readable(std::chrono::nanoseconds time)
{
	const auto res = to_human_readable(time.count(), "ns", "us");
	if (res.has_value())
		return res.value();

	const auto us = std::chrono::duration_cast<std::chrono::microseconds>(time);
	return to_human_readable(us);
}

test_result run_test(const verification_test& test, std::string_view filter)
{
	if (test.name.find(filter) == test.name.npos)
	{
		return test_result{
			test.name,
			0,
			test.expected_result,
			test_status::filtered
		};
	}
	std::cout << "Running test " << test.name << ": ";
	const auto start_time = std::chrono::high_resolution_clock::now();
	const auto res = test.test_func();
	const auto end_time = std::chrono::high_resolution_clock::now();
	const std::chrono::nanoseconds time_taken = end_time - start_time;
	std::cout << " took " << to_human_readable(time_taken) << '\n';
	auto get_result = [&](test_status status)
	{
		return test_result{ test.name,res,test.expected_result,status };
	};
	if (test.result_known && res == test.expected_result)
	{
		return get_result(test_status::pass);
	}
	else if (test.result_known && res != test.expected_result)
	{
		return get_result(test_status::fail);
	}
	return get_result(test_status::unknown);
}

bool verify_all(const std::string& filter)
{
#define ARG(func_name) std::string{ #func_name },func_name
#define TESTCASE(func_name,expected_result) make_test(ARG(func_name),expected_result)
#define FUNC_NAME(day_num,part_num) advent_ ## day_num ## _p ## part_num
#define TEST_DECL(day_num,part_num,expected_result) TESTCASE(FUNC_NAME(day_num,part_num),expected_result)
#define DAY(day_num,part1_result,part2_result) \
	TEST_DECL(day_num,1,part1_result), \
	TEST_DECL(day_num,2,part2_result)

	const verification_test tests[] =
	{
		TESTCASE(advent_one_testcase_a,"success"),
		TESTCASE(advent_one_testcase_b,123),
		DAY(one,Dummy{},Dummy{}),
		DAY(two,Dummy{},Dummy{}),
		DAY(three,Dummy{},Dummy{}),
		DAY(four,Dummy{},Dummy{}),
		DAY(five,Dummy{},Dummy{}),
		DAY(six,Dummy{},Dummy{}),
		DAY(seven,Dummy{},Dummy{}),
		DAY(eight,Dummy{},Dummy{}),
		DAY(nine,Dummy{},Dummy{}),
		DAY(ten,Dummy{},Dummy{}),
		DAY(eleven,Dummy{},Dummy{}),
		DAY(twelve,Dummy{},Dummy{}),
		DAY(thirteen,Dummy{},Dummy{}),
		DAY(fourteen,Dummy{},Dummy{}),
		DAY(fifteen,Dummy{},Dummy{}),
		DAY(sixteen,Dummy{},Dummy{}),
		DAY(seventeen,Dummy{},Dummy{}),
		DAY(eighteen,Dummy{},Dummy{}),
		DAY(nineteen,Dummy{},Dummy{}),
		DAY(twenty,Dummy{},Dummy{}),
		DAY(twentyone,Dummy{},Dummy{}),
		DAY(twentytwo,Dummy{},Dummy{}),
		DAY(twentythree,Dummy{},Dummy{}),
		DAY(twentyfour,Dummy{},Dummy{}),
		DAY(twentyfive,Dummy{},Dummy{})
	};

#undef ARG
#undef TESTCASE
#undef FUNC_NAME
#undef TEST_DECL
#undef DAY
#undef DUMMY
#undef DUMMY_DAY
	
	constexpr int NUM_TESTS = sizeof(tests) / sizeof(verification_test);
	std::array<test_result, NUM_TESTS> results;
	std::transform(tests, tests + NUM_TESTS, begin(results),
		std::bind(run_test,std::placeholders::_1,filter));
	auto result_to_string = [&filter](const test_result& result)
	{
		std::ostringstream oss;
		oss << result.name << ": " << result.result << " - ";
		switch (result.status)
		{
		case test_status::pass:
			oss << "PASS\n";
			break;
		case test_status::fail:
			oss << "FAIL (expected " << result.expected << ")\n";
			break;
		case test_status::filtered:
			return std::string{ "" };
		default: // unknown
			oss << "[Unknown]\n";
			break;
		}
		return oss.str();
	};

	std::transform(begin(results),end(results),std::ostream_iterator<std::string>(std::cout), result_to_string);

	auto get_count = [&results](auto pred)
	{
		return std::count_if(begin(results), end(results), pred);
	};

	std::cout << "RESULTS:\n"
		"    PASSED : " << get_count(check_result<test_status::pass>) << "\n"
		"    FAILED : " << get_count(check_result<test_status::fail>) << "\n"
		"    UNKNOWN: " << get_count(check_result<test_status::unknown>) << '\n';
	return std::none_of(begin(results), end(results),check_result<test_status::fail>);
}

verification_test make_test(std::string name, TestFunc func, int64_t result)
{
	return make_test(std::move(name), func, std::to_string(result));
}

verification_test make_test(std::string name, TestFunc func, ResultType result)
{
	return verification_test{ std::move(name),func,result,true };
}

verification_test make_test(std::string name, TestFunc func, Dummy)
{
	return verification_test{ std::move(name),func,"",false };
}

verification_test make_test(std::string name, Dummy, Dummy)
{
	return make_test(std::move(name), []() { std::cout << "Test not implemented yet"; return ResultType{ 0 }; }, Dummy{});
}