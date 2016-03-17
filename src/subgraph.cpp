#include "subgraph.hpp"

// ============================================================================================
// PUBLIC =====================================================================================
// ============================================================================================

subgraph_t::subgraph_t() 
#if MEMORY_OPTIMIZED == 0
  : internal_degrees_(0,0) 
#endif
{ }

// Ctor
subgraph_t::subgraph_t(std::set< id_t > nodes, bool connected) 
#if MEMORY_OPTIMIZED == 0
  : internal_degrees_(0,0) 
#endif
{
  // map ids
  for (auto it = nodes.begin(); it!=nodes.end();++it) {
    add_node(*it);
  }
  if (connected) fully_connect();
}

// Capacity
size_t subgraph_t::size_in_links() const {return links_.size();}
size_t subgraph_t::size_in_nodes() const {return id_map_.size();}

// Accessors
std::vector< unsigned int > subgraph_t::get_internal_degrees() const {
  #if MEMORY_OPTIMIZED == 0
    return internal_degrees_;
  #else 
    std::vector< unsigned int > degrees(size_in_nodes(),0);
    for (auto it = links_.begin(); it!=links_.end();++it) {
      ++degrees[it->first];
      ++degrees[it->second];
    }
    return degrees;
  #endif
}
std::set< edge_t > subgraph_t::get_local_links() const {return links_;}
std::multiset< edge_t > subgraph_t::get_global_links() const {
  std::multiset< edge_t > tmp;
  for (auto it = links_.begin();it!=links_.end();++it) {
    id_t node1 = get_global_id(it->first);
    id_t node2 = get_global_id(it->second);
    node1 <= node2 ? tmp.insert(std::make_pair(node1,node2)) : tmp.insert(std::make_pair(node2,node1));
  }
  return tmp;
}
id_t subgraph_t::get_global_id(id_t internal_node_id)  const {
  assert(node_exists(internal_node_id));
  return id_map_.at(internal_node_id); // throw exception rather than insert a new node;­
}
std::set<id_t> subgraph_t::get_global_ids() const {
  std::set<id_t> global_nodes;
  #if MEMORY_OPTIMIZED == 0
  for (auto it = nodes_.begin(); it!=nodes_.end(); ++it)
    global_nodes.insert(get_global_id(*it));
  #else
  for (auto it = id_map_.begin();it!=id_map_.end();++it)
    global_nodes.insert(get_global_id(it->second)); //marginally slower than the !MEMORY_OPTIMIZED method.
  #endif
  return global_nodes;
}
void subgraph_t::clear() {
  links_.clear();
  id_map_.clear();
  #if MEMORY_OPTIMIZED == 0
    internal_degrees_.clear();
    nodes_.clear();
  #endif
}

// Structural utilities
std::set<id_t> subgraph_t::link_creation_targets(id_t internal_node_id) {
  // if (links_.size() > 0) {
  #if MEMORY_OPTIMIZED == 0
    if (internal_degrees_[internal_node_id]==size_in_nodes()-1) return std::set<id_t>(); // early exit
  #endif

  std::set<id_t> known_nodes;
  known_nodes.insert(internal_node_id);
  for (id_t node = 0; node < internal_node_id; ++node) 
    if (links_.find(std::make_pair(node,internal_node_id))!=links_.end())
      known_nodes.insert(node);

  for (id_t node = internal_node_id+1; node < size_in_nodes(); ++node) 
    if (links_.find(std::make_pair(internal_node_id,node))!=links_.end())
      known_nodes.insert(node);

  #ifdef MEMORY_OPTIMIZED
    std::set<id_t> nodes_;
    for (id_t node = 0; node < size_in_nodes(); ++node)
      nodes_.insert(node);
  #endif

  std::set<id_t> targets;
  std::set_difference(nodes_.begin(),nodes_.end(),known_nodes.begin(),known_nodes.end(),std::inserter(targets,targets.end()));
  return targets;
  //   std::set<id_t> forbidden_nodes;
  //   forbidden_nodes.insert(internal_node_id);
  //   forbidden_nodes.insert(next_internal_id_);
  //   for (auto it = links_.begin(); it!=links_.end();++it) {
  //     if (it->first == internal_node_id) forbidden_nodes.insert(it->second);
  //     if (it->second == internal_node_id) forbidden_nodes.insert(it->first);
  //   }
  //   // build targets
  //   std::set<id_t> targets;
  //   auto it = forbidden_nodes.begin();
  //   for (id_t potential_target = 0; potential_target < next_internal_id_; ++potential_target) {
  //     if (potential_target < *it) targets.insert(potential_target);
  //     else ++it;
  //   }
  //   return targets;
  // }
  // else return std::set<id_t>();
}

id_t subgraph_t::add_node(id_t global_node_id) {
  // off by one: the new id is given by the current size in nodes
  id_map_[size_in_nodes()] = global_node_id;
  #if MEMORY_OPTIMIZED == 0
    internal_degrees_.push_back(0);
    nodes_.insert(size_in_nodes()-1);
  #endif
  return size_in_nodes()-1;
}
void subgraph_t::connect(id_t internal_node_id_1, id_t internal_node_id_2) {
  assert(node_exists(internal_node_id_1) && node_exists(internal_node_id_2));
  assert(internal_node_id_1 != internal_node_id_2);
  if (internal_node_id_1 < internal_node_id_2) {
    links_.insert(std::make_pair(internal_node_id_1,internal_node_id_2));
  }
  else {
    links_.insert(std::make_pair(internal_node_id_2,internal_node_id_1));
  }
  #if MEMORY_OPTIMIZED == 0
    ++internal_degrees_[internal_node_id_1];
    ++internal_degrees_[internal_node_id_2];
  #endif
  return;
}
void subgraph_t::fully_connect() {
  for (id_t node1 = 0; node1 < size_in_nodes(); ++node1) {
    for (id_t node2 = node1+1; node2 < size_in_nodes(); ++node2) {
      connect(node1,node2);
    }
  }
  return;
}

bool subgraph_t::node_exists(id_t internal_node_id) const {return internal_node_id < size_in_nodes();}