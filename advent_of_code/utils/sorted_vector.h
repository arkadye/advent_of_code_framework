#pragma once

#include "small_vector.h"
#include "binary_find.h"
#include <algorithm>
#include <functional>

#include "advent/advent_assert.h"

namespace utils
{
	template <typename T, typename BinaryPred = std::less<T>, std::size_t BufferSize = 1>
	class sorted_vector
	{
	public:
		using iterator = typename utils::small_vector<T,BufferSize>::iterator;
		using const_iterator = typename utils::small_vector<T,BufferSize>::const_iterator;
		using value_type = T;
		bool can_insert_at_pos(const_iterator pos, const T& value) const noexcept
		{
			const bool check_after = (pos == m_data.cend() || !m_compare(*pos, value));
			if (check_after)
			{
				const bool check_before = (pos == m_data.cbegin() || !m_compare(value, *(pos - 1)));
				if (check_before)
				{
					return true;
				}
			}
			return false;
		}
	protected:
		mutable utils::small_vector<T,BufferSize> m_data;
		BinaryPred m_compare;
		mutable bool m_sorted;
	public:
		sorted_vector() : sorted_vector(BinaryPred{}) {}
		explicit sorted_vector(const BinaryPred& compare)
			: m_data()
			, m_compare(compare)
			, m_sorted(true)
		{
			assert(m_data.empty());
		}
		template <typename InputIt>
		sorted_vector(InputIt start, InputIt finish) : sorted_vector(start, finish, BinaryPred{}) {}

		template <typename InputIt>
		sorted_vector(InputIt start, InputIt finish, BinaryPred compare)
			: m_data(start, finish)
			, m_compare(compare)
			, m_sorted(false)
		{}

		sorted_vector(std::initializer_list<T> ilist) : sorted_vector(ilist.begin(), ilist.end())
		{
			AdventCheck(m_data.size() == ilist.size());
		}

		sorted_vector(const sorted_vector&) = default;
		sorted_vector(sorted_vector&&) = default;
		sorted_vector& operator=(const sorted_vector&) = default;
		sorted_vector& operator=(sorted_vector&&) = default;

		void reserve(std::size_t new_capacity)
		{
			m_data.reserve(new_capacity);
		}

		void sort() const
		{
			if (!m_sorted)
			{
				stdr::sort(m_data, m_compare);
				m_sorted = true;
			}
		}

		void clear() noexcept
		{
			m_data.clear();
			m_sorted = true;
		}

		[[nodiscard]] bool empty() const noexcept
		{
			return m_data.empty();
		}

		[[nodiscard]] std::size_t size() const noexcept
		{
			return m_data.size();
		}

		[[nodiscard]] std::size_t capacity() const noexcept
		{
			return m_data.capacity();
		}

		iterator lower_bound(const T& value)
		{
			sort();
			return stdr::lower_bound(m_data, value, m_compare);
		}

		iterator upper_bound(const T& value)
		{
			sort();
			return stdr::upper_bound(m_data, value, m_compare);
		}

		const_iterator lower_bound(const T& value) const
		{
			sort();
			return stdr::lower_bound(m_data, value, m_compare);
		}

		const_iterator upper_bound(const T& value) const
		{
			sort();
			return stdr::upper_bound(m_data, value, m_compare);
		}

		std::pair<const_iterator, const_iterator> equal_range(const T& value) const
		{
			sort();
			return stdr::equal_range(m_data, value, m_compare);
		}

		iterator find(const T& value)
		{
			const auto result = lower_bound(value);
			if (result == end())
			{
				return end();
			}
			if (!m_compare(*result, value) && !m_compare(value, *result))
			{
				return result;
			}
			return end();
		}

		const_iterator find(const T& value) const
		{
			const auto result = lower_bound(value);
			if (result == end())
			{
				return end();
			}
			if (!m_compare(*result, value) && !m_compare(value, *result))
			{
				return result;
			}
			return end();
		}

		template <typename Pred>
		const_iterator find_if_no_sort(const Pred& predicate) const
		{
			return std::find_if(m_data.begin(), m_data.end(), predicate);
		}

		template <typename Pred>
		const_iterator find_if(const Pred& predicate) const
		{
			sort();
			return find_if_no_sort(predicate);
		}

		// ThreeWayPredicate returns a std::weak_ordering.
		// Return (iterator_value <=> reference_value).
		template <typename ReferenceType, typename ThreeWayPredicate>
		const_iterator binary_find_if(const ReferenceType& ref, const ThreeWayPredicate& predicate) const
		{
			sort();
			return utils::ranges::binary_find_by_predicate(m_data, ref, predicate);
		}

		// ThreeWayPredicate returns a std::weak_ordering.
		// Return (iterator_value <=> reference_value).
		template <typename ReferenceType, typename ThreeWayPredicate>
		iterator binary_find_if(const ReferenceType& ref, const ThreeWayPredicate& predicate)
		{
			sort();
			return utils::ranges::binary_find_by_predicate(m_data, ref, predicate);
		}

		bool contains(const T& value) const
		{
			return find(value) != end();
		}

		std::size_t count(const T& value) const
		{
			sort();
			const auto [lower_it, upper_it] = equal_range(value);
			const auto diff = std::distance(lower_it, upper_it);
			AdventCheck(diff >= decltype(diff){0});
			return static_cast<std::size_t>(diff);
		}

		const T& front() const
		{
			sort();
			return m_data.front();
		}

		T& front()
		{
			sort();
			return m_data.front();
		}

		const T& back() const
		{
			sort();
			return m_data.back();
		}

		T& back()
		{
			sort();
			return m_data.back();
		}

		bool contains(const T& value)
		{
			return find(value) != end();
		}

		void erase(const_iterator pos)
		{
			assert(m_sorted);
			m_data.erase(pos);
		}

		void erase(const_iterator start, const_iterator finish)
		{
			assert(m_sorted);
			m_data.erase(start, finish);
		}

		void erase_fast(const_iterator pos)
		{
			assert(m_sorted);
			if (pos != (end() - 1))
			{
				const auto idx = std::distance(cbegin(), pos);
				m_data[idx] = m_data.back();
				m_sorted = false;
			}
			m_data.pop_back();
		}

		void erase(const T& val)
		{
			const auto pos = find(val);
			if (pos != end())
			{
				erase(pos);
			}
		}

		void erase_fast(const T& val)
		{
			const auto pos = find(val);
			if (pos != end())
			{
				erase_fast(pos);
			}
		}

		template <typename Pred>
		void erase_if(const Pred& predicate)
		{
			auto search_pos = m_data.begin();
			auto new_end = m_data.end();
			while (search_pos != new_end)
			{
				if (predicate(*search_pos))
				{
					*search_pos = std::move(m_data.back());
					m_sorted = false;
					--new_end;
				}
				else
				{
					++search_pos;
				}
			}
		}

		template <typename InputIterator>
		void insert(InputIterator first, InputIterator last)
		{
			m_data.insert(end(m_data), first, last);
			m_sorted = false;
		}

		// Implies keep_sorted = true. Tries to insert just before hint.
		iterator insert(const_iterator hint, T&& value)
		{
			if (m_data.empty())
			{
				m_data.push_back(std::forward<T>(value));
				m_sorted = true;
				return m_data.end() - 1;
			}

			if (!m_sorted)
			{
				return insert(std::forward<T>(value));
			}

			const bool check_after = (hint == m_data.cend() || !m_compare(*hint, value));
			if (check_after)
			{
				const bool check_before = (hint == m_data.cbegin() || !m_compare(value, *(hint - 1)));
				if (check_before)
				{
					return m_data.insert(hint, std::forward<T>(value));
				}
			}
			return insert(std::forward<T>(value));
		}

		// Implies keep_sorted = true. Tries to insert just before hint.
		iterator insert(const_iterator hint, const T& value)
		{
			if (m_data.empty())
			{
				m_data.push_back(value);
				m_sorted = true;
				return m_data.end() - 1;
			}

			if (!m_sorted) // Hint is meaningless here anyway.
			{
				return insert(value);
			}

			const bool can_insert_at_hint = can_insert_at_pos(hint, value);
			return can_insert_at_hint ? m_data.insert(hint, value) : insert(value);
		}

		iterator insert(T&& value)
		{
			m_sorted = m_data.empty() || (m_sorted && !m_compare(value, m_data.back()));
			m_data.push_back(std::forward<T>(value));
			return m_data.end() - 1;
		}

		iterator insert(const T& value)
		{
			m_sorted = m_data.empty() || (m_sorted && !m_compare(value, m_data.back()));
			m_data.push_back(value);
			return m_data.end() - 1;
		}

		std::pair<iterator,bool> insert_unique(T&& value)
		{
			const const_iterator insert_pos = lower_bound(value);
			const bool can_insert_at_it = [insert_pos, &value, this]()
			{
				if (insert_pos == end()) return true;
				return m_compare(value, *insert_pos);
			}();
			if (can_insert_at_it)
			{
				const iterator result = insert(insert_pos, std::forward<T>(value));
				return std::pair{result, true};
			}
			return std::pair{end(), false};
		}

		std::pair<iterator, bool> insert_unique(const T& value)
		{
			const const_iterator insert_pos = lower_bound(value);
			const bool can_insert_at_it = [insert_pos, &value, this]()
			{
				if (insert_pos == end()) return true;
				return m_compare(value, *insert_pos);
			}();
			if (can_insert_at_it)
			{
				const iterator result = insert(insert_pos, value);
				return std::pair{ result, true };
			}
			return std::pair{ end(), false };
		}

		iterator insert_keep_sorted(T&& value)
		{
			if (!m_sorted) return insert(std::forward<T>(value));
			const_iterator ub = upper_bound(value);
			return insert(ub, std::forward<T>(value));
		}

		iterator insert_keep_sorted(const T& value)
		{
			if (!m_sorted) return insert(value);
			const_iterator ub = upper_bound(value);
			return insert(ub, value);
		}

		// For std::back_inserter compatibility
		void push_back(T&& value)
		{
			insert(std::forward<T>(value));
		}

		void push_back(const T& value)
		{
			insert(value);
		}

		void pop_back()
		{
			m_data.pop_back();
		}

		// Erase all non-unique elements. Turns a multiset into a set, effectively.
		void unique()
		{
			sort();
			const auto eraseable_range = stdr::unique(m_data);
			m_data.erase(eraseable_range.begin(), eraseable_range.end());
		}

		void swap(sorted_vector<T>& other)
		{
			m_data.swap(other.m_data);
		}

		T& operator[](std::size_t index)
		{
			AdventCheck(index < m_data.size());
			sort();
			return m_data[index];
		}
		const T& operator[](std::size_t index) const
		{
			AdventCheck(index < m_data.size());
			sort();
			return m_data[index];
		}
		auto begin() { sort(); return m_data.begin(); }
		auto begin() const { sort(); return m_data.cbegin(); }
		auto end() { sort(); return m_data.end(); }
		auto end() const { sort(); return m_data.cend(); }
		auto cbegin() const { sort(); return m_data.cbegin(); }
		auto cend() const { sort(); return m_data.cend(); }
		auto rbegin() { sort(); return m_data.rbegin(); }
		auto rbegin() const { sort(); return m_data.crbegin(); }
		auto rend() { sort(); return m_data.rend(); }
		auto rend() const { sort(); return m_data.crend(); }
		auto crbegin() const { sort(); return m_data.crbegin(); }
		auto crend() const { sort(); return m_data.crend(); }
	};

	template <std::size_t BufferSize, typename T, typename BinaryPredicate>
	inline auto make_sorted_vector_with_buffer(const BinaryPredicate& pred)
	{
		return sorted_vector<T, BinaryPredicate, BufferSize>(pred);
	}

	template <typename T, typename BinaryPredicate>
	inline auto make_sorted_vector(const BinaryPredicate& pred)
	{
		return sorted_vector<T, BinaryPredicate, 1>(pred);
	}

	template <std::size_t BufferSize, typename T, typename InputIt, typename BinaryPredicate>
	inline auto make_sorted_vector_with_buffer(InputIt first, InputIt last, const BinaryPredicate& pred)
	{
		return sorted_vector<T, BinaryPredicate, BufferSize>(first, last, pred);
	}

	template <typename T, typename InputIt, typename BinaryPredicate>
	inline auto make_sorted_vector(InputIt first, InputIt last, const BinaryPredicate& pred)
	{
		return sorted_vector<T, BinaryPredicate>(first, last, pred);
	}

	template <typename KeyType, typename MappedType, typename KeyCompare>
	struct MapComparator
	{
		using ValueType = std::pair<KeyType,MappedType>;
		constexpr bool operator()(const ValueType& left, const ValueType& right) const
		{
			KeyCompare comp{};
			return comp(left.first,right.first);
		}
	};

	template<typename KeyType, typename MappedType, typename KeyCompare = std::less<KeyType>, std::size_t BufferSize = 1>
	class flat_map : public sorted_vector<std::pair<KeyType, MappedType>, MapComparator<KeyType, MappedType, KeyCompare>, BufferSize>
	{
	public:
 		using underlying_type = sorted_vector<std::pair<KeyType, MappedType>, MapComparator<KeyType, MappedType, KeyCompare>, BufferSize>;
 		using underlying_type::operator[];
 		using underlying_type::insert;
 		using iterator = underlying_type::iterator;
 		using const_iterator = underlying_type::const_iterator;
 		using key_type = KeyType;
 		using mapped_type = MappedType;
		using value_type = underlying_type::value_type;
 
 		iterator lower_bound_by_key(const KeyType& key)
 		{
			const std::size_t result_idx = lower_bound_index(key);
			auto result = underlying_type::begin();
			std::advance(result,result_idx);
			return result;
 		}
 
 		const_iterator lower_bound_by_key(const KeyType& key) const
 		{
			const std::size_t result_idx = lower_bound_index(key);
			auto result = underlying_type::cbegin();
			std::advance(result, result_idx);
			return result;
 		}
 		
 		iterator find_by_key(const KeyType& key)
 		{
 			const iterator result = lower_bound_by_key(key);
			auto end_it = underlying_type::end();
 			if (result != end_it)
 			{
				KeyCompare comp{};
				if (!comp(key, result->first) && !comp(result->first, key))
				{
					return result;
				}
			}
			return end_it;
		}

		const_iterator find_by_key(const KeyType& key) const
		{
			const const_iterator result = lower_bound_by_key(key);
			auto end_it = underlying_type::end();
			if (result != end_it)
			{
				KeyCompare comp{};
				if (!comp(key, result->first) && !comp(result->first, key))
				{
					return result;
				}
			}
			return end_it;
		}
		
		bool contains_key(const KeyType& key) const
		{
			const const_iterator result = find_by_key(key);
			return result != underlying_type::end();
		}

		std::pair<iterator, bool> insert_unique(KeyType&& key, MappedType&& value)
		{
			const const_iterator insert_pos = lower_bound_by_key(key);
			const bool can_insert_at_pos = [insert_pos, &key, this]()
			{
				if (insert_pos == underlying_type::end()) return true;
				KeyCompare comp{};
				return !comp(key, insert_pos->first);
			}();
			if (can_insert_at_pos)
			{
				const iterator result = insert(insert_pos, std::pair{std::forward<KeyType>(key) , std::forward<MappedType>(value) });
				return std::pair{ result, true };
			}
			return std::pair{ end(*this), false };
		}

		std::pair<iterator, bool> insert_unique(KeyType&& key, const MappedType& value)
		{
			const const_iterator insert_pos = lower_bound_by_key(key);
			const bool can_insert_at_it = [insert_pos, &key, this]()
			{
				if (insert_pos == underlying_type::end()) return true;
				KeyCompare comp{};
				return comp(key, insert_pos->first);
			}();
			if (can_insert_at_it)
			{
				const iterator result = insert(insert_pos, std::pair{ std::forward<KeyType>(key) , value });
				return std::pair{ result, true };
			}
			return std::pair{ end(*this), false };
		}

		std::pair<iterator, bool> insert_unique(const KeyType& key, MappedType&& value)
		{
			const const_iterator insert_pos = lower_bound_by_key(key);
			const bool can_insert_at_it = [insert_pos,&key, this]()
			{
				if(insert_pos == underlying_type::end()) return true;
				KeyCompare comp{};
				return comp(key, insert_pos->first);
			}();
			if(can_insert_at_it)
			{
				const iterator result = insert(insert_pos, std::pair{ key , std::forward<MappedType>(value) });
				return std::pair{ result, true };
			}
			return std::pair{ end(*this), false };
		}

		std::pair<iterator, bool> insert_unique(const KeyType& key, const MappedType& value)
		{
			const const_iterator insert_pos = lower_bound_by_key(key);
			const bool can_insert_at_it = [insert_pos, &key, this]()
			{
				if (insert_pos == underlying_type::end()) return true;
				KeyCompare comp{};
				return comp(key, insert_pos->first);
			}();
			if (can_insert_at_it)
			{
				const iterator result = insert(insert_pos, std::pair{ key , value });
				return std::pair{ result, true };
			}
			return std::pair{ end(*this), false };
		}

		std::pair<iterator,bool> insert_or_assign(const KeyType& key, MappedType&& val)
		{
			const iterator find_result = find_by_key(key);
			if (find_result != underlying_type::end())
			{
				find_result->second = std::forward<MappedType>(val);
				return std::pair{find_result,false};
			}
			const auto [insert_result,success] = insert_unique(key,std::forward<MappedType>(val));
			AdventCheck(success);
			return std::pair{insert_result,true};
		}

		std::pair<iterator, bool> insert_or_assign(const KeyType& key, const MappedType& val)
		{
			return insert_or_assign(key, MappedType(val));
		}

		std::pair<iterator, bool> insert_or_assign(KeyType&& key, MappedType&& val)
		{
			const iterator find_result = find_by_key(key);
			if (find_result != underlying_type::end())
			{
				find_result->second = std::forward<MappedType>(val);
				return std::pair{ find_result,false };
			}
			const auto [insert_result, success] = insert_unique(std::forward<KeyType>(key), std::forward<MappedType>(val));
			AdventCheck(success);
			return std::pair{ insert_result,true };
		}

		std::pair<iterator, bool> insert_or_assign(KeyType&& key, const MappedType& val)
		{
			return insert_or_assign(std::forward<KeyType>(key), MappedType(val));
		}

		MappedType& operator[](const KeyType& key)
		{
			iterator result = find_by_key(key);
			if (result == underlying_type::end())
			{
				auto [insert_result,success] = insert_unique(key,MappedType{});
				AdventCheck(success);
				result = insert_result;
			}
			return result->second;
		}

		MappedType& operator[](KeyType&& key)
		{
			iterator result = find_by_key(key);
			if (result == underlying_type::end())
			{
				auto [insert_result, success] = insert_unique(std::forward<KeyType>(key), MappedType{});
				AdventCheck(success);
				result = insert_result;
			}
			return result->second;
		}

		MappedType& at(const KeyType& key)
		{
			const iterator result = find_by_key(key);
			if (result == underlying_type::end())
			{
				throw std::out_of_range{"Tried to access an element in a utils::flat_map that does not exist."};
			}
			return result->second;
		}

		const MappedType& at(const KeyType& key) const
		{
			const const_iterator result = find_by_key(key);
			if (result == underlying_type::end())
			{
				throw std::out_of_range{ "Tried to access an element in a utils::flat_map that does not exist." };
			}
			return result->second;
		}

 	private:
 		std::size_t lower_bound_index(const KeyType& key) const
		{
			if(underlying_type::empty()) return 0;
			underlying_type::sort();
			KeyCompare comp{};
			const KeyType& first = underlying_type::front().first;
			const bool first_is_less_than_key = comp(first,key);
			if(!first_is_less_than_key) return 0;
			return lower_bound_index_impl(key,0,underlying_type::size());
		}

		std::size_t lower_bound_index_impl(const KeyType& key, std::size_t lower_idx, std::size_t upper_idx) const
		{
			AdventCheck(underlying_type::m_sorted);
			AdventCheck(lower_idx <= upper_idx);

			const std::size_t gap = upper_idx - lower_idx;
			if (gap <= std::size_t{ 1 })
			{
				return upper_idx;
			}

			const std::size_t midpoint_idx = lower_idx + gap / 2;
			const value_type& midpoint_value = underlying_type::m_data[midpoint_idx];
			const KeyType& midpoint_key = midpoint_value.first;

			KeyCompare comp{};
			const bool mid_is_below_key = comp(midpoint_key, key);

			if (!mid_is_below_key)
			{
				return lower_bound_index_impl(key, lower_idx, midpoint_idx);
			}
			else
			{
				return lower_bound_index_impl(key, midpoint_idx, upper_idx);
			}
		}
	};
}

template <typename T, typename BinaryPred>
inline auto begin(utils::sorted_vector<T,BinaryPred>& sv) { return sv.begin(); }

template <typename T, typename BinaryPred>
inline auto begin(const utils::sorted_vector<T,BinaryPred>& sv) { return sv.begin(); }

template <typename T, typename BinaryPred>
inline auto end(utils::sorted_vector<T,BinaryPred>& sv) { return sv.end(); }

template <typename T, typename BinaryPred>
inline auto end(const utils::sorted_vector<T,BinaryPred>& sv) { return sv.end(); }

template <typename T, typename BinaryPred>
inline auto rbegin(utils::sorted_vector<T,BinaryPred>& sv) { return sv.rbegin(); }

template <typename T, typename BinaryPred>
inline auto rbegin(const utils::sorted_vector<T,BinaryPred>& sv) { return sv.rbegin(); }

template <typename T, typename BinaryPred>
inline auto rend(utils::sorted_vector<T,BinaryPred>& sv) { return sv.rend(); }

template <typename T, typename BinaryPred>
inline auto rend(const utils::sorted_vector<T,BinaryPred>& sv) { return sv.rend(); }

template <typename T, typename BinaryPred>
inline auto cbegin(const utils::sorted_vector<T,BinaryPred>& sv) { return sv.cbegin(); }

template <typename T, typename BinaryPred>
inline auto cend(const utils::sorted_vector<T,BinaryPred>& sv) { return sv.cend(); }

template <typename T, typename BinaryPred>
inline auto crbegin(const utils::sorted_vector<T,BinaryPred>& sv) { return sv.crbegin(); }

template <typename T, typename BinaryPred>
inline auto crend(const utils::sorted_vector<T,BinaryPred>& sv) { return sv.crend(); }