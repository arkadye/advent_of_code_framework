#pragma once

#include <cstdint>

namespace utils
{

	uint64_t isqrt(uint64_t input) noexcept;
	int64_t isqrt(int64_t input) noexcept;
	bool is_square(uint64_t input) noexcept;
	bool is_square(int64_t input) noexcept;
}