#ifndef TYPES_H
#define TYPES_H

// STL
#include <utility> // std::pair
#include <string>

/// Element (node / cluster) identifier type.
typedef unsigned int id_t;
/// Edge
typedef std::pair<id_t,id_t> edge_t;

/** @class str_opt_t
  * @brief String program argument type.
  *  @see double_opt_t, uint_opt_t, bool_opt_t.
  */
typedef struct str_opt_t {
  /// str_opt_t::value contains the argument (std::string type)
  std::string value;
  /// str_opt_t::initialized indicates if the program argument is initialized.
  bool initialized;
  str_opt_t() : initialized(false) { }
  /// initialize the object with a value.
  void init(std::string init_value) {value=init_value;initialized=true;}
} str_opt_t;
/** @class double_opt_t 
  *  @brief Double program argument type.
  *  @see str_opt_t, uint_opt_t, bool_opt_t.
  */
typedef struct double_opt_t {
  /// double_opt_t::value contains the argument (double type)
  double value;
  /// double_opt_t::initialized indicates if the program argument is initialized.
  bool initialized;
  double_opt_t() : initialized(false) { }
  /// initialize the object with a value.
  void init(double init_value) {value=init_value;initialized=true;}
} double_opt_t;
/** @class uint_opt_t 
  *  @brief Unsigned int program argument type.
  *  @see str_opt_t, double_opt_t, bool_opt_t.
  */
typedef struct uint_opt_t {
  /// uint_opt_t::value contains the argument (unsigned int type)
  unsigned int value;
  /// uint_opt_t::initialized indicates if the program argument is initialized.
  bool initialized;
  uint_opt_t() : initialized(false) { }
  /// initialize the object with a value.
  void init(unsigned int init_value) {value=init_value;initialized=true;}
} uint_opt_t;
/** @class bool_opt_t 
  *  @brief Boolean program argument type.
  *  @see str_opt_t, double_opt_t, uint_opt_t.
  */
typedef struct bool_opt_t {
  /// bool_opt_t::value contains the argument (bool type)
  bool value;
  /// bool_opt_t::initialized indicates if the program argument is initialized.
  bool initialized;
  bool_opt_t() : initialized(false) { }
  /// initialize the object with a value.
  void init(bool init_value) {value=init_value;initialized=true;}
} bool_opt_t;

/// Program parameters structure
typedef struct spa_parameters_t {
  //Mandatory~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  /// Community birth probability.
  double_opt_t p;
  /// Probability that a node is a new one, for growth and birth events.
  double_opt_t q;
  /// Ratio of the internal link creation and recruiting rates.
  double_opt_t r;
  /// Final size of the network (in nodes).
  uint_opt_t final_size;
  /// Seed of the pseudo random number generator (Mersenne-twister 19937).
  uint_opt_t seed;
  //Output files~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  /// Edge file path (output).
  str_opt_t edge_list;
  /// Connected edge file path (output). (assuming full connectivity of communities).
  str_opt_t connected_edge_list;
  /// Cluster content file path (output).
  str_opt_t cluster_contents;
  /// Node participations file path (output).
  str_opt_t node_participations;
  /// Cluster sizes file path (output).
  str_opt_t cluster_sizes;
  /// Node memberships file path (output).
  str_opt_t node_memberships;
  /// Degrees file path (output).
  str_opt_t degrees;
  /// Connected degrees file path (output). (assuming full connectivity of communities).
  str_opt_t connected_degrees;
  /// Internal degrees file path (output).
  str_opt_t internal_degrees;
  /// Internal degrees file path [once duplicates are removed] (output).
  str_opt_t collapsed_internal_degrees;
  /// Internal degrees file path [once duplicates are removed and overlap is taken into account] (output).
  str_opt_t projected_internal_degrees;
  /// Timer result file path (output).
  str_opt_t timer;
  //Optional~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  /// Size of a community when it is created.
  uint_opt_t basic_size;
  /// Smallest considered communities for the averaging process.
  uint_opt_t minimal_size;
  /// Number of communities at time t=0.
  uint_opt_t initial_size;
  /// Base output path (a suffix that is added to every file path)
  str_opt_t base_path;
  /// Append option: if true all outputs are appended, instead of written over existing content.
  bool_opt_t append;

  /**  Replace non-initialized parameters in param1 by initialized parameters in param2. 
    *  @remark Parameters that are already initialized in param1 are not modified. 
    */
  void combine(const spa_parameters_t & rhs) {
    if ( !p.initialized && rhs.p.initialized ) p.init(rhs.p.value);
    if ( !q.initialized && rhs.q.initialized ) q.init(rhs.q.value);
    if ( !r.initialized && rhs.r.initialized ) r.init(rhs.r.value);
    if ( !final_size.initialized && rhs.final_size.initialized ) final_size.init(rhs.final_size.value);
    if ( !seed.initialized && rhs.seed.initialized ) seed.init(rhs.seed.value);
    if ( !edge_list.initialized && rhs.edge_list.initialized ) edge_list.init(rhs.edge_list.value);
    if ( !connected_edge_list.initialized && rhs.connected_edge_list.initialized ) connected_edge_list.init(rhs.connected_edge_list.value);
    if ( !cluster_contents.initialized && rhs.cluster_contents.initialized ) cluster_contents.init(rhs.cluster_contents.value);
    if ( !node_participations.initialized && rhs.node_participations.initialized ) node_participations.init(rhs.node_participations.value);
    if ( !cluster_sizes.initialized && rhs.cluster_sizes.initialized ) cluster_sizes.init(rhs.cluster_sizes.value);
    if ( !node_memberships.initialized && rhs.node_memberships.initialized ) node_memberships.init(rhs.node_memberships.value);
    if ( !degrees.initialized && rhs.degrees.initialized ) degrees.init(rhs.degrees.value);
    if ( !connected_degrees.initialized && rhs.connected_degrees.initialized ) connected_degrees.init(rhs.connected_degrees.value);
    if ( !internal_degrees.initialized && rhs.internal_degrees.initialized ) internal_degrees.init(rhs.internal_degrees.value);
    if ( !projected_internal_degrees.initialized && rhs.projected_internal_degrees.initialized ) projected_internal_degrees.init(rhs.projected_internal_degrees.value);
    if ( !collapsed_internal_degrees.initialized && rhs.collapsed_internal_degrees.initialized ) collapsed_internal_degrees.init(rhs.collapsed_internal_degrees.value);
    if ( !timer.initialized && rhs.timer.initialized ) timer.init(rhs.timer.value);
    if ( !basic_size.initialized && rhs.basic_size.initialized ) basic_size.init(rhs.basic_size.value);
    if ( !minimal_size.initialized && rhs.minimal_size.initialized ) minimal_size.init(rhs.minimal_size.value);
    if ( !initial_size.initialized && rhs.initial_size.initialized ) initial_size.init(rhs.initial_size.value);
    if ( !base_path.initialized && rhs.base_path.initialized ) base_path.init(rhs.base_path.value);
    if ( !append.initialized && rhs.append.initialized ) append.init(rhs.append.value);
    return;
  }

  /// True if one or more output path is initialized.
  bool output_requested() const {
    return (edge_list.initialized || connected_edge_list.initialized  || 
            cluster_contents.initialized || node_participations.initialized || 
            cluster_sizes.initialized  || node_memberships.initialized || 
            degrees.initialized  || connected_degrees.initialized  || 
            internal_degrees.initialized || projected_internal_degrees.initialized ||
            collapsed_internal_degrees.initialized ||timer.initialized);
  }
} spa_parameters_t;

#endif // TYPES_H