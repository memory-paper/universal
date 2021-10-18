// quadratic.cpp: demonstration of catastrophic cancellation in the quadratic formula
//
// Copyright (C) 2017-2021 Stillwater Supercomputing, Inc.
//
// This file is part of the universal numbers project, which is released under an MIT Open Source license.
#include <limits>
#include <cmath>   // lerp and midpoint
#if (__cplusplus == 202003L) || (_MSVC_LANG == 202003L)
#include <numbers>    // high-precision numbers
#endif

#include <universal/number/integer/integer.hpp>
#include <universal/number/fixpnt/fixpnt.hpp>
#include <universal/number/posit/posit.hpp>

#include <cstddef>
#include <stdexcept>
#include <cstring>
#include <ostream>

template<typename Real>
void CompareTerms(Real a, Real b, Real c) {
    std::cout << "a                    : " << sw::universal::to_binary(a) << " : " << a << '\n';
    std::cout << "b                    : " << sw::universal::to_binary(b) << " : " << b << '\n';
    std::cout << "c                    : " << sw::universal::to_binary(c) << " : " << c << '\n';
	Real b_square = b * b;
	Real fourac = 4 * a * c;
	Real difference = b_square - fourac;
	Real sqrt_b_square_minus_fourac = sqrt(difference);
    std::cout << "b^2                  : " << sw::universal::to_binary(b_square) << " : " << (b_square) << '\n';
    std::cout << "4ac                  : " << sw::universal::to_binary(fourac) << " : " << (fourac) << '\n';
	std::cout << "    (b^2 - 4ac)      : " << sw::universal::to_binary(difference) << " : " << difference << '\n';
	std::cout << "sqrt(b^2 - 4ac)      : " << sw::universal::to_binary(sqrt_b_square_minus_fourac) << " : " << sqrt_b_square_minus_fourac << '\n';
	Real numerator = -b + sqrt_b_square_minus_fourac;
	Real denominator = 2 * a;
	std::cout << "-b + sqrt(b^2 - 4ac) : " << sw::universal::to_binary(numerator) << " : " << numerator << '\n';
	std::cout << "2a                   : " << sw::universal::to_binary(denominator) << " : " << denominator << '\n';
	Real root = numerator / denominator;
	std::cout << "root                 : " << sw::universal::to_binary(root) << " : " << root << '\n';
}

int main(int argc, char** argv)
try {
	using namespace sw::universal;

	std::cout << "catastrophic cancellation in the quadratic formula\n";

	auto precision = std::cout.precision();
	std::cout << std::setprecision(15);

	using Float32 = float;
	using Float64 = double;
	using Posit32 = sw::universal::posit<32, 2>;
	using Posit64 = sw::universal::posit<64, 2>;

	float a = 1.0f;
	float b = 1.0e4f;
	float c = 1.0f;

	std::cout << "single precision floating-point\n";
	CompareTerms<Float32>(a, b, c);
	std::cout << '\n';

	std::cout << "double precision floating-point\n";
	CompareTerms<Float64>(a, b, c);
	std::cout << '\n';

	std::cout << "single precision posit<32, 2>\n";
	CompareTerms<Posit32>(a, b, c);
	std::cout << '\n';

	std::cout << "custom precision posit<40, 2>\n";
	CompareTerms<posit<40,2>>(a, b, c);
	std::cout << '\n';

	std::cout << "double precision posit<64, 2>\n";
	CompareTerms<Posit64>(a, b, c);
	std::cout << '\n';

	integer<64> inta(a);
	integer<64> intb(b);
	integer<64> intc(c);
	integer<64> difference = intb * intb - 4 * inta * intc;
	std::cout << "    (b^2 - 4ac)      : " << sw::universal::to_binary(difference) << " : " << difference << '\n';;

	 std::cout << std::setprecision(precision);
	 std::cout << std::endl;
	
	return EXIT_SUCCESS;
}
catch (char const* msg) {
	std::cerr << "Caught exception: " << msg << std::endl;
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
