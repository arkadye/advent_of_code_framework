#pragma once

#include "coords.h"
#include "int_range.h"


namespace utils::coords_iterators
{
	namespace internal_helpers
	{
		template <std::integral T>
		inline constexpr std::size_t width(const basic_coords<T>& first, const basic_coords<T>& last) noexcept
		{
			const auto [x_min, x_max] = std::minmax(first.x, last.x);
			const auto w = x_max - x_min;
			return static_cast<std::size_t>(w);
		}

		template <std::integral T>
		inline constexpr std::ptrdiff_t swidth(const basic_coords<T>& first, const basic_coords<T>& last) noexcept
		{
			return static_cast<std::ptrdiff_t>(width(first, last));
		}

		template <std::integral T>
		inline constexpr std::size_t height(const basic_coords<T>& first, const basic_coords<T>& last) noexcept
		{
			const auto [y_min, y_max] = std::minmax(first.y, last.y);
			const auto h = y_max - y_min;
			return static_cast<std::size_t>(h);
		}

		template <std::integral T>
		inline constexpr std::ptrdiff_t sheight(const basic_coords<T>& first, const basic_coords<T>& last) noexcept
		{
			return static_cast<std::ptrdiff_t>(height(first, last));
		}

		template <std::integral T>
		inline constexpr std::size_t size(const basic_coords<T>& first, const basic_coords<T>& last) noexcept
		{
			const auto w = width(first, last);
			const auto h = height(first, last);
			return w * h;
		}

		template <std::integral T>
		inline constexpr std::ptrdiff_t ssize(const basic_coords<T>& first, const basic_coords<T>& last) noexcept
		{
			return static_cast<std::ptrdiff_t>(size(first, last));
		}

		template <std::integral T>
		inline T get_at_idx(T first, T last, std::size_t idx)
		{
			const bool incrementing = (first <= last);
			const T tidx = static_cast<T>(idx);
			const T result = incrementing ? first + tidx : first - tidx;
			return result;
		};

		template <std::integral T>
		class elem_adaptor
		{
			basic_coords<T> start;
			basic_coords<T> finish;
		public:
			constexpr elem_adaptor(const basic_coords<T>& first, const basic_coords<T>& last) : start{ first }, finish{ last } {}
			constexpr basic_coords<T> operator()(std::size_t idx) const noexcept
			{
				AdventCheck(idx < size(start, finish));
				const auto w = width(start, finish);
				const auto widx = idx % w;
				const auto hidx = idx / w;
				const T x = get_at_idx(start.x, finish.x, widx);
				const T y = get_at_idx(start.y, finish.y, hidx);
				return basic_coords<T>{x, y};
			}
		};

		template <std::integral T>
		using elem_range_base = int_range_adaptor<elem_adaptor<T>>;
	}

	template <std::integral T>
	class elem_range : public internal_helpers::elem_range_base<T>
	{
	public:
		constexpr elem_range(const basic_coords<T>& first, const basic_coords<T>& last) noexcept
			: internal_helpers::elem_range_base<T>{ internal_helpers::elem_adaptor{first,last} , internal_helpers::ssize(first,last) }
		{}
		explicit constexpr elem_range(const basic_coords<T>& last) noexcept : elem_range{ basic_coords<T>{},last } {}

		static elem_range<T> get_row(T x_start, T x_finish, T y_const) noexcept
		{
			const basic_coords<T> first{ x_start,y_const };
			const basic_coords<T> last{ x_finish,y_const + 1 };
			return elem_range<T>{first, last};
		}

		static elem_range<T> get_row(const basic_coords<T>& first, T x_finish) noexcept
		{
			return get_row(first.x, x_finish, first.y);
		}

		static elem_range<T> get_row(T x_start, const basic_coords<T>& finish) noexcept
		{
			return get_row(x_start, finish.x, finish.y);
		}

		static elem_range<T> get_row(const basic_coords<T>& start, const basic_coords<T>& finish) noexcept
		{
			AdventCheck(start.y == finish.y);
			return get_row(start, finish.x);
		}

		static elem_range<T> get_column(T x_const, T y_start, T y_finish) noexcept
		{
			const basic_coords<T> first{ x_const,y_start };
			const basic_coords<T> last{ x_const + 1,y_finish };
			return elem_range<T>{first, last};
		}

		static elem_range<T> get_column(const basic_coords<T>& first, T y_finish) noexcept
		{
			return get_column(first.x, first.y, y_finish);
		}

		static elem_range<T> get_column(T y_start, const basic_coords<T>& finish) noexcept
		{
			return get_column(finish.x, y_start, finish.y);
		}

		static elem_range<T> get_column(const basic_coords<T>& start, const basic_coords<T>& finish) noexcept
		{
			AdventCheck(start.x == finish.x);
			return get_column(start, finish.y);
		}

		static elem_range<T> get_range(const basic_coords<T>& start, const basic_coords<T>& finish) noexcept
		{
			const bool going_right = finish.x >= start.x;
			const bool going_up = finish.y >= start.y;
			const T finish_x = going_right ? finish.x + 1 : finish.x-1;
			const T finish_y = going_up ? finish.y + 1 : finish.y - 1;
			return elem_range<T>{start, basic_coords<T>{finish_x,finish_y} };
		}
	};

	namespace internal_helpers
	{
		template <std::integral T>
		class row_adaptor
		{
			basic_coords<T> start;
			basic_coords<T> finish;
		public:
			constexpr row_adaptor(const basic_coords<T>& first, const basic_coords<T>& last) noexcept : start{ first }, finish{ last } {}
			constexpr elem_range<T> operator[](std::size_t idx) const noexcept
			{
				AdventCheck(idx < height(start, finish));
				const T y = get_at_idx(start.y, finish.y, idx);
				return elem_range<T>::get_row(start.x, finish.x, y);
			}
		};

		template <std::integral T>
		using row_range_base = int_range_adaptor<row_adaptor<T>>;

		template <std::integral T>
		class column_adaptor
		{
			basic_coords<T> start;
			basic_coords<T> finish;
		public:
			constexpr column_adaptor(const basic_coords<T>& first, const basic_coords<T>& last) noexcept : start{ first }, finish{ last } {}
			constexpr elem_range<T> operator[](std::size_t idx) const noexcept
			{
				AdventCheck(idx < width(start, finish));
				const T x = get_at_idx(start.x, finish.x, idx);
				return elem_range<T>::get_column(x, start.y, finish.y);
			}
		};

		template <std::integral T>
		using column_range_base = int_range_adaptor<column_adaptor<T>>;
	}

	template <std::integral T>
	class row_range : public internal_helpers::row_range_base<T>
	{
	public:
		constexpr row_range(const basic_coords<T>& first, const basic_coords<T>& last)
			: int_range_adaptor<internal_helpers::row_adaptor<T>>{ internal_helpers::row_adaptor{first,last},internal_helpers::height(first,last) } {}
		explicit constexpr row_range(const basic_coords<T>& last) : row_range{ basic_coords<T>{},last } {}
	};

	template <std::integral T>
	class column_range : internal_helpers::column_range_base<T>
	{
	public:
		constexpr column_range(const basic_coords<T>& first, const basic_coords<T>& last)
			: int_range_adaptor<internal_helpers::column_adaptor<T>>{ internal_helpers::column_adaptor{first,last},internal_helpers::width(first,last) } {}
		explicit constexpr column_range(const basic_coords<T>& last) : column_range{ basic_coords<T>{},last } {}
	};
}