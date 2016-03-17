#ifndef MISC_FUNCTIONS_HPP
#define MISC_FUNCTIONS_HPP

// STL
#include <map>  // global id to local id (create_projected_internal_degrees(3))
#include <set>  // spa_network_t accessors use sets and multisets
#include <vector> // edge list container
// Project files
#include "spa_network.hpp" // spa_network_t
#include "subgraph.hpp" // subgraph_t
// Typdef and helpers
#include "types.hpp" // id_t, edge_t
#include "includes/logger.hpp" // LOGGER macros


/** @name create_edge_list
  * Create an edge list from a network object. Duplicate links and self-loops
  * are not considered.
  * @remark Return by reference.
  * @param[in] <network> Initialized network object with link handling.
  * @param[out] <edge_list> The edge list. 
  */
void create_edge_list(const spa_network_t & network, std::set<edge_t> & edge_list);
/** @name create_edge_list
  * Create an edge list from a network object, assuming complete
  * connectivity for the communities. Duplicate links and self-loops
  * are not considered.
  * @remark Return by reference.
  * @param[in] <network> Initialized network object.
  * @param[out] <connected_edge_list> The connected edge list. 
  */
void create_connected_edge_list(const spa_network_t & network,  std::set<edge_t> & connected_edge_list);
/** @name create_projected_internal_degrees
  * Create internal degree sequences from projected network (within subgraphs:
  * duplicate nodes are merged, self-loops are removed, within the network:
  * links due to overlap are taken into account in multiple communities).
  * @remark Return by reference.
  * @param[in] <edge_list> Initialized edge list. 
  * @param[in] <network> Initialized network object.
  * @param[out] <projected_internal_degrees> The internal degree sequences. 
  */
void create_projected_internal_degrees(const std::set< edge_t > edge_list, const spa_network_t & network, std::vector<std::vector<unsigned int> > & projected_internal_degrees);
/** @name create_collapsed_internal_degrees
  * Create internal degree sequences from collapsed internal communities (within subgraphs:
  * duplicate nodes are merged, self-loops are removed).
  * @remark Return by reference.
  * @param[in] <network> Initialized network object.
  * @param[out] <collapsed_internal_degrees> The internal degree sequences. 
  */
void create_collapsed_internal_degrees(const spa_network_t & network, std::vector<std::vector<unsigned int> > & collapsed_internal_degrees);
#endif //MISC_FUNCTIONS_HPP