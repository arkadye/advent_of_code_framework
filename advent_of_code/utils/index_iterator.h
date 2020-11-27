#pragma once

#include <iterator>
#include <functional>
#include <cassert>
#include <type_traits>
#include <compare>

class index_iterator_base
{
public:
	template <typename T>
	static auto is_index_iterator(T in)
	{
		static_assert(std::is_base_of_v<index_iterator_base, T>);
		return std::forward<T>(in);
	}
};

// Container must have operator[] defined, and
// a size() method.
template <class Container>
class index_iterator : public index_iterator_base
{
private:
	static auto get_index_return_type() { return Container{} [0] ; }
public:
	// Traits
	using difference_type = std::ptrdiff_t;
	using value_type = typename Container::value_type;
	using pointer = typename Container::pointer_type;
	using reference = typename Container::reference_type;
	using iterator_category = std::random_access_iterator_tag;

private:
	Container* container = nullptr;

protected: 
	difference_type pos = 0;

	index_iterator(Container* c, difference_type p) noexcept : container{ c }, pos{ p }{}
	reference get_from_real_index(difference_type index) const
	{
		assert(container != nullptr);
		assert(index >= 0);
		assert(static_cast<decltype(container->size())>(index) < container->size());
		return container->operator[](index);
	}
public:
	// Constructors
	index_iterator() noexcept {}
	index_iterator(const index_iterator&) noexcept = default;
	index_iterator(index_iterator&&) noexcept = default;
	index_iterator& operator=(const index_iterator&) noexcept = default;
	index_iterator& operator=(index_iterator&&) noexcept = default;

	static index_iterator begin(Container& c) noexcept { return index_iterator{ &c,0 }; }
	static index_iterator end(Container& c) noexcept { return index_iterator{ &c,static_cast<difference_type>(c.size()) }; }

	// Dereference
	virtual reference operator[](difference_type offset) const
	{
		return get_from_real_index(pos + offset);
	}

	auto operator*() const { return (*this)[0]; }

	// Comparisons
	std::strong_ordering operator<=>(const index_iterator& other) const noexcept
	{
		assert(container == other.container);
		return pos <=> other.pos;
	}
	// These aren't defined YET on my impementation
	bool operator==(const index_iterator& other) const noexcept
	{
		return (*this <=> other) == 0;
	}
	bool operator!=(const index_iterator& other) const noexcept { return !(*this == other); }
	bool operator<(const index_iterator& other) const noexcept
	{
		return (*this <=> other) < 0;
	}
	bool operator>(const index_iterator& other) const noexcept { return other < *this; }
	bool operator<=(const index_iterator& other) const noexcept { return !(*this > other); }
	bool operator>=(const index_iterator& other) const noexcept { return !(*this < other); }

	// Arithmetic
	virtual index_iterator& operator+=(difference_type offset) noexcept
	{
		pos += offset;
		return *this;
	}
	index_iterator& operator-=(difference_type offset) noexcept
	{
		return operator+=(-offset);
	}
	index_iterator& operator++(int) noexcept { return operator+=(1); }
	index_iterator& operator--(int) noexcept { return operator-=(1); }
	index_iterator operator++() noexcept
	{
		const auto res = operator++(0);
		return res;
	}
	index_iterator operator--() noexcept
	{
		const auto res = operator--(0);
		return res;
	}
	index_iterator operator+(difference_type offset) const noexcept
	{
		auto res = *this;
		res += offset;
		return res;
	}
	index_iterator operator-(difference_type offset) const noexcept
	{
		auto res = *this;
		res -= offset;
		return res;
	}
	difference_type operator-(index_iterator other) const noexcept
	{
		assert(container == other.container);
		return static_cast<difference_type>(pos) - static_cast<difference_type>(other.pos);
	}
};

template <class Container>
using const_index_iterator = index_iterator<const Container>;

template <class Container>
class reverse_index_iterator : public index_iterator<Container>
{
public:
	using Base = index_iterator<Container>;
	using difference_type = Base::difference_type;
	using value_type = Base::value_type;
	using pointer = Base::pointer;
	using reference = Base::reference;
	using iterator_category = Base::iterator_category;
private:
	reverse_index_iterator(Container* c, difference_type p) noexcept : index_iterator{ c,p } {}
	using Base::pos;
	using Base::get_from_real_index;
public:
	reverse_index_iterator() noexcept : index_iterator{} {}
	reverse_index_iterator& operator+=(difference_type offset) noexcept override final 
	{
		pos -= offset;
		return *this;
	}

	virtual reference operator[](difference_type offset) const noexcept override final
	{
		// This looks weird, but is done to make this act like the standard
		// reverse iterators when they are converted to reverse ones.
		return get_from_real_index(pos - offset - 1);
	}

	static reverse_index_iterator begin(Container& c) noexcept
	{
		return reverse_index_iterator{ &c,c.size() - 1 };
	}

	static reverse_index_iterator end(Container& c) noexcept
	{
		return reverse_index_iterator{ &c,-1 };
	}
};

template <class Container>
using const_reverse_index_iterator = reverse_index_iterator<const Container>;

// Use INDEX_ITERATOR_MEMBER_BOILERPLATE to add begin, end, etc... class functions.
// Use INDEX_ITERATOR_NONMEMBER_BOILERPLATE to add standalone begin, end, etc... class functions
// Use INDEX_ITERATOR_NONMEMBER_TEMPLATE_BOILERPLATE for templates.

#define INDEX_ITERATOR_DECL_ITERATOR_TYPE_NONCONST(ITERATOR,BEGIN,END) \
auto BEGIN() noexcept { \
	return index_iterator_base::is_index_iterator(ITERATOR::begin(*this)); \
} \
auto END() noexcept { \
	return index_iterator_base::is_index_iterator(ITERATOR::end(*this)); \
}

#define INDEX_ITERATOR_DECL_ITERATOR_TYPE_CONST(ITERATOR,BEGIN,END) \
auto BEGIN() const noexcept { \
	return index_iterator_base::is_index_iterator(ITERATOR::begin(*this)); \
} \
auto END() const noexcept { \
	return index_iterator_base::is_index_iterator(ITERATOR::end(*this)); \
}

#define INDEX_ITERATOR_MEMBER_BOILERPLATE(...) \
using iterator = index_iterator<__VA_ARGS__>; \
using const_iterator = const_index_iterator<__VA_ARGS__>; \
using reverse_iterator = reverse_index_iterator<__VA_ARGS__>; \
using const_reverse_iterator = const_reverse_index_iterator<__VA_ARGS__>; \
INDEX_ITERATOR_DECL_ITERATOR_TYPE_NONCONST(iterator,begin,end) \
INDEX_ITERATOR_DECL_ITERATOR_TYPE_NONCONST(reverse_iterator,rbegin,rend) \
INDEX_ITERATOR_DECL_ITERATOR_TYPE_CONST(const_iterator,begin,end) \
INDEX_ITERATOR_DECL_ITERATOR_TYPE_CONST(const_iterator,cbegin,cend) \
INDEX_ITERATOR_DECL_ITERATOR_TYPE_CONST(const_reverse_iterator,rbegin,rend) \
INDEX_ITERATOR_DECL_ITERATOR_TYPE_CONST(const_reverse_iterator,crbegin,crend)

#define INDEX_ITERATOR_MEMBER_BOILERPLATE_CONST(...) \
using iterator = const_index_iterator<__VA_ARGS__>; \
using const_iterator = const_index_iterator<__VA_ARGS__>; \
using reverse_iterator = const_reverse_index_iterator<__VA_ARGS__>; \
using const_reverse_iterator = const_reverse_index_iterator<__VA_ARGS__>; \
INDEX_ITERATOR_DECL_ITERATOR_TYPE_NONCONST(const_iterator,begin,end) \
INDEX_ITERATOR_DECL_ITERATOR_TYPE_NONCONST(const_reverse_iterator,rbegin,rend) \
INDEX_ITERATOR_DECL_ITERATOR_TYPE_CONST(const_iterator,begin,end) \
INDEX_ITERATOR_DECL_ITERATOR_TYPE_CONST(const_iterator,cbegin,cend) \
INDEX_ITERATOR_DECL_ITERATOR_TYPE_CONST(const_reverse_iterator,rbegin,rend) \
INDEX_ITERATOR_DECL_ITERATOR_TYPE_CONST(const_reverse_iterator,crbegin,crend)

using std::begin;
using std::end;
using std::cbegin;
using std::cend;
using std::rbegin;
using std::rend;
using std::crbegin;
