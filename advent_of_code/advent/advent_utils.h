#pragma once

#include <fstream>
#include <sstream>
#include <cassert>
#include <string>
#include <string_view>

namespace advent
{

	inline std::ifstream open_puzzle_input(int day)
	{
		std::ostringstream name;
		name << "advent" << day << "/advent" << day << ".txt";
		auto result = std::ifstream{ name.str() };
		assert(result.is_open());
		return result;
	}

	inline std::ifstream open_testcase_input(int day, char id)
	{
		std::ostringstream name;
		name << "advent" << day << "/testcase_" << id << ".txt";
		auto result = std::ifstream{ name.str() };
		assert(result.is_open());
		return result;
	}

	class test_failed
	{
		std::string m_what;
	public:
		std::string_view what() const { return m_what; }
		explicit test_failed(std::string what_happened) : m_what{ std::move(what_happened) } {}
	};

	namespace
	{
		inline std::string make_what_happened_str_impl(std::ostringstream& msg)
		{
			return msg.str();
		}

		template <typename T, typename...Args>
		inline std::string make_what_happened_str_impl(std::ostringstream& msg, const T& first, const Args&...rest)
		{
			msg << ' ' << first;
			return make_what_happened_str_impl(msg, rest...);
		}

		template <typename T, typename...Args>
		inline std::string make_what_happened_str(const T& first, const Args& ... args)
		{
			std::ostringstream msg;
			msg << first;
			return make_what_happened_str_impl(msg, args...);
		}

		template <typename...Args>
		inline void check_advent_assert_impl(
			std::string_view file, 
			int line_no, 
			bool check_passes, 
			std::string_view check_str, 
			const Args&...msg)
		{
			if (!check_passes)
			{
				const auto file_break = file.find_last_of("\\/");
				if (file_break < file.size())
				{
					file.remove_prefix(file_break + 1);
				}
				auto what = make_what_happened_str(
					file,
					'(',
					line_no,
					"): '",
					check_str,
					'\'',
					msg...
				);

				test_failed error{ std::move(what) };
				throw error;
			}
		}

		inline void check_advent_assert(std::string_view file, int line_no, bool check_passes, std::string_view check_str)
		{
			check_advent_assert_impl(file, line_no, check_passes, check_str);
		}

		template <typename...Args>
		inline void check_advent_assert_msg(std::string_view file, int line_no, bool check_passes, std::string_view check_str, const Args&...args)
		{
			check_advent_assert_impl(file, line_no, check_passes, check_str," Msg: " , args...);
		}
	}
}

#ifdef _MSC_VER
#define InternalAdventPlatformSpecificUnreachable __assume(false)
#endif

#define AdventCheck(test_bool) advent::check_advent_assert(__FILE__,__LINE__,test_bool,#test_bool)
#define AdventCheckMsg(test_bool,...) advent::check_advent_assert_msg(__FILE__,__LINE__,test_bool,#test_bool,__VA_ARGS__)
#define AdventUnreachable() advent::check_advent_assert_msg(__FILE__,__LINE__,false,"Entered unreachable location!"); \
	InternalAdventPlatformSpecificUnreachable