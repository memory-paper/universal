// addition.cpp: test suite runner for addition on classic floats
//
// Copyright (C) 2017-2021 Stillwater Supercomputing, Inc.
//
// This file is part of the universal numbers project, which is released under an MIT Open Source license.
#include <universal/utility/directives.hpp>
// minimum set of include files to reflect source code dependencies
#define CFLOAT_VERBOSE_OUTPUT
#define CFLOAT_TRACE_ADD
#define BLOCKTRIPLE_VERBOSE_OUTPUT
#define BLOCKTRIPLE_TRACE_ADD
#include <universal/number/cfloat/cfloat.hpp>
#include <universal/verification/test_suite.hpp>
#include <universal/verification/cfloat_test_suite.hpp>
#include <universal/number/cfloat/table.hpp>

// Regression testing guards: typically set by the cmake configuration, but MANUAL_TESTING is an override
#define MANUAL_TESTING 1
// REGRESSION_LEVEL_OVERRIDE is set by the cmake file to drive a specific regression intensity
// It is the responsibility of the regression test to organize the tests in a quartile progression.
//#undef REGRESSION_LEVEL_OVERRIDE
#ifndef REGRESSION_LEVEL_OVERRIDE
#undef REGRESSION_LEVEL_1
#undef REGRESSION_LEVEL_2
#undef REGRESSION_LEVEL_3
#undef REGRESSION_LEVEL_4
#define REGRESSION_LEVEL_1 1
#define REGRESSION_LEVEL_2 0
#define REGRESSION_LEVEL_3 0
#define REGRESSION_LEVEL_4 0
#endif

/*
  Minimum number of operand bits for the adder = <abits> 
  to yield correctly rounded addition

                          number of exponent bits = <es>
  nbits   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16
	 1    -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -
	 2    -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -
	 3    2   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -
	 4    3   3   -   -   -   -   -   -   -   -   -   -   -   -   -   -
	 5    4   4   4   -   -   -   -   -   -   -   -   -   -   -   -   -
	 6    5   5   6   4   -   -   -   -   -   -   -   -   -   -   -   -
	 7    6   6   8   6   4   -   -   -   -   -   -   -   -   -   -   -
	 8    7   7  10   8   6   4   -   -   -   -   -   -   -   -   -   -
	 9    8   8  11  10   8   6   4   -   -   -   -   -   -   -   -   -
	10    9   9  12  12  10   8   6   4   -   -   -   -   -   -   -   -
	11   10  10  13  14  12  10   8   6   4   -   -   -   -   -   -   -
	12   11  11  14  16  14  12  10   8   6   4   -   -   -   -   -   -
	13   12  12  15  18  16  14  12  10   8   6   ?   -   -   -   -   -
	14   13  13  16  20  18  16  14  12  10   8   ?   ?   -   -   -   -
	15   14  14  17  22  20  18  16  14  12  10   ?   ?   ?   -   -   -
	16   15  15  18  24  22  20  18  16  14  12   ?   ?   ?   ?   -   -

*/

int main()
try {
	using namespace sw::universal;

	// cfloat encoding configuration for the test
	constexpr bool hasSubnormals = true;
	constexpr bool hasSupernormals = true;
	constexpr bool isSaturating = false;

	std::string test_suite = "classic cfloat_ttf addition validation";
	std::string test_tag   = "cfloat_ttf addition";
	std::cout << test_suite << '\n';
	bool bReportIndividualTestCases = false;
	int nrOfFailedTestCases = 0;

#if MANUAL_TESTING

	GenerateCfloatExponentBounds();

	TestCase< cfloat<4, 1, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, float>(TestCaseOperator::ADD, 0.5f, 1.5f);

	return 0;
	{
		float fa = 0.017578125;
//		float fa = std::numeric_limits<float>::infinity();
//		float fb = std::numeric_limits<float>::signaling_NaN();
//		float fb = std::numeric_limits<float>::quiet_NaN();
		float fb = -0.5f;

		using Cfloat = cfloat < 8, 4, uint8_t, hasSubnormals, hasSupernormals, isSaturating >;
		Cfloat a, b, c;
		a.constexprClassParameters();
		a = fa;
		b = fb;
		c = a + b;
		std::cout << a << " + " << b << " = " << c << '\n';
		std::cout << to_binary(a) << " + " << to_binary(b) << " = " << to_binary(c) << '\n';

		TestCase< Cfloat, float>(TestCaseOperator::ADD, fa, fb);
	}

	{
		constexpr float fa = std::numeric_limits<float>::infinity();
		constexpr float fb = -fa;
		std::cout << fa << " + " << fa << " = " << (fa + fa) << '\n';
		std::cout << fa << " + " << fb << " = " << (fa + fb) << '\n';
		std::cout << fb << " + " << fa << " = " << (fb + fa) << '\n';
		std::cout << fb << " + " << fb << " = " << (fb + fb) << '\n';
		std::cout << to_binary(fa + fb) << '\n';
	}

	// generate individual testcases to hand trace/debug
	TestCase< cfloat<8, 2, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, float>(TestCaseOperator::ADD, 1.0f, 1.0f);
	TestCase< cfloat<16, 8, uint16_t, hasSubnormals, hasSupernormals, isSaturating>, double>(TestCaseOperator::ADD, INFINITY, INFINITY);

	nrOfFailedTestCases += ReportTestResult(
		VerifyCfloatAddition< cfloat<4, 1, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(true), "cfloat<4,1,uint8_t, t,t,f>", "addition");
	return 0;
	nrOfFailedTestCases += ReportTestResult(
		VerifyCfloatAddition< cfloat<8, 2, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(true), "cfloat<8,2,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(
		VerifyCfloatAddition< cfloat<8, 3, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(true), "cfloat<8,3,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(
		VerifyCfloatAddition< cfloat<8, 4, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(true), "cfloat<8,4,uint8_t, t,t,f>", "addition");

	ReportTestSuiteResults(test_suite, nrOfFailedTestCases);
	return EXIT_SUCCESS; // ignore failures
#else  // !MANUAL_TESTING

#if REGRESSION_LEVEL_1
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<3, 1, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat< 3, 1,uint8_t, t,t,f>", "addition");

	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<4, 1, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat< 4, 1,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<4, 2, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat< 4, 2,uint8_t, t,t,f>", "addition");

	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<5, 1, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat< 5, 1,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<5, 2, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat< 5, 2,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<5, 3, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat< 5, 3,uint8_t, t,t,f>", "addition");

	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<6, 1, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat< 6, 1,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<6, 2, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat< 6, 2,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<6, 3, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat< 6, 3,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<6, 4, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat< 6, 4,uint8_t, t,t,f>", "addition");

	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<7, 1, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat< 7, 1,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<7, 2, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat< 7, 2,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<7, 3, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat< 7, 3,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<7, 4, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat< 7, 4,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<7, 5, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat< 7, 5,uint8_t, t,t,f>", "addition");

	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<8, 1, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat< 8, 1,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<8, 2, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat< 8, 2,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<8, 3, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat< 8, 3,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<8, 4, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat< 8, 4,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<8, 5, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat< 8, 5,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<8, 6, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat< 8, 6,uint8_t, t,t,f>", "addition");
#endif

#if REGRESSION_LEVEL_2
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<9, 1, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat< 9, 1,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<9, 2, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat< 9, 2,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<9, 3, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat< 9, 3,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<9, 4, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat< 9, 4,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<9, 5, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat< 9, 5,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<9, 6, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat< 9, 6,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<9, 7, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat< 9, 7,uint8_t, t,t,f>", "addition");

	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<10, 1, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<10, 1,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<10, 2, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<10, 2,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<10, 3, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<10, 3,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<10, 4, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<10, 4,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<10, 5, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<10, 5,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<10, 6, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<10, 6,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<10, 7, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<10, 7,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<10, 8, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<10, 8,uint8_t, t,t,f>", "addition");

	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<11, 1, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<11, 1,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<11, 2, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<11, 2,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<11, 3, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<11, 3,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<11, 4, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<11, 4,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<11, 5, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<11, 5,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<11, 6, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<11, 6,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<11, 7, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<11, 7,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<11, 8, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<11, 8,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<11, 9, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<11, 9,uint8_t, t,t,f>", "addition");
#endif

#if REGRESSION_LEVEL_3
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<12, 1, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<12, 1,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<12, 2, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<12, 2,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<12, 3, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<12, 3,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<12, 4, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<12, 4,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<12, 5, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<12, 5,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<12, 6, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<12, 6,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<12, 7, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<12, 7,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<12, 8, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<12, 8,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<12, 9, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<12, 9,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<12,10, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<12,10,uint8_t, t,t,f>", "addition");

	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<13, 3, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<13, 3,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<13, 4, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<13, 4,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<13, 5, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<13, 5,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<13, 6, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<13, 6,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<13, 7, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<13, 7,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<13, 8, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<13, 8,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<13, 9, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<13, 9,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<13, 10, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<13,10,uint8_t ,t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<13, 11, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<13,11,uint8_t, t,t,f>", "addition");
#endif

#if REGRESSION_LEVEL_4
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<14, 3, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<14, 3,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<14, 4, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<14, 4,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<14, 5, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<14, 5,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<14, 6, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<14, 6,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<14, 7, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<14, 7,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<14, 8, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<14, 8,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<14, 9, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<14, 9,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<14, 10, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<14,10,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<14, 11, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<14,11,uint8_t, t,t,f>", "addition");

	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<15, 3, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<15, 3,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<15, 4, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<15, 4,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<15, 5, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<15, 5,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<15, 6, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<15, 6,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<15, 7, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<15, 7,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<15, 8, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<15, 8,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<15, 9, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<15, 9,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<15, 10, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<15,10,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<15, 11, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<15,11,uint8_t, t,t,f>", "addition");

	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<16, 3, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<16, 3,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<16, 4, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<16, 4,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<16, 5, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<16, 5,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<16, 6, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<16, 6,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<16, 7, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<16, 7,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<16, 8, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<16, 8,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<16, 9, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<16, 9,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<16, 10, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<16,10,uint8_t, t,t,f>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatAddition< cfloat<16, 11, uint8_t, hasSubnormals, hasSupernormals, isSaturating> >(bReportIndividualTestCases), "cfloat<16,11,uint8_t, t,t,f>", "addition");
#endif

	ReportTestSuiteResults(test_suite, nrOfFailedTestCases);
	return (nrOfFailedTestCases > 0 ? EXIT_FAILURE : EXIT_SUCCESS);
#endif  // MANUAL_TESTING
}
catch (char const* msg) {
	std::cerr << "Caught ad-hoc exception: " << msg << std::endl;
	return EXIT_FAILURE;
}
catch (const sw::universal::universal_arithmetic_exception& err) {
	std::cerr << "Caught unexpected universal arithmetic exception : " << err.what() << std::endl;
	return EXIT_FAILURE;
}
catch (const sw::universal::universal_internal_exception& err) {
	std::cerr << "Caught unexpected universal internal exception: " << err.what() << std::endl;
	return EXIT_FAILURE;
}
catch (const std::runtime_error& err) {
	std::cerr << "Caught runtime exception: " << err.what() << std::endl;
	return EXIT_FAILURE;
}
catch (...) {
	std::cerr << "Caught unknown exception" << std::endl;
	return EXIT_FAILURE;
}
