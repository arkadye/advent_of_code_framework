#pragma once

#include "advent_testcase_setup.h"

static const std::string DEFAULT_FILTER = "";

static const verification_test tests[] =
{
	DAY(one,Dummy{},Dummy{}),
	DAY(two,Dummy{},Dummy{}),
	DAY(three,Dummy{},Dummy{}),
	DAY(four,Dummy{},Dummy{}),
	DAY(five,Dummy{},Dummy{}),
	DAY(six,Dummy{},Dummy{}),
	DAY(seven,Dummy{},Dummy{}),
	DAY(eight,Dummy{},Dummy{}),
	DAY(nine,Dummy{},Dummy{}),
	DAY(ten,Dummy{},Dummy{}),
	DAY(eleven,Dummy{},Dummy{}),
	DAY(twelve,Dummy{},Dummy{}),
	DAY(thirteen,Dummy{},Dummy{}),
	DAY(fourteen,Dummy{},Dummy{}),
	DAY(fifteen,Dummy{},Dummy{}),
	DAY(sixteen,Dummy{},Dummy{}),
	DAY(seventeen,Dummy{},Dummy{}),
	DAY(eighteen,Dummy{},Dummy{}),
	DAY(nineteen,Dummy{},Dummy{}),
	DAY(twenty,Dummy{},Dummy{}),
	DAY(twentyone,Dummy{},Dummy{}),
	DAY(twentytwo,Dummy{},Dummy{}),
	DAY(twentythree,Dummy{},Dummy{}),
	DAY(twentyfour,Dummy{},Dummy{}),
	DAY(twentyfive,Dummy{},"MERRY CHRISTMAS!")
};

#undef ARG
#undef TESTCASE
#undef FUNC_NAME
#undef TEST_DECL
#undef DAY
#undef DUMMY
#undef DUMMY_DAY