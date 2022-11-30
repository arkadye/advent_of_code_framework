#include "../utils/parse_utils.h"

#include <cassert>

std::string_view utils::remove_specific_prefix(std::string_view input, std::string_view prefix)
{
	assert(input.starts_with(prefix));
	input.remove_prefix(prefix.size());
	return input;
}

std::string_view utils::remove_specific_suffix(std::string_view input, std::string_view suffix)
{
	assert(input.ends_with(suffix));
	input.remove_suffix(suffix.size());
	return input;
}