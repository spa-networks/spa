#ifndef SPA_NETWORK_HPP
#define SPA_NETWORK_HPP
/**
  * @file spa_network.hpp
  * @brief Network class for SPA processes.
  * @date 8-10-2014
  * @author Jean-Gabriel Young <jean.gabriel.young@gmail.com>
  * @copyright MIT
  * @version 0.4
  */

// C libraries
#include <math.h>    // floor
#include <stddef.h>  // size_t
#include <assert.h> // error checking: turn off with #define NDEBUG
// STL
#include <set>      // std::multiset (private data members)
#include <vector>   // std::vector< std::multiset > (private data members)
#include <tuple>    // return type for event 
#include <random>   // uniform_real_distribution
// Project files
#include "modular_structure.hpp" // base-class on which spa_network_t is built
#include "subgraph.hpp" // a private data member
// Typdefs
#include "types.hpp" // id_t


/** @class spa_network_t
  * @brief Network class for SPA processes
  *
  * This class implements the SPA growth process (stochastic network growth).
  * All growth and linking methods are provided, both for the network and its 
  * cluster structure. A number of output / input methods are also provided to
  * allow for check pointing and analysis. Data structures and algorithms are 
  * specifically designed for high performance computing. Execution speed is 
  * always selected over memory efficiency. Hence, some features (e.g. the 
  * incidence matrix) have redundant implementation to speed up computation. 
  */
class spa_network_t : public modular_structure_t {
public:
  /** @name Constructor
    * Available constructors for the spa_network_t class.
    */
  //@{
  /** Constructs an empty network of given maximal size. Reserves and 
    * allocates memory for every vector / sets of predictable size. 
    * @param[in] <N> Target size of the network.
    * @param[in] <s> Basic unit of SPA (1:=node, 2:=link, 3:=triangle, etc.)
    * @param[in] <handle_links> Determine whether links are handled explicitly 
    *     with SPA* or using legacy behavior (fully connected subgraphs).
    */
  spa_network_t(size_t N, size_t s, bool handle_links=true);
  //@}

  /** @name Capacity 
    * Functions that are related to the capacity of the network.
    */
  //@{
  /** Return the target size of the network, in nodes. */
  size_t target_size() const;
  /** Return the basic unit size used in the SPA process. */
  size_t basic_size() const;
  /** Expand the target size of the network to a larger size. If N is smaller than current)size, this method does nothing.*/
  void expand_target_size(size_t N);
  //@}

  /** @name Accessors
    */
  //@{
  /** Return a subgraph object, identified by its cluster ID. */
  subgraph_t get_subgraph(id_t cluster) const;
  /** Set link handling status.
    * @warning This method will fail if the network already started to grow.
    *   It is only provided to loosen scoping constraints.
    * @return true if the status change worked, false otherwise.
    */
  bool set_handle_links_status(bool status);
  //@}

  /** @name SPA Operations
    * Functions that act directly on the network structure.
    */
  //@{
  /** Creates a network of disjoint communities. 
    * @param[in] <n> Initial number communities of size ".basic_size(0)".
    * @see basic_size
    */
  void initialize(unsigned int n);
  /** Creates a new cluster of size s. (s-1) nodes are selected preferentially 
    * among existing ones, and the last node is either new or old, as specified
    * by @p new_individual. 
    * @remark Regardless of the links handling method, cluster are always born
    *   with complete connectivity.
    * @param[in] <new_individual> Determine whether the node is new or old.
    * @param[in] <engine> Initialized pseudo-RNG engine of the std::mt19937 type.
    * @return The identifier of the new cluster.
    */
  id_t cluster_birth(bool new_individual, std::mt19937& engine);
  /** A preferentially chosen cluster gains one node, chosen preferentially to
    * its number of participations (membership number). 
    * @remark If links are handled explicitly, a link is created with 
    *   uniform probability between the new node and any node in the growing 
    *   cluster.
    * @param[in] <new_individual> Determine whether the node is new or old.
    * @param[in] <engine> Initialized pseudo-RNG engine of the std::mt19937 type.
    * @return A std::tuple<id_t,id_t> containing the identifiers of the node (<0>) 
    *   and cluster (<1>) involved in the growth event. 
    */
  std::tuple<id_t, id_t> cluster_growth(bool new_individual, std::mt19937& engine);
  /** Creates a link within a uniformly chosen cluster of a preferentially 
    * chosen node. The target node is chosen uniformly from the set of
    * available stubs.
    * @warning Does not change the state of the network when link handling is
    *   not turned on.
    * @remark This method is allowed to fail if a node of maximal degree is 
    *   selected. This ensures that temporal mapping are respected (see articles).
    * @param[in] <engine> Initialized pseudo-RNG engine of the std::mt19937 type.
    * @return A std::tuple<id_t,id_t,id_t,bool>, where
    *   <0> and <1> contain the identifiers of the source and target nodes,
    *   <2> contains the identifier of the cluster in which the link creation event occurred,
    *   <3> contains a boolean that indicates whether the event was successful or not. 
    */
  std::tuple<id_t, id_t, id_t, bool> link_creation_step(std::mt19937& engine);
  /** Delete every cluster and subgraph of size bellow a certain threshold.
    * @param[in] <min_cluster_size> Cluster size threshold.
    * @warning This member function effectively reduces some elements 
    * of the clusters and participations vector to size 0.
    * Output and data analysis must be done accordingly.
    */
  void purge(size_t min_cluster_size);
  //@}

private:
  /// Empty constructor are forbidden.
  spa_network_t();
  /** @Name Private member function */
  //@{
  id_t disjoint_cluster_birth();
  void new_subgraph(id_t cluster, bool connected);
  id_t uniform_pick(const std::set<id_t> & a_set, std::mt19937& engine);
  id_t uniform_pick(unsigned int upper_bound, std::mt19937& engine);
  //@}

  /** @name Private data members */
  //@{
  /// Basic unit size of the SPA process.
  size_t s_;
  /// Target size of the network.
  size_t N_;
  /// Determine whether links are handled using SPA*, or legacy versions (full connectivity).
  bool handle_links_;
  //@}

  /// Internal distribution. Generator must be passed as a service
  std::uniform_real_distribution<double> rand_real_;

  /** @name State container */
  //@{
  /// Explicit subgraph connectivity container
  std::vector< subgraph_t > subgraphs_;
  //@}
};

#endif //SPA_NETWORK_HPP
