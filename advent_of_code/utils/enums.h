#pragma once

#include <type_traits>
#include <array>

namespace utils
{
	template <typename T>
	concept enumeration = std::is_enum_v<T>;

	template <enumeration InType, typename OutType = std::underlying_type_t<InType>>
	inline constexpr OutType to_idx(InType in) noexcept
	{
		return static_cast<OutType>(in);
	}

	[[nodiscard]] inline constexpr auto enum_order(enumeration auto left, enumeration auto right) noexcept
	{
		const auto left_val = to_idx(left);
		const auto right_val = to_idx(right);
		return left_val <=> right_val;
	}

	template <enumeration T>
	struct EnumSorter
	{
		[[nodiscard]] constexpr auto operator()(T left, T right) const noexcept
		{
			return enum_order(left,right);
		}
	};

	
}