#pragma once

#include <compare>
#include <numeric>
#include <cassert>
#include <cmath>
#include <array>
#include <iostream>

namespace utils
{

	enum class direction : char
	{
		up = 0,
		right,
		down,
		left
	};

	enum class turn_dir : char
	{
		clockwise,
		anticlockwise
	};

	inline direction rotate(direction dir, turn_dir td)
	{
		int increment = 0;
		switch (td)
		{
		case turn_dir::clockwise:
			increment = 1;
			break;
		case turn_dir::anticlockwise:
			increment = -1;
			break;
		}
		int dir_i = static_cast<int>(dir);
		dir_i += increment;
		constexpr int NUM_DIRECTIONS = 4;
		dir_i += NUM_DIRECTIONS;
		dir_i %= NUM_DIRECTIONS;
		return static_cast<direction>(dir_i);
	}

	template <typename T>
	struct basic_coords
	{
		T x, y;
		auto operator<=>(const basic_coords&) const noexcept = default;

		auto direction_to(const basic_coords& other) const noexcept;
		T size_squared() const noexcept { return x * x + y * y; }
		double angle() const noexcept;
		auto reduce() const noexcept;
		auto manhatten_distance() const noexcept { return std::abs(x) + std::abs(y); }
		auto manhatten_distance(const basic_coords& other) const noexcept;
		constexpr basic_coords(T x_, T y_) : x{ x_ }, y{ y_ }{}
		constexpr basic_coords(T init) : basic_coords{ init,init } {}
		constexpr basic_coords() : basic_coords{ 0 } {}

		basic_coords& operator=(const basic_coords&) noexcept = default;

		template <typename T2>
		basic_coords& operator+=(const basic_coords<T2>& other) noexcept
		{
			x += other.x;
			y += other.y;
			return *this;
		}

		template <typename T2>
		basic_coords& operator-=(const basic_coords<T2>& other) noexcept
		{
			x -= other.x;
			y -= other.y;
			return *this;
		}

		template <typename RHSTYPE>
		basic_coords& operator*=(RHSTYPE other) noexcept
		{
			x *= other;
			y *= other;
			return *this;
		}

		template <typename RHSTYPE>
		basic_coords& operator/=(RHSTYPE other) noexcept
		{
			assert(other != static_cast<RHSTYPE>(0));
			x /= other;
			y /= other;
			return *this;
		}


		static basic_coords up() noexcept { return basic_coords{ 0,1 }; }
		static basic_coords down() noexcept { return basic_coords{ 0,-1 }; }
		static basic_coords left() noexcept { return basic_coords{ -1,0 }; }
		static basic_coords right() noexcept { return basic_coords{ 1,0 }; }
		static basic_coords dir(direction dir) noexcept
		{
			switch (dir)
			{
			case direction::up:
				return up();
			case direction::down:
				return down();
			case direction::left:
				return left();
			case direction::right:
				return right();
			default:
				assert(false);
				return basic_coords{};
			}
		}

		// Return North, south, east, and west tiles.
		auto neighbours() const
		{
			return std::array<basic_coords, 4>
			{
				*this + up(),
				*this + down(),
				*this + left(),
				*this + right()
			};
		}

		auto neighbours_plus_diag() const
		{
			return std::array<basic_coords, 8>
			{
				*this + up(),
				*this + up() + right(),
				*this + right(),
				*this + right() + down(),
				*this + down(),
				*this + down() + left(),
				*this + left(),
				*this + left() + up()
			};
		}
	};

	using coords = basic_coords<int>;

	inline double pi()
	{
		return 4.0 * atan(1);
	}

	template <typename T>
	inline double basic_coords<T>::angle() const noexcept
	{
		double angle = std::atan2(x, -y);
		if (angle < 0.0) angle += (2.0 * pi());
		return angle;
	}


	template <typename T1, typename T2>
	inline auto operator-(const basic_coords<T1>& l, const basic_coords<T2>& r) noexcept
	{
		basic_coords result = l;
		result -= r;
		return result;
	}

	template <typename T1, typename T2>
	inline auto operator+(const basic_coords<T1>& l, const basic_coords<T2>& r) noexcept
	{
		basic_coords result = l;
		result += r;
		return result;
	}

	template <typename T>
	inline auto operator/(const basic_coords<T>& l, int r) noexcept
	{
		basic_coords result = l;
		result /= r;
		return result;
	}

	template <typename T>
	inline auto operator*(const basic_coords<T>& l, int r) noexcept
	{
		basic_coords result = l;
		result *= r;
		return result;
	}

	template <typename T>
	inline auto operator*(int l, const basic_coords<T>& r) noexcept
	{
		return r * l;
	}

	template <typename T>
	inline auto basic_coords<T>::reduce() const noexcept
	{
		return size_squared() != 0 ? *this / std::gcd(x, y) : *this;
	}

	template <typename T>
	inline auto basic_coords<T>::direction_to(const basic_coords& other) const noexcept
	{
		const basic_coords direction = other - *this;
		if (direction.size_squared() == 0) return direction;
		return direction.reduce();
	}

	template <typename T>
	inline auto basic_coords<T>::manhatten_distance(const basic_coords& other) const noexcept
	{
		const basic_coords t = *this - other;
		return t.manhatten_distance();
	}

	template <typename T>
	inline auto manhatten_distance(const basic_coords<T>& a, const basic_coords<T>& b)
	{
		return a.manhatten_distance(b);
	}

	template <typename T>
	inline std::ostream& operator<<(std::ostream& out, const basic_coords<T>& c)
	{
		out << "{ " << c.x << " , " << c.y << " }";
		return out;
	}
}