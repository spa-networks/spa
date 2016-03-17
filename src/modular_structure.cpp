#include "spa_network.hpp"

// ============================================================================================
// PROTECTED ==================================================================================
// ============================================================================================

// CTOR
modular_structure_t::modular_structure_t()  : rand_real_(0,1) {
  incidence_sum_ = 0;
}
modular_structure_t::modular_structure_t(size_t size_in_nodes, size_t size_in_clusters)  : rand_real_(0,1) {
  incidence_sum_ = 0;
  participations_.reserve(size_in_nodes);
  content_.reserve(size_in_clusters);
}


// ACCESSORS
// GET ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
size_t modular_structure_t::get_incidence_sum() const {
  return incidence_sum_;
}
id_t modular_structure_t::preferential_pick(std::string viewpoint, std::mt19937& engine) {
  assert(viewpoint.compare("node") == 0 || viewpoint.compare("cluster") == 0);
  if (viewpoint.compare("cluster") == 0) return preferential_pick(content_, engine);
  else return preferential_pick(participations_, engine);
}
// SET~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void modular_structure_t::set_size_in_nodes(size_t size_in_nodes) {
  participations_.resize(size_in_nodes);
  return;
}
void modular_structure_t::set_size_in_clusters(size_t size_in_clusters) {
  content_.resize(size_in_clusters);
  return;
}
//ABSTACT SET~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void modular_structure_t::add_node_to_cluster(id_t node, id_t cluster) {
  assert(node_exists(node) && cluster_exists(cluster));
  content_[cluster].insert(node);
  participations_[node].insert(cluster);
  ++incidence_sum_;
  return;
}
void modular_structure_t::add_cluster_to_node(id_t cluster, id_t node) {
  return add_node_to_cluster(node,cluster);
}

id_t modular_structure_t::new_node() {
  participations_.push_back(std::multiset<id_t>());
  return participations_.size()-1;
}
id_t modular_structure_t::new_node(std::multiset<id_t> participations) {
  #ifndef NDEBUG 
    // Explicit NDEBUG macro to avoid empty loops at compilation-time.
    for (auto cluster = participations.begin();cluster!=participations.end();++cluster) assert(cluster_exists(*cluster));
  #endif
  id_t new_node_id = new_node();
  for (auto cluster = participations.begin();cluster!=participations.end();++cluster) {add_node_to_cluster(new_node_id,*cluster); }
  return new_node_id;
}

id_t modular_structure_t::new_cluster() {
  content_.push_back(std::multiset<id_t>());
  return content_.size()-1;
}
id_t modular_structure_t::new_cluster(std::multiset<id_t> content) {
  #ifndef NDEBUG 
    // Explicit NDEBUG macro to avoid empty loops at compilation-time.
    for (auto node = content.begin();node!=content.end();++node) assert(node_exists(*node));
  #endif
  id_t new_cluster_id = new_cluster();
  for (auto node = content.begin();node!=content.end();++node) {add_node_to_cluster(*node, new_cluster_id);}
  return new_cluster_id;
}

void modular_structure_t::purge_modular_structure(size_t min_community_size) {
  for(id_t cluster = 0 ; cluster<content_.size();++cluster) {
    if (content_[cluster].size()<min_community_size) {
      for (auto it=content_[cluster].begin(); it!=content_[cluster].end(); ++it) {
        participations_[*it].erase(cluster);
      }
      content_[cluster].clear();
    }
  }
  return;
}

// Boolean tests
bool modular_structure_t::node_exists(id_t node) const {return node < size_in_nodes();}
bool modular_structure_t::cluster_exists(id_t cluster) const {return cluster < size_in_clusters();}


// ============================================================================================
// PUBLIC =====================================================================================
// ============================================================================================

// CAPACITY
size_t modular_structure_t::size_in_nodes() const {
  return participations_.size();
}
size_t modular_structure_t::size_in_clusters() const {
  return content_.size();
}
size_t modular_structure_t::membership(id_t node, bool unique) const {
  assert(node_exists(node));
  if (!unique) return participations_[node].size();
  else         return get_unique_participations(node).size();
}
size_t modular_structure_t::cluster_size(id_t cluster, bool unique) const {
  assert(cluster_exists(cluster));
  if (!unique)  return content_[cluster].size(); //direct ref to private member removes a copy operation.
  else          return get_unique_content(cluster).size();
}

// GET~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
std::multiset<id_t> modular_structure_t::get_participations(id_t node) const {
  assert(node_exists(node));
  return participations_[node];
}
std::multiset<id_t> modular_structure_t::get_content(id_t cluster) const {
  assert(cluster_exists(cluster));
  return content_[cluster];
}
std::set<id_t> modular_structure_t::get_unique_participations(id_t node) const {
  assert(node_exists(node));
  std::set<id_t> tmp;
  for (auto it = participations_[node].begin();it!=participations_[node].end();++it) {
    tmp.insert(*it);
  }
  return tmp;
}
std::set<id_t> modular_structure_t::get_unique_content(id_t cluster) const {
  assert(cluster_exists(cluster));
  std::set<id_t> tmp;
  for (auto it = content_[cluster].begin();it!=content_[cluster].end();++it) {
    tmp.insert(*it);
  }
  return tmp;
}


// ============================================================================================
// PRIVATE ====================================================================================
// ============================================================================================

// RNG-related.
id_t modular_structure_t::preferential_pick(std::vector< std::multiset<id_t> > & vec_of_multiset, std::mt19937& engine) {
  /* declarations */
  id_t pick=0;
  id_t local_count=0;
  id_t global_count=0;
  /* choose target "ticket" (# of ticket for node i prop. to. vec_of_multiset[i].size() */
  id_t target_idx = (id_t) ceil(rand_real_(engine)*(double)incidence_sum_);
  /* find the node to which the ticket belongs */
  do {
    if (local_count == vec_of_multiset[pick].size()) {
      local_count = 0;
      ++pick;
    }
    else {
      ++global_count;
      ++local_count;
    }
  } while (global_count!=target_idx);
  return pick;
}