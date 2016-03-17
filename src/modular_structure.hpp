#ifndef MODULAR_STRUCTURE_HPP
#define MODULAR_STRUCTURE_HPP
/**
  * @file modular_structure.hpp
  * @brief Subgraph class for modular networks.
  * @date 9-10-2014
  * @author Jean-Gabriel Young <jean.gabriel.young@gmail.com>
  * @copyright MIT
  * @version 0.1
  */

// Configuration file
#include "config.h"
// C libraries
#include <math.h>   // ceil
#include <assert.h> // error checking: turn off with #define NDEBUG
#include <stddef.h> // size_t
// STL
#include <set>      // std::multiset (private data members)
#include <vector>   // std::vector< std::multiset > (private data members)
#include <random>   // uniform_real_distribution
#include <iterator> // std::advance
// Typdef and helpers
#include "types.hpp" // id_t
  
/** @class modular_structure_t
  * @brief Modular structure type for networks with community structures.
  *
  *  @p modular_structure_t is a protected class that provides a backbone for modular networks.
  *  It is designed with public inheritance in mind: daughter classes can manipulate the 
  *  modular structure safely via SET accessors, but public access is only limited to
  *  a minimal GET interface. 
  *
  *  @p modular_structure_t provides an easy to grasp interface with what is effectively
  *  a sparse, community incidence matrix, with possibly multiple occurrences of a given
  *  node in any given cluster, and vice-versa. 
  *
  *  A built-in preferential random GET access methods is also available, to facilitate
  *  preferential attachment algorithms implementation.
  *
  */
class modular_structure_t {

protected:
  /** @name Constructor */
  //@{
  /// Empty constructor
  modular_structure_t();
  /// Reserve constructor
  modular_structure_t(size_t size_in_nodes, size_t size_in_clusters);
  //@}

  /** @name Protected accessors */
  //@{
  //GET~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  /** Return the sum of the incidence matrix (participations / contents). */
  size_t get_incidence_sum() const;
  /** Pick a random node or cluster proportionally to its number of activity. 
    * @param[in] <viewpoint> "node" or "cluster".
    * @param[in] <engine> Reference to a std::mt19937 engine.
    * @see uniform_pick.
    */
  id_t preferential_pick(std::string viewpoint, std::mt19937& engine);
  //SET~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  /** Act as STL resize(1), i.e. extra nodes do not belong to any clusters if the new size is larger. Destroy information otherwise. */
  void set_size_in_nodes(size_t size_in_nodes);
  /** Act as STL resize(1), i.e. extra clusters do not contain any nodes if the new size is larger. Destroy information otherwise. */
  void set_size_in_clusters(size_t size_in_nodes);
  //ABSTACT SET~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  /** Make new associations between existing elements. Performs assert unless NDEBUG is defined check. */
  void add_node_to_cluster(id_t node, id_t cluster);
  /** Make new associations between existing elements. Performs assert unless NDEBUG is defined check. */
  void add_cluster_to_node(id_t cluster, id_t node);
  /** Create a new, participation-less, node. Act like set_size_in_node(this->size_in_node()+1);. 
    * @return Id of the new node.
    */
  id_t new_node();
  /** Create a new node with participations via push_back like mechanism. 
    * Performs and OOR assert unless NDEBUG is defined check for the participations.
    * @return Id of the new node.
    */
  id_t new_node(std::multiset<id_t> participations);
  /** Create a new empty cluster. Act like set_size_in_clusters(this->size_in_clusters()+1);.
    * @return Id of the new cluster.
  */
  id_t new_cluster();
  /** Create a new cluster with non-empty content via a push_back like mechanism.
    * Performs and OOR assert unless NDEBUG is defined check for the content.
    * @return Id of the new cluster.
    */
  id_t new_cluster(std::multiset<id_t> content);
  /** Delete all cluster of sizes bellow a certain threshold.
    * @param[in] <min_community_size> Community size threshold.
    * @warning This member function only reduces some elements 
    * of the clusters and participations vector to size 0. 
    * Output and data analysis must be done accordingly.
    */
  void purge_modular_structure(size_t min_community_size);
  //@}

  /** @name Boolean tests. */
  //@{
  bool node_exists(id_t node) const;
  bool cluster_exists(id_t cluster) const;
  //@}
public:
  /// @name Capacity 
  //@{    
  /** Return the current number of nodes. */
  size_t size_in_nodes() const;
  /** Return the current number of clusters. */
  size_t size_in_clusters() const;
  /** Return the current membership numer of a node. */
  size_t membership(id_t node, bool unique=false) const;
  /** Return the current size of a cluster. 
    * @param[in] <cluster> Identifier of the cluster.
    * @param[in] <unique> Count nodes only one time, or multiple times. 
    */
  size_t cluster_size(id_t cluster, bool unique=false) const;
  //@}

  /// @name Get accessors 
  //@{
  //GET~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  /** Return the participations of a node */
  std::multiset<id_t> get_participations(id_t node) const;
  /** Return the nodes of a cluster */
  std::multiset<id_t> get_content(id_t cluster) const;
  /** Return the unique participations of a node */
  std::set<id_t> get_unique_participations(id_t node) const;
  /** Return the unique nodes of a cluster */
  std::set<id_t> get_unique_content(id_t cluster) const;
  //@}

private:
  /** @name RNG-related. */
  //@{
  id_t preferential_pick(std::vector< std::multiset<id_t> > & vec_of_multiset, std::mt19937& engine);
  //@}

  /** @name Private data members */
  //@{
  // Trackers ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  /// Incidence sum (i.e. participations cardinality or cluster cardinality sum).
  size_t incidence_sum_;
  //  State container ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  /*  General remark: We use multisets, and thus a node can appear more than once within a given structure. */
  /// Sparse incidence matrix; communities view point
  std::vector< std::multiset<id_t> > content_;
  /// Sparse incidence matrix; node view point  
  std::vector< std::multiset<id_t> > participations_;
  /// Internal distribution. Generator must be passed as a service
  std::uniform_real_distribution<double> rand_real_;
  //@}
};

#endif //MODULAR_STRUCTURE_HPP