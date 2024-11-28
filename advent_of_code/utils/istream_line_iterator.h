#pragma once

#include <istream>
#include <string>
#include <iterator>
#include <stdexcept>
#include <optional>

namespace utils
{

	class istream_line_iterator
	{
	private:
		mutable std::istream* m_stream;
		char m_sentinental;
		bool is_at_end() const { return m_stream == nullptr; }
		mutable std::optional<std::string> m_cached_result;
		void read_next_sequence() const
		{
			if (is_at_end())
			{
				throw std::range_error{ "Cannot dereference an at-the-end stream line iterator" };
			}
			std::string result;
			std::getline(*m_stream, result, m_sentinental);
			m_cached_result = std::move(result);

			if (m_stream->eof())
			{
				m_stream = nullptr;
			}
		}
		void maybe_read_next_sequence() const
		{
			if (!m_cached_result.has_value())
			{
				read_next_sequence();
			}
		}
	public:
		using pointer = const std::string_view*;
		using reference = const std::string_view&;
		using value_type = std::string_view;
		using difference_type = int;
		using iterator_category = std::input_iterator_tag;
		explicit istream_line_iterator(std::istream& stream, char sentinental = '\n') noexcept
			: m_stream{ &stream },  m_sentinental{ sentinental }{}
		istream_line_iterator() noexcept : m_stream{ nullptr }, m_sentinental{ 0 }{}
		istream_line_iterator(const istream_line_iterator&) noexcept = default;
		istream_line_iterator& operator=(const istream_line_iterator&) noexcept = default;

		bool operator==(const istream_line_iterator& other) const noexcept
		{
			return is_at_end() && other.is_at_end();
		}

		bool operator!=(const istream_line_iterator& other) const noexcept
		{
			return !(this->operator==(other));
		}

		std::string_view operator*() const
		{
			maybe_read_next_sequence();
			return m_cached_result.value();
		}

		istream_line_iterator& operator++() noexcept
		{
			maybe_read_next_sequence();
			m_cached_result.reset();
			return *this;
		}
		istream_line_iterator  operator++(int) noexcept
		{
			istream_line_iterator result = *this;
			++(*this);
			return result;
		}
	};

	class istream_line_range
	{
		std::istream& stream;
		char m_sentinental;
	public:
		explicit istream_line_range(std::istream& input, char splitter = '\n') : stream{ input } , m_sentinental{splitter} {}
		istream_line_range(const istream_line_range& other) = default;
		istream_line_range() = delete;
		istream_line_iterator begin() const { return istream_line_iterator{ stream , m_sentinental }; }
		istream_line_iterator end() const { return istream_line_iterator{}; }
	};
}

inline utils::istream_line_iterator begin(utils::istream_line_range lr) { return lr.begin(); }
inline utils::istream_line_iterator end(utils::istream_line_range lr) { return lr.end(); }