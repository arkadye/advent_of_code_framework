#include "utils/parse_utils.h"

#include "advent/advent_assert.h"

std::string_view utils::remove_specific_prefix(std::string_view input, std::string_view prefix)
{
	AdventCheck(input.starts_with(prefix));
	input.remove_prefix(prefix.size());
	return input;
}

std::string_view utils::remove_specific_suffix(std::string_view input, char suffix)
{
	std::string_view sv{&suffix,1};
	return remove_specific_suffix(input, sv);
}

std::string_view utils::remove_specific_suffix(std::string_view input, std::string_view suffix)
{
	AdventCheck(input.ends_with(suffix));
	input.remove_suffix(suffix.size());
	return input;
}

std::string_view utils::remove_specific_prefix(std::string_view input, char prefix)
{
	std::string_view sv{&prefix,1};
	return remove_specific_prefix(input,sv);
}
