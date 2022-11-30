#pragma once

#include <vector>
#include <algorithm>
#include <functional>
#include <cassert>

namespace utils
{
	template <typename T, typename BinaryPred = std::less<T>>
	class sorted_vector
	{
	public:
		using iterator = typename std::vector<T>::iterator;
		using const_iterator = typename std::vector<T>::const_iterator;
		using value_type = T;
	private:
		mutable std::vector<T> m_data;
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
			assert(m_data.size() == ilist.size());
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
				std::sort(m_data.begin(), m_data.end(), m_compare);
				m_sorted = true;
			}
		}

		void clear()
		{
			m_data.clear();
			m_sorted = true;
		}

		bool empty() const
		{
			return m_data.empty();
		}

		std::size_t size() const
		{
			return m_data.size();
		}

		iterator lower_bound(const T& value)
		{
			sort();
			return std::lower_bound(m_data.begin(), m_data.end(), value, m_compare);
		}

		iterator upper_bound(const T& value)
		{
			sort();
			return std::upper_bound(m_data.begin(), m_data.end(), value, m_compare);
		}

		const_iterator lower_bound(const T& value) const
		{
			sort();
			return std::lower_bound(m_data.begin(), m_data.end(), value, m_compare);
		}

		const_iterator upper_bound(const T& value) const
		{
			sort();
			return std::upper_bound(m_data.begin(), m_data.end(), value, m_compare);
		}

		std::pair<const_iterator, const_iterator> equal_range(const T& value)
		{
			sort();
			return std::equal_range(m_data.begin(), m_data.end(), value, m_compare);
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

		bool contains(const T& value) const
		{
			return find(value) != end();
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
			break;
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

			const bool check_after = (hint == m_data.cend() || !m_compare(*hint, value));
			if (check_after)
			{
				const bool check_before = (hint == m_data.cbegin() || !m_compare(value, *(hint - 1)));
				if (check_before)
				{
					return m_data.insert(hint, value);
				}
			}
			return insert(value);
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
			const auto new_end = std::unique(m_data.begin(), m_data.end());
			m_data.erase(new_end, m_data.end());
		}

		void swap(sorted_vector<T>& other)
		{
			m_data.swap(other.m_data);
		}

		T& operator[](std::size_t index)
		{
			assert(index < m_data.size());
			sort();
			return m_data[index];
		}
		const T& operator[](std::size_t index) const
		{
			assert(index < m_data.size());
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

	template <typename T, typename BinaryPredicate>
	inline auto make_sorted_vector(const BinaryPredicate& pred)
	{
		return sorted_vector<T, BinaryPredicate>(pred);
	}

	template <typename T, typename InputIt, typename BinaryPredicate>
	inline auto make_sorted_vector(InputIt first, InputIt last, const BinaryPredicate& pred)
	{
		return sorted_vector<T, BinaryPredicate>(first, last, pred);
	}
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