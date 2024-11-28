#pragma once

#include <string_view>
#include <stdexcept>
#include <optional>
#include "split_string.h"
#include "trim_string.h"

#include "advent/advent_assert.h"

namespace utils
{
	class string_line_iterator
	{
	public:
		enum class trim_behaviour : int8_t
		{
			trim_before_incrementing,
			do_not_trim
		};
	private:
		std::string_view m_string;
		std::string_view m_sentinental;
		char m_sentinental_storage;
		trim_behaviour m_trim_behaviour;
		mutable std::optional<std::pair<std::string_view,std::string_view>> m_cached_split_result;
		std::pair<std::string_view, std::string_view> get_split_result() const
		{
			if (is_at_end())
			{
				throw std::range_error{ "Cannot dereference or increment an at-the-end string line iterator" };
			}

			if (!m_cached_split_result.has_value())
			{
				m_cached_split_result = utils::split_string_at_first(m_string,m_sentinental);
			}

			AdventCheck(m_cached_split_result.has_value());
			return m_cached_split_result.value();
		}
		bool is_at_end() const { return m_string.empty(); }
		std::string_view try_trim(std::string_view in) const noexcept
		{
			if(m_trim_behaviour == trim_behaviour::trim_before_incrementing)
			{
				in = trim_left(in);
			}
			return in;
		}
	public:
		using pointer = const std::string_view*;
		using reference = const std::string_view&;
		using value_type = std::string_view;
		using difference_type = int;
		using iterator_category = std::forward_iterator_tag;
		string_line_iterator(std::string_view string_to_split, std::string_view sentinental, trim_behaviour behaviour) noexcept
			: m_string{string_to_split}, m_sentinental{sentinental}, m_sentinental_storage{'\0'}, m_trim_behaviour{behaviour}
		{
			m_string = try_trim(m_string);
		}
		string_line_iterator(std::string_view string_to_split, char sentinental, trim_behaviour behaviour) noexcept
			: string_line_iterator{string_to_split, std::string_view{&m_sentinental_storage,1}, behaviour}
		{
			m_sentinental_storage = sentinental;
		}
		string_line_iterator(std::string_view string_to_split, std::string_view sentinental) noexcept
			: string_line_iterator{ string_to_split , sentinental , trim_behaviour::trim_before_incrementing }{}
		string_line_iterator(std::string_view string_to_split, char sentinental) noexcept
			: string_line_iterator{string_to_split, sentinental , trim_behaviour::trim_before_incrementing}{}
		string_line_iterator(std::string_view string_to_split, trim_behaviour behaviour) noexcept
			: string_line_iterator{string_to_split, '\n', behaviour}{}
		explicit string_line_iterator(std::string_view string_to_split) noexcept
			: string_line_iterator{ string_to_split, trim_behaviour::trim_before_incrementing } {}
		string_line_iterator()
			: string_line_iterator{"",'\0'}{}
		string_line_iterator(const string_line_iterator&) noexcept = default;
		string_line_iterator& operator=(const string_line_iterator&) noexcept = default;

		bool operator==(const string_line_iterator& other) const noexcept
		{
			return is_at_end() && other.is_at_end();
		}

		bool operator!=(const string_line_iterator& other) const noexcept
		{
			return !(this->operator==(other));
		}

		std::string_view operator*() const noexcept
		{
			auto [result,unneeded] = get_split_result();
			return result;
		}

		string_line_iterator& operator++() noexcept
		{
			auto [unneeded,new_string] = get_split_result();
			m_string = try_trim(new_string);
			m_cached_split_result.reset();
			return *this;
		}
		string_line_iterator  operator++(int) noexcept
		{
			string_line_iterator result = *this;
			operator++();
			return result;
		}
		string_line_iterator operator+(std::size_t num) noexcept
		{
			for(std::size_t i=0;i<num;++i)
			{
				operator++();
			}
			return *this;
		}
	};

	class string_line_range
	{
		std::string_view m_string;
		std::string_view m_sentinental;
		char m_sentinental_storage = '\0';
		string_line_iterator::trim_behaviour m_trim_behaviour;
	public:
		string_line_range(std::string_view string_to_split, std::string_view sentinental, string_line_iterator::trim_behaviour behaviour) noexcept
			: m_string{ string_to_split }, m_sentinental{ sentinental }, m_sentinental_storage{ '\0' }, m_trim_behaviour{ behaviour } {}
		string_line_range(std::string_view string_to_split, char sentinental, string_line_iterator::trim_behaviour behaviour) noexcept
			: string_line_range{ string_to_split, std::string_view{&m_sentinental_storage,1}, behaviour }
		{
			m_sentinental_storage = sentinental;
		}
		string_line_range(std::string_view string_to_split, std::string_view sentinental) noexcept
			: string_line_range{ string_to_split , sentinental , string_line_iterator::trim_behaviour::trim_before_incrementing } {}
		string_line_range(std::string_view string_to_split, char sentinental) noexcept
			: string_line_range{ string_to_split, sentinental , string_line_iterator::trim_behaviour::trim_before_incrementing } {}
		string_line_range(std::string_view string_to_split, string_line_iterator::trim_behaviour behaviour) noexcept
			: string_line_range{ string_to_split, '\n', behaviour } {}
		explicit string_line_range(std::string_view string_to_split) noexcept
			: string_line_range{ string_to_split, string_line_iterator::trim_behaviour::trim_before_incrementing } {}
		string_line_range(const string_line_range& other) noexcept = default;
		string_line_range() = delete;
		string_line_iterator begin() const noexcept { return string_line_iterator{ m_string ,m_sentinental }; }
		string_line_iterator end() const noexcept { return string_line_iterator{}; }
	};
}

inline utils::string_line_iterator begin(utils::string_line_range lr) noexcept { return lr.begin(); }
inline utils::string_line_iterator end(utils::string_line_range lr) noexcept { return lr.end(); }