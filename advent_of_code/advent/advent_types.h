#pragma once

#include <string>
#include <variant>
#include <cstdint>
#include <ranges>

using ResultType = std::variant<std::string, int64_t, uint64_t>;
enum class AdventDay
{
	one, two
};

namespace stdr = std::ranges;
namespace stdv = stdr::views;