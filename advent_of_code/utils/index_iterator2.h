#pragma once

#include <iterator>

#include "advent/advent_assert.h"

namespace utils
{
	template <typename Container>
	class const_index_iterator2
	{
	protected:
		const Container* container;
		std::size_t pos;
	public:
		using iterator_category = std::random_access_iterator_tag;
		using value_type = typename Container::value_type;
		using difference_type = typename Container::difference_type;
		using pointer = typename Container::pointer_type;
		using reference = typename Container::reference_type;
		const_index_iterator2(const Container& c, std::size_t initial_pos) : container{ &c }, pos{ initial_pos }{}
		const_index_iterator2() : container{ nullptr }, pos{ 0 }{}
		const_index_iterator2(const const_index_iterator2&) = default;
		const_index_iterator2(const_index_iterator2&&) = default;
		const_index_iterator2& operator=(const const_index_iterator2&) = default;
		const_index_iterator2& operator=(const_index_iterator2&&) = default;

		bool operator==(const const_index_iterator2& other) const
		{
			AdventCheck(container == other.container);
			return pos == other.pos;
		}

		bool operator!=(const const_index_iterator2& other) const
		{
			return !(operator==(other));
		}

		difference_type operator-(const const_index_iterator2& other) const
		{
			AdventCheck(container == other.container);
			return static_cast<difference_type>(pos) - other.pos;
		}

//		friend const_index_iterator2<Container>& operator+=(const_index_iterator2<Container>& lhs, difference_type rhs);
//		friend const_index_iterator2<Container>& operator-=(const_index_iterator2<Container>& lhs, difference_type rhs);
		const value_type& operator*() const { return (*container)[pos]; }
		const value_type& operator->() const { return (*container)[pos]; }
		const_index_iterator2& operator+=(difference_type rhs) { pos += rhs; return *this; }
		const_index_iterator2& operator-=(difference_type rhs) { pos -= rhs; return *this; }
		const_index_iterator2& operator++() { return (*this) += difference_type{1}; }
		const_index_iterator2& operator--() { return (*this) -= difference_type{1}; }
		const_index_iterator2 operator++(int)
		{
			const const_index_iterator2 res{ *this };
			++(*this);
			return res;
		}
		const_index_iterator2 operator--(int)
		{
			const const_index_iterator2 res{ *this };
			--(*this);
			return res;
		}
	};

	template <typename Container>
	class index_iterator2 : public const_index_iterator2<Container>
	{
	private:
		using const_index_iterator2<Container>::container;
		using const_index_iterator2<Container>::pos;
	public:
		index_iterator2(const Container& c, std::size_t initial_pos) : const_index_iterator2<Container>{ c,initial_pos } {}
		index_iterator2() : const_index_iterator2<Container>{} {}
		index_iterator2(const index_iterator2&) = default;
		index_iterator2(index_iterator2&&) = default;
		index_iterator2& operator=(const index_iterator2&) = default;
		index_iterator2& operator=(index_iterator2&&) = default;

		typename Container::value_type& operator*() const { return (*const_cast<Container*>(container))[pos]; }
		typename Container::value_type& operator->() const { return (*const_cast<Container*>(container))[pos]; }
		index_iterator2& operator+=(int rhs) { pos += rhs; return *this; }
		index_iterator2& operator-=(int rhs) { pos -= rhs; return *this; }
		index_iterator2& operator++() { return (*this) += 1; }
		index_iterator2& operator--() { return (*this) -= 1; }
		index_iterator2 operator++(int)
		{
			const index_iterator2 res{ *this };
			++(*this);
			return res;
		}
		index_iterator2 operator--(int)
		{
			const index_iterator2 res{ *this };
			--(*this);
			return res;
		}
	};

	template <typename Container>
	inline const_index_iterator2<Container> make_idx_it_cbegin(const Container& c)
	{
		return const_index_iterator2<Container>{ c,0 };
	}

	template <typename Container>
	inline const_index_iterator2<Container> make_idx_it_cend(const Container& c)
	{
		return const_index_iterator2<Container>{ c,c.size() };
	}

	template <typename Container>
	inline const_index_iterator2<Container> make_idx_it_begin(const Container& c)
	{
		return make_idx_it_cbegin(c);
	}

	template <typename Container>
	inline const_index_iterator2<Container> make_idx_it_end(const Container& c)
	{
		return make_idx_it_cend(c);
	}
	
	template <typename Container>
	inline index_iterator2<Container> make_idx_it_begin(Container& c)
	{
		return index_iterator2<Container>{ c,0 };
	}
	
	template <typename Container>
	inline index_iterator2<Container> make_idx_it_end(Container& c)
	{
		return index_iterator2<Container>{ c,c.size() };
	}
}

// template <typename Container>
// inline utils::const_index_iterator2<Container>& operator+=(utils::const_index_iterator2<Container>& lhs, typename utils::const_index_iterator2<Container>::difference_type rhs)
// {
// 	lhs.pos += rhs;
// 	return lhs;
// }
// 
// template <typename Container>
// inline utils::const_index_iterator2<Container>& operator-=(utils::const_index_iterator2<Container>& lhs, typename utils::const_index_iterator2<Container>::difference_type rhs)
// {
// 	lhs.pos -= rhs;
// 	return lhs;
// }

template <typename Container>
inline utils::const_index_iterator2<Container> operator+(const utils::const_index_iterator2<Container>& left, int right)
{
	auto result = left;
	result += right;
	return result;
}

template <typename Container>
inline utils::const_index_iterator2<Container> operator-(const utils::const_index_iterator2<Container>& left, int right)
{
	auto result = left;
	result -= right;
	return result;
}

template <typename Container>
inline utils::index_iterator2<Container> operator+(const utils::index_iterator2<Container>& left, int right)
{
	auto result = left;
	result += right;
	return result;
}

template <typename Container>
inline utils::index_iterator2<Container> operator-(const utils::index_iterator2<Container>& left, int right)
{
	auto result = left;
	result -= right;
	return result;
}