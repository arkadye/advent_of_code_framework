#pragma once

#include <cstddef>

#include "advent/advent_assert.h"

namespace utils
{
	enum class Axis : std::size_t
	{
		X = 0,
		Y = 1,
		Z = 2
	};

	template <typename T>
	class Vector3D
	{
	private:
		T m_x;
		T m_y;
		T m_z;
	public:
		// Constructors
		Vector3D() : Vector3D{ T{0} } {}
		explicit Vector3D(T init) : Vector3D{ init,init,init } {}
		Vector3D(T x, T y, T z) : m_x{ x }, m_y{ y }, m_z{ z }{}
		template <typename X_T, typename Y_T, typename Z_T>
		Vector3D(X_T x, Y_T y, Z_T z)
			: Vector3D{ static_cast<T>(x),static_cast<T>(y),static_cast<T>(z) } {}
		Vector3D(const Vector3D&) = default;
		template <typename OtherT>
		explicit Vector3D(const Vector3D<OtherT>& other)
			: Vector3D{ other.m_x, other.m_y, other.m_z } {}
		auto operator<=>(const Vector3D&) const = default;
		T& at(Axis axis)
		{
			switch (axis)
			{
			case Axis::X:
				return m_x;
			case Axis::Y:
				return m_y;
			case Axis::Z:
				return m_z;
			default:
				break;
			}
			AdventUnreachable();
			return m_x;
		}
		T at(Axis axis) const
		{
			switch (axis)
			{
			case Axis::X:
				return m_x;
			case Axis::Y:
				return m_y;
			case Axis::Z:
				return m_z;
			default:
				break;
			}
			AdventUnreachable();
			return m_x;
		}
		T& operator[](Axis axis)
		{
			return at(axis);
		}
		T operator[](Axis axis) const
		{
			return at(axis);
		}
		T& operator[](std::size_t axis)
		{
			AdventCheck(axis < 3u);
			return at(static_cast<Axis>(axis));
		}
		T operator[](std::size_t axis) const
		{
			AdventCheck(axis < 3u);
			return at(static_cast<Axis>(axis));
		}

		template <typename U>
		Vector3D& operator+=(const Vector3D<U>& other)
		{
			for (std::size_t i = 0u; i < 3; ++i)
			{
				(*this)[i] += other[i];
			}
			return *this;
		}

		template <typename ScalarType>
		Vector3D& operator*=(const ScalarType& val)
		{
			for (std::size_t i = 0u; i < 3; ++i)
			{
				(*this)[i] *= val;
			}
			return *this;
		}

		Vector3D operator-() const
		{
			Vector3D result = *this;
			result *= static_cast<T>(-1);
			return result;
		}

		// +ve x.
		static Vector3D forward()
		{
			return Vector3D{ static_cast<T>(1),static_cast<T>(0),static_cast<T>(0) };
		}

		// -ve x
		static Vector3D backward()
		{
			return Vector3D{ static_cast<T>(-1),static_cast<T>(0),static_cast<T>(0) };
		}

		// +ve y
		static Vector3D right()
		{
			return Vector3D{ static_cast<T>(0),static_cast<T>(1),static_cast<T>(0) };
		}

		// -ve y
		static Vector3D left()
		{
			return Vector3D{ static_cast<T>(0),static_cast<T>(-1),static_cast<T>(0) };
		}

		// +ve z
		static Vector3D up()
		{
			return Vector3D{ static_cast<T>(0),static_cast<T>(0),static_cast<T>(1) };
		}

		// -ve z
		static Vector3D down()
		{
			return Vector3D{ static_cast<T>(0),static_cast<T>(0),static_cast<T>(-1) };
		}
	};
}

template <typename TLeft, typename TRight>
auto operator+(const utils::Vector3D<TLeft>& left, const utils::Vector3D<TRight>& right)
{
	auto result = left;
	result += right;
	return result;
}