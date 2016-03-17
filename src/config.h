// Default configs

// boost::program_options: Set to 0 if libboostprogram options is not present
#define HAVE_LIBBOOST_PROGRAM_OPTIONS 1

// std::steady_lock: Set to 0 if using an early version of C++11.
#define HAVE_STEADY_CLOCK 1

// Logging level: goes from 0 to 3 (verbose to no output). Outputs to std::clog.
//  0: DEBUG (very verbose output)
//  1: INFO (output a lof ot state information along the way)
//  2: CONFIG (only log the parameters)
//  3: NONE (no logs)
#define LOGGER_LEVEL 2

// Reduce memory footprint at the cost of speed. Set to 1 to enable the tradeoff.
#define MEMORY_OPTIMIZED 0

// Compile assertations out. Should improve performance when branch prediction is poor.
#define REMOVE_ASSERTS 1

// Do not modify.
#if REMOVE_ASSERTS == 1
    #define NDEBUG 1
#endif
