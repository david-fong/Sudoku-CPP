
/**
 * Choose build flags for the Repl and Solver here.
 */
#include "./buildflag.hpp"

#include "./sudoku/solver/solver.cpp"
#include "./sudoku/repl.cpp"

#include <iostream>     // cout,
#include <fstream>      // ofstream,
#include <random>       // random_device,

using Sudoku::Repl::Repl;

/**
 * ARGUMENTS
 * 0. executable name (fixed).
 * 1. grid order (default: 4).
 * 2. scramble random key (default: time).
 * 3. output file name.
 */
int main(const int argc, char const *const argv[]) {
    // My implementation specifies this as safe:
    std::ios_base::sync_with_stdio(false);

    unsigned int    userOrder;      // 1
    unsigned int    srandKey;       // 2
    std::string     outFileName;    // 3
    std::ostream*   os;             // 3

    userOrder = (argc > 1) ? std::stoi(argv[1]) : DEFAULT_ORDER;
    if (argc > 2 && !std::string(argv[2]).empty()) {
        srandKey = std::stoi(argv[2]);
    } else {
        srandKey = std::random_device()();
    }
    if (argc > 3) {
        // TODO [feat] Handle if file already exists: prompt user for whether to overwrite.
        outFileName = argv[3];
        os = new std::ofstream(outFileName);
    } else {
        outFileName = "std::cout";
        os = &std::cout;
    }

    std::cout << "\nPARSED ARGUMENTS:"
    << "\n- ARG 1 [[ grid order  ]] : " << userOrder
    << "\n- ARG 2 [[ srand key   ]] : " << srandKey
    << "\n- ARG 3 [[ output file ]] : " << outFileName
    << std::endl;

    // Scramble the random number generator (std::rand is no longer used):
    Sudoku::Solver::VALUE_RNG.seed(srandKey);

    // Create a Solver of the specified order:
    // (It will automatically enter its REPL).
    switch (static_cast<Sudoku::Order>(userOrder)) {
        case 3: { Repl<3> s(*os); break; }
        case 4: { Repl<4> s(*os); break; }
        case 5: { Repl<5> s(*os); break; }
        //case 6: { Repl<6> s(*os); break; }
        default:
            std::cout << Ansi::RED.ON << "\nILLEGAL ARGUMENT:\n  order must be one of: { ";
            for (unsigned i = 3; i <= 5; i++) {
                std::cout << i << ", ";
            }
            std::cout << "}" << Ansi::RED.OFF << std::endl;
            break;
    }

    // End of program:
    std::cout << "\nbye bye!\n" << std::endl;
    return 0;
}
