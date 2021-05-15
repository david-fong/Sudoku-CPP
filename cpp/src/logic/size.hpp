#ifndef HPP_SUDOKU_SIZE
#define HPP_SUDOKU_SIZE
// This file is a helper for `./solver.hpp`.

#include "../buildflag.hpp"
#include "./enum.hpp"

namespace Sudoku {
	/**
	 * The primary goal of this added complexity is to make effective
	 * use of space, which should help improve cache performance.
	 *
	 * As I explored my implementation options, I set these goals:
	 * 1. Grid order template parameter is bounded within reason.
	 * 2. Caller code can choose which template expansions to compile.
	 * 3. Conditional sizing typedefs are kept internal.
	 *
	 * Constructs I could have used in less favourable implementations
	 * included: partial template specification, alias templating,
	 * using an enum type for valid grid orders.
	 *
	 * Note that there is another similar typedef named `order_t`,
	 * which is conditionally defined to be large enough for whatever
	 * value is passed as a template parameter of this type (Order)
	 * as a maximum value.
	 */
	typedef uint8_t Order;
	constexpr Order MAX_REASONABLE_ORDER = 10u;

	/**
	 * Note: when printing, make sure to cast uint8_t to int.
	 */
	template <Order O>
	class Size {
	public:
		// mask width `order^2` bits.
		// order:  2   3   4   5   6   7   8   9  10  11
		// width:  4   9  16  25  36  49  64  81 100 121
		// round:  8  16  16  32  64  64  64 128 128 128
		typedef
			typename std::conditional_t<(O < 3), std::uint_fast8_t,
			typename std::conditional_t<(O < 5), std::uint_fast16_t,
			typename std::conditional_t<(O < 6), std::uint_fast32_t,
			typename std::conditional_t<(O < 9), std::uint_fast64_t,
			unsigned long long
		>>>> occmask_t;

		// uint range [0, order].
		// not used in any critical code, so it doesn't need to be fast type.
		typedef std::uint8_t order_t;

		// uint range [0, order^2].
		// order:   2    3    4    5    6    7    8    9   10   11   12   13   14   15   16
		// length:  4    9   16   25   36   49   64   81  100  121  144  169  196  225  256
		// bits:    3    3    5    5    6    6    7    7    7    7    8    8    8    8    9
		typedef
			typename std::conditional_t<(O < 16), std::uint_fast8_t,
			std::uint_fast16_t
		> length_t;

		// uint range [0, order^4].
		// order:   2    3    4    5     6     7     8     9     10
		// area:   16   81  256  625  1296  2401  4096  6561  10000
		// bits:    5    7    9    9    11    12    17    17     18
		typedef
			typename std::conditional_t<(O <   4), std::uint8_t,
			typename std::conditional_t<(O <   8), std::uint16_t,
			typename std::conditional_t<(O < 256), std::uint32_t,
			std::uint64_t
		>>> area_t;

		// uint range [0, order^2].
		typedef length_t value_t;
	};
} // namespace Sudoku

namespace Sudoku::Solver {

	// Container for a very large number.
	// See Solver::GIVEUP_THRESHOLD for more discussion on the average
	// number of operations taken to generate a solution by grid-order.
	typedef unsigned long long opcount_t;

	/**
	 * Note: when printing, make sure to cast uint8_t to int.
	 */
	template <Order O>
	class SolverSize {
	public:
		// Note that this should always be smaller than opcount_t.
		typedef
			// Make sure this can fit `GIVEUP_THRESHOLD<BACKTRACKS>`.
			//typename std::conditional_t<(O < 4), std::uint_fast8_t,
			typename std::conditional_t<(O < 5), std::uint_fast16_t,
			typename std::conditional_t<(O < 6), std::uint_fast32_t,
			std::uint64_t
		>> backtrack_t;

		/**
		 * Give up if the giveup condition variable meets this value.
		 * Measured stats for operations: https://www.desmos.com/calculator/8taqzelils
		 */
		static constexpr opcount_t GIVEUP_THRESHOLD = ((const opcount_t[]){
			// Note: Make sure entries of `backtrackCounts` can fit these.
			0,  1,  3,  150,  1'125,  560'000,  1'000'000'000,
		})[O];
		// TODO [tune] The current values for order-6 are just predictions.
	};

} // namespace Sudoku::Solver

#endif