#include <functional>
#include <string>
#include <iostream>
#include <array>
#include <algorithm>
#include <sstream>
#include <chrono>
#include <optional>
#include <iomanip>
#include <cassert>
#include <numeric>

#include "../advent/advent_of_code.h"
#include "../advent/advent_headers.h"
#include "../advent/advent_setup.h"
#include "../advent/advent_assert.h"

namespace
{
	struct ResultStringifier
	{
		std::string operator()(const std::string& in) const noexcept { return in; }
		template <std::integral T>
		std::string operator()(T in) const { return std::to_string(in); }
	};
}

std::string to_string(const ResultType& rt)
{
	return std::visit(ResultStringifier{}, rt);
}

std::string to_string(const std::optional<std::string>& os)
{
	return os.value_or("");
}

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
	std::string result;
	std::string expected;
	test_status status = test_status::unknown;
	std::chrono::nanoseconds time_taken;
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

template <typename TestType>
ResultType test_execute_wrapper(TestType test)
{
#ifdef NDEBUG
	return test.execute();
#else
	try
	{
		return test.execute();
	}
	catch (const advent::test_failed& tf)
	{
		return std::string{ "ERROR: " } + std::string{ tf.what() };
	}
#endif
}

template <typename TestType>
std::pair<ResultType,std::chrono::nanoseconds> run_test_func(TestType test)
{
	const auto start_time = std::chrono::high_resolution_clock::now();
	const ResultType res = test_execute_wrapper(std::move(test));
	const auto end_time = std::chrono::high_resolution_clock::now();
	return std::pair{res, end_time - start_time};
}

struct TestExecutor
{
	template <typename TestType>
	std::pair<ResultType,std::chrono::nanoseconds> operator()(TestType test) { return run_test_func(std::move(test)); }
};

test_result run_test(const verification_test& test, const std::vector<std::string_view>& filter)
{
	if(!filter.empty())
	{
		auto filter_pred = [name = std::string_view{ test.name }](std::string_view filter_item)
		{
			return name.find(filter_item) != name.npos;
		};
		const bool matches_filter = std::ranges::any_of(filter, filter_pred);
		if(!matches_filter)
		{
			return test_result{
				test.name,
				"",
				to_string(test.expected_result),
				test_status::filtered
			};
		}
	}
	std::cout << "Running test " << test.name << "...";
	const auto [res,time_taken] = std::visit(TestExecutor{}, test.test_func);
	const auto string_result = to_string(res);
	std::cout << "\nFinished " << test.name << ": took " << to_human_readable(time_taken) <<  " and got " << string_result << '\n';
	auto get_result = [&](test_status status)
	{
		return test_result{ test.name,string_result,to_string(test.expected_result),status,time_taken };
	};

	if(!test.expected_result.has_value())
	{
		return get_result(test_status::unknown);
	}
	else
	{
		return get_result(string_result == *test.expected_result ? test_status::pass : test_status::fail);
	}
}

bool verify_all(const std::vector<std::string_view>& filter)
{
	constexpr auto NUM_TESTS = std::size(tests);
	std::array<test_result, NUM_TESTS> results;
	std::ranges::transform(tests, begin(results),
		[&filter](const verification_test& test)
		{
			return run_test(test, filter);
		});

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

	std::ranges::transform(results,std::ostream_iterator<std::string>(std::cout), result_to_string);

	auto get_count = [&results](auto pred)
	{
		return std::ranges::count_if(results, pred);
	};

	const auto total_time = std::transform_reduce(begin(results), end(results), std::chrono::nanoseconds{ 0 },
		std::plus<std::chrono::nanoseconds>{}, [](const test_result& result) {return result.time_taken; });

	std::cout << 
		"RESULTS:\n"
		"    PASSED : " << get_count(check_result<test_status::pass>) << "\n"
		"    FAILED : " << get_count(check_result<test_status::fail>) << "\n"
		"    UNKNOWN: " << get_count(check_result<test_status::unknown>) << "\n"
		"    TIME   : " << to_human_readable(total_time) << '\n';
	return std::ranges::none_of(results,check_result<test_status::fail>);
}

verification_test make_test(std::string name, TestFunc func, int64_t result)
{
	return make_test(std::move(name), func, std::to_string(result));
}

verification_test make_test(std::string name, TestFunc func, std::string result)
{
	return verification_test{ std::move(name),func,std::move(result) };
}

verification_test make_test(std::string name, TestFunc func, Dummy)
{
	return verification_test{ std::move(name),func };
}

verification_test make_test(std::string name, TestFuncWithArg func, int64_t result, std::string arg)
{
	return make_test(std::move(name), func, std::to_string(result), std::move(arg));
}

verification_test make_test(std::string name, TestFuncWithArg func, std::string result, std::string arg)
{
	return verification_test{std::move(name), func, std::move(arg), std::move(result)};
}

verification_test make_test(std::string name, TestFuncWithArg func, Dummy, std::string arg)
{
	return verification_test{std::move(name), func, std::move(arg)};
}

ResultType TestWithArgExecutable::execute()
{
	std::istringstream iss{ std::move(arg)};
	return func(iss);
}