#include "misc_functions.hpp"

void create_edge_list(const spa_network_t & network, std::set<edge_t> & edge_list) {
  edge_list.clear();
  for (id_t cluster = 0 ; cluster < network.size_in_clusters(); ++ cluster) {
    std::multiset< edge_t > subgraph_edges = network.get_subgraph(cluster).get_global_links();
    for (auto it = subgraph_edges.begin(); it!= subgraph_edges.end(); ++it) {
      if (it->first != it-> second) edge_list.insert(*it);
    }
  }
  return;
}
void create_connected_edge_list(const spa_network_t & network,  std::set<edge_t> & connected_edge_list) {
  connected_edge_list.clear();
  for (id_t cluster = 0 ; cluster < network.size_in_clusters(); ++ cluster) {
    subgraph_t subgraph = network.get_subgraph(cluster);
    subgraph.fully_connect();
    std::multiset< edge_t > subgraph_edges = subgraph.get_global_links();
    for (auto it = subgraph_edges.begin(); it!= subgraph_edges.end(); ++it) {
      if (it->first != it-> second) connected_edge_list.insert(*it);
    }
  }
  return;
}
void create_projected_internal_degrees(const std::set< edge_t > edge_list, const spa_network_t & network, std::vector<std::vector<unsigned int> > & projected_internal_degrees) {
  projected_internal_degrees.clear();
  projected_internal_degrees.resize(network.size_in_clusters());
  for (id_t cluster = 0 ; cluster < network.size_in_clusters(); ++cluster) {
    LOGGER_WRITE(Logger::DEBUG,VAR_PRINT(cluster))
    // Fetch subgraph info
    std::multiset<edge_t> duplicate_edges = network.get_subgraph(cluster).get_global_links();
    std::set<id_t> nodes = network.get_subgraph(cluster).get_global_ids();
    std::map<id_t, id_t> global_to_local;
    

    // Keep a single copy of each internal edge 
    LOGGER_WRITE(Logger::DEBUG,"Keep a single copy of each internal edge.")
    std::set<edge_t> unique_edges;
    for (auto it = duplicate_edges.begin(); it!=duplicate_edges.end(); ++it) 
      unique_edges.insert(*it);
    duplicate_edges.clear();

    // Project the network onto the subgraph (and build global_to_local map)
    LOGGER_WRITE(Logger::DEBUG,"Project the network onto the subgraph")
    id_t id_counter = 0;
    for (auto it1 = nodes.begin(); it1 != nodes.end();++it1) {
      global_to_local[*it1] = id_counter;
      for (auto it2 = std::next(it1); it2 != nodes.end();++it2) {
        edge_t edge = *it1 < *it2 ? std::make_pair(*it1,*it2) : std::make_pair(*it2,*it1);
        if (edge_list.find(edge)!=edge_list.end()) {
          unique_edges.insert(edge);
        }
      }
      ++id_counter;
    }

    // Get degrees
    LOGGER_WRITE(Logger::DEBUG,"Get degrees")
    std::vector< unsigned int > degrees(nodes.size(),0);
    for (auto it = unique_edges.begin();it!=unique_edges.end();++it) {
      ++degrees[global_to_local.at(it->first)];
      ++degrees[global_to_local.at(it->second)];
    }

    projected_internal_degrees[cluster] = degrees;
  }
  return;
}
void create_collapsed_internal_degrees(const spa_network_t & network, std::vector<std::vector<unsigned int> > & collapsed_internal_degrees) {
  collapsed_internal_degrees.clear();
  collapsed_internal_degrees.resize(network.size_in_clusters());
  for (id_t cluster = 0 ; cluster < network.size_in_clusters(); ++cluster) {
    LOGGER_WRITE(Logger::DEBUG,VAR_PRINT(cluster))
    // Fetch subgraph info
    std::multiset<edge_t> duplicate_edges = network.get_subgraph(cluster).get_global_links();
    std::set<id_t> nodes = network.get_subgraph(cluster).get_global_ids();
    std::map<id_t, id_t> global_to_local;
    

    // Keep a single copy of each internal edge 
    LOGGER_WRITE(Logger::DEBUG,"Keep a single copy of each internal edge.")
    std::set<edge_t> unique_edges;
    for (auto it = duplicate_edges.begin(); it!=duplicate_edges.end(); ++it) 
      unique_edges.insert(*it);
    duplicate_edges.clear();


    // Build global_to_local map
    LOGGER_WRITE(Logger::DEBUG,"Build global_to_local map")
    id_t id_counter = 0;
    for (auto it1 = nodes.begin(); it1 != nodes.end();++it1) {
      global_to_local[*it1] = id_counter;
      ++id_counter;
    }

    // Get degrees
    LOGGER_WRITE(Logger::DEBUG,"Get degrees")
    std::vector< unsigned int > degrees(nodes.size(),0);
    for (auto it = unique_edges.begin();it!=unique_edges.end();++it) {
      ++degrees[global_to_local.at(it->first)];
      ++degrees[global_to_local.at(it->second)];
    }

    collapsed_internal_degrees[cluster] = degrees;
  }
  return;
}