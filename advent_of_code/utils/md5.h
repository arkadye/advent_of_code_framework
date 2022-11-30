#pragma once

#include <sstream>
#include <string>
#include <compare>
#include <iomanip>
#include <array>
#include <vector>

namespace utils
{
	class MD5Digest
	{
	private:
		uint32_t a, b, c, d;
	public:
		MD5Digest(uint32_t A, uint32_t B, uint32_t C, uint32_t D) noexcept
			: a{ A }, b{ B }, c{ C }, d{ D }{}
		auto operator<=>(const MD5Digest& other) const noexcept = default;
		std::string to_string() const;
		uint32_t get_word(int i) const noexcept;
		constexpr std::size_t num_words() const noexcept { return 4; }
		uint8_t get_byte(int i) const noexcept;
		constexpr std::size_t num_bytes() const noexcept { return sizeof(uint32_t) * num_words(); }
		uint8_t get_nybble(int i) const noexcept;
		constexpr std::size_t num_nybbles() const noexcept { return 2 * num_bytes(); }
		char get_hex_char(int i) const noexcept;
	};

	class MD5Hasher
	{
		using Block = std::array<uint32_t, 16>;
		mutable std::vector<Block> blocks;
		uint64_t message_size = 0;
		void push_data() noexcept
		{
			return;
		}
		void push_char_non_msg(uint8_t c, uint64_t location) const noexcept;
		void push_char(uint8_t c) noexcept;
		void push_string(const std::string& str) noexcept
		{
			for (auto c : str)
			{
				push_char(c);
			}
		}

		template <typename T>
		void push_single_item(T&& item)
		{
			std::ostringstream oss;
			oss << std::forward<T>(item);
			push_string(oss.str());
		}
	public:
		template <typename T, typename...Rest>
		void push_data(T&& data, Rest&&...rest)
		{
			push_single_item(std::forward<T>(data));
			push_data(std::forward<Rest>(rest)...);
		}

		MD5Digest get_digest() const noexcept;
	};

	template <typename...T>
	inline MD5Digest get_digest(T&&... input)
	{
		MD5Hasher hasher;
		hasher.push_data(std::forward<T>(input)...);
		return hasher.get_digest();
	}

	class MD5InputIterator
	{
	public:
		class DereferenceResult
		{
			friend class MD5InputIterator;
			MD5Hasher* base;
			DereferenceResult(MD5Hasher* ref) : base{ ref } {}
		public:
			template <typename T>
			DereferenceResult& operator=(T&& data)
			{
				if (base != nullptr)
				{
					base->push_data(std::forward<T>(data));
				}
				return *this;
			}
		};
	private:
		DereferenceResult base;
	public:
		MD5InputIterator(MD5Hasher& hasher) : base{ &hasher } {}
		MD5InputIterator() : base{ nullptr } {}
		DereferenceResult& operator*() { return base; }
		MD5InputIterator& operator++() { return *this; }
	};
}

template <typename T>
inline utils::MD5Hasher& operator<<(utils::MD5Hasher& hasher, T&& data)
{
	hasher.push_data(std::forward<T>(data));
	return hasher;
}