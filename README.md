# Advent of Code Framework

A framework to build solutions to the Advent of Code competition on. It includes some basic utilities, and a test suite to verify your solutions. Example cases can also be tested.

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

## To filter which testcases run

In main.cpp, a filter can be added to the argument of `verify_all()` to only run some of hte testcases, instead of everything. For example, if your day one solution is very slow you can put an argument of `"_two_"` in, and it will only run functions on the `verification_tests` list which have `two` in the function name (e.g. `advent_two_p1()` and `advent_two_p2()` as well as `advent_two_p1_testcase_a()` if you added it. To run everything, leave the testcase blank (`""` or `std::string{}`).

## Best practices

If you add testcases, name them `advent_[day number]_[p1 or p2, depending which part]_testcase_[letter]()` in order to make the filtering easy.

For functions specifically for solving a particular solution, put them in an unnamed namespace in the right file.

If 2019's code was anything to go by, don't be afraid to add common tasks to their own headers. For an example: a generic A* search function should probably in be the utility file, and I had all the Intcode stuff in its own folder last time. Anything that used the Intcode machine had `_ic` added to the function names which meant that I could edit the Intmachine and then run my testcases with `_ic` as the filter to make sure I didn't break any previous compatibility. This is a great project to test your ability to reuse old code.

## CMake setup

1. Install CMake.
2. Open the base folder, with `CMakeLists.txt` in it.
3. Open a command window on that folder.

You can then run whatever CMake command you like in there. I use `cmake -S . -G "Visual Studio 17 2022"`

There is a secret argument you can add. If you put `-DRESET_ALL_SOLUTIONS=True` onto the CMake command it will delete all solutions and reset them. I mainly use this to copy the repro from one year to the next and give myself a clean slate.

## TODO List

- Verify XCode and other setups. (I don't have non-Windows platforms running, so feel free to add a topic telling me it works and what special steps, if any, you had to use)
- Grab puzzle inputs from the web automatically if an empty file / non-existant puzzle input is detected. (CAREFULLY!)
- Add testing to CMake setup
