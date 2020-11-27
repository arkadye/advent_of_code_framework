#pragma once

#ifndef NDEBUG
#include "threadsafe_ofstream.h"
#include <string_view>
#endif
#include <string>

#ifdef NDEBUG
class AdventLogger
{
public:
	explicit AdventLogger(const char*) {}
	template <typename T>
	AdventLogger& operator<<(T&&) { return *this; }
};
#else
using AdventLogger = threadsafe_ofstream;
#endif

static AdventLogger advlog{ "advent_log.txt" };
#ifndef NDEBUG
namespace
{
	std::string_view cut_file_down(std::string_view in)
	{
		in.remove_prefix(in.find_last_of("/\\"));
		return in;
	}
}
#define LOG advlog << cut_file_down(__FILE__) << ":" << __LINE__ << ": "
#define ERR advlog << "ERROR: " << __FILE__ ":" << __LINE__ << ": "
#else
#include <iostream>
#define LOG advlog
#define ERR std::cerr
#endif