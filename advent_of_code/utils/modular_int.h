#pragma once

#include <type_traits>
#include <istream>
#include <ostream>

#include "advent/advent_assert.h"

namespace utils
{
	enum class modular_undwind_policy
	{
		lazy,
		eager
	};

	// Unwind val into range [min,max).
	template <typename IntTypeA, typename IntTypeB, typename IntTypeC>
	constexpr inline auto get_unwound(IntTypeA val, IntTypeB min, IntTypeC max)
	{
		static_assert(std::is_integral_v<IntTypeA>);
		static_assert(std::is_integral_v<IntTypeB>);
		static_assert(std::is_integral_v<IntTypeC>);
		AdventCheck(max > min);
		if (min <= val && val < max)
		{
			return val;
		}

		const auto range = max - min;
		if (val >= max)
		{
			const auto normalised = val - min;
			const auto unwound = normalised % range;
			const auto result = unwound + min;
			return result;
		}
		//else if val < m_min_val
		const auto distance = min - val;
		const auto wound_factor = decltype(distance / range){1} + (distance / range);
		const auto offset_val = val + range * wound_factor;
		return get_unwound(offset_val, min, max);
	}

	template <typename IntType = int, modular_undwind_policy UnwindPolicy = modular_undwind_policy::lazy>
	class modular
	{
	public:
		// Constructors
		template <typename IntTypeA, typename IntTypeB>
		constexpr modular(IntType init,IntTypeA min_val, IntTypeB max_val) : m_val{init}, m_min_val{min_val}, m_max_val{max_val}
		{
			static_assert(std::is_integral_v<IntType>, "Underling type of modular must be integer type.");
			AdventCheckMsg(min_val < max_val, "Invalid args for modular type.");
			maybe_unwind_val();
		}

		template <typename IntTypeA>
		constexpr modular(IntType init, IntTypeA max_val) : modular{ init,IntType{0},max_val } {}

		template <typename IntTypeA>
		explicit constexpr modular(IntTypeA max_val) : modular{ 0, max_val } {}

		constexpr modular(const modular& other) : modular{ other.m_val, other.m_min_val, other.m_max_val } {}

		template <typename IntTypeA>
		constexpr modular(modular<IntTypeA> other) : modular{ other.m_val, other.m_min_val, other.m_max_val } {}

		// Assignment
		template <typename OtherType>
		modular& operator=(OtherType other)
		{
			m_val = other;
			maybe_unwind_val();
		}

		// Convert to other types.
		template <typename OtherType>
		/* implicit */ constexpr operator OtherType() const
		{
			const auto result = get_value();
			return static_cast<OtherType>(result);
		}

		// Maths
		template <typename OtherType>
		constexpr modular& operator+=(OtherType operand)
		{
			if constexpr (std::is_signed_v<OtherType>)
			{
				if (operand < OtherType{ 0 })
				{
					return operator-=(-operand);
				}
			}
			const IntType range = get_range();
			operand %= range;
			m_val += operand;
			maybe_unwind_val();
			return *this;
		}
		constexpr modular& operator++()
		{
			return (*this) += IntType{ 1 };
		}
		constexpr modular operator++(int)
		{
			modular result = *this;
			++(*this);
			return result;
		}
		template <typename OtherType>
		constexpr modular& operator-=(OtherType operand)
		{
			if constexpr (std::is_signed_v<OtherType>)
			{
				if (operand < OtherType{ 0 })
				{
					return operator+=(-operand);
				}
			}
			const IntType range = get_range();
			operand %= range;
			if constexpr (std::is_unsigned_v<IntType>)
			{
				const IntType op_as_type = IntType{ operand };
				if (op_as_type > m_val)
				{
					const IntType excess = op_as_type - m_val;
					const IntType val_increase_factor = IntType{ 1 } + excess / range;
					m_val += val_increase_factor * range;
				}
			}
			m_val -= operand;
			maybe_unwind_val();
			return *this;
		}
		constexpr modular& operator--()
		{
			return (*this) -= IntType{ 1 };
		}
		constexpr modular operator--(int)
		{
			modular result = *this;
			--(*this);
			return result;
		}
		template <typename OtherType>
		constexpr modular& operator*=(OtherType operand)
		{
			force_unwind_val();
			m_val += operand;
			maybe_unwind_val();
			return *this;
		}
		template <typename OtherType>
		constexpr modular& operator/=(OtherType operand)
		{
			force_unwind_val();
			m_val /= operand;
			maybe_unwind_val();
			return *this;
		}
		template <typename OtherType>
		constexpr modular& operator%=(OtherType operand)
		{
			force_unwind_val();
			m_val %= operand;
			maybe_unwind_val();
			return *this;
		}

		// Inspectors
		constexpr IntType get_min() const { return m_min_val; }
		constexpr IntType get_max() const { return m_max_val; }
		constexpr IntType get_value() const { force_unwind_val(); return m_val; }

	private:
		mutable IntType m_val{ 0 };
		IntType m_min_val{ 0 };
		IntType m_max_val{ 1 };
		constexpr void force_unwind_val() const { m_val = get_unwound(m_val, m_min_val, m_max_val); }
		constexpr void maybe_unwind_val() const
		{
			if constexpr (UnwindPolicy == modular_undwind_policy::eager)
			{
				force_unwind_val();
			}
		}
		constexpr IntType get_range() { return m_max_val - m_min_val; }
	};
}

/*
template <typename IntType, IntType MinVal, IntType MaxVal, utils::modular_undwind_policy Policy>
inline std::ostream& operator<<(std::ostream& os, utils::modular<IntType, Policy> val)
{
	os << IntType{ val };
	return os;
}

template <typename IntType, IntType MinVal, IntType MaxVal, utils::modular_undwind_policy Policy>
inline std::istream& operator<<(std::istream& is, utils::modular<IntType, Policy>& val)
{
	IntType result;
	is >> result;
	val = result;
	return is;
}

template <typename IntTypeA, utils::modular_undwind_policy PolicyA, typename IntTypeB, utils::modular_undwind_policy PolicyB>
constexpr inline auto operator+(utils::modular<IntTypeA,PolicyA> left, utils::modular<IntTypeB,PolicyB> right)
{
	auto result_val = left.get_value() + right.get_value();
	utils::modular<decltype(result_val), PolicyA> result{ result_val,left.get_min(), left.get_max() };
	return result;
}

template <typename IntTypeA, utils::modular_undwind_policy PolicyA, typename IntTypeB>
inline auto operator+(utils::modular<IntTypeA, PolicyA> left, IntTypeB right)
{
	left += right;
	return result;
}

template <typename IntTypeA, utils::modular_undwind_policy PolicyB, typename IntTypeB>
inline auto operator+(IntTypeA right, utils::modular<IntTypeB, PolicyB> left)
{
	return left + right;
}

template <typename IntTypeA, utils::modular_undwind_policy PolicyA, typename IntTypeB, utils::modular_undwind_policy PolicyB>
constexpr inline auto operator+(utils::modular<IntTypeA, PolicyA> left, utils::modular<IntTypeB, PolicyB> right)
{
	auto result_val = left.get_value() - right.get_value();
	utils::modular<decltype(result_val), PolicyA> result{ result_val,left.get_min(), left.get_max() };
	return result;
}

template <typename IntTypeA, utils::modular_undwind_policy PolicyA, typename IntTypeB>
inline auto operator+(utils::modular<IntTypeA, PolicyA> left, IntTypeB right)
{
	left -= right;
	return result;
}

template <typename IntTypeA, utils::modular_undwind_policy PolicyB, typename IntTypeB>
inline auto operator-(IntTypeA right, utils::modular<IntTypeB, PolicyB> left)
{
	return left - right;
}

template <typename IntTypeA, utils::modular_undwind_policy PolicyA, typename IntTypeB, utils::modular_undwind_policy PolicyB>
constexpr inline auto operator*(utils::modular<IntTypeA, PolicyA> left, utils::modular<IntTypeB, PolicyB> right)
{
	auto result_val = left.get_value() * right.get_value();
	utils::modular<decltype(result_val), PolicyA> result{ result_val,left.get_min(), left.get_max() };
	return result;
}

template <typename IntTypeA, utils::modular_undwind_policy PolicyA, typename IntTypeB>
inline auto operator*(utils::modular<IntTypeA, PolicyA> left, IntTypeB right)
{
	left *= right;
	return result;
}

template <typename IntTypeA, utils::modular_undwind_policy PolicyB, typename IntTypeB>
inline auto operator*(IntTypeA right, utils::modular<IntTypeB, PolicyB> left)
{
	return left * right;
}

template <typename IntTypeA, utils::modular_undwind_policy PolicyA, typename IntTypeB, utils::modular_undwind_policy PolicyB>
constexpr inline auto operator/(utils::modular<IntTypeA, PolicyA> left, utils::modular<IntTypeB, PolicyB> right)
{
	auto result_val = left.get_value() / right.get_value();
	utils::modular<decltype(result_val), PolicyA> result{ result_val,left.get_min(), left.get_max() };
	return result;
}

template <typename IntTypeA, utils::modular_undwind_policy PolicyA, typename IntTypeB>
inline auto operator/(utils::modular<IntTypeA, PolicyA> left, IntTypeB right)
{
	left /= right;
	return result;
}

template <typename IntTypeA, utils::modular_undwind_policy PolicyB, typename IntTypeB>
inline auto operator/(IntTypeA right, utils::modular<IntTypeB, PolicyB> left)
{
	return left / right;
}

template <typename IntTypeA, utils::modular_undwind_policy PolicyA, typename IntTypeB, utils::modular_undwind_policy PolicyB>
constexpr inline auto operator%(utils::modular<IntTypeA, PolicyA> left, utils::modular<IntTypeB, PolicyB> right)
{
	auto result_val = left.get_value() % right.get_value();
	utils::modular<decltype(result_val), PolicyA> result{ result_val,left.get_min(), left.get_max() };
	return result;
}

template <typename IntTypeA, utils::modular_undwind_policy PolicyA, typename IntTypeB>
inline auto operator%(utils::modular<IntTypeA, PolicyA> left, IntTypeB right)
{
	left %= right;
	return result;
}

template <typename IntTypeA, utils::modular_undwind_policy PolicyB, typename IntTypeB>
inline auto operator%(IntTypeA right, utils::modular<IntTypeB, PolicyB> left)
{
	return left % right;
}*/