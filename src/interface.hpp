#ifndef INTERFACE_HPP
#define INTERFACE_HPP

// Configuration file
#include "config.h"
// C libraries
#include <stdlib.h> // C error codes
// STL
#include <iostream> // std::cout, std::clog and config_file handling
#include <sstream>  // config_file handling
#include <fstream>  // config_file handling
#include <string> // config_file handling
// Project files
#include "spa_network.hpp" // spa_network_t
// Typdef and helpers
#include "types.hpp" // spa_parameters_t, id_t, edge_t
#include "errors.hpp" // detailed error code 
#include "includes/logger.hpp"  // LOGGER macros
#include "includes/bofstream.hpp" // buffered output stream


#if HAVE_LIBBOOST_PROGRAM_OPTIONS == 1
#include <boost/program_options.hpp>
#define CHECK_OPTION(VAR) if (var_map.count(#VAR)>0) {param.VAR.initialized = true;}
/** @name boost_interface
  * Load parameters from program arguments using libboost_program_options.
  * @param[in] <argc> First argument of int main(int argc, char const *argv[]).
  * @parem[in] <argv> Second argument of int main((int argc, char const *argv[])).
  * @param[out] <param> Parsed parameters. 
  */
int boost_interface(int argc, const char *argv[], spa_parameters_t & param);

#endif //HAVE_LIBBOOST_PROGRAM_OPTIONS

/** @name barebone_interface
  * Load parameters from program arguments without any special library.
  * @param[in] <argc> First argument of int main(int argc, char const *argv[]).
  * @parem[in] <argv> Second argument of int main((int argc, char const *argv[])).
  * @param[out] <param> Parsed parameters. 
  */
int barebone_interface(int argc, const char *argv[], spa_parameters_t & param);
/** @name Interface utilities */
//@{
/** @name parse_config_file
  * Load parameters from a standard configuration file.
  * Return value passed by function argument.
  * @warning This method should handle commenting, indentation,
  *          permutation and omission just fine. However, since 
  *          the parser is very rudimentary, avoid tampering 
  *          with the template as much as possible!
  */
int parse_config_file(std::string config_file_path, spa_parameters_t & param);
/// Generate conform configuration file template in stdout.
void generate_template();
/// Perform basic logical test on parameters
int test_parameters(const spa_parameters_t & param);
/// Informative output to the stdlog
void log_parameters(const spa_parameters_t & param);
//@}

// These macro greatly reduces the amount of redundant code and facilitate
// further additions of program options. 
#define PARSE_OPTION(VAR)\
  if (token.compare(#VAR)==0) {\
    line >> char_buffer; line >> param.VAR.value;\
    param.VAR.initialized=true;}

#define PARSE_OPTION_STR(VAR)\
  if (token.compare(#VAR)==0) {\
      line >> char_buffer; line >> char_buffer; line >> value;\
      if (char_buffer!='"') {\
          if (char_buffer!='=') {\
            param.VAR.value = char_buffer+value.substr(0,value.find_last_of('\"')-1);\
            param.VAR.initialized = true;\
          }\
      }\
      else {\
        param.VAR.value = value.substr(0,value.find_last_of("\""));\
        param.VAR.initialized = true;\
      }}

#define PARSE_OPTION_BOOL(VAR)\
  if (token.compare(#VAR)==0) {\
    line >> char_buffer; line >> value;\
    if (value.compare("true")==0)  {param.VAR.value=true;  param.VAR.initialized=true;}\
    if (value.compare("false")==0) {param.VAR.value=false; param.VAR.initialized=true;}\
  }

#endif //INTERFACE_HPP