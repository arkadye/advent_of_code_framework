#pragma once

#include <istream>
#include <string>
#include <string_view>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <optional>

namespace utils
{

	class istream_block_iterator
	{
	private:
		std::istream* m_stream;
		std::string_view m_sentinental;
		bool is_at_end() const { return m_stream == nullptr; }
		std::optional<std::string> m_cached_result;
		void read_next_block()
		{
			if (is_at_end())
			{
				throw std::range_error{ "Cannot deference an at-the-end stream block iterator" };
			}

			std::ostringstream result_stream;
			std::string line;
			while (true)
			{
				std::getline(*m_stream, line);
				if (line == m_sentinental)
				{
					break;
				}
				result_stream << line << '\n';
				if (m_stream->eof())
				{
					m_stream = nullptr;
					break;
				}
			}

			// Get the result and remove the trailing '\n'.
			std::string result = result_stream.str();
			if (!result.empty())
			{
				AdventCheck(result.back() == '\n');
				result.pop_back();
			}
			m_cached_result = std::move(result);
		}
		void maybe_read_next_block()
		{
			if (!m_cached_result.has_value())
			{
				read_next_block();
			}
		}
	public:
		using pointer = const std::string*;
		using reference = const std::string&;
		using value_type = std::string;
		using difference_type = int;
		using iterator_category = std::input_iterator_tag;
		explicit istream_block_iterator(std::istream& stream, std::string_view sentinental = "") noexcept
			: m_stream{ &stream }, m_sentinental{ sentinental }{}
		istream_block_iterator() noexcept : m_stream{ nullptr }, m_sentinental{}{}
		istream_block_iterator(const istream_block_iterator&) noexcept = default;
		istream_block_iterator& operator=(const istream_block_iterator&) noexcept = default;

		bool operator==(const istream_block_iterator& other) const noexcept
		{
			return is_at_end() && other.is_at_end();
		}

		bool operator!=(const istream_block_iterator& other) const noexcept
		{
			return !(this->operator==(other));
		}

		std::string_view operator*()
		{
			maybe_read_next_block();
			return m_cached_result.value();
		}

		istream_block_iterator& operator++() noexcept
		{
			maybe_read_next_block();
			m_cached_result.reset();
			return *this;
		}
		istream_block_iterator  operator++(int) noexcept
		{	
			istream_block_iterator result = *this;
			++(*this);
			return result;
		}
	};

	class istream_block_range
	{
		std::istream& stream;
	public:
		explicit istream_block_range(std::istream& input) : stream{ input } {}
		istream_block_range(const istream_block_range& other) = default;
		istream_block_range() = delete;
		istream_block_iterator begin() const { return istream_block_iterator{ stream }; }
		istream_block_iterator end() const { return istream_block_iterator{}; }
	};
}

inline utils::istream_block_iterator begin(utils::istream_block_range lr) { return lr.begin(); }
inline utils::istream_block_iterator end(utils::istream_block_range lr) { return lr.end(); }