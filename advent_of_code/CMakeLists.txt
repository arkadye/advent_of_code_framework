cmake_minimum_required(VERSION 3.13)

project(advent2024)

function(reset_solution TXT DIGIT)
	foreach(FILESUFFIX IN ITEMS "h" "cpp" "txt")
		set(DAYTXT ${TXT})
		set(DAYDIGIT ${DIGIT})
		set(FROM_FILE "templates/advent_template.${FILESUFFIX}.in")
		set(TO_FILE "advent${DAYDIGIT}/advent${DAYDIGIT}.${FILESUFFIX}")
		message("Copying ${FROM_FILE} to ${TO_FILE}")
		configure_file(${FROM_FILE} ${TO_FILE} @ONLY)
	endforeach()
endfunction()

message("RESET_ALL_SOLUTIONS=${RESET_ALL_SOLUTIONS}")
if(${RESET_ALL_SOLUTIONS})
	message("Resetting all solutions")
	reset_solution("one" 1)
	reset_solution("two" 2)
	reset_solution("three" 3)
	reset_solution("four" 4)
	reset_solution("five" 5)
	reset_solution("six" 6)
	reset_solution("seven" 7)
	reset_solution("eight" 8)
	reset_solution("nine" 9)
	reset_solution("ten" 10)
	reset_solution("eleven" 11)
	reset_solution("twelve" 12)
	reset_solution("thirteen" 13)
	reset_solution("fourteen" 14)
	reset_solution("fifteen" 15)
	reset_solution("sixteen" 16)
	reset_solution("seventeen" 17)
	reset_solution("eighteen" 18)
	reset_solution("nineteen" 19)
	reset_solution("twenty" 20)
	reset_solution("twentyone" 21)
	reset_solution("twentytwo" 22)
	reset_solution("twentythree" 23)
	reset_solution("twentyfour" 24)
	reset_solution("twentyfive" 25)
	configure_file("templates/advent_setup.h.in" "advent/advent_setup.h")
	configure_file("templates/advent_solutions.h.in" "advent/advent_solutions.h")
	configure_file("templates/advent_test_inputs.h.in" "advent/advent_test_inputs.h")
endif()

message("Building...")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
include_directories(. "./utils")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

message ("cxx Flags:" ${CMAKE_CXX_FLAGS})

set(EXENAME advent2024)

source_group("framework\\src" FILES "main.cpp")
add_executable(${EXENAME} "main.cpp")

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${EXENAME})

set( TEMPLATE_FILES
	"templates/advent_template.h.in"
	"templates/advent_template.cpp.in"
	"templates/advent_template.txt.in"
	"templates/advent_setup.h.in"
	"templates/advent_solutions.h.in"
	"templates/advent_test_inputs.h.in"
)

source_group("templates" FILES ${TEMPLATE_FILES})
target_sources(${EXENAME} PUBLIC ${TEMPLATE_FILES})

set( FRAMEWORK_FILES
	"advent/advent_assert.h"
	"advent/advent_headers.h"
	"advent/advent_of_code.h"
	"advent/advent_testcase_setup.h"
	"advent/advent_types.h"
	"advent/advent_utils.h"
)

set( FRAMEWORK_SOURCE_FILES "src/advent_of_code_testcases.cpp" )

source_group("framework" FILES ${FRAMEWORK_FILES})
source_group("framework\\src" FILES ${FRAMEWORK_SOURCE_FILES})
target_sources(${EXENAME} PUBLIC ${FRAMEWORK_FILES} ${FRAMEWORK_SOURCE_FILES})

set(TEST_CONFIG_FILES
	"advent/advent_setup.h"
	"advent/advent_solutions.h"
	"advent/advent_test_inputs.h"
)

source_group("test_config" FILES ${TEST_CONFIG_FILES})
target_sources(${EXENAME} PUBLIC ${TEST_CONFIG_FILES})



set( UTILS_FILES
	"utils/a_star.h"
	"utils/binary_find.h"
	"utils/bit_ops.h"
	"utils/brackets.h"
	"utils/combine_maps.h"
	"utils/comparisons.h"
	"utils/conway_simulation.h"
	"utils/coords.h"
	"utils/coords_iterators.h"
	"utils/coords3d.h"
	"utils/enums.h"
	"utils/erase_remove_if.h"
	"utils/grid.h"
	"utils/has_duplicates.h"
	"utils/index_iterator.h"
	"utils/index_iterator2.h"
	"utils/int_range.h"
	"utils/is_sorted.h"
	"utils/isqrt.h"
	"utils/istream_block_iterator.h"
	"utils/istream_line_iterator.h"
	"utils/md5.h"
	"utils/modular_int.h"
	"utils/parse_utils.h"
	"utils/position3d.h"
	"utils/push_back_unique.h"
	"utils/range_contains.h"
	"utils/ring_buffer.h"
	"utils/shared_lock_guard.h"
	"utils/small_vector.h"
	"utils/sorted_vector.h"
	"utils/span.h"
	"utils/sparse_array.h"
	"utils/split_string.h"
	"utils/string_line_iterator.h"
	"utils/swap_remove.h"
	"utils/to_value.h"
	"utils/transform_if.h"
	"utils/trim_string.h"
)

set (UTILS_SOURCE_FILES "utils/aoc_utils.natvis" "utils/isqrt.cpp" "utils/md5.cpp" "utils/parse_utils.cpp")

source_group("utils" FILES ${UTILS_FILES})
source_group("utils\\src" FILES ${UTILS_SOURCE_FILES})

target_sources(${EXENAME} PUBLIC ${UTILS_FILES} ${UTILS_SOURCE_FILES})

# Add extra files as extra parameters
function(add_day day_num)
	set(THESE_FILES "advent${day_num}/advent${day_num}.h" "advent${day_num}/advent${day_num}.cpp" "advent${day_num}/advent${day_num}.txt" ${ARGN})
	source_group("advent${day_num}" FILES ${THESE_FILES})
	target_sources(${EXENAME} PUBLIC ${THESE_FILES})
	message("Added day " ${day_num} " files")
endfunction()

add_day(1)
add_day(2)
add_day(3)
add_day(4)
add_day(5)
add_day(6)
add_day(7)
add_day(8)
add_day(9)
add_day(10)
add_day(11)
add_day(12)
add_day(13)
add_day(14)
add_day(15)
add_day(16)
add_day(17)
add_day(18)
add_day(19)
add_day(20)
add_day(21)
add_day(22)
add_day(23)
add_day(24)
add_day(25)