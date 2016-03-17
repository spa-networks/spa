#ifndef SUBGRAPH_HPP
#define SUBGRAPH_HPP
/**
  * @file subgraph.hpp
  * @brief Subgraph class for modular networks.
  * @date 9-10-2014
  * @author Jean-Gabriel Young <jean.gabriel.young@gmail.com>
  * @copyright MIT
  * @version 0.1
  */


// Configuration file
#include "config.h"
// C libraries
#include <assert.h> // error checking: turn off with #define NDEBUG
#include <stddef.h> // size_t
// STL
#include <set> // set of edge_t
#include <map> // local id to global id
#include <vector> // internal degrees
#include <algorithm> //set_difference
#include <utility>  // std::pair
// Typdef and helpers
#include "types.hpp" // edge_t, id_t
  



/** @class subgraph_t
  * @brief Subgraph class for modular networks.
  */
class subgraph_t {
public:
  /** @name Constructor
    * Available constructors for the subgraph_t class.
    */
  //@{
  /// Empty constructor
  subgraph_t();
  /// Constructor with a initial list of nodes
  subgraph_t(std::set< id_t > nodes, bool connected);
  //@}

  /** @name Capacity 
    * Functions that are related to the size/capacity of the subgraph.
    */
  //@{
  size_t size_in_links() const;
  size_t size_in_nodes() const;
  //@}

  /** @name Accessors 
    * Return information relative to the subgraph structure.
    */
  //@{
  /// Get internal degrees 
  std::vector< unsigned int > get_internal_degrees() const;
  /** Return a links of the subgraph (identified by local identifiers) */
  std::set< edge_t > get_local_links() const;
  /** Return the links of the subgraph (identified by global identifiers) */
  std::multiset< edge_t > get_global_links() const;
  /** Retun the global identifier of a node */
  id_t get_global_id(id_t internal_node_id) const;
  /** Retun the global identifier of all nodes */
  std::set<id_t> get_global_ids() const;
  /** Clear content */
  void clear();
  //@}

  /** @name Structural utilities
    * Functions that provide an abstract interface to manipulate that 
    * manipulates the structure of the subgraph.
    */
  //@{
  /** Create a set of nodes that are NOT part of the neighborhood of @p internal_node_id 
    * but that are part of the subgraph, i.e. potential link creation targets.
    * @remark Complexity: Neighbor reserach is O( n log(m) ) [because we use edge lists, 
    *                     with adj lists it would be O(log(n)) or O(1), depending on 
    *                     the implementation]. The set difference operation is O(n).
    * @param[in] <internal_node_id> Identifier of the first node in the subgraph.
    */
  std::set<id_t> link_creation_targets(id_t internal_node_id);
  /** Add a new node to the subgraph 
    * @param[in] <global_node_id> Identifier of the node in the original network.
    * @return Unique internal identifier of the node.
    */
  id_t add_node(id_t global_node_id);
  /** Connect two nodes, 
    * @param[in] <internal_node_id_1> Identifier of the first node in the subgraph.
    * @param[in] <internal_node_id_2> Identifier of the second node in the subgraph.
    * @warning The interface is somewhat inconsistent -- connect(2) uses
    * internal identifiers (i.e. node identifier within the  community)
    * whereas add_node(1) uses global identifiers (i.e. node identifier
    * at the scale of the whole network).
    */
  void connect(id_t internal_node_id_1, id_t internal_node_id_2);
  /**  Connect every possible pair of nodes. */
  void fully_connect();
  //@}

  /** @name Boolean tests. */
  //@{
  bool node_exists(id_t internal_node_id) const;
  //@}

private:
  /** @name Private data members */
  //@{
  /// Link container
  std::set< edge_t > links_; 
  /// Identifier mapping : global_id = id_map_[internal_id]
  std::map<id_t,id_t> id_map_;
  #if MEMORY_OPTIMIZED == 0
    /// Internal degree vectors (only defined if the MEMORY_OPTIMIZED macro is defined to 0)
    std::vector< unsigned int > internal_degrees_;
    std::set< id_t > nodes_;
  #endif
  //@}

  /*  On the uniqueness of node IDs:
   *  Unique nodes (within the network) may appear multiple time in a same
   *  subgraph in the context of preferential attachment (SPA*). 
   *  Thus, we assign unique internal ids to node within a subgraph, and
   *  and provide a mapping.
   */
};

#endif //SUBGRAPH_HPP