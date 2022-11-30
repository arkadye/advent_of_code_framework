#pragma once

#include <cstddef>
#include <memory>
#include <iterator>
#include <cassert>
#include <stdexcept>
#include <compare>
#include <algorithm>

namespace utils
{
	template <typename T, std::size_t STACK_SIZE, typename ALLOC = std::allocator<T>>
	class small_vector
	{
	public:
		// Typedefs
		using value_type = T;
		using allocator_type = ALLOC;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = typename std::allocator_traits<allocator_type>::pointer;
		using const_pointer = typename std::allocator_traits<allocator_type>::const_pointer;
		using iterator = T*;
		using const_iterator = const T*;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		// Constructors
		constexpr small_vector() noexcept(noexcept(allocator_type())) : small_vector(allocator_type()) {}
		constexpr explicit small_vector(const allocator_type& alloc) noexcept;
		constexpr small_vector(size_type count, const T& init, const allocator_type& alloc = allocator_type());
		constexpr explicit small_vector(size_type count, const allocator_type& alloc = allocator_type());
		template <typename InputIt>
		constexpr small_vector(InputIt first, InputIt last, const allocator_type& alloc = allocator_type());
		constexpr small_vector(const small_vector& other);
		constexpr small_vector(const small_vector& other, const allocator_type& alloc);
		constexpr small_vector(small_vector&& other) noexcept(std::is_nothrow_move_constructible_v<T>);
		constexpr small_vector(small_vector&& other, const allocator_type& alloc);
		constexpr small_vector(std::initializer_list<T> init, const allocator_type& alloc = allocator_type());
		~small_vector();

		// Assignment
		constexpr small_vector& operator=(const small_vector& other);
		constexpr small_vector& operator=(small_vector&& other) noexcept(std::is_nothrow_move_assignable_v<T> && std::is_nothrow_move_constructible_v<T>);
		constexpr small_vector& operator=(std::initializer_list<T> init);

		constexpr void assign(size_type count, const T& value);
		template <typename InputIt>
		constexpr void assign(InputIt first, InputIt last);
		constexpr void assign(std::initializer_list<T> init);

		// Allocator
		constexpr allocator_type get_allocator() const noexcept { return allocator_type(); }

		// Element access
		constexpr reference at(size_type pos);
		constexpr const_reference at(size_type pos) const;
		constexpr reference operator[](size_type pos);
		constexpr const_reference operator[](size_type pos) const;
		constexpr reference front();
		constexpr const_reference front() const;
		constexpr reference back();
		constexpr const_reference back() const;
		constexpr T* data() noexcept;
		constexpr const T* data() const noexcept;

		// Iterators
		[[nodiscard]] iterator begin() noexcept { return data(); }
		[[nodiscard]] iterator end() noexcept { return data() + size(); }
		[[nodiscard]] const_iterator begin() const noexcept { return cbegin(); }
		[[nodiscard]] const_iterator end() const noexcept { return cend(); }
		[[nodiscard]] const_iterator cbegin() const noexcept { return data(); }
		[[nodiscard]] const_iterator cend() const noexcept { return data() + size(); }
		[[nodiscard]] reverse_iterator rbegin() noexcept { return std::make_reverse_iterator(end()); }
		[[nodiscard]] reverse_iterator rend() noexcept { return std::make_reverse_iterator(begin()); }
		[[nodiscard]] const_reverse_iterator rbegin() const noexcept { return crbegin(); }
		[[nodiscard]] const_reverse_iterator rend() const noexcept { return crend(); }
		[[nodiscard]] const_reverse_iterator crbegin() const noexcept { return std::make_reverse_iterator(cend()); }
		[[nodiscard]] const_reverse_iterator crend() const noexcept { return std::make_reverse_iterator(cbegin()); }

		// Capacity
		[[nodiscard]] constexpr bool empty() const noexcept { return size() == 0; }
		[[nodiscard]] constexpr size_type size() const noexcept { return m_num_elements; }
		[[nodiscard]] constexpr size_type max_size() const noexcept;
		constexpr void reserve(size_type new_cap);
		[[nodiscard]] constexpr size_type capacity() const noexcept { return m_capacity; }
		constexpr void shrink_to_fit();

		// Modifiers
		constexpr void clear() noexcept;
		constexpr iterator insert(const_iterator pos, const T& value) { return insert(pos, 1, value); }
		constexpr iterator insert(const_iterator pos, T&& value);
		constexpr iterator insert(const_iterator pos, size_type count, const T& value);
		template <typename InputIt>
		constexpr iterator insert(const_iterator pos, InputIt first, InputIt last);
		constexpr iterator insert(const_iterator pos, std::initializer_list<T> init);
		template <typename ...Args>
		constexpr iterator emplace(const_iterator pos, Args&& ... args);
		constexpr iterator erase(const_iterator pos) { return erase(pos, pos + 1); }
		constexpr iterator erase(const_iterator first, const_iterator last);
		constexpr void push_back(const T& value) { emplace_back(value); }
		constexpr void push_back(T&& value) { emplace_back(std::forward<T>(value)); }
		template <typename ...Args>
		constexpr reference emplace_back(Args&&...args);
		constexpr void pop_back();
		constexpr void resize(size_type count) { resize(count, T()); }
		constexpr void resize(size_type count, const T& value);
		constexpr void swap(small_vector& other) noexcept;

	private:
		constexpr static std::size_t stack_buffer_size() noexcept
		{
			constexpr std::size_t min_storage = sizeof(T*) / sizeof(T);
			return min_storage > STACK_SIZE ? min_storage : STACK_SIZE;
		}
		constexpr void grow(std::size_t min_capacity)
		{
			if (min_capacity <= capacity())
			{
				return;
			}
			std::size_t target_capacity = capacity();
			while (target_capacity < min_capacity)
			{
				target_capacity += 1 + target_capacity / 2;
			}
			reserve(target_capacity);
		}

		struct alignas(T) stack_buffer
		{
			std::byte memory[sizeof(T) * stack_buffer_size()];
		};

		union data_access
		{
			stack_buffer stack_buffer;
			T* heap_data;
		};

		constexpr T* get_stack_buffer() noexcept
		{
			return reinterpret_cast<T*>(m_data.stack_buffer.memory);
		}

		constexpr const T* get_stack_buffer() const noexcept
		{
			return reinterpret_cast<const T*>(m_data.stack_buffer.memory);
		}

		data_access m_data;
		std::size_t m_num_elements;
		std::size_t m_capacity;

		constexpr bool using_heap() const noexcept
		{
			return m_capacity > stack_buffer_size();
		}

		constexpr bool using_stack() const
		{
			return !using_heap();
		}

		constexpr static bool can_use_move_internally() noexcept
		{
			return !std::is_trivially_copyable_v<T> &&
				(std::is_nothrow_move_assignable_v<T> || !std::is_nothrow_copy_assignable_v<T>);
		}

		template <typename U>
		struct BasicBuffer
		{
			U* start;
			U* finish;
			BasicBuffer(U* init_start, U* init_finish) : start{ init_start }, finish{ init_finish }{}
			BasicBuffer() : BasicBuffer{ nullptr,nullptr } {}
			constexpr std::size_t size() const noexcept { return std::distance(start, finish); }
			constexpr bool empty() const noexcept { return start == finish; }
			U* begin() { return start; }
			U* end() { return finish; }
		};

		using Buffer = BasicBuffer<T>;

		struct InitialisedBuffer : Buffer
		{
			using Buffer::Buffer;
		};
		struct RawMemory : Buffer
		{
			using Buffer::Buffer;
		};

		constexpr InitialisedBuffer get_initialised_memory() noexcept
		{
			return InitialisedBuffer{ begin(),end() };
		}

		constexpr RawMemory get_unitialised_memory() noexcept
		{
			return RawMemory{ end(),begin() + capacity() };
		}

		template <typename Op>
		constexpr static void buffer_pair_operation(Buffer a_buf, Buffer b_buf, const Op& operation)
		{
			assert(a_buf.size() <= b_buf.size());
			for (T* a = a_buf.start, *b = b_buf.start; a != a_buf.finish; ++a, ++b)
			{
				operation(a, b);
			}
		}

		template <typename Op>
		constexpr static void buffer_copy_op(InitialisedBuffer from, Buffer to, const Op& copy_op)
		{
			assert(from.size() <= to.size());
			if constexpr (std::is_trivially_copyable_v<T>)
			{
				std::memmove(to.start, from.start, sizeof(T) * std::distance(from.start, from.finish));
			}
			else
			{
				buffer_pair_operation(from, to, copy_op);
			}
		}

		constexpr static void copy_buffer_to_raw_memory(InitialisedBuffer from, RawMemory to)
		{
			buffer_copy_op(from, to, [](T* f, T* t) { new(t) T(*f); });
		}

		constexpr static void copy_buffer_to_initialised_memory(InitialisedBuffer from, InitialisedBuffer to)
		{
			buffer_copy_op(from, to, [](T* f, T* t) { *t = *f; });
		}

		constexpr static void delete_data_in_buffer(InitialisedBuffer buf)
		{
			if constexpr (!std::is_trivially_destructible_v<T>)
			{
				for (T* it = buf.start; it != buf.finish; ++it)
				{
					it->~T();
				}
			}
		}

		constexpr static void move_buffer_to_raw_memory(InitialisedBuffer from, RawMemory to)
		{
			assert(from.size() <= to.size());
			if constexpr (!can_use_move_internally())
			{
				copy_buffer_to_raw_memory(from,to);
			}
			else
			{
				buffer_pair_operation(from, to, [](T* from_it, T* to_it) {new(to_it) T(std::move(*from_it)); });
			}
		}

		constexpr static void move_buffer_to_initialised_memory(InitialisedBuffer from, InitialisedBuffer to)
		{
			assert(from.size() <= to.size());
			if constexpr (!can_use_move_internally())
			{
				copy_buffer_to_initialised_memory(from,to);
			}
			else
			{
				buffer_pair_operation(from, to, [](T* from_it, T* to_it) {*to_it = *from_it; });
			}
		}

		struct GapDescription
		{
			InitialisedBuffer initialised_memory;
			RawMemory uninitialised_memory;
			constexpr std::size_t size() const noexcept { return initialised_memory.size() + uninitialised_memory.size(); }
			constexpr Buffer get_unified_buffer() const noexcept
			{
				if (uninitialised_memory.empty())
				{
					return initialised_memory;
				}
				if (initialised_memory.empty())
				{
					return uninitialised_memory;
				}
				assert(initialised_memory.finish == uninitialised_memory.start);
				return Buffer{ initialised_memory.start,uninitialised_memory.finish };
			}
		};

		constexpr static void move_buffer_to_memory(InitialisedBuffer from, const GapDescription to)
		{
			if (from.empty())
			{
				return;
			}
			if constexpr (std::is_trivially_copyable_v<T>)
			{
				const std::size_t num_bytes = from.size() * sizeof(T);
				const Buffer target_buffer = to.get_unified_buffer();
				std::memmove(target_buffer.start, from.start, num_bytes);
			}
			else
			{
				if (!to.initialised_memory.empty())
				{
					if (from.size() <= to.initialised_memory.size())
					{
						move_buffer_to_initialised_memory(from, to.initialised_memory);
						return;
					}
					else
					{
						T* split_point = from.start + to.initialised_memory.size();
						const Buffer from_initialised{ from.start,split_point };
						move_buffer_to_initialised_memory(from_initialised, to.initialised_memory);
						from.start = split_point;
					}
				}

				assert(from.size() <= to.unitialised_memory.size());
				if (!to.uninitialised_memory.empty())
				{
					move_buffer_to_raw_memory(from, to.uninitialised_memory);
				}
			}
		}

		constexpr static void copy_buffer_to_memory(InitialisedBuffer from, const GapDescription to)
		{
			if (from.empty())
			{
				return;
			}
			if constexpr (std::is_trivially_copyable_v<T>)
			{
				const std::size_t num_bytes = from.size() * sizeof(T);
				const Buffer target_buffer = to.get_unified_buffer();
				std::memmove(target_buffer.start, from.start, num_bytes);
			}
			else
			{
				if (!to.initialised_memory.empty())
				{
					if (from.size() <= to.initialised_memory.size())
					{
						copy_buffer_to_initialised_memory(from, to.initialised_memory);
						return;
					}
					else
					{
						T* split_point = from.start + to.initialised_memory.size();
						const InitialisedBuffer from_initialised{ from.start,split_point };
						copy_buffer_to_initialised_memory(from_initialised, to.initialised_memory);
						from.start = split_point;
					}
				}

				assert(from.size() <= to.uninitialised_memory.size());
				if (!to.uninitialised_memory.empty())
				{
					copy_buffer_to_raw_memory(from, to.uninitialised_memory);
				}
			}
		}

		void assert_iterator(const_iterator it) noexcept
		{
			assert(cbegin() <= it);
			assert(it <= cend());
		}

		constexpr GapDescription make_gap_for_insert(const_iterator pos, size_type gap_size)
		{
			assert(size > 0);
			assert_iterator(pos);
			const size_type distance_from_start = std::distance(cbegin(), pos);
			const size_type distance_from_end = std::distance(pos, cend());
			grow(size() + gap_size);
			pos = cbegin() + distance_from_start;
			assert_iterator(pos);
			if (distance_from_end == 0)
			{
				return GapDescription{ InitialisedBuffer{},RawMemory{end(),end() + gap_size} };
			}

			if constexpr (std::is_trivially_copyable_v<T>)
			{
				T* const source = pos;
				T* const target = pos + gap_size;
				const std::size_t bytes = distance_from_end * sizeof(T);
				std::memmove(target, source, bytes);
				return GapDescription{ InitialisedBuffer{},RawMemory{source,target} };
			}
			else
			{
				for (size_type fwd_i = 0;fwd_i<gap_size;++fwd_i)
				{
					const size_type i = gap_size - fwd_i - 1;
					const size_type from_idx = distance_from_start + i;
					const size_type target_idx = from_idx + gap_size;
					T* from_loc = data() + from_idx;
					T* to_loc = data() + target_idx;
					const InitialisedBuffer from_buf{from_loc,from_loc+1 };
					if (target_idx < size())
					{
						const InitialisedBuffer to_buf{ to_loc,to_loc + 1 };
						move_buffer_to_initialised_memory(from_buf, to_buf);
					}
					else
					{
						const RawMemory to_buf{ to_loc,to_loc + 1 };
						move_buffer_to_raw_memory(from_buf, to_buf);
					}
				}
				return distance_from_end >= gap_size ?
					GapDescription{ InitialisedBuffer{pos,pos + gap_size},RawMemory{} } :
					GapDescription{ InitialisedBuffer{pos,end()},RawMemory{end(),pos + gap_size} };
			}
		}

		static constexpr bool can_fill_with_memset() noexcept
		{
			return std::is_trivially_constructible_v<T> && (sizeof(T) == 1);
		}

		constexpr void memset_buffer(Buffer memory, const T& value)
		{
			static_assert(can_fill_with_memset());
			std::memset(memory.start, reinterpret_cast<int>(value), memory.size());
		}

		template <typename Op>
		constexpr void fill_memory_with_op(Buffer memory, const T& value, const Op& op)
		{
			if constexpr (can_fill_with_memset())
			{
				memset_buffer(memory, value);
			}
			for (T* it = memory.start; it != memory.finish; ++it)
			{
				op(it, value);
			}
		}

		constexpr void fill_initialised_memory(InitialisedBuffer memory, const T& value)
		{
			fill_memory_with_op(memory, value, [](T* loc, const T& val) {*loc = val; });
		}

		constexpr void fill_raw_memory(RawMemory memory, const T& value)
		{
			fill_memory_with_op(memory, value, [&value](T* loc, const T& val) {loc = new (loc) T(value); });
		}

		constexpr void fill_memory(GapDescription memory, const T& value)
		{
			if constexpr (can_fill_with_memset())
			{
				memset_buffer(memory.get_unified_memory(), value);
			}
			fill_initialised_memory(memory.initialised_memory, value);
			fill_raw_memory(memory.uninitialised_memory, value);
		}
	};
}

template <typename T, std::size_t STACK_SIZE, typename ALLOC>
inline constexpr utils::small_vector<T, STACK_SIZE, ALLOC>::small_vector(const allocator_type& alloc) noexcept
	: m_num_elements{ 0 }, m_capacity{ stack_buffer_size() }{}

template <typename T, std::size_t STACK_SIZE, typename ALLOC>
inline constexpr utils::small_vector<T, STACK_SIZE, ALLOC>::small_vector(size_type count, const allocator_type& alloc)
	: small_vector(count, T(), alloc) {}

template<typename T, std::size_t STACK_SIZE, typename ALLOC>
inline constexpr typename utils::small_vector<T, STACK_SIZE, ALLOC>::size_type utils::small_vector<T, STACK_SIZE, ALLOC>::max_size() const noexcept
{
	return std::allocator_traits<ALLOC>::max_size(ALLOC());
}

template<typename T, std::size_t STACK_SIZE, typename ALLOC>
inline constexpr utils::small_vector<T, STACK_SIZE, ALLOC>::small_vector(size_type count, const T& init, const allocator_type& alloc)
	: small_vector(alloc)
{
	assign(count, init);
}

template<typename T, std::size_t STACK_SIZE, typename ALLOC>
template<typename InputIt>
inline constexpr utils::small_vector<T, STACK_SIZE, ALLOC>::small_vector(InputIt first, InputIt last, const allocator_type& alloc)
	: small_vector(alloc)
{
	assign(first, last);
}

template<typename T, std::size_t STACK_SIZE, typename ALLOC>
inline constexpr utils::small_vector<T, STACK_SIZE, ALLOC>::small_vector(const small_vector<T, STACK_SIZE, ALLOC>& other)
	: small_vector(other,allocator_type())
{
}

template<typename T, std::size_t STACK_SIZE, typename ALLOC>
inline constexpr utils::small_vector<T, STACK_SIZE, ALLOC>::small_vector(const small_vector<T, STACK_SIZE, ALLOC>& other, const allocator_type& alloc)
	: small_vector(other.begin(),other.end(),alloc)
{
}

template<typename T, std::size_t STACK_SIZE, typename ALLOC>
inline constexpr utils::small_vector<T, STACK_SIZE, ALLOC>::small_vector(small_vector<T, STACK_SIZE, ALLOC>&& other) noexcept(std::is_nothrow_move_constructible_v<T>)
	: small_vector(std::forward<small_vector<T,STACK_SIZE,ALLOC>>(other),allocator_type())
{
}

template<typename T, std::size_t STACK_SIZE, typename ALLOC>
inline constexpr utils::small_vector<T, STACK_SIZE, ALLOC>::small_vector(std::initializer_list<T> init, const allocator_type& alloc)
	: small_vector(alloc)
{
	assign(init);
}

template<typename T, std::size_t STACK_SIZE, typename ALLOC>
inline constexpr utils::small_vector<T, STACK_SIZE, ALLOC>::small_vector(small_vector<T,STACK_SIZE,ALLOC>&& other, const allocator_type& alloc)
	: small_vector(alloc)
{
	*this = std::forward<small_vector<T, STACK_SIZE, ALLOC>>(other);
}

template<typename T, std::size_t STACK_SIZE, typename ALLOC>
inline constexpr utils::small_vector<T, STACK_SIZE, ALLOC>& utils::small_vector<T, STACK_SIZE, ALLOC>::operator=(const small_vector<T, STACK_SIZE, ALLOC>& other)
{
	if (&other != this)
	{
		assign(other.begin(), other.end());
	}
	return *this;
}

template<typename T, std::size_t STACK_SIZE, typename ALLOC>
inline constexpr utils::small_vector<T, STACK_SIZE, ALLOC>& utils::small_vector<T, STACK_SIZE, ALLOC>::operator=(std::initializer_list<T> init)
{
	assign(init);
	return *this;
}

template<typename T, std::size_t STACK_SIZE, typename ALLOC>
inline constexpr void utils::small_vector<T, STACK_SIZE, ALLOC>::assign(std::initializer_list<T> init)
{
	auto move_it = [](auto it)
	{
		return std::make_move_iterator(it);
	};
	assign(move_it(init.begin()), move_it(init.end()));
}

template<typename T, std::size_t STACK_SIZE, typename ALLOC>
inline utils::small_vector<T, STACK_SIZE, ALLOC>::~small_vector()
{
	clear();
	shrink_to_fit();
}

template<typename T, std::size_t STACK_SIZE, typename ALLOC>
inline constexpr typename utils::small_vector<T, STACK_SIZE, ALLOC>::reference utils::small_vector<T, STACK_SIZE, ALLOC>::operator[](size_type pos)
{
	assert(pos < size());
	return data()[pos];
}

template<typename T, std::size_t STACK_SIZE, typename ALLOC>
inline constexpr typename utils::small_vector<T, STACK_SIZE, ALLOC>::const_reference utils::small_vector<T, STACK_SIZE, ALLOC>::operator[](size_type pos) const
{
	assert(pos < size());
	return data()[pos];
}

template<typename T, std::size_t STACK_SIZE, typename ALLOC>
inline constexpr typename utils::small_vector<T, STACK_SIZE, ALLOC>::reference utils::small_vector<T, STACK_SIZE, ALLOC>::at(size_type pos)
{
	if (pos >= size())
	{
		throw std::out_of_range("Out of range error: pos >= small_vector::size()");
	}
	return operator[](pos);
}

template<typename T, std::size_t STACK_SIZE, typename ALLOC>
inline constexpr typename utils::small_vector<T, STACK_SIZE, ALLOC>::const_reference utils::small_vector<T, STACK_SIZE, ALLOC>::at(size_type pos) const
{
	if (pos >= size())
	{
		throw std::out_of_range("Out of range error: pos >= small_vector::size()");
	}
	return operator[](pos);
}

template<typename T, std::size_t STACK_SIZE, typename ALLOC>
inline constexpr T* utils::small_vector<T, STACK_SIZE, ALLOC>::data() noexcept
{
	return using_heap() ? m_data.heap_data : get_stack_buffer();
}

template<typename T, std::size_t STACK_SIZE, typename ALLOC>
inline constexpr const T* utils::small_vector<T, STACK_SIZE, ALLOC>::data() const noexcept
{
	return using_heap() ? m_data.heap_data : get_stack_buffer();
}

template<typename T, std::size_t STACK_SIZE, typename ALLOC>
inline constexpr void utils::small_vector<T, STACK_SIZE, ALLOC>::reserve(size_type new_cap)
{
	if (new_cap <= capacity())
	{
		return;
	}

	T* new_data = get_allocator().allocate(new_cap);
	const InitialisedBuffer old_buffer = get_initialised_memory();
	const RawMemory new_buffer{ new_data,new_data + new_cap };
	move_buffer_to_raw_memory(old_buffer,new_buffer);
	delete_data_in_buffer(old_buffer);

	if (using_heap())
	{
		get_allocator().deallocate(old_buffer.start, capacity());
	}

	m_data.heap_data = new_data;
	m_capacity = new_cap;
}

template<typename T, std::size_t STACK_SIZE, typename ALLOC>
inline constexpr void utils::small_vector<T, STACK_SIZE, ALLOC>::shrink_to_fit()
{
	if (size() == capacity()) // Nothing to do.
	{
		return;
	}

	if (using_stack()) // Can't shrink.
	{
		return;
	}

	const InitialisedBuffer old_buffer = get_initialised_memory();
	const RawMemory new_buffer = [this]()
	{
		if (size() <= stack_buffer_size())
		{
			return RawMemory{ get_stack_buffer(),get_stack_buffer() + size() };
		}
		T* new_start = get_allocator().allocate(size());
		return RawMemory{ new_start,new_start + size() };
	}();
	move_buffer_to_raw_memory(old_buffer, new_buffer);
	delete_data_in_buffer(old_buffer);
	get_allocator().deallocate(old_buffer.start,capacity());
	m_capacity = std::max(stack_buffer_size(), size());
}

template<typename T, std::size_t STACK_SIZE, typename ALLOC>
inline constexpr utils::small_vector<T, STACK_SIZE, ALLOC>& utils::small_vector<T, STACK_SIZE, ALLOC>::operator=(small_vector&& other) noexcept(std::is_nothrow_move_assignable_v<T>&& std::is_nothrow_move_constructible_v<T>)
{
	if (this == &other)
	{
		return *this;
	}
	if (other.using_heap())
	{
		clear();
		shrink_to_fit();
		m_data.heap_data = other.m_data.heap_data;
		m_capacity = other.capacity();
		m_num_elements = other.size();
		other.m_capacity = other.stack_buffer_size();
		other.m_num_elements = 0;
		return *this;
	}
	
	assert(capacity() >= other.size());
	if constexpr (std::is_trivially_copy_assignable_v<T>)
	{
		std::memcpy(begin(), other.begin(), sizeof(T) * other.size());
		m_num_elements = other.size();
		return *this;
	}

	if (size() == other.size())
	{
		move_buffer_to_initialised_memory(other.get_initialised_memory(), get_initialised_memory());
	}
	else if (size() < other.size())
	{
		move_buffer_to_initialised_memory(InitialisedBuffer{ other.begin(),other.begin() + size() }, get_initialised_memory());
		move_buffer_to_raw_memory(InitialisedBuffer{ other.begin() + size(),other.end() }, get_unitialised_memory());
	}
	else // size() > other.size()
	{
		move_buffer_to_initialised_memory(other.get_initialised_memory(), get_initialised_memory());
		delete_data_in_buffer(InitialisedBuffer{ begin() + other.size(),end() });
	}
	m_num_elements = other.size();
	return *this;
}

template<typename T, std::size_t STACK_SIZE, typename ALLOC>
inline constexpr void utils::small_vector<T, STACK_SIZE, ALLOC>::clear() noexcept
{
	delete_data_in_buffer(get_initialised_memory());
	m_num_elements = 0;
}

template<typename T, std::size_t STACK_SIZE, typename ALLOC>
inline constexpr typename utils::small_vector<T, STACK_SIZE, ALLOC>::iterator utils::small_vector<T, STACK_SIZE, ALLOC>::insert(const_iterator pos, T&& value)
{
	const GapDescription gap = make_gap_for_insert(pos, 1);
	assert(gap.initialized_memory.empty() != gap.uninitialised_memory.size());
	move_buffer_to_memory(Buffer{ &value,(&value) + 1 }, gap);
	++m_num_elements;
	return gap.initialised_memory.empty() ? gap.uninitialised_memory.finish : gap.initialised_memory.finish;
}

template<typename T, std::size_t STACK_SIZE, typename ALLOC>
inline constexpr typename utils::small_vector<T, STACK_SIZE, ALLOC>::iterator utils::small_vector<T, STACK_SIZE, ALLOC>::insert(const_iterator pos, size_type count, const T& value)
{
	const GapDescription gap = make_gap_for_insert(pos, count);
	fill_memory(gap, value);
	m_num_elements += count;
}

template<typename T, std::size_t STACK_SIZE, typename ALLOC>
inline constexpr typename utils::small_vector<T, STACK_SIZE, ALLOC>::iterator utils::small_vector<T, STACK_SIZE, ALLOC>::insert(const_iterator pos, std::initializer_list<T> init)
{
	auto move_it = [](auto it)
	{
		return std::make_move_iterator(it);
	};
	return insert(pos, move_it(init.begin()), move_it(init.end()));
}

template<typename T, std::size_t STACK_SIZE, typename ALLOC>
template<typename ...Args>
inline constexpr typename utils::small_vector<T, STACK_SIZE, ALLOC>::iterator utils::small_vector<T, STACK_SIZE, ALLOC>::emplace(const_iterator pos, Args && ...args)
{
	if (pos == cend())
	{
		emplace_back(std::forward<Args>(args)...);
		return end();
	}
	GapDescription gap = make_gap_for_insert(pos, 1);
	assert(gap.initialised_memory.size() == 1);
	assert(gap.uninitualised_memory.empty());
	*gap.initialised_memory.first = T(std::forward<Args>(args)...);
	++m_num_elements;
	return gap.initialised_memory.last;
}

template<typename T, std::size_t STACK_SIZE, typename ALLOC>
template<typename InputIt>
inline constexpr typename utils::small_vector<T, STACK_SIZE, ALLOC>::iterator utils::small_vector<T, STACK_SIZE, ALLOC>::insert(const_iterator pos, InputIt first, InputIt last)
{
	using ItCategory = typename std::iterator_traits<InputIt>::iterator_category;
	if constexpr (std::is_convertible_v<ItCategory, std::input_iterator_tag>)
	{
		const auto size_increase = std::distance(first, last);
		GapDescription gap = make_gap_for_insert(pos, size_increase);
		assert(gap.size() == size_increase);
		InputIt it = first;
		for (T* init = gap.initialised_memory.start; init != gap.initialised_memory.finish; ++init)
		{
			*init = *(it++);
		}
		for (T* uninit = gap.uninitialised_memory.start; uninit != gap.uninitialised_memory.finish; ++uninit)
		{
			uninit = new(uninit) T(*(it++));
		}
		m_num_elements += size_increase;
		return gap.get_unified_buffer().finish;
	}
	// Can't use std::difference
	for (auto it = first; it != last; ++it)
	{
		pos = insert(pos, *it);
	}
	return pos;
}

template<typename T, std::size_t STACK_SIZE, typename ALLOC>
template<typename ...Args>
inline constexpr typename utils::small_vector<T, STACK_SIZE, ALLOC>::reference utils::small_vector<T, STACK_SIZE, ALLOC>::emplace_back(Args && ...args)
{
	grow(size() + 1);
	new(data() + size()) T(std::forward<Args>(args)...);
	++m_num_elements;
	return back();
}

template<typename T, std::size_t STACK_SIZE, typename ALLOC>
inline constexpr typename utils::small_vector<T, STACK_SIZE, ALLOC>::iterator utils::small_vector<T, STACK_SIZE, ALLOC>::erase(const_iterator first, const_iterator last)
{
	auto to_nc_it = [this](const_iterator it) -> iterator
	{
		const auto idx = std::distance(cbegin(), it);
		return begin() + idx;
	};
	if (first == last)
	{
		return to_nc_it(last);
	}

	const auto num_removed = std::distance(first, last);
	assert(static_cast<std::size_t>(num_removed) <= size());

	const auto tail_length = static_cast<std::size_t>(std::distance(last, cend()));
	
	if (last != cend())
	{
		if constexpr (std::is_trivially_copy_assignable_v<T>)
		{
			std::memmove(to_nc_it(first), to_nc_it(last), sizeof(T) * tail_length);
		}
		else
		{
			for (auto from_it = to_nc_it(last), to_it = to_nc_it(first); from_it != cend(); ++from_it, ++to_it)
			{
				if constexpr (can_use_move_internally())
				{
					*to_it = std::move(*from_it);
				}
				else
				{
					*to_it = *from_it;
				}
			}
		}
	}

	InitialisedBuffer tail_buffer{ end() - num_removed,end() };
	delete_data_in_buffer(tail_buffer);

	m_num_elements -= num_removed;
	const auto return_idx = std::distance(cbegin(), first);
	return begin() + return_idx;
}

template<typename T, std::size_t STACK_SIZE, typename ALLOC>
inline constexpr void utils::small_vector<T, STACK_SIZE, ALLOC>::pop_back()
{
	assert(!empty());
	erase(cend() - 1);
}

template<typename T, std::size_t STACK_SIZE, typename ALLOC>
inline constexpr void utils::small_vector<T, STACK_SIZE, ALLOC>::resize(size_type count, const T& value)
{
	if (count == 0)
	{
		clear();
	}

	if (count <= size())
	{
		InitialisedBuffer to_remove{ begin() + count,end() };
		delete_data_in_buffer(to_remove);
	}
	else
	{
		grow(count);
		RawMemory to_add{ end(),begin() + count };
		fill_raw_memory(to_add, value);
	}

	m_num_elements = count;
}

template<typename T, std::size_t STACK_SIZE, typename ALLOC>
inline constexpr void utils::small_vector<T, STACK_SIZE, ALLOC>::swap(small_vector& other) noexcept
{
	if (using_heap() && other.using_heap())
	{
		std::swap(m_data.heap_data, other.m_data.heap_data);
	}
	else
	{
		small_vector<T, STACK_SIZE, ALLOC> temp = std::move(other);
		other = std::move(this);
		*this = std::move(temp);
	}
}

template<typename T, std::size_t STACK_SIZE, typename ALLOC>
inline constexpr void utils::small_vector<T, STACK_SIZE, ALLOC>::assign(size_type count, const T& value)
{
	grow(count);

	T* new_end = begin() + count;
	if (count <= size())
	{
		fill_initialised_memory(InitialisedBuffer{ begin(),new_end }, value);
		delete_data_in_buffer(InitialisedBuffer{ new_end,end() });
	}
	else
	{
		GapDescription memory{ InitialisedBuffer{begin(),end()},RawMemory{end(),new_end} };
		fill_memory(memory, value);
	}

	m_num_elements = count;
}

template<typename T, std::size_t STACK_SIZE, typename ALLOC>
template<typename InputIt>
inline constexpr void utils::small_vector<T, STACK_SIZE, ALLOC>::assign(InputIt first, InputIt last)
{
	using ItCategory = typename std::iterator_traits<InputIt>::iterator_category;

	static_assert(!std::is_same_v<ItCategory, std::output_iterator_tag>, "Cannot assign from an output iterator.");

	const auto clear_tail = [this](std::size_t num_copied)
	{
		if (num_copied < size())
		{
			InitialisedBuffer to_delete{ begin() + num_copied,end() };
			delete_data_in_buffer(to_delete);
			m_num_elements = num_copied;
		}
	};

	const auto memberwise_copy = [this, &first, &last]()
	{
		std::size_t num_copied = 0;
		while (first != last)
		{
			if (num_copied < size())
			{
				(*this)[num_copied] = *first++;
			}
			else
			{
				push_back(*first++);
			}
			++num_copied;
		}
		return num_copied;
	};

	// Only one pass is available of the input, so use it carefully.
	if constexpr (std::is_same_v<ItCategory, std::input_iterator_tag>)
	{
		const std::size_t num_copied = memberwise_copy();

		clear_tail(num_copied);
		
		assert(size() == num_copied);
		return;
	}

	// This option allows us to check the size of the range and plan accordingly.
	const auto new_size = static_cast<std::size_t>(std::distance(first, last));
	if (new_size > capacity())
	{
		// Clear everything to save copying it later, as it will only be overridden anyway.
		clear();
		reserve(new_size);
	}

	// If we have pointer-style iterators, we can use an optimised version
	if constexpr (std::is_pointer_v<InputIt> && std::is_same_v<InputIt,T>)
	{
		GapDescription target{
			get_initialised_memory(),
			get_unitialised_memory()
		};
		InitialisedBuffer input{ const_cast<T*>(first),const_cast<T*>(last) };
		copy_buffer_to_memory(input, target);
		m_num_elements = new_size;
		return;
	}
	
	// Otherwise we have to do a memberwise copy.
	memberwise_copy();
	m_num_elements = new_size;
	return;
}

template<typename T, std::size_t STACK_SIZE, typename ALLOC>
inline constexpr typename utils::small_vector<T, STACK_SIZE, ALLOC>::reference utils::small_vector<T, STACK_SIZE, ALLOC>::front()
{
	assert(!empty());
	return (*this)[0];
}

template<typename T, std::size_t STACK_SIZE, typename ALLOC>
inline constexpr typename utils::small_vector<T, STACK_SIZE, ALLOC>::const_reference utils::small_vector<T, STACK_SIZE, ALLOC>::front() const
{
	assert(!empty());
	return (*this)[0];
}

template<typename T, std::size_t STACK_SIZE, typename ALLOC>
inline constexpr typename utils::small_vector<T, STACK_SIZE, ALLOC>::reference utils::small_vector<T, STACK_SIZE, ALLOC>::back()
{
	assert(!empty());
	return (*this)[size() - 1];
}

template<typename T, std::size_t STACK_SIZE, typename ALLOC>
inline constexpr typename utils::small_vector<T, STACK_SIZE, ALLOC>::const_reference utils::small_vector<T, STACK_SIZE, ALLOC>::back() const
{
	assert(!empty());
	return (*this)[size() - 1];
}

template<typename T_L, typename T_R, std::size_t STACK_SIZE_L, std::size_t STACK_SIZE_R, typename ALLOC_L, typename ALLOC_R>
inline constexpr bool operator==(const utils::small_vector<T_L, STACK_SIZE_L, ALLOC_L>& left, const utils::small_vector<T_R, STACK_SIZE_R, ALLOC_R>& right) noexcept
{
	return std::equal(begin(left), end(left), begin(right), end(right));
}

template<typename T_L, typename T_R, std::size_t STACK_SIZE_L, std::size_t STACK_SIZE_R, typename ALLOC_L, typename ALLOC_R>
inline constexpr auto operator<=>(const utils::small_vector<T_L, STACK_SIZE_L, ALLOC_L>& left, const utils::small_vector<T_R, STACK_SIZE_R, ALLOC_R>& right) noexcept
{
	return std::lexicographical_compare_three_way(begin(left), end(left), begin(right), end(right));
}