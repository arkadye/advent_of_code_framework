#include "../utils/md5.h"
#include "../utils/int_range.h"
#include "../utils/range_contains.h"

#include <type_traits>
#include <numeric>
#include <algorithm>
#include <cassert>

#ifdef NDEBUG
#define MD5_PRINT_STEPS 0
#else
#define MD5_PRINT_STEPS 0
#endif

#if MD5_PRINT_STEPS
#include <cstdio>
#include "../../temp/md5.h"
#endif

using namespace utils;

namespace
{
	constexpr auto BLOCK_LENGTH = 512 / CHAR_BIT;
	constexpr auto DIGEST_LENGTH = 128 / CHAR_BIT;
	using Val = uint32_t;
	using Block = std::array<Val, BLOCK_LENGTH / sizeof(Val)>;
	constexpr auto NUM_ROUNDS = 64;
	void validate_round(int index) noexcept
	{
		assert(range_contains_exc(index, 0, NUM_ROUNDS));
	}

	template <typename Int>
	uint8_t get_char_in_pos(Int i, int pos) noexcept
	{
		static_assert(std::is_integral_v<Int>);
		static_assert(std::is_unsigned_v<Int>);
		assert(pos < sizeof(i));

		const Int mask = 0b11111111;
		i = i >> (CHAR_BIT * pos);
		return i & mask;
	}

	struct MD5State
	{
		// Init with magic numbers.
		std::array<Val, 4> vals{ 0x67452301,0xefcdab89,0x98badcfe,0x10325476 };
		MD5State operator+=(const MD5State& other) noexcept
		{
			std::transform(begin(vals), end(vals), begin(other.vals), begin(vals), std::plus<Val>{});
			return *this;
		}
	};

	MD5State operator+(const MD5State& left, const MD5State& right) noexcept
	{
		MD5State result = left;
		result += right;
		return result;
	}

	uint8_t get_shift_amount(int index) noexcept
	{
		const std::array<uint8_t,64> values{
			7,12,17,22,	7,12,17,22,	7,12,17,22,	7,12,17,22,
			5, 9,14,20,	5, 9,14,20,	5, 9,14,20,	5, 9,14,20,
			4,11,16,23,	4,11,16,23,	4,11,16,23,	4,11,16,23,
			6,10,15,21,	6,10,15,21,	6,10,15,21,	6,10,15,21
		};
		assert(range_contains_exc<int>(index, 0, values.size()));
		return values[index];
	}

	uint8_t get_block_index(int index) noexcept
	{
		const auto result = [index]()
		{
			switch (index / 16)
			{
			default:
				assert(false);
				return -1;
			case 0:
				return index;
			case 1:
				return 5 * index + 1;
			case 2:
				return 3 * index + 5;
			case 3:
				return 7 * index;
			}
		}();
		return static_cast<uint8_t>(result % 16);
	}

	Val calculate_k_value(int index) noexcept
	{
		validate_round(index);
		return static_cast<Val>(static_cast<double>(0x1'0000'0000) * std::abs(std::sin(index + 1)));
	}

	std::array<Val, NUM_ROUNDS> calculate_all_k_vals() noexcept
	{
		std::array<Val, NUM_ROUNDS> result;
		const auto range = int_range(static_cast<int>(result.size()));
		std::transform(begin(range), end(range), begin(result), calculate_k_value);
		return result;
	}

	const std::array<Val, NUM_ROUNDS> all_k_vals = calculate_all_k_vals();

	Val get_k_val(int index) noexcept
	{
		validate_round(index);
		return all_k_vals[index];
	}

	Val func0(Val B, Val C, Val D) noexcept
	{
		return (B & C) | (~B & D);
	}

	Val func1(Val B, Val C, Val D) noexcept
	{
		return (D & B) | (~D & C);
	}

	Val func2(Val B, Val C, Val D) noexcept
	{
		return B ^ C ^ D;
	}

	Val func3(Val B, Val C, Val D) noexcept
	{
		return C ^ (B | ~D);
	}

	Val func(int i, Val B, Val C, Val D) noexcept
	{
		validate_round(i);
		switch (i / 16)
		{
		default:
			assert(false);
			return 0;
		case 0:
			return func0(B, C, D);
		case 1:
			return func1(B, C, D);
		case 2:
			return func2(B, C, D);
		case 3:
			return func3(B, C, D);
		}
		return 0;
	}

	struct RoundResult
	{
		Val func_result;
		Val k_value;
		uint8_t block_index;
		uint8_t shift_amount;
	};

	RoundResult get_round_result(Val A, Val B, Val C, Val D, int index) noexcept
	{
		validate_round(index);
		return RoundResult{
			func(index, B, C, D),
			get_k_val(index),
			get_block_index(index),
			get_shift_amount(index)
		};
	}

	template <typename UInt>
	UInt lrotate_bits(UInt val, uint8_t distance) noexcept
	{
		static_assert(std::is_integral_v<UInt>);
		static_assert(std::is_unsigned_v<UInt>);
		constexpr auto num_bits = sizeof(val) * CHAR_BIT;
		assert(distance < num_bits);

		const auto new_bottom_bits = val >> (num_bits - distance);
		const auto new_top_bits = val << distance;
		assert((new_bottom_bits & new_top_bits) == 0);
		return new_top_bits + new_bottom_bits;
	}

	MD5State do_round(MD5State state, const Block& block, int index) noexcept
	{
#if MD5_PRINT_STEPS
		std::printf("MD5 state Pass %d; a=%08x b=%08x c=%08x d=%08x\n",
			index, state.vals[0], state.vals[1], state.vals[2], state.vals[3]);
#endif
		auto get_rotated = [&state, index](int i) -> Val&
		{
			const auto idx = 64 + i - index;
			return state.vals[idx % 4];
		};
		auto& A = get_rotated(0);
		auto& B = get_rotated(1);
		auto& C = get_rotated(2);
		auto& D = get_rotated(3);
		const auto rr = get_round_result(A,B,C,D, index);
		assert(rr.block_index < block.size());
		A = rr.func_result + A + rr.k_value + block[rr.block_index];
		A = lrotate_bits(A, rr.shift_amount) + B;
		return state;
	}

	MD5State hash_block(const MD5State& state, const Block& block) noexcept
	{
#if MD5_PRINT_STEPS
		printf("MD5 Block= {\n");
		for (int i = 0; i < 16; ++i)
		{
			printf("    [%08x]\n", block[i]);
		}
		printf("}\n");
#endif
		const auto range = int_range(NUM_ROUNDS);
		const auto result = std::accumulate(begin(range), end(range), state,
			[&block](const MD5State& s, int index)
		{
			return do_round(s, block, index);
		});
#if MD5_PRINT_STEPS
		std::printf("MD5 state Pass %d; a=%08x b=%08x c=%08x d=%08x\n",
			64, result.vals[0], result.vals[1], result.vals[2], result.vals[3]);
#endif
		return result;
	}
}

MD5Digest MD5Hasher::get_digest() const noexcept
{
#if MD5_PRINT_STEPS
	{
		assert(message_size <= std::numeric_limits<std::size_t>::max());
		const auto msg_size = static_cast<std::size_t>(message_size);
		std::vector<char> test_buffer;
		test_buffer.resize(msg_size + 1, 0);
		std::memcpy(test_buffer.data(), blocks.data(), msg_size);
		MD5String(test_buffer.data());
	}
#endif
	auto fake_size = message_size;
	push_char_non_msg(static_cast<char>(0x80), fake_size++);
	while ((fake_size % BLOCK_LENGTH) != BLOCK_LENGTH - sizeof(uint64_t))
	{
		push_char_non_msg(0, fake_size++);
	}
	const auto suffix = message_size * 8;
	for (auto i : int_range(sizeof(uint64_t)))
	{
		push_char_non_msg(get_char_in_pos(suffix, i), fake_size++);
	}
	assert(fake_size % BLOCK_LENGTH == 0);
	const auto result = std::accumulate(begin(blocks), end(blocks), MD5State{},
		[](const MD5State& s, const Block& b)
	{
		return s + hash_block(s, b);
	});
#if MD5_PRINT_STEPS
	printf("MD5 State final: a=%08x b=%08x, c=%08x d=%08x\n", result.vals[0], result.vals[1], result.vals[2], result.vals[3]);
#endif
	return MD5Digest{ result.vals[0],result.vals[1],result.vals[2],result.vals[3] };
}

void MD5Hasher::push_char_non_msg(uint8_t c, uint64_t location) const noexcept
{
	const int block_idx = static_cast<int>(location / BLOCK_LENGTH);
	blocks.resize(block_idx + 1, Block{});
	Block& block = blocks[block_idx];

	const std::size_t val_idx = (location / sizeof(Val)) % block.size();
	Val& val = block[val_idx];
	const auto byte_idx = location % sizeof(Val);
	assert(byte_idx != 0 || val == 0);
	
	const auto shift_amount = byte_idx * CHAR_BIT;
	assert(((Val{ 0xFF } << shift_amount) & val) == 0);
	const auto mask = static_cast<Val>(c) << shift_amount;
	val = val | mask;
}

void MD5Hasher::push_char(uint8_t c) noexcept
{
	push_char_non_msg(c, message_size++);
}

uint32_t MD5Digest::get_word(int i) const noexcept
{
	assert(i < 4);
	switch (i)
	{
	default:
		assert(false);
		return 0;
	case 0:
		return a;
	case 1:
		return b;
	case 2:
		return c;
	case 3:
		return d;
	}
}

uint8_t MD5Digest::get_byte(int i) const noexcept
{
	assert(i < DIGEST_LENGTH);
	const auto word = get_word(i / 4);
	const auto byte_index = i % sizeof(word);
	return get_char_in_pos(word, byte_index);
}

uint8_t MD5Digest::get_nybble(int i) const noexcept
{
	assert(i < 2 * DIGEST_LENGTH);
	const auto byte = get_byte(i / 2);
	const auto result = (i % 2 ? byte & 0xF : byte >> 4);
	return result;
}

char MD5Digest::get_hex_char(int i) const noexcept
{
	assert(i < 2 * DIGEST_LENGTH);
	constexpr const char* lookup = "0123456789abcdef";
	assert(std::string{ lookup }.size() == 0x10);
	const auto val = get_nybble(i);
	assert(val < 0x10);
	const auto result = lookup[val];
	return result;
}

std::string utils::MD5Digest::to_string() const
{
	std::string result;
	result.reserve(2 * DIGEST_LENGTH);
	const auto range = int_range{ 2 * DIGEST_LENGTH };
	std::transform(begin(range), end(range), std::back_inserter(result),
		[this](int i) {return get_hex_char(i); });
	return result;
}