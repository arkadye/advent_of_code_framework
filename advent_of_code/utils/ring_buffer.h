#pragma once

#include <vector>
#include <algorithm>

#include "index_iterator2.h"

namespace utils
{
	template <typename  T, std::size_t SIZE>
	class ring_buffer
	{
		std::vector<T> m_data;
		std::size_t m_front_idx;
		constexpr std::size_t get_idx(std::size_t i) const noexcept
		{
			return (i + m_front_idx) % SIZE;
		}
	public:
		using value_type = T;
		using side_type = std::size_t;
		using difference_type = std::ptrdiff_t;
		using reference_type = T&;
		using const_reference_type = const T&;
		using pointer_type = T*;
		using const_pointer_type = const T*;
		using iterator = index_iterator2<ring_buffer<T, SIZE>>;
		using const_iterator = const_index_iterator2<ring_buffer<T, SIZE>>;

		explicit ring_buffer() { m_data.resize(SIZE); }

		ring_buffer(const ring_buffer&) = default;
		ring_buffer(ring_buffer&&) = default;
		ring_buffer& operator=(const ring_buffer&) = default;
		ring_buffer& operator=(ring_buffer&&) = default;
		T& operator[](std::size_t i) noexcept { return m_data[get_idx(i)]; }
		const T& operator[](std::size_t i) const noexcept { return m_data[get_idx(i)]; }
		T& front() { return (*this)[0]; }
		const T& front() const { return (*this)[0]; }
		T& back() { return (*this)[SIZE - 1]; }
		const T& back() const { return (*this)[SIZE - 1]; }

		bool empty() const { return m_data.empty(); }
		std::size_t size() const { return m_data.size(); }
		std::size_t max_size() const { return m_data.max_size(); }

		void fill(const T& value) { m_data.fill(value); }
		void swap(ring_buffer& other) { m_data.swap(other.m_data); std::swap(m_front_idx, other.m_front_idx); }
		void rotate(std::size_t new_front) { m_front_idx = get_idx(new_front); }
		void rotate(const_iterator new_front) { rotate(new_front - begin()); }

		const_iterator cbegin() const
		{
			return make_idx_it_cbegin(*this);
		}
		const_iterator cend() const
		{
			return make_idx_it_cend(*this);
		}
		const_iterator begin() const
		{
			return cbegin();
		}
		const_iterator end() const
		{
			return cend();
		}
		iterator begin()
		{
			return make_idx_it_begin(*this);
		}
		iterator end()
		{
			return make_idx_it_end(*this);
		}
	};
}

template <typename T, std::size_t SIZE>
inline bool operator==(const utils::ring_buffer<T, SIZE>& left, const utils::ring_buffer<T, SIZE>& right)
{
	return std::equal(begin(left), end(left), begin(right), end(right));
}

template <typename T, std::size_t SIZE>
inline bool operator!=(const utils::ring_buffer<T, SIZE>& left, const utils::ring_buffer<T, SIZE>& right)
{
	return !(left == right);
}

template <typename T, std::size_t SIZE>
inline bool operator<(const utils::ring_buffer<T, SIZE>& left, const utils::ring_buffer<T, SIZE>& right)
{
	return std::lexicographical_compare(begin(left), end(left), begin(right), end(right));
}

template <typename T, std::size_t SIZE>
inline bool operator>(const utils::ring_buffer<T, SIZE>& left, const utils::ring_buffer<T, SIZE>& right)
{
	return right < left;
}

template <typename T, std::size_t SIZE>
inline bool operator<=(const utils::ring_buffer<T, SIZE>& left, const utils::ring_buffer<T, SIZE>& right)
{
	return !(right < left);
}

template <typename T, std::size_t SIZE>
inline bool operator>=(const utils::ring_buffer<T, SIZE>& left, const utils::ring_buffer<T, SIZE>& right)
{
	return !(left < right);
}