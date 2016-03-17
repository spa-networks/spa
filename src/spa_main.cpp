/*****************************************************************
 Author: J.-G. Young, <jean.gabriel.young@gmail.com>
****************************************************************** 
 This program creates a network using the complete structural 
 preferential attachment model described in

  A) Hébert-Dufresne et al., Phys.  Rev. Lett. 107, 1, (2011).
  B) Hébert-Dufresne et al., Phys. Rev. E 85, 026108, (2012).
  C) Young et al., Phys. Rev. E (2016).

****************************************************************** 
 Usage:
****************************************************************** 
  Compilation: 
    g++ -o3 -W -Wall -Wextra -pedantic -std=c++0x spa_main.cpp spa_network.cpp -lboost_program_options -o SPA 

  Call:
    Run ./SPA -h to see the list of parameters.	

*****************************************************************/

// Configuration file
#include "config.h"
// C libraries
#include <stdlib.h> // C error codes
// STL
#include <string>   // const char* to std::string()
#include <chrono>   // c++11, benchmarking
// boost
#if HAVE_LIBBOOST_PROGRAM_OPTIONS == 1
  #include <boost/program_options.hpp>
#endif
// Project files
#include "spa_network.hpp" // spa_network_t
#include "interface.hpp"  // boost_interface(3) and barbone_interface(3)
#include "output_functions.hpp"  // print_outputs(2)
#include "spa_algorithm.hpp" // spa_star(5) and spa_basic(4)
// Typdef and helpers
#include "types.hpp" // spa_parameters_t
#include "errors.hpp" // detailed error code 
#include "includes/logger.hpp" // LOGGER macros

int main(int argc, char const *argv[]) {
  
  LOGGER_START("")

  // Interface (parse argv, handle parameter errors, help message, etc.)

  spa_parameters_t prog_params;

  #if HAVE_LIBBOOST_PROGRAM_OPTIONS == 1
    int interface_return = boost_interface(argc, argv, prog_params);
  #else
    int interface_return = barebone_interface(argc, argv, prog_params);
  #endif

  switch(interface_return) {
    case EXIT_FAILURE :
      LOGGER_WRITE(Logger::DEBUG, "Exiting with interface error: invalid invocation of "+std::string(argv[0])+",")
      LOGGER_STOP()
      return EXIT_FAILURE;

    case FILE_ERROR :
      LOGGER_WRITE(Logger::ERROR, "Exiting with interface error: could not open the configuration file.")
      LOGGER_STOP()
      return EXIT_FAILURE;

    case ELEM_MISSING_ERROR :
      LOGGER_WRITE(Logger::ERROR, "Exiting with interface error: missing parameters.")
      LOGGER_STOP()
      return EXIT_FAILURE;

    case LOGIC_ERROR :
      LOGGER_WRITE(Logger::ERROR, "Exiting with interface error: parameter error.")
      LOGGER_STOP()
      return EXIT_FAILURE;

    case STOP_WITH_SUCCESS :
      LOGGER_WRITE(Logger::DEBUG, "Exiting with interface success: successfully parsed input (help message or template produced).")
      LOGGER_STOP()
      return EXIT_SUCCESS;

    default: break;
  }

  // Object declarations
  #if HAVE_STEADY_CLOCK == 1
    const auto simulation_begins = std::chrono::steady_clock::now();
  #else 
    const auto simulation_begins = std::chrono::monotonic_clock::now();
  #endif

  LOGGER_WRITE(Logger::DEBUG, "Instantiation of the spa_network_t class.")
  spa_network_t network(prog_params.final_size.value, prog_params.basic_size.value);

  LOGGER_WRITE(Logger::DEBUG, "Instantiation of the random number generator engine "+VAR_PRINT(prog_params.seed.value)+".")
  std::mt19937 engine(prog_params.seed.value);

  // Setup and simulation
  if (prog_params.edge_list.initialized || prog_params.degrees.initialized || prog_params.internal_degrees.initialized || prog_params.projected_internal_degrees.initialized) {
    LOGGER_WRITE(Logger::INFO, "Connectivity information requested: complete simulation mode.")
    network.initialize(prog_params.initial_size.value);
    spa_star(prog_params.p.value, prog_params.q.value, prog_params.r.value, network, engine);
  }
  else if (prog_params.connected_edge_list.initialized || prog_params.connected_degrees.initialized) {
    LOGGER_WRITE(Logger::INFO, "Legacy connectivity information requested: legacy simulation mode.")
    network.set_handle_links_status(false);
    network.initialize(prog_params.initial_size.value);
    spa_basic(prog_params.p.value, prog_params.q.value, network, engine);
  }
  else {
    LOGGER_WRITE(Logger::INFO, "No connectivity information requested: legacy simulation mode.")
    network.set_handle_links_status(false);
    network.initialize(prog_params.initial_size.value);
    spa_basic(prog_params.p.value, prog_params.q.value, network, engine);
  }
  LOGGER_WRITE(Logger::INFO, "Simulation completed.")

  // Timer 
  #if HAVE_STEADY_CLOCK == 1
    const auto simulation_ends = std::chrono::steady_clock::now();
  #else 
    const auto simulation_ends = std::chrono::monotonic_clock::now();
  #endif
  double simulation_duration = std::chrono::duration_cast<std::chrono::milliseconds>(simulation_ends-simulation_begins).count()/1000.0;

  // Outputs
  LOGGER_WRITE(Logger::INFO, "Processing outputs.")
  process_and_output(network, prog_params, simulation_duration); // this alters the network.

  LOGGER_WRITE(Logger::INFO, "Exiting without error.")
  LOGGER_STOP()
	  
  return EXIT_SUCCESS;
}