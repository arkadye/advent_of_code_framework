#pragma once

#include <istream>
#include <string>
#include <iterator>

namespace utils
{

	class istream_line_iterator
	{
	private:
		std::istream* m_stream;
		bool m_at_end;
		char m_sentinental;
	public:
		using pointer = const std::string*;
		using reference = const std::string&;
		using value_type = std::string;
		using difference_type = int;
		using iterator_category = std::input_iterator_tag;
		explicit istream_line_iterator(std::istream& stream, char sentinental = '\n') noexcept
			: m_stream{ &stream }, m_at_end{ false }, m_sentinental{ sentinental }{}
		istream_line_iterator() noexcept : m_stream{ nullptr }, m_at_end{ true }, m_sentinental{ 0 }{}
		istream_line_iterator(const istream_line_iterator&) noexcept = default;
		istream_line_iterator& operator=(const istream_line_iterator&) noexcept = default;

		bool operator==(const istream_line_iterator& other) const noexcept
		{
			return m_at_end && other.m_at_end;
		}

		bool operator!=(const istream_line_iterator& other) const noexcept
		{
			return !(this->operator==(other));
		}

		std::string operator*()
		{
			std::string result;
			std::getline(*m_stream, result, m_sentinental);
			m_at_end = m_stream->eof();
			return result;
		}

		istream_line_iterator& operator++() noexcept { return *this; }
		istream_line_iterator  operator++(int) noexcept { return *this; }
	};

	class line_range
	{
		std::istream& stream;
	public:
		explicit line_range(std::istream& input) : stream{ input } {}
		line_range(const line_range& other) = default;
		line_range() = delete;
		istream_line_iterator begin() const { return istream_line_iterator{ stream }; }
		istream_line_iterator end() const { return istream_line_iterator{}; }
	};
}

inline utils::istream_line_iterator begin(utils::line_range lr) { return lr.begin(); }
inline utils::istream_line_iterator end(utils::line_range lr) { return lr.end(); }