#ifndef OUTPUT_FUNCTIONS_HPP
#define OUTPUT_FUNCTIONS_HPP

// Configuration file
#include "config.h"
// STL
#include <iostream> // ostream
#include <set>  // spa_network_t accessors use sets and multisets
#include <vector> // edge list container
#include <string> // separators in generic output functions
// Project files
#include "spa_network.hpp" // spa_network_t
#include "subgraph.hpp" // subgraph_t
#include "misc_functions.hpp" //edge_list builders
// Typdef and helpers
#include "types.hpp" // spa_parameters_t, id_t, edge_t
#include "includes/logger.hpp"  // LOGGER macros
#include "includes/bofstream.hpp" // buffered output stream

/// Unified interface with main()
void process_and_output(spa_network_t & network, const spa_parameters_t & param, const double duration);
/// Generic functions
//@{
void output_vector_uint_content(const std::vector<unsigned int> & uint_vec, std::ostream& os, std::string separator);
void output_multiset_id_content(const std::multiset<id_t> & id_multiset, std::ostream& os, std::string separator);
void output_multiset_id_size(const std::multiset<id_t> & id_multiset, std::ostream& os);
void output_set_id_size(const std::set<id_t> & id_multiset, std::ostream& os);
//@}

/// Processed data output
//@{
void output_edge_list(const std::set< edge_t > & edge_list, std::ostream& os);
void output_connected_edge_list(const std::set< edge_t > & connected_edge_list, std::ostream& os);
void output_degrees(const spa_network_t & network, const std::set< edge_t > & edge_list, std::ostream& os);
void output_connected_degrees(const spa_network_t & network, const std::set< edge_t > & connected_edge_list, std::ostream& os);
void output_timer(const double duration, std::ostream& os);
void output_projected_internal_degrees(const std::vector<std::vector<unsigned int> > & projected_internal_degrees, std::ostream& os);
void output_collapsed_internal_degrees(const std::vector<std::vector<unsigned int> > & collapsed_internal_degrees, std::ostream& os);
//@}

/// Straightforward outputs
//@{
void output_cluster_contents(const spa_network_t & network, std::ostream& os);
void output_cluster_sizes(const spa_network_t & network, std::ostream& os);
void output_node_participations(const spa_network_t & network, std::ostream& os);
void output_node_memberships(const spa_network_t & network, std::ostream& os);
void output_internal_degrees(const spa_network_t & network, std::ostream& os);
//@}
#endif // OUTPUT_FUNCTIONS_HPP