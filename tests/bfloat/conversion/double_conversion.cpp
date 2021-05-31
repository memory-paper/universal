// double_conversion.cpp: test suite runner for double conversions to bfloats
//
// Copyright (C) 2017-2021 Stillwater Supercomputing, Inc.
//
// This file is part of the universal numbers project, which is released under an MIT Open Source license.
#include <universal/utility/directives.hpp>
#include <iostream>
#include <iomanip>
// Configure the bfloat template environment
// first: enable general or specialized configurations
#define BFLOAT_FAST_SPECIALIZATION
// second: enable/disable arithmetic exceptions
#define BFLOAT_THROW_ARITHMETIC_EXCEPTION 0
// third: enable trace conversion
#define TRACE_CONVERSION 0

// minimum set of include files to reflect source code dependencies
#include <universal/number/bfloat/bfloat.hpp>
#include <universal/number/bfloat/manipulators.hpp>
#include <universal/number/bfloat/math_functions.hpp>
#include <universal/verification/test_suite_conversion.hpp>
#include <universal/verification/bfloat_test_suite.hpp>
#include <universal/number/bfloat/table.hpp> // only used for value table generation

// sign of 0 is flipped on MSVC Release builds
void CompilerBug() {
	using namespace std;
	using namespace sw::universal;
	{
		bfloat<5, 1> a;
		a.setbits(0x0);
		cout << "bfloat<5,1> : " << to_binary(a) << " : " << a << endl;
		float f = float(a);
		cout << "float      : " << f << endl;
		double d = double(a);
		cout << "double     : " << d << endl;
	}
	{
		bfloat<5, 1> a;
		a.setbits(0x10);
		cout << "bfloat<5,1> : " << to_binary(a) << " : " << a << endl;
		float f = float(a);
		cout << "float      : " << f << endl;
		double d = double(a);
		cout << "double     : " << d << endl;
	}

	{
		bfloat<6, 1> a;
		a.setbits(0x0);
		cout << "bfloat<6,1> : " << to_binary(a) << " : " << a << endl;
		float f = float(a);
		cout << "float      : " << f << endl;
		double d = double(a);
		cout << "double     : " << d << endl;
	}
	{
		bfloat<6, 1> a;
		a.setbits(0x20);
		cout << "bfloat<6,1> : " << to_binary(a) << " : " << a << endl;
		float f = float(a);
		cout << "float      : " << f << endl;
		double d = double(a);
		cout << "double     : " << d << endl;
	}
}

/* IEEE-754 double precision subnormals
b0.00000000000.0000000000000000000000000000000000000000000000000001 : 4.940656458412465e-324
b0.00000000000.0000000000000000000000000000000000000000000000000001 : 4.940656458412465e-324
b0.00000000000.0000000000000000000000000000000000000000000000000010 : 9.881312916824931e-324
b0.00000000000.0000000000000000000000000000000000000000000000000100 : 1.976262583364986e-323
b0.00000000000.0000000000000000000000000000000000000000000000001000 : 3.952525166729972e-323
b0.00000000000.0000000000000000000000000000000000000000000000010000 : 7.905050333459945e-323
b0.00000000000.0000000000000000000000000000000000000000000000100000 : 1.581010066691989e-322
b0.00000000000.0000000000000000000000000000000000000000000001000000 : 3.162020133383978e-322
b0.00000000000.0000000000000000000000000000000000000000000010000000 : 6.324040266767956e-322
b0.00000000000.0000000000000000000000000000000000000000000100000000 : 1.264808053353591e-321
b0.00000000000.0000000000000000000000000000000000000000001000000000 : 2.529616106707182e-321
b0.00000000000.0000000000000000000000000000000000000000010000000000 : 5.059232213414365e-321
b0.00000000000.0000000000000000000000000000000000000000100000000000 : 1.011846442682873e-320
b0.00000000000.0000000000000000000000000000000000000001000000000000 : 2.023692885365746e-320
b0.00000000000.0000000000000000000000000000000000000010000000000000 : 4.047385770731492e-320
b0.00000000000.0000000000000000000000000000000000000100000000000000 : 8.094771541462983e-320
b0.00000000000.0000000000000000000000000000000000001000000000000000 : 1.618954308292597e-319
b0.00000000000.0000000000000000000000000000000000010000000000000000 : 3.237908616585193e-319
b0.00000000000.0000000000000000000000000000000000100000000000000000 : 6.475817233170387e-319
b0.00000000000.0000000000000000000000000000000001000000000000000000 : 1.295163446634077e-318
b0.00000000000.0000000000000000000000000000000010000000000000000000 : 2.590326893268155e-318
b0.00000000000.0000000000000000000000000000000100000000000000000000 : 5.180653786536309e-318
b0.00000000000.0000000000000000000000000000001000000000000000000000 : 1.036130757307262e-317
b0.00000000000.0000000000000000000000000000010000000000000000000000 : 2.072261514614524e-317
b0.00000000000.0000000000000000000000000000100000000000000000000000 : 4.144523029229047e-317
b0.00000000000.0000000000000000000000000001000000000000000000000000 : 8.289046058458095e-317
b0.00000000000.0000000000000000000000000010000000000000000000000000 : 1.657809211691619e-316
b0.00000000000.0000000000000000000000000100000000000000000000000000 : 3.315618423383238e-316
b0.00000000000.0000000000000000000000001000000000000000000000000000 : 6.631236846766476e-316
b0.00000000000.0000000000000000000000010000000000000000000000000000 : 1.326247369353295e-315
b0.00000000000.0000000000000000000000100000000000000000000000000000 : 2.65249473870659e-315
b0.00000000000.0000000000000000000001000000000000000000000000000000 : 5.304989477413181e-315
b0.00000000000.0000000000000000000010000000000000000000000000000000 : 1.060997895482636e-314
b0.00000000000.0000000000000000000100000000000000000000000000000000 : 2.121995790965272e-314
b0.00000000000.0000000000000000001000000000000000000000000000000000 : 4.243991581930545e-314
b0.00000000000.0000000000000000010000000000000000000000000000000000 : 8.487983163861089e-314
b0.00000000000.0000000000000000100000000000000000000000000000000000 : 1.697596632772218e-313
b0.00000000000.0000000000000001000000000000000000000000000000000000 : 3.395193265544436e-313
b0.00000000000.0000000000000010000000000000000000000000000000000000 : 6.790386531088871e-313
b0.00000000000.0000000000000100000000000000000000000000000000000000 : 1.358077306217774e-312
b0.00000000000.0000000000001000000000000000000000000000000000000000 : 2.716154612435549e-312
b0.00000000000.0000000000010000000000000000000000000000000000000000 : 5.432309224871097e-312
b0.00000000000.0000000000100000000000000000000000000000000000000000 : 1.086461844974219e-311
b0.00000000000.0000000001000000000000000000000000000000000000000000 : 2.172923689948439e-311
b0.00000000000.0000000010000000000000000000000000000000000000000000 : 4.345847379896878e-311
b0.00000000000.0000000100000000000000000000000000000000000000000000 : 8.691694759793755e-311
b0.00000000000.0000001000000000000000000000000000000000000000000000 : 1.738338951958751e-310
b0.00000000000.0000010000000000000000000000000000000000000000000000 : 3.476677903917502e-310
b0.00000000000.0000100000000000000000000000000000000000000000000000 : 6.953355807835004e-310
b0.00000000000.0001000000000000000000000000000000000000000000000000 : 1.390671161567001e-309
b0.00000000000.0010000000000000000000000000000000000000000000000000 : 2.781342323134002e-309
b0.00000000000.0100000000000000000000000000000000000000000000000000 : 5.562684646268003e-309
b0.00000000000.1000000000000000000000000000000000000000000000000000 : 1.112536929253601e-308
b0.00000000001.0000000000000000000000000000000000000000000000000000 : 2.225073858507201e-308
b0.00000000010.0000000000000000000000000000000000000000000000000000 : 4.450147717014403e-308
*/

// double subnormals with the last entry being the smallest normal value
constexpr double ieee754_double_subnormals[53] = {
 4.940656458412465e-324,
 9.881312916824931e-324,
 1.976262583364986e-323,
 3.952525166729972e-323,
 7.905050333459945e-323,
 1.581010066691989e-322,
 3.162020133383978e-322,
 6.324040266767956e-322,
 1.264808053353591e-321,
 2.529616106707182e-321,
 5.059232213414365e-321,
 1.011846442682873e-320,
 2.023692885365746e-320,
 4.047385770731492e-320,
 8.094771541462983e-320,
 1.618954308292597e-319,
 3.237908616585193e-319,
 6.475817233170387e-319,
 1.295163446634077e-318,
 2.590326893268155e-318,
 5.180653786536309e-318,
 1.036130757307262e-317,
 2.072261514614524e-317,
 4.144523029229047e-317,
 8.289046058458095e-317,
 1.657809211691619e-316,
 3.315618423383238e-316,
 6.631236846766476e-316,
 1.326247369353295e-315,
 2.65249473870659e-315,
 5.304989477413181e-315,
 1.060997895482636e-314,
 2.121995790965272e-314,
 4.243991581930545e-314,
 8.487983163861089e-314,
 1.697596632772218e-313,
 3.395193265544436e-313,
 6.790386531088871e-313,
 1.358077306217774e-312,
 2.716154612435549e-312,
 5.432309224871097e-312,
 1.086461844974219e-311,
 2.172923689948439e-311,
 4.345847379896878e-311,
 8.691694759793755e-311,
 1.738338951958751e-310,
 3.476677903917502e-310,
 6.953355807835004e-310,
 1.390671161567001e-309,
 2.781342323134002e-309,
 5.562684646268003e-309,
 1.112536929253600691545e-308,
 2.2250738585072013831e-308        // smallest normal value
 };

double smallest_normal = 2.225073858507201e-308;

void GenerateDoublePrecisionSubnormals()
{
	constexpr size_t nbits = 64;
	constexpr size_t es = 11;
	using bt = uint64_t;
	using namespace sw::universal;
	bfloat<nbits, es, bt> a, b;
	++a;
	double d = double(a);
	std::cout << std::setprecision(20);
	std::cout << to_binary(a) << " : " << a << '\n';
	std::cout << to_binary(d) << " : " << d << '\n';
	for (int i = 0; i < 53; ++i) {
		d *= 2;
		std::cout << to_binary(d) << " : " << d << '\n';
	}
	for (int i = 0; i < 53; ++i) {
		d = ieee754_double_subnormals[i];
		std::cout << to_binary(d) << " : " << d << '\n';
	}
	std::cout << std::setprecision(5);
}

// conditional compile flags
#define MANUAL_TESTING 1
#define STRESS_TESTING 0

int main(int argc, char** argv)
try {
	using namespace std;
	using namespace sw::universal;

	print_cmd_line(argc, argv);

	int nrOfFailedTestCases = 0;
	std::string tag = "double to bfloat conversion: ";

#if MANUAL_TESTING

	// to track conversion in more detail
	std::cout << std::setprecision(15);
	std::cerr << std::setprecision(15);

	{
		bfloat<6, 2, uint8_t> a;
		a.constexprClassParameters();
		double testValue = 0.0625000074505806;
		a = testValue;
		double da = double(a);
		std::cout << to_binary(a) << " : " << a << " : " << da << " : " << setprecision(8) << testValue << endl;
	}

	bool bReportIndividualTestCases = true;
	nrOfFailedTestCases += ReportTestResult(VerifyDoubleSubnormals<uint8_t>(bReportIndividualTestCases), tag, "bfloat<64, 11, uint8_t>");
	nrOfFailedTestCases += ReportTestResult(VerifyDoubleSubnormals<uint16_t>(bReportIndividualTestCases), tag, "bfloat<64, 11, uint16_t>");
	nrOfFailedTestCases += ReportTestResult(VerifyDoubleSubnormals<uint32_t>(bReportIndividualTestCases), tag, "bfloat<64, 11, uint32_t>");
	nrOfFailedTestCases += ReportTestResult(VerifyDoubleSubnormals<uint64_t>(bReportIndividualTestCases), tag, "bfloat<64, 11, uint64_t>");

	nrOfFailedTestCases += ReportTestResult(VerifyFloat2BfloatConversionRnd< bfloat<64, 8, uint8_t> >(true), tag, "bfloat<64, 8, uint8_t>");
	nrOfFailedTestCases += ReportTestResult(VerifyFloat2BfloatConversionRnd< bfloat<64, 8, uint16_t> >(true), tag, "bfloat<64, 8, uint16_t>");
	nrOfFailedTestCases += ReportTestResult(VerifyFloat2BfloatConversionRnd< bfloat<64, 8, uint32_t> >(true), tag, "bfloat<64, 8, uint32_t>");
	nrOfFailedTestCases += ReportTestResult(VerifyFloat2BfloatConversionRnd< bfloat<64, 8, uint64_t> >(true), tag, "bfloat<64, 8, uint64_t>");

	// es = 1
	nrOfFailedTestCases = ReportTestResult(VerifyBfloatConversion< bfloat< 4, 1>, double >(bReportIndividualTestCases), tag, "bfloat< 4,1>");
	nrOfFailedTestCases = ReportTestResult(VerifyBfloatConversion< bfloat< 5, 1>, double >(bReportIndividualTestCases), tag, "bfloat< 5,1>");
	nrOfFailedTestCases = ReportTestResult(VerifyBfloatConversion< bfloat< 6, 1>, double >(bReportIndividualTestCases), tag, "bfloat< 6,1>");
	nrOfFailedTestCases = ReportTestResult(VerifyBfloatConversion< bfloat< 7, 1>, double >(bReportIndividualTestCases), tag, "bfloat< 7,1>");
	nrOfFailedTestCases = ReportTestResult(VerifyBfloatConversion< bfloat< 8, 1>, double >(bReportIndividualTestCases), tag, "bfloat< 8,1>");
	nrOfFailedTestCases = ReportTestResult(VerifyBfloatConversion< bfloat< 9, 1>, double >(bReportIndividualTestCases), tag, "bfloat< 9,1>");
	nrOfFailedTestCases = ReportTestResult(VerifyBfloatConversion< bfloat<10, 1>, double >(bReportIndividualTestCases), tag, "bfloat<10,1>");
	nrOfFailedTestCases = ReportTestResult(VerifyBfloatConversion< bfloat<12, 1>, double >(bReportIndividualTestCases), tag, "bfloat<12,1>");

	std::cout << "failed tests: " << nrOfFailedTestCases << endl;
	nrOfFailedTestCases = 0; // in manual testing we ignore failures for the regression system

#if STRESS_TESTING

	// manual exhaustive test

#endif

#else  // !MANUAL_TESTING
	bool bReportIndividualTestCases = false;
	cout << "BFLOAT conversion from double validation" << endl;

	// es = 1
	nrOfFailedTestCases = ReportTestResult(VerifyBfloatConversion< bfloat< 4, 1>, double >(bReportIndividualTestCases), tag, "bfloat< 4,1>");
	nrOfFailedTestCases = ReportTestResult(VerifyBfloatConversion< bfloat< 5, 1>, double >(bReportIndividualTestCases), tag, "bfloat< 5,1>");
	nrOfFailedTestCases = ReportTestResult(VerifyBfloatConversion< bfloat< 6, 1>, double >(bReportIndividualTestCases), tag, "bfloat< 6,1>");
	nrOfFailedTestCases = ReportTestResult(VerifyBfloatConversion< bfloat< 7, 1>, double >(bReportIndividualTestCases), tag, "bfloat< 7,1>");
	nrOfFailedTestCases = ReportTestResult(VerifyBfloatConversion< bfloat< 8, 1>, double >(bReportIndividualTestCases), tag, "bfloat< 8,1>");
	nrOfFailedTestCases = ReportTestResult(VerifyBfloatConversion< bfloat< 9, 1>, double >(bReportIndividualTestCases), tag, "bfloat< 9,1>");
	nrOfFailedTestCases = ReportTestResult(VerifyBfloatConversion< bfloat<10, 1>, double >(bReportIndividualTestCases), tag, "bfloat<10,1>");
	nrOfFailedTestCases = ReportTestResult(VerifyBfloatConversion< bfloat<12, 1>, double >(bReportIndividualTestCases), tag, "bfloat<12,1>");


	// es = 2
	nrOfFailedTestCases = ReportTestResult(VerifyBfloatConversion< bfloat< 5, 2>, double >(bReportIndividualTestCases), tag, "bfloat< 5,2>");
	nrOfFailedTestCases = ReportTestResult(VerifyBfloatConversion< bfloat< 6, 2>, double >(bReportIndividualTestCases), tag, "bfloat< 6,2>");
	nrOfFailedTestCases = ReportTestResult(VerifyBfloatConversion< bfloat< 7, 2>, double >(bReportIndividualTestCases), tag, "bfloat< 7,2>");
	nrOfFailedTestCases = ReportTestResult(VerifyBfloatConversion< bfloat< 8, 2>, double >(bReportIndividualTestCases), tag, "bfloat< 8,2>");
	nrOfFailedTestCases = ReportTestResult(VerifyBfloatConversion< bfloat<10, 2>, double >(bReportIndividualTestCases), tag, "bfloat<10,2>");
	nrOfFailedTestCases = ReportTestResult(VerifyBfloatConversion< bfloat<12, 2>, double >(bReportIndividualTestCases), tag, "bfloat<12,2>");
	nrOfFailedTestCases = ReportTestResult(VerifyBfloatConversion< bfloat<14, 2>, double >(bReportIndividualTestCases), tag, "bfloat<14,2>");


	// es = 3
	nrOfFailedTestCases = ReportTestResult(VerifyBfloatConversion< bfloat< 6, 3>, double >(bReportIndividualTestCases), tag, "bfloat< 6,3>");
	nrOfFailedTestCases = ReportTestResult(VerifyBfloatConversion< bfloat< 7, 3>, double >(bReportIndividualTestCases), tag, "bfloat< 7,3>");
	nrOfFailedTestCases = ReportTestResult(VerifyBfloatConversion< bfloat< 8, 3>, double >(bReportIndividualTestCases), tag, "bfloat< 8,3>");
	nrOfFailedTestCases = ReportTestResult(VerifyBfloatConversion< bfloat<10, 3>, double >(bReportIndividualTestCases), tag, "bfloat<10,3>");
	nrOfFailedTestCases = ReportTestResult(VerifyBfloatConversion< bfloat<12, 3>, double >(bReportIndividualTestCases), tag, "bfloat<12,3>");
	nrOfFailedTestCases = ReportTestResult(VerifyBfloatConversion< bfloat<14, 3>, double >(bReportIndividualTestCases), tag, "bfloat<14,3>");


	// es = 4
	nrOfFailedTestCases = ReportTestResult(VerifyBfloatConversion< bfloat< 7, 4>, double >(bReportIndividualTestCases), tag, "bfloat< 7,4>");
	nrOfFailedTestCases = ReportTestResult(VerifyBfloatConversion< bfloat< 8, 4>, double >(bReportIndividualTestCases), tag, "bfloat< 8,4>");
	nrOfFailedTestCases = ReportTestResult(VerifyBfloatConversion< bfloat<10, 4>, double >(bReportIndividualTestCases), tag, "bfloat<10,4>");
	nrOfFailedTestCases = ReportTestResult(VerifyBfloatConversion< bfloat<12, 4>, double >(bReportIndividualTestCases), tag, "bfloat<12,4>");
	nrOfFailedTestCases = ReportTestResult(VerifyBfloatConversion< bfloat<14, 4>, double >(bReportIndividualTestCases), tag, "bfloat<14,4>");


	// es = 5
	nrOfFailedTestCases = ReportTestResult(VerifyBfloatConversion< bfloat< 8, 5>, double >(bReportIndividualTestCases), tag, "bfloat< 8,5>");
	nrOfFailedTestCases = ReportTestResult(VerifyBfloatConversion< bfloat<10, 5>, double >(bReportIndividualTestCases), tag, "bfloat<10,5>");
	nrOfFailedTestCases = ReportTestResult(VerifyBfloatConversion< bfloat<12, 5>, double >(bReportIndividualTestCases), tag, "bfloat<12,5>");
	nrOfFailedTestCases = ReportTestResult(VerifyBfloatConversion< bfloat<14, 5>, double >(bReportIndividualTestCases), tag, "bfloat<14,5>");
	nrOfFailedTestCases = ReportTestResult(VerifyBfloatConversion< bfloat<16, 5>, double >(bReportIndividualTestCases), tag, "bfloat<16,5>");
	nrOfFailedTestCases = ReportTestResult(VerifyBfloatConversion< bfloat<16, 5, uint16_t>, double >(bReportIndividualTestCases), tag, "bfloat<16,5, uint16_t>");

#ifdef LATER
	// es = 6
	nrOfFailedTestCases = ReportTestResult(VerifyBfloatConversion< bfloat< 9, 6>, double >(bReportIndividualTestCases), tag, "bfloat< 9,6>");
	nrOfFailedTestCases = ReportTestResult(VerifyBfloatConversion< bfloat<10, 6>, double >(bReportIndividualTestCases), tag, "bfloat<10,6>");
	nrOfFailedTestCases = ReportTestResult(VerifyBfloatConversion< bfloat<12, 6>, double >(bReportIndividualTestCases), tag, "bfloat<12,6>");
	nrOfFailedTestCases = ReportTestResult(VerifyBfloatConversion< bfloat<14, 6>, double >(bReportIndividualTestCases), tag, "bfloat<14,6>");


	// es = 7
	nrOfFailedTestCases = ReportTestResult(VerifyBfloatConversion< bfloat<10, 7>, double >(bReportIndividualTestCases), tag, "bfloat<10,7>");
	nrOfFailedTestCases = ReportTestResult(VerifyBfloatConversion< bfloat<12, 7>, double >(bReportIndividualTestCases), tag, "bfloat<12,7>");
	nrOfFailedTestCases = ReportTestResult(VerifyBfloatConversion< bfloat<14, 7>, double >(bReportIndividualTestCases), tag, "bfloat<14,7>");


	// es = 8
	nrOfFailedTestCases = ReportTestResult(VerifyBfloatConversion< bfloat<11, 8>, double >(bReportIndividualTestCases), tag, "bfloat<11,8>");
	nrOfFailedTestCases = ReportTestResult(VerifyBfloatConversion< bfloat<12, 8>, double >(bReportIndividualTestCases), tag, "bfloat<12,8>");
	nrOfFailedTestCases = ReportTestResult(VerifyBfloatConversion< bfloat<14, 8>, double >(bReportIndividualTestCases), tag, "bfloat<14,8>");

#endif // LATER

#if STRESS_TESTING

#endif  // STRESS_TESTING

#endif  // MANUAL_TESTING

	return (nrOfFailedTestCases > 0 ? EXIT_FAILURE : EXIT_SUCCESS);
}
catch (char const* msg) {
	std::cerr << "Caught exception: " << msg << std::endl;
	return EXIT_FAILURE;
}
catch (const sw::universal::bfloat_arithmetic_exception& err) {
	std::cerr << "Uncaught bfloat arithmetic exception: " << err.what() << std::endl;
	return EXIT_FAILURE;
}
catch (const sw::universal::bfloat_internal_exception& err) {
	std::cerr << "Uncaught bfloat internal exception: " << err.what() << std::endl;
	return EXIT_FAILURE;
}
catch (const std::runtime_error& err) {
	std::cerr << "Uncaught runtime exception: " << err.what() << std::endl;
	return EXIT_FAILURE;
}
catch (...) {
	std::cerr << "Caught unknown exception" << std::endl;
	return EXIT_FAILURE;
}


/*

  To generate:
  	GenerateFixedPointComparisonTable<4, 0>(std::string("-"));
	GenerateFixedPointComparisonTable<4, 1>(std::string("-"));
	GenerateFixedPointComparisonTable<4, 2>(std::string("-"));
	

 */
