#include "spa_network.hpp"

// ============================================================================================
// PUBLIC =====================================================================================
// ============================================================================================

//CTOR
spa_network_t::spa_network_t(size_t N, size_t s, bool handle_links)  : modular_structure_t(N,N), rand_real_(0,1) {
  // initialize parameters
  N_ = N;
  s_ = s;
  handle_links_ = handle_links;
  // initialize containers
  subgraphs_.clear();
  if (handle_links_) {
    subgraphs_.reserve(N);
  }
}

//CAPACITY
size_t spa_network_t::target_size() const { return N_; }
size_t spa_network_t::basic_size() const { return s_; }
void spa_network_t::expand_target_size(size_t N) { if (N>N_) {N_=N;} return; }

//ACCESSORS
subgraph_t spa_network_t::get_subgraph(id_t cluster) const {return subgraphs_[cluster];}
bool spa_network_t::set_handle_links_status(bool status) {
  if (get_incidence_sum()>0) return false;
  else {
    handle_links_=status;
    return true;
  }
}

//SPA OPERATIONS
void spa_network_t::initialize(unsigned int n) {
  for (unsigned int i = 0; i < n; ++i) disjoint_cluster_birth();
  return;
}

id_t spa_network_t::cluster_birth(bool new_individual, std::mt19937& engine) {
   /* Prepare the new structural cluster */
  std::multiset<id_t> tmp;   // content of the new cluster
  // choose the existing nodes preferentially
  while (tmp.size() < basic_size()-1) tmp.insert(preferential_pick("node",engine));
  // and the last node according to new_individual
  if (new_individual) tmp.insert(new_node());
  else tmp.insert(preferential_pick("node",engine));

  /* Structural cluster birth */
  id_t new_cluster_id = new_cluster(tmp);

  if (handle_links_) {
    /* Subgraph handling ( new connected subgraph ) */
    new_subgraph(new_cluster_id, true);
  }

  return new_cluster_id;
}
std::tuple<id_t, id_t> spa_network_t::cluster_growth(bool new_individual, std::mt19937& engine) {
  /* Prepare the growth event (structural viewpoint) */
  id_t cluster = preferential_pick("cluster",engine);
  id_t node;
  if (new_individual) node = new_node();
  else node = preferential_pick("node",engine);

  /* Perform the growth event */
  add_node_to_cluster(node,cluster);
  
  if (handle_links_) {
  // Subgraph handling (create the recruiting link)
    id_t target_node_internal_id = uniform_pick(subgraphs_[cluster].size_in_nodes(),engine);
    id_t node_internal_id = subgraphs_[cluster].add_node(node);
    subgraphs_[cluster].connect(target_node_internal_id,node_internal_id);  
  }
  return std::make_tuple(node, cluster);
}
std::tuple<id_t, id_t, id_t, bool> spa_network_t::link_creation_step(std::mt19937& engine) {
  if (handle_links_) {
    id_t cluster = preferential_pick("cluster", engine); // Choose cluster (preferentially to its size)
    id_t creating_node_internal_id = uniform_pick(subgraphs_[cluster].size_in_nodes(), engine); // Choose a link creating node
    id_t target_node_internal_id;
    std::set<id_t> targets_internal_id = subgraphs_[cluster].link_creation_targets(creating_node_internal_id);
    
    // we respect the effective size by allowing link creation steps to fail (target of size 0)
    if (targets_internal_id.size()!=0) {
      /* choose target node (uniform) */
      target_node_internal_id = uniform_pick(targets_internal_id,engine);
      /* connect */
      subgraphs_[cluster].connect(creating_node_internal_id,target_node_internal_id);
      return std::make_tuple(subgraphs_[cluster].get_global_id(creating_node_internal_id),
                             subgraphs_[cluster].get_global_id(target_node_internal_id), 
                             cluster, true);
    }
    return std::make_tuple(subgraphs_[cluster].get_global_id(creating_node_internal_id), 0, cluster, false);
  }
  else {
    // nothing happens if links are not handled explicitly.
    return std::make_tuple(0,0,0,false);
  }
}
void spa_network_t::purge(size_t min_community_size) {
  if (handle_links_) {
    for(id_t cluster = 0 ; cluster<size_in_clusters();++cluster) {
      if (cluster_size(cluster,true)<min_community_size) {
        subgraphs_[cluster].clear();
      }
    }
  }
  purge_modular_structure(min_community_size);
  return;
}


// ============================================================================================
// PRIVATE ====================================================================================
// ============================================================================================

//spa-related
id_t spa_network_t::disjoint_cluster_birth() {
  std::multiset<id_t> nodes;
  for (id_t new_node_id = 0; new_node_id < basic_size(); ++ new_node_id ) {
    nodes.insert(new_node());
  }
  id_t new_cluster_id =  new_cluster(nodes);

  if (handle_links_) {
    new_subgraph(new_cluster_id, true);
  }
  return new_cluster_id;
}

void spa_network_t::new_subgraph(id_t cluster, bool connected) {
  assert(cluster_exists(cluster));
  subgraphs_.push_back(subgraph_t(get_unique_content(cluster), connected));
  return;
}

id_t spa_network_t::uniform_pick(const std::set<id_t> & a_set, std::mt19937& engine)  {
  // safe, since rand_real_(0,1) excludes 1.
  id_t target_idx = (id_t) floor(rand_real_(engine)* (double) a_set.size());
  auto it=a_set.begin();
  std::advance(it,target_idx);
  return *it;
}
id_t spa_network_t::uniform_pick(unsigned int upper_bound, std::mt19937& engine)  {
  return (id_t) floor(rand_real_(engine)* (double) upper_bound);
}