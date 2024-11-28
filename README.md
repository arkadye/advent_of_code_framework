# Advent of Code Framework

A framework to build solutions to the Advent of Code competition on. It includes some basic utilities, and a test suite to verify your solutions. Example cases can also be tested.

## To add solutions

Open `advent/advent_solutions.h` and replace the macro for the solution with your solution.

## DO NOT CHECK IN SOLUTIONS OR PUZZLE TEXTS

Common file formats for solution texts are excluded by default via the .gitignore.

The only exception to this is `advent_solutions.h`. You are STRONGLY recommended to put this onto its own commit named "DO NOT PUSH" and never push it, so you can't accidentally commit it some other way.

## To add a testcase

See the examples in advent1.h, advent1.cpp, and advent_of_code_testcases.cpp.

1. Declare the function in the approriate header (normally adventx.h, where x is the day number) and implement in the appropriate .cpp file. It must take no arguments, and return ResultType. E.g.:

     `// advent42.h`  
     `ResultType advent_fortytwo_testcase_a();`
     
     `// advent42.cpp`    
     `ResultType advent_fortytwo_testcase_a() { return "Life The Universe and Everything"; }`
     
2. In advent_of_code_testcases.cpp, go to `const verification_tests tests[]` and add the line `TESTCASE(your_functionName, expected_result),` at the appropriate point in the array. `expected_result` should be a string. In this example it would be `TESTCASE(advent_fortytwo_testcase_a,"Life The Universe and Everything"),`. Importantly: remember to add a COMMA at the end, not a semi-colon. (It's obvious, but muscle memory and habit will make you want to use the semi-colon.)

Now when you run the tests, the testcase should appear, and will report success or failure.

## To filter which testcases/solutions run

In main.cpp, a filter can be added to the argument of `verify_all()` to only run some of hte testcases, instead of everything. For example, if your day one solution is very slow you can put an argument of `"_two_"` in, and it will only run functions on the `verification_tests` list which have `two` in the function name (e.g. `advent_two_p1()` and `advent_two_p2()` as well as `advent_two_p1_testcase_a()` if you added it. To run everything, leave the testcase blank (`""` or `std::string{}`).

Arguments on the command line will also be passed to this filter.

## Best practices

If you add testcases, name them `advent_[day number]_[p1 or p2, depending which part]_testcase_[letter]()` in order to make the filtering easy.

For functions specifically for solving a particular solution, put them in an unnamed namespace in the right file.

If 2019's code was anything to go by, don't be afraid to add common tasks to their own headers. For an example: a generic A* search function should probably in be the utility file, and I had all the Intcode stuff in its own folder last time. Anything that used the Intcode machine had `_ic` added to the function names which meant that I could edit the Intmachine and then run my testcases with `_ic` as the filter to make sure I didn't break any previous compatibility. This is a great project to test your ability to reuse old code.

## CMake setup

1. Install CMake.
2. Open the base folder, with `CMakeLists.txt` in it.
3. Open a command window on that folder.

You can then run whatever CMake command you like in there. I use `cmake -S . -G "Visual Studio 17 2022"`

### Adding testcase inputs

In `CMakeLists.txt` there are functions called `add_day(...)`. Extra files can be added there. So for a day 1 testcase input you can add `testcase_a.txt` to the `advent1` folder, and then update the relevant line to read `add_day(1 "testcase_a.txt")` and the file will be added to the project.

### Resetting all solutions (WARNING: YOU WILL LOSE DATA)

There is a secret argument you can add. If you put `-DRESET_ALL_SOLUTIONS=True` onto the CMake command it will delete all solutions and reset them. I mainly use this to copy the repro from one year to the next and give myself a clean slate.

## Advent headers overview

There are few general purpose headers for use with the solutions.

### `advent_assert.h`

Very useful. Gives an `AdventCheck`, `AdventCheckMsg` and `AdventUnreachable` message. Depending on the build mode these either throw an exception, or emit a compiler hint.

### `advent_logger.h`

Appears to depend on a header that no longer exists in the repo. Hmm...

### `advent_setup.h`

Put your day-to-day testcases in here.

### `advent_solutions.h`

Put your solutions here once they're known and DO NOT SUBMIT THIS FILE TO SOURCE CONTROL.

### `advent_test_inputs.h`

Put inputs for testcases in here. 

### `advent_types.h`

Collects some universal types. In particular, `ResultType` which is a `std::variant<std::string, uint64_t, int64_t>` which can capture any puzzle output.

### `advent_utils.h`

Helper functions for opening input files. In particular `advent::open_testcase_input(1,'b')` will open `advent1/testcase_b.txt`, which is really useful if you follow that naming convention.

## `utils` overview

The utils folder has many general purpose algorithms and a few containers that I've found useful over the years. These were largely created before C++17 was available to me, so some features are from more modern C++ implementations are duplicated.

Not all of these work well or perfectly, but some a very reliable.

### `a_star.h`

An A* implementation that takes parameterised function objects. I've found this is always really slow, for some reason, which is surprising since I'd expect lambdas passed in to be inlined and optimised. But it still provides a reference implementation. This is basically supplanted by `grid.h`.

### `advent_utils.h`

Deprecated.

TODO: Remove this.

### `binary_find.h`

Modelled from `std::find` but only works on sorted inputs. Several useful overloads and some range-kind-of implementations as well. This isn't used a lot, but some other utils do build on it.

TODO: Use concepts to clean this up.

### `bit_ops.h`

Some bitwise operations. Currently only population is implemented (i.e. counting the number of `1` bits in an unsigned integer), but if others come in handy - e.g. an easy way to do a bitset - that will go in here too.

TODO: Use concepts to clean this up.

### `brackets.h`

A very regularly library. Pass in strings and it will find the closing bracket. It can also do a `bracket_aware_find` where it will find a substring within a string, but ignore anything in brackets. Very useful.

TODO: Remove `advent_utils.h` requirement.

### `combine_maps.h`

Surprisingly useful in lots of search puzzles. Put two maps in it and it will combine them into one map. It accepts a `combine_fun` to tell it how to combine values when both maps have the same key. (E.g. take the larger/smaller/sum of the two values).

TODO: Use concepts to clean this up

### `comparisons.h`

Several useful variations on `min` and `max functions here. While the min and max with transform functions are supplanted by `std::ranges`, the `Larger` and `Smaller` structs are very useful and the variadic `max` function is really useful. For some reason the variadic `min` function isn't implemented, so I guess I never actually used that much.

TODO: Use concepts to allow overloads of variadic `min` and `max` taking a functor.  
TODO: Implement variadic `min`

### `conway_simulation.h`

I don't think I've used this since the year where there were lots of cellula automata, but it's some functions for that. I probably won't try to update this until more puzzles using them shows up.

### `Coords.h`

Should have a lower-case C at the start, this was one of the first things I made for this library and use extensively. It has enums for directions, and helper functions for transforming those enums (e.g. rotation, reversing) and then integer 2D coordinates which with all the natural operators overloaded. It's a must-use on any grid-based solution, and is also used with other libraries.

### `coords_3d.h`

3D coordinates simple mathematical operations. Deprecated in favour of `position3d.h`

### `coords_iterators.h`

An adaptor for the 2D coords to iterate over a field defined by coordinates. For example: `for(utils::coords c : utils::elem_range{ utils::coords{0,0} , utils::coords{10,10} })` will iterate over all the coordinates in that field. It can also give you row and column range.

### 'enums.h`

Has utilities for turning an enum into an index, and for sorting on an enum value allowing the enum to be used as a key in maps or an ordered container.

### `erase_remove_if.h`

Shortcuts the erase-remove idiom.

### `grid.h`

Convenient ways to interact with grids. This includes automatically creating them from the input, by providing a `char` --> `NodeType` converter, and pathfinding through them with an A* search. It's fairly new and a little wonky/buggy still, but very useful even so.

### `has_duplicates.h`

Helper functions for asking whether a container or range has duplicates.

TODO: Concepts.   
TODO: Optimised path for sorted ranges.  
TODO: Pick optimised path at compile time for ranges that are always sorted.  
TODO: Overload for custom comparison functions.

### `index_iterator.h` and `index_iterator2.h`

The first never really worked, so I use the second of them... or the other way around, I can't remember right now. These adapt indexable containers by adding iterators that just index into the container. This makes building custom containers much easier. They aren't general purpose. The first one underpins the *very* useful `int_range.h` header.

### `int_range.h`

A *very* useful header I use in almost every solution. The `int_range` thing allows for things like `for(int x : utils::int_range(0,10,2))` which will iterate from `0` to `10` with a stride of `2` (essentially: `0`, `2`, `4`, `6`, `8`). `utils::int_range(4,10)` will go from `4` to `10` with a stride of `1` (`4`,`5`,`6`,`7`,`8`,`9`). And providing only one argument will start at `0`: `utils::int_range(4)` will go `0`,`1`,`2`,`3`).

The adaptor will create a range-type function from an existing indexable container. This is useful for things that are indexable but don't already have a range associated with them.

### `is_sorted.h`

I'm not sure what this is doing here, given `std::is_sorted` exists.

### `isqrt.h`

Integer square roots. Guaranteed not to have any issues with floating point inaccuracy (e.g. returning a `double` that's slightly *below* the actual result and then getting rounded down by integer truncation).

TODO: A version that returns any type of integer you want.

### `istream_block_iterator.h` and `istream_line_iterator.h`

Really really useful. The first allows iteration over a `std::istream` with each "element" being a `std::string_view` of either a block (multi-line section delimited by empty lines) or a line (delimited by a '\n'). In both cases the sentintal can be customized. The `line` version can take any `char` as a sentinental and the `block` version can take any `std::string_view` as sentinental, and split elements at a line matching that. E.g. `"123\n456\n---\n789\n101"` parsed with `"---"` as the sentential would have elements `123\n456` and `789\n101`.

There are also ranged versions.

### `md5.h`

Does an MD5 hash of the input. This was used for one puzzle in the early days.

### `min_transform.h`

This can be replicated more easily with ranges now.

### `modular_int.h`

A new addition and still needs some fleshing out. This has components for doing modular arithmatic with integers - something that comes up surprisingly often on Advent of Code.

TODO: Concepts  
TODO: Some unfinished functionality.

### `parse_utils.h`

Some really useful still in here. `get_string_elements.h` will grab particular parts of a string, based on a list of indices, and is an extension of `split_string.h`'s offerings in that regard. Also the `remove_specific_prefix` and `_suffix` functions will do error checking on a `std::string_view` to make sure you're removing what you expect to remove.

### `position3d.h`

Looks similar to `coords3d.h`

### `push_back_unique.h`

Calls `push_back` on a container after checking that the container doesn't already contain it. Useful for flat set type structures. It will return an iterator to the new element or the previously exising one if one exists.

### `range_contains.h`

For a numeric range, check whether a value is in that range.

TODO: Concepts?

### `ring_buffer.h`

A ring buffer.

### `shared_lock_guard.h`

Similar to `std::lock_guard` already in the standard library, but for use with shared mutexes. I rarely multithread my solutions so this doesn't see much use.

TODO: `[[noexcept]]`  
TODO: Concept?

### `small_vector.h`

This is a vector implementation with a small vector optimisation. I use this a lot, and it also underpins a few other container-type things in here. Use it instead of `std::vector` when you know you're expecting only a few elements.

### `sorted_vector.h`

The `std::flat_*` containers introduced in C++23 will make this obsolete, but until then this is a useful one. Deferred sorting isn't the performance gain I'd hoped for in most cases, though.

### `span.h`

Because `std::span` didn't arrive until C++20.

### `sparse_array.h`

A barebones sparse array.

### `split_string.h`

A number of ways to performantly split a string up either at a given point or at the first or last delimiter.

### `string_line_iterator.h`

Like the `istream_line_iterator.h` but operates on a `std::string_view` instead of a `std::istream`.

### `swap_remove.h`

Using the swap-remove idiom for removing things from a vector-like container where order isn't important. Probably supplanted by `std::flat_set` in C++23.

### `to_value.h`

I use this one a lot too. It has an `is_value` check, and `to_value` wraps `std::from_chars` with more useful error checking.

### `transform_if.h`

I dont' think this has an equivalent in the standard library. There are versions where the `if` is applied before AND after the transform is, and also a range-style implementation for help.

### `trim_string.h`

Removes trailing or leading whitespace (or both) from a `std::string_view`.

## TODO List

- Verify XCode and other setups. (I don't have non-Windows platforms running, so feel free to add a topic telling me it works and what special steps, if any, you had to use)
- Grab puzzle inputs from the web automatically if an empty file / non-existant puzzle input is detected. (CAREFULLY!)
- Add testing to CMake setup
- Start using concepts for various bits of utils. E.g.: `binary_find`; `bit_ops`; `combine_maps`
- Remove `advent_utils.h` dependancy from files. E.g. `brackets.h`
