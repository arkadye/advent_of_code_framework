#pragma once

#include <type_traits>

#ifdef _WIN32
#include <intrin.h>
#ifndef _WIN64
#include <cstdint>
#endif
#endif

namespace utils
{
	template <typename T>
	T population(T input) noexcept
	{
		static_assert(std::is_integral_v<T>, "Can only get population of an integer type");
		static_assert(std::is_unsigned_v<T>, "Can only get population of an unsigned type.");
#ifdef _WIN32
		int cpu_info[4];
		__cpuid(cpu_info, 1);
		constexpr int popcnt_mask = 1 << 23;
		if (cpu_info[2] & popcnt_mask)
		{
			if constexpr (sizeof(T) <= 2)
			{
				return static_cast<T>(__popcnt16(input));
			}
			if constexpr (sizeof(T) <= 4)
			{
				return static_cast<T>(__popcnt(input));
			}
#ifdef _WIN64
			if constexpr (sizeof(T) == 8)
			{
				return static_cast<T>(__popcnt64(input));
			}
#endif
			if constexpr (sizeof(T) > 4)
			{
				T result = 0;
				while (result > 0)
				{
					const uint32_t low_bits = input;
					input = input >> 32;
					result += __popcnt(low_bits);
				}
				return result;
			}
		}
#endif
		T result = 0;
		while (input != 0)
		{
			result += input & T{ 1 };
			input = input >> 1;
		}
		return result;
	}
}