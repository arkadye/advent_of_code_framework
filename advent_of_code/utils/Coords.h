#pragma once

#include <compare>
#include <numeric>
#include <cassert>
#include <cmath>
#include <array>
#include <iostream>

enum class Direction : char
{
	Up = 0,
	Right,
	Down,
	Left
};

enum class TurnDir : char
{
	Clockwise,
	Anticlockwise
};

inline Direction rotate(Direction dir, TurnDir td)
{
	int increment = 0;
	switch (td)
	{
	case TurnDir::Clockwise:
		increment = 1;
		break;
	case TurnDir::Anticlockwise:
		increment = -1;
		break;
	}
	int dir_i = static_cast<int>(dir);
	dir_i += increment;
	constexpr int NUM_DIRECTIONS = 4;
	dir_i += NUM_DIRECTIONS;
	dir_i %= NUM_DIRECTIONS;
	return static_cast<Direction>(dir_i);
}

template <typename T>
struct BasicCoords
{
	T x , y;
	auto operator<=>(const BasicCoords&) const noexcept = default;

	auto direction_to(const BasicCoords& other) const noexcept;
	T size_squared() const noexcept { return x * x + y * y; }
	double angle() const noexcept;
	auto reduce() const noexcept;
	auto manhatten_distance() const noexcept { return std::abs(x) + std::abs(y); }
	auto manhatten_distance(const BasicCoords& other) const noexcept;
	BasicCoords(T x_, T y_) : x{ x_ }, y{ y_ }{}
	BasicCoords(T init) : BasicCoords{ init,init } {}
	BasicCoords() : BasicCoords{ 0 } {}

	BasicCoords& operator=(const BasicCoords&) noexcept = default;

	template <typename T2>
	BasicCoords& operator+=(const BasicCoords<T2>& other) noexcept
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	template <typename T2>
	BasicCoords& operator-=(const BasicCoords<T2>& other) noexcept
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	template <typename RHSTYPE>
	BasicCoords& operator*=(RHSTYPE other) noexcept
	{
		x *= other;
		y *= other;
		return *this;
	}

	template <typename RHSTYPE>
	BasicCoords& operator/=(RHSTYPE other) noexcept
	{
		assert(other != static_cast<RHSTYPE>(0));
		x /= other;
		y /= other;
		return *this;
	}


	static BasicCoords up() noexcept { return BasicCoords{ 0,1 }; }
	static BasicCoords down() noexcept { return BasicCoords{ 0,-1 }; }
	static BasicCoords left() noexcept { return BasicCoords{ -1,0 }; }
	static BasicCoords right() noexcept { return BasicCoords{ 1,0 }; }
	static BasicCoords dir(Direction dir) noexcept
	{
		switch (dir)
		{
		case Direction::Up:
			return up();
		case Direction::Down:
			return down();
		case Direction::Left:
			return left();
		case Direction::Right:
			return right();
		default:
			assert(false);
			return BasicCoords{};
		}
	}
	auto neighbours() const
	{
		return std::array<BasicCoords, 4>
		{
			*this + up(),
			*this + down(),
			*this + left(),
			*this + right()
		};
	}
};

using Coords = BasicCoords<int>;

inline double pi()
{
	return 4.0 * atan(1);
}

template <typename T>
inline double BasicCoords<T>::angle() const noexcept
{
	double angle = std::atan2(x, -y);
	if (angle < 0.0) angle += (2.0 * pi());
	return angle;
}


template <typename T1, typename T2>
inline auto operator-(const BasicCoords<T1>& l, const BasicCoords<T2>& r) noexcept
{
	BasicCoords result = l;
	result -= r;
	return result;
}

template <typename T1, typename T2>
inline auto operator+(const BasicCoords<T1>& l, const BasicCoords<T2>& r) noexcept
{
	BasicCoords result = l;
	result += r;
	return result;
}

template <typename T>
inline auto operator/(const BasicCoords<T>& l, int r) noexcept
{
	BasicCoords result = l;
	result /= r;
	return result;
}

template <typename T>
inline auto BasicCoords<T>::reduce() const noexcept
{
	return size_squared() != 0 ? *this / std::gcd(x, y) : *this;
}

template <typename T>
inline auto BasicCoords<T>::direction_to(const BasicCoords& other) const noexcept
{
	const BasicCoords direction = other - *this;
	if (direction.size_squared() == 0) return direction;
	return direction.reduce();
}

template <typename T>
inline auto BasicCoords<T>::manhatten_distance(const BasicCoords& other) const noexcept
{
	const BasicCoords t = *this - other;
	return t.manhatten_distance();
}

template <typename T>
inline auto manhatten_distance(const BasicCoords<T>& a, const BasicCoords<T>& b)
{
	return a.manhatten_distance(b);
}

template <typename T>
inline std::ostream& operator<<(std::ostream& out, const BasicCoords<T>& c)
{
	out << "{ " << c.x << " , " << c.y << " }";
	return out;
}