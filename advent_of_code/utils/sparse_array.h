#pragma once

#include "sorted_vector.h"
#include <utility>

namespace utils
{
	template <typename ValueType, typename IndexType = std::size_t, std::size_t BufferSize = 1>
	class sparse_array
	{
	public:
		using DataType = std::pair<IndexType, ValueType>;
		struct Compare
		{
			bool operator()(const DataType& l, const DataType& r) const
			{
				return std::less<IndexType>{}(l.first, r.first);
			}
		};
		using Container = utils::sorted_vector<DataType,Compare>;

	private:
		Container m_data;
		ValueType m_default_val;

	public:
		const Container& get_data() const { return m_data; }
		const ValueType& get_default_value() const { return m_default_val; }
		explicit sparse_array(ValueType&& default_val) : m_default_val{ default_val } {}
		sparse_array() : sparse_array{ ValueType{} } {}
		void reserve(std::size_t new_capacity) { m_data.reserve(new_capacity); }

		ValueType get(const IndexType& idx) const
		{
			const auto find_it = m_data.binary_find_if(idx ,[]( const DataType& dt, const IndexType & idx)
			{
				return dt.first <=> idx;
			});
			return (find_it != end(m_data) ? find_it->second : m_default_val);
		}

		void set(const IndexType& idx, ValueType&& val)
		{
			const auto find_it = m_data.binary_find_if(idx, [](const DataType & dt, const IndexType & idx)
			{
				return dt.first <=> idx;
			});
			
			if (find_it != end(m_data))
			{
				if (val != m_default_val)
				{
					find_it->second = std::forward<ValueType>(val);
				}
				else
				{
					m_data.erase_fast(find_it);
				}
			}
			else
			{
				if (val != m_default_val)
				{
					m_data.push_back(DataType{ idx,std::forward<ValueType>(val) });
				}
			}
		}

		void set(const IndexType& idx, const ValueType& val)
		{
			const auto find_it = m_data.binary_find_if(idx, [](const DataType & dt, const IndexType & idx)
			{
				return dt.first <=> idx;
			});

			if (find_it != end(m_data))
			{
				if (val != m_default_val)
				{
					find_it->second = val;
				}
				else
				{
					m_data.erase_fast(find_it);
				}
			}
			else
			{
				if (val != m_default_val)
				{
					m_data.push_back(DataType{ idx, val });
				}
			}
		}
	};
}