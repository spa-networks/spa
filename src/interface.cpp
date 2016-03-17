#include "interface.hpp"

#if HAVE_LIBBOOST_PROGRAM_OPTIONS == 1

namespace po = boost::program_options;

int boost_interface(int argc, const char *argv[], spa_parameters_t & param) {
  // Boost program option CLI options
  std::string config_file;
  po::options_description description("Options");
  description.add_options()
  //Mandatory~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ("p,p",po::value<double>(&param.p.value), "Community birth probability. The complementary probability (1-p) is the community growth probability.")
  ("q,q",po::value<double>(&param.q.value),"Probability that a node is a new one, for growth and birth events.")
  ("r,r",po::value<double>(&param.r.value),"Ratio of the internal link creation and recruiting rates.\n\n")
  ("final_size,N",po::value<unsigned int>(&param.final_size.value),"Final size of the network, in nodes).")
  ("seed,d",po::value<unsigned int>(&param.seed.value),"Seed of the pseudo random number generator (Mersenne-twister 19937).\n\n")
  //Output files~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ("edge_list,E",po::value<std::string>(&param.edge_list.value),"Path of the edge list file (output).\n If set to \"\", redirect the output to stdout.\n Each line contains the identifiers of 2 nodes that are related by an undirected, unweighted link.")
  ("connected_edge_list,Z",po::value<std::string>(&param.connected_edge_list.value),"Path of the connected edge list file (output).\n If set to \"\", redirect the output to stdout.\n Each line contains the identifiers of 2 nodes that are related by an undirected, unweighted link, assuming that each cluster is fully connected.")
  ("cluster_contents,C",po::value<std::string>(&param.cluster_contents.value),"Path of the cluster contents file (output).\n If set to \"\", redirect the output to stdout.\n Each line corresponds to a cluster (starting from cluster 0 on the first line), and contains the identifiers of the nodes that belong to that cluster.")
  ("node_participations,P",po::value<std::string>(&param.node_participations.value),"Path of the node participations file (output).\n If set to \"\", redirect the output to stdout.\n Each line corresponds to a node (starting from node 0 on the first line), and contains the identifiers of the clusters to which this node belongs.\n\n")
  ("cluster_sizes,S",po::value<std::string>(&param.cluster_sizes.value),"Path of the community sizes file (output).\n If set to \"\", redirect the output to stdout.\n Each line corresponds to a cluster (starting from cluster 0 on the first line), and contains a size, calculated in nodes.")
  ("node_memberships,M",po::value<std::string>(&param.node_memberships.value),"Path of the node memberships file (output).\n If set to \"\", redirect the output to stdout.\n Each line corresponds to a node (starting from node 0 on the first line), and contains a membership number, calculated in clusters.")
  ("degrees,K",po::value<std::string>(&param.degrees.value),"Path of the degrees file (output).\n If set to \"\", redirect the output to stdout.\n  Each line corresponds to a node (starting from node 0 on the first line), and contains its degree, i.e. the number of neighbors.")
  ("connected_degrees,D",po::value<std::string>(&param.connected_degrees.value),"Path of the connected degrees file, i.e. degrees once we assume complete connectivity for every cluster (output).\n If set to \"\", redirect the output to stdout.\n Each line corresponds to a node (starting from node 0 on the first line), and contains this special degree.")
  ("internal_degrees,I",po::value<std::string>(&param.internal_degrees.value),"Path of the internal degrees files(output).\n If set to \"\", redirect the output to stdout.\n Each line corresponds to a cluster (starting from cluster 0 on the first line), and contains the internal degrees of the nodes that belong to that cluster, i.e the number of neighbors they have within the community.")
  ("projected_internal_degrees,O",po::value<std::string>(&param.projected_internal_degrees.value),"Path of the projected internal degrees files (output).\n If set to \"\", redirect the output to stdout.\n Each line corresponds to a cluster (starting from cluster 0 on the first line), and contains the internal degrees of the nodes that belong to that cluster, i.e the number of neighbors they have within the community, once explicit community structure information is ignored.     More precisely, this means that the degree sequences are not built from the subgraphs, but rather from the edge list, such that overlap now plays a non-negligible role. Additionally, in the context of this computation, we do not distinguish between a node and its duplicates, such that both the community actual size, and internal degrees are lowered.\n\n")
  ("collapsed_internal_degrees,L",po::value<std::string>(&param.collapsed_internal_degrees.value),"Path of the collapsed internal degrees files (output).\n If set to \"\", redirect the output to stdout.\n Each line corresponds to a cluster (starting from cluster 0 on the first line), and contains the internal degrees of the nodes that belong to that cluster, i.e the number of neighbors they have within the community. We do not distinguish between a node and its duplicates, such that both the community actual size, and internal degrees are lowered.\n\n")
  ("timer,T",po::value<std::string>(&param.timer.value),"Path of the timer (output).\n If set to \"\", redirect the output to stdout.\n Contains a single entry with the execution time in seconds, up to the millisecond precision.\n\n")
  //Optional~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ("basic_size,s",po::value<unsigned int>(&param.basic_size.value),"Size of a community when it is created (s=1 is node based SPA, s=2 is link based, etc.)")
  ("minimal_size,m",po::value<unsigned int>(&param.minimal_size.value),"Smallest communities threshold. Communities bellow that threshold are removed before processing the output. This parameter is useful if one wishes to model a modular structure that was detected by an algorithms that imposes a minimum resolution, e.g. clique percolation.")
  ("initial_size,n",po::value<unsigned int>(&param.initial_size.value),"Number of communities at time t=0.")
  ("base_path,B",po::value<std::string>(&param.base_path.value),"Base output path (e.g. directory path). Output paths are all relative to this base path.")
  ("append,a",po::value<bool>(&param.append.value),"Outputs are appended to existing files. Overwriting is the default behavior.\n\n")
  //Misc~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ("config_file,c",po::value<std::string>(&config_file),"If this options is present, program parameters are first read from the specified configuration file, then from the command line. Command line arguments override duplicates in the configuration file. A sample configuration file can be generated in the stdout by executing this program with the --generate_conf_template (or -g) flag.\n\n")
  ("generate_conf_template,g","Generate a configuration file template in the stdout.")
  ("help,h","Produce this help message.")
  ;
  po::variables_map var_map;
  po::store(po::parse_command_line(argc,argv,description), var_map);
  po::notify(var_map);

  // For consistency with the configuration file method, we must manually indicates whether an option was found or not.
  // The CHECK_OPTION(VAR) macro (see interface.hpp) accomplishes this task.
  CHECK_OPTION(p)
  CHECK_OPTION(q)
  CHECK_OPTION(r)
  CHECK_OPTION(final_size)
  CHECK_OPTION(seed)
  CHECK_OPTION(edge_list)
  CHECK_OPTION(connected_edge_list)
  CHECK_OPTION(cluster_contents)
  CHECK_OPTION(node_participations)
  CHECK_OPTION(cluster_sizes)
  CHECK_OPTION(node_memberships)
  CHECK_OPTION(degrees)
  CHECK_OPTION(connected_degrees)
  CHECK_OPTION(internal_degrees)
  CHECK_OPTION(projected_internal_degrees)
  CHECK_OPTION(collapsed_internal_degrees)
  CHECK_OPTION(timer)
  CHECK_OPTION(basic_size)
  CHECK_OPTION(minimal_size)
  CHECK_OPTION(initial_size)
  CHECK_OPTION(base_path)
  CHECK_OPTION(append)

  // Default values. &param.VAR can't be passed directly to po:: because this would mark the variable as initialized,
  // through an erroneous count in CHECK_OPTION (essentially, var_map.count("VAR") will count as defaulted value as 
  // present).
  if (!param.basic_size.initialized)    param.basic_size.value    = 1;
  if (!param.minimal_size.initialized)  param.minimal_size.value  = 1;
  if (!param.initial_size.initialized)  param.initial_size.value  = 1;
  if (!param.base_path.initialized)     param.base_path.value     = "./";
  if (!param.append.initialized)        param.append.value        = false;

  // Act on options
  if (argc==1) {
    std::cerr << "Possible usages:\n"
              << "\t"+std::string(argv[0])+" [--option_1=value] [--option_s2=value] ...\n"
              << "\t"+std::string(argv[0])+" [-c |--config_file=]config_file.txt [--option_1=value] [--option_s2=value] ...\n";
    std::cerr << "Try '"+std::string(argv[0])+" -h' or '"+std::string(argv[0])+" --help' for more information.\n";
    return EXIT_FAILURE;
  }
  if (var_map.count("help")>0) { 
    //help message are considered normal execution and should go to stdout
    std::cout << "Possible usages:\n"
              << "\t"+std::string(argv[0])+" [--option_1=value] [--option_s2=value] ...\n"
              << "\t"+std::string(argv[0])+" [-c |--config_file=]config_file.txt [--option_1=value] [--option_s2=value] ...\n";
    std::cout << description;
    return STOP_WITH_SUCCESS;
  }
  if (var_map.count("generate_conf_template")>0) {
    LOGGER_WRITE(Logger::INFO,"Generate the configuration file template.")
    generate_template();
    return STOP_WITH_SUCCESS;
  }
  if (var_map.count("config_file")>0) {
    spa_parameters_t config_file_param;
    int file_parser_return = parse_config_file(config_file, config_file_param);
    if (file_parser_return==FILE_ERROR) return FILE_ERROR;
    else param.combine(config_file_param);
  }

  // Log parameters
  LOGGER_WRITE(Logger::CONFIG,"========================================================================")
  if (var_map.count("config_file")>0) {LOGGER_WRITE(Logger::CONFIG,"Combined parameters (command line + configuration file)")}
  else {LOGGER_WRITE(Logger::CONFIG,"Command line parameters")}
  LOGGER_WRITE(Logger::CONFIG,"========================================================================")
  log_parameters(param);


  // Test parameters and return test results
  return test_parameters(param);
}

#endif //HAVE_LIBBOOST_PROGRAM_OPTIONS

int barebone_interface(int argc, const char *argv[], spa_parameters_t & param) {
  // Act on options
  if (argc==1) {
    std::cerr << "Possible usages:\n"
              << "\t"+std::string(argv[0])+" [-c |--config_file=]config_file.txt\n"
              << "\t"+std::string(argv[0])+" [-g|--generate_conf_template]\n";
    std::cerr << "Try '"+std::string(argv[0])+" -h' or '"+std::string(argv[0])+" --help' for more information.\n";
    return EXIT_FAILURE;
  }
  else if (std::string(argv[1]).compare("-h")==0 || std::string(argv[1]).compare("--help")==0) { 
    //help message are considered normal execution and should go to stdout
    std::cout << "Possible usages:\n"
              << "\t"+std::string(argv[0])+" [-c |--config_file=]config_file.txt   Run a simulation using the configuration file.\n"
              << "\t"+std::string(argv[0])+" [-g|--generate_conf_template]         Generates an empty configuration file in the STDOUT.\n";
    return STOP_WITH_SUCCESS;
  }
  else if (std::string(argv[1]).compare("-g")==0 || std::string(argv[1]).compare("--generate_conf_template")==0) { 
    LOGGER_WRITE(Logger::INFO,"Generate the configuration file template.")
    generate_template();
    return STOP_WITH_SUCCESS;
  }
  else if (std::string(argv[1]).compare("-c")==0) {
    int file_parser_return = parse_config_file(std::string(argv[2]), param);
    if (file_parser_return==FILE_ERROR) return FILE_ERROR;
  }
  else if (std::string(argv[1]).substr(0,14).compare("--config_file=")==0) {
    int file_parser_return = parse_config_file(std::string(argv[1]).substr(14,std::string::npos), param);
    if (file_parser_return==FILE_ERROR) return FILE_ERROR;
  }
  else {
    LOGGER_WRITE(Logger::ERROR,"Could not parse the command line.")
    return EXIT_FAILURE;
  }

  // Log parameters
  LOGGER_WRITE(Logger::CONFIG,"========================================================================")
  LOGGER_WRITE(Logger::CONFIG,"Configuration file parameters")
  LOGGER_WRITE(Logger::CONFIG,"========================================================================")
  log_parameters(param);

  // Test parameters and return test results
  return test_parameters(param);
}
int parse_config_file(std::string config_file, spa_parameters_t & param) {
  LOGGER_WRITE(Logger::INFO,"parse_config_file("+VAR_PRINT(config_file)+");");
  // Path test 
  std::ifstream file(config_file.c_str(),std::ios::in);
  if (!file.is_open()) return FILE_ERROR;

  // Default values
  param.basic_size.value    = 1;
  param.minimal_size.value  = 1;
  param.initial_size.value  = 1;
  param.base_path.value     = "./";
  param.append.value        = false;

  // Variable declarations 
  std::string line_buffer;
  std::string token;
  std::string value;
  char char_buffer;

  // Read file
  while( getline(file,line_buffer) ) {
    LOGGER_WRITE(Logger::DEBUG,"Read: '"+line_buffer+"'");
    std::stringstream lbs(line_buffer);
    // Check for comments
    do {
      lbs >> char_buffer;
    } while (char_buffer==' ' && lbs.good());
    // If it's not a comment, parse the line
    if (char_buffer != '#') {
      std::stringstream line(line_buffer);
      line >> token;
      LOGGER_WRITE(Logger::DEBUG,"Found a token: '"+VAR_PRINT(token)+"'");
      //Mandatory~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      PARSE_OPTION(p)
      PARSE_OPTION(q)
      PARSE_OPTION(r)
      PARSE_OPTION(final_size)
      PARSE_OPTION(seed)
      //Output files~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      PARSE_OPTION_STR(edge_list)
      PARSE_OPTION_STR(connected_edge_list)
      PARSE_OPTION_STR(cluster_contents)
      PARSE_OPTION_STR(node_participations)
      PARSE_OPTION_STR(cluster_sizes)
      PARSE_OPTION_STR(node_memberships)
      PARSE_OPTION_STR(degrees)
      PARSE_OPTION_STR(connected_degrees)
      PARSE_OPTION_STR(internal_degrees)
      PARSE_OPTION_STR(projected_internal_degrees)
      PARSE_OPTION_STR(collapsed_internal_degrees)
      PARSE_OPTION_STR(timer)
      //Optional~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      PARSE_OPTION(basic_size)
      PARSE_OPTION(minimal_size)
      PARSE_OPTION(initial_size)
      PARSE_OPTION_STR(base_path)
      PARSE_OPTION_BOOL(append)
    }
  }
  file.close();
  LOGGER_WRITE(Logger::DEBUG,"Done reading configuration file.");
  return EXIT_SUCCESS;
}

void generate_template() {
  std::cout << "#=================================================================\n";
  std::cout << "# Warning:\n";
  std::cout << "#   Comments, indentations, permutations and omissions *should* be\n";
  std::cout << "#   handled properly. However, since the configuration parser is\n";
  std::cout << "#   very rudimentary, avoid tampering with the template to avoid\n";
  std::cout << "#   dubious results!\n";
  std::cout << "#=================================================================\n";
  std::cout << "# Mandatory arguments\n";
  std::cout << "#   Basic parameters of the SPA process.\n";
  std::cout << "#=================================================================\n";
  std::cout << "# Random-related parameters {expected format: doubles}.\n";
  std::cout << "p = \n";
  std::cout << "q = \n";
  std::cout << "# Structural parameter {expected format: unsigned integer}.\n";
  std::cout << "final_size = \n";
  std::cout << "# Iteration related parameters {expected format: unsigned integer}.\n";
  std::cout << "seed = \n";
  std::cout << "#================================================================= \n";
  std::cout << "# Mostly mandatory argument\n";
  std::cout << "#   \'r\' is only mandatory when the degrees output file is\n";
  std::cout << "#   specified, i.e. when the explicit local process must be\n";
  std::cout << "#   executed.\n";
  std::cout << "#================================================================= \n";
  std::cout << "# Random-related parameter {expected format: doubles}.\n";
  std::cout << "r = \n";
  std::cout << "#================================================================= \n";
  std::cout << "# Output files\n";
  std::cout << "#   If a path is left unspecified, the corresponding quantity\n";
  std::cout << "#   will not be outputted, nor computed if it's avoidable.\n";
  std::cout << "#\n";
  std::cout << "#   Note:   The expected format of unspecified and specified \n";
  std::cout << "#           paths are, respectively:\n";
  std::cout << "#               token = \n";
  std::cout << "#               token = \"data_file.dat\"\n";
  std::cout << "#           A empty, but not omitted path, i.e.\n";
  std::cout << "#               token = \"\"\n";
  std::cout << "#           will re-route the output to the stdout.\n";
  std::cout << "#================================================================= \n";
  std::cout << "# Network state {expected format: string}.\n";
  std::cout << "edge_list = \n";
  std::cout << "connected_edge_list = \n";
  std::cout << "cluster_contents = \n";
  std::cout << "node_participations = \n";
  std::cout << "# Structural data {expected format: string}.\n";
  std::cout << "cluster_sizes = \n";
  std::cout << "node_memberships = \n";
  std::cout << "# Degree-related data {expected format: string}.\n";
  std::cout << "degrees = \n";
  std::cout << "connected_degrees = \n";
  std::cout << "internal_degrees = \n";
  std::cout << "projected_internal_degrees = \n";
  std::cout << "collapsed_internal_degrees = \n";
  std::cout << "# Benchmarking data {expected format: string}.\n";
  std::cout << "timer = \n";
  std::cout << "#================================================================= \n";
  std::cout << "# Optional arguments (have defaulted values)\n";
  std::cout << "#   If a parameter is left unspecified (or deleted, or commented),\n";
  std::cout << "#   the default value will be used.\n";
  std::cout << "#\n";
  std::cout << "#   Defaults:\n";
  std::cout << "#       basic_size = 1\n";
  std::cout << "#       minimal_size = 1\n";
  std::cout << "#       initial_size = 1\n";
  std::cout << "#       base_path = \"./\"\n";
  std::cout << "#       append = false\n";
  std::cout << "#================================================================= \n";
  std::cout << "# Structural parameters  {expected format: unsigned integer}.\n";
  std::cout << "basic_size = \n";
  std::cout << "minimal_size = \n";
  std::cout << "initial_size = \n";
  std::cout << "# Base output path (suffix) {expected format: string}.\n";
  std::cout << "base_path = \n";
  std::cout << "# Options {expected format: boolean}.\n";
  std::cout << "append = \n";
  return;
}

int test_parameters(const spa_parameters_t & param) {
  LOGGER_WRITE(Logger::INFO, "Parameters validation.")
  // Test parameters
  try {
    // Missing crucial parameters
    if (!param.p.initialized) throw(std::string("[Missing parameter] The community birth probability needs to be specified."));
    if (!param.q.initialized) throw(std::string("[Missing parameter] The node birth probability needs to be specified."));
    if (!param.final_size.initialized) throw(std::string("[Missing parameter] A final size (in nodes) needs to be specified."));
    if (!param.seed.initialized) throw(std::string("[Missing parameter] A seed needs to be provided to the RNG."));
    if (!param.r.initialized && param.degrees.initialized) throw(std::string("[Missing parameter] The link creation ratio needs to be specified to produce a connected network with degrees."));
    if (!param.r.initialized && param.edge_list.initialized) throw(std::string("[Missing parameter] The link creation ratio needs to be specified to produce a connected network with a edge list."));
    if (!param.r.initialized && param.internal_degrees.initialized) throw(std::string("[Missing parameter] The link creation ratio needs to be specified to produce a connected network with internal degrees."));
    if (!param.r.initialized && param.projected_internal_degrees.initialized) throw(std::string("[Missing parameter] The link creation ratio needs to be specified to produce a connected network with projected degrees."));
    if (!param.r.initialized && param.collapsed_internal_degrees.initialized) throw(std::string("[Missing parameter] The link creation ratio needs to be specified to produce a connected network with collapsed degrees."));
  }
  catch (const std::string& error_str) {
    LOGGER_WRITE(Logger::ERROR, error_str)
    return ELEM_MISSING_ERROR;
  }
  try {
    // Logical inconstancy.
    if (param.p.value < 0 || param.p.value > 1) throw(std::string("[Forbidden value] The community birth probability p must be in the [0,1] interval ("+VAR_PRINT(param.p.value)+")."));
    if (param.q.value < 0 || param.q.value > 1) throw(std::string("[Forbidden value] The node birth probability q must be in the [0,1] interval ("+VAR_PRINT(param.q.value)+")."));
    if (param.r.value < 0 && param.r.initialized==true) throw(std::string("[Forbidden value] The link creation ratio r must be a positive real ("+VAR_PRINT(param.r.value)+")."));
    if (param.final_size.value < param.initial_size.value*param.basic_size.value) throw(std::string("[Forbidden value] The final size must be greater or equal to the initial size ("+VAR_PRINT(param.final_size.value)+", "+VAR_PRINT(param.initial_size.value*param.basic_size.value)+")."));
    if (!param.output_requested()) throw(std::string("[Logical error] No output was requested."));
  }
  catch (const std::string& error_str) {
    LOGGER_WRITE(Logger::ERROR, error_str)
    return LOGIC_ERROR;
  }
  LOGGER_WRITE(Logger::INFO, "Parameters are valid.")
  return EXIT_SUCCESS;
}

void log_parameters(const spa_parameters_t & param) {
  if (param.p.initialized)                            LOGGER_WRITE(Logger::CONFIG, "p: "+VAR_EVAL(param.p.value))
  if (param.q.initialized)                            LOGGER_WRITE(Logger::CONFIG, "q: "+VAR_EVAL(param.q.value))
  if (param.r.initialized)                            LOGGER_WRITE(Logger::CONFIG, "r: "+VAR_EVAL(param.r.value))
  if (param.final_size.initialized)                   LOGGER_WRITE(Logger::CONFIG, "final_size: "+VAR_EVAL(param.final_size.value))
  if (param.seed.initialized)                         LOGGER_WRITE(Logger::CONFIG, "seed: "+VAR_EVAL(param.seed.value))
  if (param.edge_list.initialized)                    LOGGER_WRITE(Logger::CONFIG, "edge_list: "+VAR_EVAL(param.edge_list.value))
  if (param.connected_edge_list.initialized)          LOGGER_WRITE(Logger::CONFIG, "connected_edge_list: "+VAR_EVAL(param.connected_edge_list.value))
  if (param.cluster_contents.initialized)             LOGGER_WRITE(Logger::CONFIG, "cluster_contents: "+VAR_EVAL(param.cluster_contents.value))
  if (param.node_participations.initialized)          LOGGER_WRITE(Logger::CONFIG, "node_participations: "+VAR_EVAL(param.node_participations.value))
  if (param.cluster_sizes.initialized)                LOGGER_WRITE(Logger::CONFIG, "cluster_sizes: "+VAR_EVAL(param.cluster_sizes.value))
  if (param.node_memberships.initialized)             LOGGER_WRITE(Logger::CONFIG, "node_memberships: "+VAR_EVAL(param.node_memberships.value))
  if (param.degrees.initialized)                      LOGGER_WRITE(Logger::CONFIG, "degrees: "+VAR_EVAL(param.degrees.value))
  if (param.connected_degrees.initialized)            LOGGER_WRITE(Logger::CONFIG, "connected_degrees: "+VAR_EVAL(param.connected_degrees.value))
  if (param.internal_degrees.initialized)             LOGGER_WRITE(Logger::CONFIG, "internal_degrees: "+VAR_EVAL(param.internal_degrees.value))
  if (param.projected_internal_degrees.initialized)   LOGGER_WRITE(Logger::CONFIG, "projected_internal_degrees: "+VAR_EVAL(param.projected_internal_degrees.value))
  if (param.collapsed_internal_degrees.initialized)   LOGGER_WRITE(Logger::CONFIG, "collapsed_internal_degrees: "+VAR_EVAL(param.collapsed_internal_degrees.value))
  if (param.timer.initialized)                        LOGGER_WRITE(Logger::CONFIG, "timer: "+VAR_EVAL(param.timer.value))
  if (param.basic_size.initialized)                   LOGGER_WRITE(Logger::CONFIG, "basic_size: "+VAR_EVAL(param.basic_size.value))
  if (param.minimal_size.initialized)                 LOGGER_WRITE(Logger::CONFIG, "minimal_size: "+VAR_EVAL(param.minimal_size.value))
  if (param.initial_size.initialized)                 LOGGER_WRITE(Logger::CONFIG, "initial_size: "+VAR_EVAL(param.initial_size.value))
  if (param.base_path.initialized)                    LOGGER_WRITE(Logger::CONFIG, "base_path: "+VAR_EVAL(param.base_path.value))
  if (param.append.initialized)                       LOGGER_WRITE(Logger::CONFIG, "append: "+VAR_EVAL(param.append.value))
  return;
}