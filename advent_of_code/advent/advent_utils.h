#pragma once

#include <fstream>
#include <string>
#include <iostream>
#include <filesystem>
#include <format>

#include "advent_assert.h"

namespace advent
{
	inline std::ifstream open_input(const std::string& filename)
	{
		auto result = std::ifstream{ filename };
		AdventCheck(result.is_open());
#ifndef NDEBUG
		if (std::filesystem::file_size(filename) <= 0)
		{
			std::cerr << "\nWARNING! File '" << filename << "' is empty.";
		}
#endif
		return result;
	}

	// Opens a file with the name "adventX/adventX.txt"
	inline std::ifstream open_puzzle_input(int day)
	{
		const std::string name = std::format("advent{0}/advent{0}.txt", day);
		return open_input(name);
	}

	// Open a file with the format "adventX/testcase_Y.txt"
	inline std::ifstream open_testcase_input(int day, char id)
	{
		const std::string name = std::format("advent{0}/testcast_{1}.txt", day, id);
		return open_input(name);
	}
}