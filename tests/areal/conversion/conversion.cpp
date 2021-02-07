// conversion.cpp: test suite runner for areal conversions
//
// Copyright (C) 2017-2021 Stillwater Supercomputing, Inc.
//
// This file is part of the universal numbers project, which is released under an MIT Open Source license.
#include <iostream>
#include <iomanip>
// Configure the areal template environment
// first: enable general or specialized configurations
#define AREAL_FAST_SPECIALIZATION
// second: enable/disable arithmetic exceptions
#define AREAL_THROW_ARITHMETIC_EXCEPTION 0
// third: enable trace conversion
#define TRACE_CONVERSION 0

// minimum set of include files to reflect source code dependencies
#include <universal/number/areal/areal.hpp>
#include <universal/number/areal/manipulators.hpp>
#include <universal/number/areal/math_functions.hpp>
#include <universal/verification/test_suite_arithmetic.hpp>
#include <universal/number/areal/table.hpp> // only used for value table generation

// generate specific test case that you can trace with the trace conditions
// for most bugs they are traceable with _trace_conversion and _trace_add
template<size_t nbits, size_t es, typename Ty>
void GenerateTestCase(Ty _a, Ty _b) {
	Ty ref;
	sw::universal::areal<nbits, es> a, b, cref, result;
	a = _a;
	b = _b;
	result = a + b;
	ref = _a + _b;
	cref = ref;
	std::streamsize oldPrecision = std::cout.precision();
	std::cout << std::setprecision(nbits - 2);
	std::cout << std::setw(nbits) << _a << " + " << std::setw(nbits) << _b << " = " << std::setw(nbits) << ref << std::endl;
	std::cout << a << " + " << b << " = " << result << " (reference: " << cref << ")   " ;
	std::cout << (cref == result ? "PASS" : "FAIL") << std::endl << std::endl;
	std::cout << std::dec << std::setprecision(oldPrecision);
}

// sign of 0 is flipped on MSVC Release builds
void CompilerBug() {
	using namespace std;
	using namespace sw::universal;
	{
		areal<5, 1> a;
		a.set_raw_bits(0x0);
		cout << "areal<5,1> : " << to_binary(a) << " : " << a << endl;
		float f = float(a);
		cout << "float      : " << f << endl;
		double d = double(a);
		cout << "double     : " << d << endl;
	}
	{
		areal<5, 1> a;
		a.set_raw_bits(0x10);
		cout << "areal<5,1> : " << to_binary(a) << " : " << a << endl;
		float f = float(a);
		cout << "float      : " << f << endl;
		double d = double(a);
		cout << "double     : " << d << endl;
	}

	{
		areal<6, 1> a;
		a.set_raw_bits(0x0);
		cout << "areal<6,1> : " << to_binary(a) << " : " << a << endl;
		float f = float(a);
		cout << "float      : " << f << endl;
		double d = double(a);
		cout << "double     : " << d << endl;
	}
	{
		areal<6, 1> a;
		a.set_raw_bits(0x20);
		cout << "areal<6,1> : " << to_binary(a) << " : " << a << endl;
		float f = float(a);
		cout << "float      : " << f << endl;
		double d = double(a);
		cout << "double     : " << d << endl;
	}
}

namespace sw::universal {

	template<typename TestType>
	void ReportIntervalConversionError(const std::string& test_case, const std::string& op, double input, const TestType& reference, const TestType& result) {
		constexpr size_t nbits = result.nbits;  // number system concept requires a static member indicating its size in bits
		auto old_precision = std::cerr.precision();
		std::cerr << test_case
			<< " " << op << " "
			<< std::setw(NUMBER_COLUMN_WIDTH) << input
			<< " did not convert to "
			<< std::setw(NUMBER_COLUMN_WIDTH) << reference << " instead it yielded  "
			<< std::setw(NUMBER_COLUMN_WIDTH) << result
			<< "  raw " << to_binary(result)
			<< std::setprecision(old_precision)
			<< std::endl;
	}

	template<typename TestType>
	void ReportIntervalConversionSuccess(const std::string& test_case, const std::string& op, double input, const TestType& reference, const TestType& result) {
		constexpr size_t nbits = result.nbits;  // number system concept requires a static member indicating its size in bits
		std::cerr << test_case
			<< " " << op << " "
			<< std::setw(NUMBER_COLUMN_WIDTH) << input
			<< " success            "
			<< std::setw(NUMBER_COLUMN_WIDTH) << result << " golden reference is "
			<< std::setw(NUMBER_COLUMN_WIDTH) << reference
			<< "  raw " << std::setw(nbits) << to_binary(result)
			<< std::endl;
	}

	template<typename TestType>
	int Compare(double input, const TestType& testValue, const TestType& reference, bool bReportIndividualTestCases) {
		int fail = 0;
		if (testValue != reference) {
			fail++;
			if (bReportIndividualTestCases)	ReportIntervalConversionError("FAIL", "=", input, reference, testValue);
		}
		else {
			//if (bReportIndividualTestCases) ReportIntervalConversionSuccess("PASS", "=", input, reference, testValue);
		}
		return fail;
	}

	/// <summary>
	/// enumerate all conversion cases for a number system with ubits
	/// </summary>
	/// <typeparam name="TestType">the test configuration</typeparam>
	/// <typeparam name="RefType">the reference configuration</typeparam>
	/// <param name="tag">string to indicate what is being tested</param>
	/// <param name="bReportIndividualTestCases">if true print results of each test case. Default is false.</param>
	/// <returns>number of failed test cases</returns>
	template<typename TestType>
	int VerifyArealIntervalConversion(const std::string& tag, bool bReportIndividualTestCases) {
		// areal<> is organized as a set of exact samples followed by an interval to the next exact value
		//
		// vprev    exact value          ######-0     ubit = false     some value [vprev,vprev]
		//          interval value       ######-1     ubit = true      (vprev, v)
		// v        exact value          ######-0     ubit = false     some value [v,v]
		//          interval value       ######-1     ubit = true      (v, vnext)
		// vnext    exact value          ######-0     ubit = false     some value [vnext,vnext]
		//          interval value       ######-1     ubit = true      (vnext, vnextnext)
		//
		// the assignment test can thus be constructed by enumerating the exact values of a configuration
		// and taking a -diff to obtain the interval value of vprev, 
		// and taking a +diff to obtain the interval value of v
		constexpr size_t nbits = TestType::nbits;
		constexpr size_t NR_TEST_CASES = (size_t(1) << nbits);

		const unsigned max = nbits > 20 ? 20 : nbits + 1;
		size_t max_tests = (size_t(1) << max);
		if (max_tests < NR_TEST_CASES) {
			std::cout << "VerifyArealIntervalConversion " << typeid(TestType).name() << ": NR_TEST_CASES = " << NR_TEST_CASES << " clipped by " << max_tests << std::endl;
		}

		// execute the test
		int nrOfFailedTests = 0;
		TestType positive_minimum;
		double dminpos = double(minpos(positive_minimum));
		TestType negative_maximum;
		double dmaxneg = double(maxneg(negative_maximum));

		// NUT: number under test
		TestType nut;

		for (size_t i = 0; i < NR_TEST_CASES && i < max_tests; i += 2) {
			TestType current, interval;
			double testValue{ 0.0 };
			current.set_raw_bits(i);
			interval.set_raw_bits(i + 1);  // sets the ubit
			double da = double(current);
//			std::cout << "current : " << to_binary(current) << " : " << current << std::endl;
//			std::cout << "interval: " << to_binary(interval) << " : " << interval << std::endl;
			// da - delta = (prev, current)
			// da         = [current, current]
			// da + delta = (current, next)

			if (current.iszero()) {
				double delta = dminpos / 4.0;  // the test value between 0 and minpos
				if (current.sign()) {
					// da         = [-0]
					testValue = da;
					nut = testValue;
					nrOfFailedTests += Compare(testValue, nut, current, bReportIndividualTestCases);
					// da - delta = (-0,-minpos)
					testValue = da - delta;
					nut = testValue;
					nrOfFailedTests += Compare(testValue, nut, interval, bReportIndividualTestCases);
				}
				else {
					// da         = [0]
					testValue = da;
					nut = testValue;
					nrOfFailedTests += Compare(testValue, nut, current, bReportIndividualTestCases);
					// da + delta = (0,minpos)
					testValue = da + delta;
					nut = testValue;
					nrOfFailedTests += Compare(testValue, nut, interval, bReportIndividualTestCases);
				}
			}
			else if (current.isinf(INF_TYPE_NEGATIVE)) {

			}
			else if (current.isinf(INF_TYPE_POSITIVE)) {

			}
			else if (current.isinf(NAN_TYPE_SIGNALLING)) {  // sign is true

			}
			else if (current.isinf(NAN_TYPE_QUIET)) {       // sign is false

			}
			else {
				TestType previous, previousInterval;
				previous.set_raw_bits(i - 2);
				previousInterval.set_raw_bits(i - 1);
//				std::cout << "previous: " << to_binary(previous) << " : " << previous << std::endl;
//				std::cout << "interval: " << to_binary(previousInterval) << " : " << previousInterval << std::endl;
				double delta = (da - double(previous)) / 2.0;  // NOTE: the sign will flip the relationship between the enumeration and the values
//				std::cout << "delta   : " << delta << std::endl;
															   // da - delta = (prev,current) == previous + ubit
				testValue = da - delta;
				nut = testValue;
				nrOfFailedTests += Compare(testValue, nut, previousInterval, bReportIndividualTestCases);
				// da         = [v]
				testValue = da;
				nut = testValue;
				nrOfFailedTests += Compare(testValue, nut, current, bReportIndividualTestCases);
				// da + delta = (v+,next) == current + ubit
				testValue = da + delta;
				nut = testValue;
				nrOfFailedTests += Compare(testValue, nut, interval, bReportIndividualTestCases);
			}
		}
		return nrOfFailedTests;
	}

} // namespace sw::universal

// conditional compile flags
#define MANUAL_TESTING 1
#define STRESS_TESTING 0

int main(int argc, char** argv)
try {
	using namespace std;
	using namespace sw::universal;

	int nrOfFailedTestCases = 0;

	std::string tag = "conversion: ";

#if MANUAL_TESTING

	bool bReportIndividualTestCases = true;

	// areal<> is organized as a set of exact samples and an interval to the next exact value
	//
	// vprev    exact value          ######-0     ubit = false     some value [vprev,vprev]
	//          interval value       ######-1     ubit = true      (vprev, v)
	// v        exact value          ######-0     ubit = false     some value [v,v]
	//          interval value       ######-1     ubit = true      (v, vnext)
	// vnext    exact value          ######-0     ubit = false     some value [vnext,vnext]
	//          interval value       ######-1     ubit = true      (vnext, vnextnext)
	//
	// the assignment test can thus be constructed by enumerating the exact values
	// and taking a -diff to obtain the interval value of vprev, 
	// and taking a +diff to obtain the interval value of v

//	GenerateArealTable<4, 1>(cout, true);
	/*
FAIL =               0.0625 did not convert to                    [0.0625] instead it yielded                     (0.0625, 0.125)  raw b00000011
FAIL =               0.1875 did not convert to                    [0.1875] instead it yielded                     (0.1875, 0.25)  raw b00000111
FAIL =               0.3125 did not convert to                    [0.3125] instead it yielded                     (0.3125, 0.375)  raw b00001011
FAIL =               0.4375 did not convert to                    [0.4375] instead it yielded                     (0.4375, 0.5)  raw b00001111
FAIL =               0.5625 did not convert to                    [0.5625] instead it yielded                     (0.5625, 0.625)  raw b00010011
FAIL =               0.6875 did not convert to                    [0.6875] instead it yielded                     (0.6875, 0.75)  raw b00010111
FAIL =               0.8125 did not convert to                    [0.8125] instead it yielded                     (0.8125, 0.875)  raw b00011011
FAIL =               0.9375 did not convert to                    [0.9375] instead it yielded                     (0.9375, 1)  raw b00011111
FAIL =              -0.0625 did not convert to                    [-0.0625] instead it yielded                     (-0.0625, -0.125)  raw b10000011
FAIL =              -0.1875 did not convert to                    [-0.1875] instead it yielded                     (-0.1875, -0.25)  raw b10000111
FAIL =              -0.3125 did not convert to                    [-0.3125] instead it yielded                     (-0.3125, -0.375)  raw b10001011
FAIL =              -0.4375 did not convert to                    [-0.4375] instead it yielded                     (-0.4375, -0.5)  raw b10001111
FAIL =              -0.5625 did not convert to                    [-0.5625] instead it yielded                     (-0.5625, -0.625)  raw b10010011
FAIL =              -0.6875 did not convert to                    [-0.6875] instead it yielded                     (-0.6875, -0.75)  raw b10010111
FAIL =              -0.8125 did not convert to                    [-0.8125] instead it yielded                     (-0.8125, -0.875)  raw b10011011
FAIL =              -0.9375 did not convert to                    [-0.9375] instead it yielded                     (-0.9375, -1)  raw b10011111
conversion:  areal<8,2> FAIL 16 failed test cases
*/
	areal<8,2> a;
	a = 0.0624; cout << a << endl;
	a = 0.0625; cout << a << endl;
	a = 0.1; cout << a << endl;
	a = 0.1875; cout << a << endl;
	a = 0.3125; cout << a << endl;
	a = 0.4375; cout << a << endl;
	a = 0.5625; cout << a << endl;
	a = 0.6875; cout << a << endl;
	a = 0.8125; cout << a << endl;
	a = 0.9375; cout << a << endl;


	nrOfFailedTestCases = ReportTestResult(VerifyArealIntervalConversion< areal<4, 1> >(tag, bReportIndividualTestCases), tag, "areal<4,1>");
	nrOfFailedTestCases = ReportTestResult(VerifyArealIntervalConversion< areal<5, 1> >(tag, bReportIndividualTestCases), tag, "areal<5,1>");
	nrOfFailedTestCases = ReportTestResult(VerifyArealIntervalConversion< areal<6, 1> >(tag, bReportIndividualTestCases), tag, "areal<6,1>");
	nrOfFailedTestCases = ReportTestResult(VerifyArealIntervalConversion< areal<7, 1> >(tag, bReportIndividualTestCases), tag, "areal<7,1>");
	nrOfFailedTestCases = ReportTestResult(VerifyArealIntervalConversion< areal<7, 2> >(tag, bReportIndividualTestCases), tag, "areal<7,2>");

	nrOfFailedTestCases = ReportTestResult(VerifyArealIntervalConversion< areal<8, 1> >(tag, bReportIndividualTestCases), tag, "areal<8,1>");
	nrOfFailedTestCases = ReportTestResult(VerifyArealIntervalConversion< areal<8, 2> >(tag, bReportIndividualTestCases), tag, "areal<8,2>");
	nrOfFailedTestCases = ReportTestResult(VerifyArealIntervalConversion< areal<8, 3> >(tag, bReportIndividualTestCases), tag, "areal<8,3>");
	nrOfFailedTestCases = ReportTestResult(VerifyArealIntervalConversion< areal<8, 4> >(tag, bReportIndividualTestCases), tag, "areal<8,4>");



//	nrOfFailedTestCases = ReportTestResult(VerifyArealIntervalConversion< areal<12, 3> >(tag, bReportIndividualTestCases), tag, "areal<12,3>");

	cout << "failed tests: " << nrOfFailedTestCases << endl;

#if STRESS_TESTING

	// manual exhaustive test

#endif

#else  // !MANUAL_TESTING

	cout << "AREAL conversion validation" << endl;

//	bool bReportIndividualTestCases = false;
//	nrOfFailedTestCases = ReportTestResult(VerifyConversion<areal<4, 1, uint8_t>, areal<5, 1, uint8_t>>(tag, bReportIndividualTestCases), tag, "areal<4,1,uint8_t>");

//	nrOfFailedTestCases = ReportTestResult(VerifyConversion< areal<8, 2, uint8_t>, areal<9, 2, uint8_t>>(tag, bReportIndividualTestCases), tag, "areal<8,2,uint8_t>");
//	nrOfFailedTestCases = ReportTestResult(VerifyConversion< areal<8, 3, uint8_t>, areal<9, 4, uint8_t>>(tag, bReportIndividualTestCases), tag, "areal<8,3,uint8_t>");

//	nrOfFailedTestCases = ReportTestResult(VerifyConversion< areal<12, 2, uint8_t>, areal<13, 2, uint8_t>>(tag, bReportIndividualTestCases), tag, "areal<12,2,uint8_t>");
//	nrOfFailedTestCases = ReportTestResult(VerifyConversion< areal<12, 3, uint8_t>, areal<13, 3, uint8_t>>(tag, bReportIndividualTestCases), tag, "areal<12,3,uint8_t>");
//	nrOfFailedTestCases = ReportTestResult(VerifyConversion< areal<12, 4, uint8_t>, areal<13, 4, uint8_t>>(tag, bReportIndividualTestCases), tag, "areal<12,4,uint8_t>");

//	nrOfFailedTestCases = ReportTestResult(VerifyConversion< areal<16, 3, uint8_t>, areal<17, 3, uint8_t>>(tag, bReportIndividualTestCases), tag, "areal<16,3,uint16_t>");
//	nrOfFailedTestCases = ReportTestResult(VerifyConversion< areal<16, 4, uint8_t>, areal<17, 4, uint8_t>>(tag, bReportIndividualTestCases), tag, "areal<16,4,uint16_t>");
//	nrOfFailedTestCases = ReportTestResult(VerifyConversion< areal<16, 8, uint8_t>, areal<17, 8, uint8_t>>(tag, bReportIndividualTestCases), tag, "areal<16,8,uint16_t>");

#if STRESS_TESTING

#endif  // STRESS_TESTING

#endif  // MANUAL_TESTING

	return (nrOfFailedTestCases > 0 ? EXIT_FAILURE : EXIT_SUCCESS);
}
catch (char const* msg) {
	std::cerr << msg << std::endl;
	return EXIT_FAILURE;
}
catch (const sw::universal::areal_arithmetic_exception& err) {
	std::cerr << "Uncaught areal arithmetic exception: " << err.what() << std::endl;
	return EXIT_FAILURE;
}
catch (const sw::universal::areal_internal_exception& err) {
	std::cerr << "Uncaught areal internal exception: " << err.what() << std::endl;
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
