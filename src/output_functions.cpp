#include "output_functions.hpp"

void process_and_output(spa_network_t & network, const spa_parameters_t & param, const double duration) {
  // Output must follow a very strict order: 
  //  1) Edge list / degrees / projected internal degrees 
  //  2) Connected edge list / connected degrees
  //  3) Internal degrees
  //  4) Cluster contents / sizes and node participations / memberships
  // This is motivated by the fact that we alter the network structure 
  // and build / destroy temporary object as we output the results of the 
  // simulation to optimize the speed AND memory usage of the program.
  //
  // Detailed explanation:
  // 
  //  Degrees rely on the edge list. Therefore, the latter must not be destroyed 
  //  prior to the degree output.
  //  Moreover, projected internal degrees also rely on the edge list. Hence,
  //  it is preferable to process these 3 files at the same time.
  // 
  //  Similarly, connected degrees rely on the connected edge list, such that 
  //  these 2 outputs are somewhat tied together. 
  //  Note that projected and / or complete internal degrees are of no interest
  //  since they are trivially equal to the size minus 1.
  //
  //  Internal degrees (not projected, nor connected) do not depend on the edge 
  //  list and are built directly from the subgraph objects. Therefore, this
  //  output is not tied to any other output.
  // 
  //  Community structure information must come last, because a "purge" step 
  //  is performed, during which communities smaller than a certain size are 
  //  removed. The rational is that these communities shape the connectivity
  //  of the network, bust cannot be detected by algorithms with a given 
  //  minimal resolution. Thus, for comparison purposes, they must be taken 
  //  into account when constructing the connected edge list, but ignored 
  //  when it comes to community structure (i.e cluster contents, node 
  //  participations)­.

  bool app = param.append.value;

  // Edge list and degrees
  std::set< edge_t > edge_list;
  if (param.edge_list.initialized || param.degrees.initialized || param.connected_degrees.initialized) {
    LOGGER_WRITE(Logger::INFO, "Build edge list.")
    create_edge_list(network,edge_list);
  }
  if (param.edge_list.initialized) {
    if ( param.edge_list.value.compare("") == 0 ) {
      LOGGER_WRITE(Logger::INFO, "Output edge list to the stdout.")
      output_edge_list(edge_list, std::cout);
    }
    else {
       LOGGER_WRITE(Logger::INFO, "Output edge list to path "+(param.base_path.value + param.edge_list.value)+" with append="+VAR_EVAL(app)+".")
       bofstream stream( (param.base_path.value+param.edge_list.value).c_str(), app ? std::ios::app : std::ios::out);
       output_edge_list(edge_list,stream);
    }
  }
  if (param.degrees.initialized) {
    if ( param.degrees.value.compare("") == 0 ) {
      LOGGER_WRITE(Logger::INFO, "Output degrees to the stdout.")
      output_degrees(network, edge_list, std::cout);
    }
    else {
       LOGGER_WRITE(Logger::INFO, "Output degrees to path "+(param.base_path.value + param.degrees.value)+" with append="+VAR_EVAL(app)+".")
       bofstream stream( (param.base_path.value+param.degrees.value).c_str(), app ? std::ios::app : std::ios::out);
       output_degrees(network, edge_list,stream);
    }
  }
  // Projected internal degrees (require edge list)
  if (param.projected_internal_degrees.initialized) {
    LOGGER_WRITE(Logger::INFO, "Build projected internal degrees.")
    std::vector<std::vector<unsigned int> > projected_internal_degrees;
    create_projected_internal_degrees(edge_list,network,projected_internal_degrees);
    if ( param.projected_internal_degrees.value.compare("") == 0 ) {
      LOGGER_WRITE(Logger::INFO, "Output projected internal degrees to the stdout.")
      output_projected_internal_degrees(projected_internal_degrees, std::cout);
    }
    else {
       LOGGER_WRITE(Logger::INFO, "Output projected internal degrees to path "+(param.base_path.value + param.projected_internal_degrees.value)+" with append="+VAR_EVAL(app)+".")
       bofstream stream( (param.base_path.value+param.projected_internal_degrees.value).c_str(), app ? std::ios::app : std::ios::out);
       output_projected_internal_degrees(projected_internal_degrees,stream);
    }
  }
  if (param.collapsed_internal_degrees.initialized) {
    LOGGER_WRITE(Logger::INFO, "Build collapsed internal degrees.")
    std::vector<std::vector<unsigned int> > collapsed_internal_degrees;
    create_collapsed_internal_degrees(network,collapsed_internal_degrees);
    if ( param.collapsed_internal_degrees.value.compare("") == 0 ) {
      LOGGER_WRITE(Logger::INFO, "Output collapsed internal degrees to the stdout.")
      output_collapsed_internal_degrees(collapsed_internal_degrees, std::cout);
    }
    else {
       LOGGER_WRITE(Logger::INFO, "Output collapsed internal degrees to path "+(param.base_path.value + param.collapsed_internal_degrees.value)+" with append="+VAR_EVAL(app)+".")
       bofstream stream( (param.base_path.value+param.collapsed_internal_degrees.value).c_str(), app ? std::ios::app : std::ios::out);
       output_collapsed_internal_degrees(collapsed_internal_degrees,stream);
    }
  }

  edge_list.clear();

  // Internal degrees (do not require edge list)
  if (param.internal_degrees.initialized) {
    if ( param.internal_degrees.value.compare("") == 0 ) {
      LOGGER_WRITE(Logger::INFO, "Output internal degrees to the stdout.")
      output_internal_degrees(network, std::cout);
    }
    else {
       LOGGER_WRITE(Logger::INFO, "Output internal degrees to path "+(param.base_path.value + param.internal_degrees.value)+" with append="+VAR_EVAL(app)+".")
       bofstream stream( (param.base_path.value+param.internal_degrees.value).c_str(), app ? std::ios::app : std::ios::out);
       output_internal_degrees(network,stream);
    }
  }


  // Connected edge list and connected degrees
  std::set< edge_t > connected_edge_list;
  if (param.connected_edge_list.initialized || param.connected_degrees.initialized) {
    LOGGER_WRITE(Logger::INFO, "Build connected edge list.")
    create_connected_edge_list(network,connected_edge_list);
  }
  if (param.connected_edge_list.initialized) {
    if ( param.connected_edge_list.value.compare("") == 0 ) {
      LOGGER_WRITE(Logger::INFO, "Output connected edge list to the stdout.")
      output_connected_edge_list(connected_edge_list, std::cout);
    }
    else {
       LOGGER_WRITE(Logger::INFO, "Output connected edge list to path "+(param.base_path.value + param.connected_edge_list.value)+" with append="+VAR_EVAL(app)+".")
       bofstream stream( (param.base_path.value+param.connected_edge_list.value).c_str(), app ? std::ios::app : std::ios::out);
       output_connected_edge_list(connected_edge_list,stream);
    }
  }
  if (param.connected_degrees.initialized) {
    if ( param.connected_degrees.value.compare("") == 0 ) {
      LOGGER_WRITE(Logger::INFO, "Output connected degrees to the stdout.")
      output_connected_degrees(network, connected_edge_list, std::cout);
    }
    else {
       LOGGER_WRITE(Logger::INFO, "Output connected connected to path "+(param.base_path.value + param.connected_degrees.value)+" with append="+VAR_EVAL(app)+".")
       bofstream stream( (param.base_path.value+param.connected_degrees.value).c_str(), app ? std::ios::app : std::ios::out);
       output_connected_degrees(network, connected_edge_list,stream);
    }
  }
  connected_edge_list.clear();
  

  
  // Purge step 
  LOGGER_WRITE(Logger::INFO, "Ignore communities smaller than "+VAR_PRINT(param.minimal_size.value)+" nodes.")
  network.purge(param.minimal_size.value);

  // // Straightforward outputs
  if (param.cluster_contents.initialized) {
    if ( param.cluster_contents.value.compare("") == 0 ) {
      LOGGER_WRITE(Logger::INFO, "Output cluster contents to the stdout.")
      output_cluster_contents(network, std::cout);
    }
    else {
       LOGGER_WRITE(Logger::INFO, "Output cluster contents to path "+(param.base_path.value + param.cluster_contents.value)+" with append="+VAR_EVAL(app)+".")
       bofstream stream( (param.base_path.value+param.cluster_contents.value).c_str(), app ? std::ios::app : std::ios::out);
       output_cluster_contents(network,stream);
    }
  }
  if (param.cluster_sizes.initialized) {
    if ( param.cluster_sizes.value.compare("") == 0 ) {
      LOGGER_WRITE(Logger::INFO, "Output cluster sizes to the stdout.")
      output_cluster_sizes(network, std::cout);
    }
    else {
       LOGGER_WRITE(Logger::INFO, "Output cluster sizes to path "+(param.base_path.value + param.cluster_sizes.value)+" with append="+VAR_EVAL(app)+".")
       bofstream stream( (param.base_path.value+param.cluster_sizes.value).c_str(), app ? std::ios::app : std::ios::out);
       output_cluster_sizes(network,stream);
    }
  }
  if (param.node_participations.initialized) {
    if ( param.node_participations.value.compare("") == 0 ) {
      LOGGER_WRITE(Logger::INFO, "Output node participations to the stdout.")
      output_node_participations(network, std::cout);
    }
    else {
       LOGGER_WRITE(Logger::INFO, "Output node participations to path "+(param.base_path.value + param.node_participations.value)+" with append="+VAR_EVAL(app)+".")
       bofstream stream( (param.base_path.value+param.node_participations.value).c_str(), app ? std::ios::app : std::ios::out);
       output_node_participations(network,stream);
    }
  }
  if (param.node_memberships.initialized) {
    if ( param.node_memberships.value.compare("") == 0 ) {
      LOGGER_WRITE(Logger::INFO, "Output node memberships to the stdout.")
      output_node_memberships(network, std::cout);
    }
    else {
       LOGGER_WRITE(Logger::INFO, "Output node memberships to path "+(param.base_path.value + param.node_memberships.value)+" with append="+VAR_EVAL(app)+".")
       bofstream stream( (param.base_path.value+param.node_memberships.value).c_str(), app ? std::ios::app : std::ios::out);
       output_node_memberships(network,stream);
    }
  }
  if (param.timer.initialized) {
    if ( param.timer.value.compare("") == 0 ) {
      LOGGER_WRITE(Logger::INFO, "Output timer to the stdout.")
      output_timer(duration, std::cout);
    }
    else {
       LOGGER_WRITE(Logger::INFO, "Output timer to path "+(param.base_path.value + param.timer.value)+" with append="+VAR_EVAL(app)+".")
       bofstream stream( (param.base_path.value+param.timer.value).c_str(), app ? std::ios::app : std::ios::out);
       output_timer(duration,stream);
    }
  }
  return;
}
// Generic functions (Helpers) ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// could be converted to templates
void output_vector_uint_content(const std::vector<unsigned int> & uint_vec, std::ostream& os, std::string separator) {
  for (id_t idx = 0; idx < uint_vec.size(); ++idx) 
    os << uint_vec[idx] << separator;
  if (separator.compare(" ")==0) os << "\n";
}
void output_multiset_id_content(const std::multiset<id_t> & id_multiset, std::ostream& os, std::string separator) {
  for ( auto it = id_multiset.begin() ; it!=id_multiset.end() ; ++it ) 
    os << *it << separator;
  if (separator.compare(" ")==0) os << "\n";
  return;
}
void output_multiset_id_size(const std::multiset<id_t> & id_multiset, std::ostream& os) {
  os << id_multiset.size() <<"\n";
  return;
}
void output_set_id_size(const std::set<id_t> & id_multiset, std::ostream& os) {
  os << id_multiset.size() <<"\n";
  return;
}

// Processed data output ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void output_edge_list(const std::set< edge_t > & edge_list, std::ostream& os) {
  for (auto it = edge_list.begin(); it!=edge_list.end(); ++it)
    os << it->first << " " << it->second << "\n";
  return;
}
void output_connected_edge_list(const std::set< edge_t > & connected_edge_list, std::ostream& os) {
  for (auto it = connected_edge_list.begin(); it!=connected_edge_list.end(); ++it)
    os << it->first << " " << it->second << "\n";
  return;
}
void output_degrees(const spa_network_t & network, const std::set< edge_t > & edge_list, std::ostream& os) {
  // build degrees
  std::vector<unsigned int> degrees(network.size_in_nodes(),0);
  for (auto it = edge_list.begin(); it!=edge_list.end();++it) {
    degrees[it->first]+=1;
    degrees[it->second]+=1;
  }
  // output
  output_vector_uint_content(degrees,os,"\n");
  return;
}
void output_connected_degrees(const spa_network_t & network, const std::set< edge_t > & connected_edge_list, std::ostream& os) {
  // build degrees
  std::vector<unsigned int> connected_degrees(network.size_in_nodes(),0);
  for (auto it = connected_edge_list.begin(); it!=connected_edge_list.end();++it) {
    connected_degrees[it->first]+=1;
    connected_degrees[it->second]+=1;
  }
  // output
  output_vector_uint_content(connected_degrees,os,"\n");
  return;
}
void output_timer(const double duration, std::ostream& os) {os << duration << "\n";}
void output_projected_internal_degrees(const std::vector<std::vector<unsigned int> > & projected_internal_degrees, std::ostream& os) {
  for (id_t cluster = 0; cluster < projected_internal_degrees.size(); ++cluster)
    output_vector_uint_content(projected_internal_degrees[cluster],os," ");
  return;
}
void output_collapsed_internal_degrees(const std::vector<std::vector<unsigned int> > & collapsed_internal_degrees, std::ostream& os) {
  for (id_t cluster = 0; cluster < collapsed_internal_degrees.size(); ++cluster)
    output_vector_uint_content(collapsed_internal_degrees[cluster],os," ");
  return;
}

// Straightforward outputs ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void output_cluster_contents(const spa_network_t & network, std::ostream& os) {
  for (id_t cluster = 0; cluster < network.size_in_clusters(); ++cluster) 
    output_multiset_id_content(network.get_content(cluster),os," ");
  return;
}
void output_cluster_sizes(const spa_network_t & network, std::ostream& os) {
  for (id_t cluster = 0; cluster < network.size_in_clusters(); ++cluster) 
    output_multiset_id_size(network.get_content(cluster),os);
  return;
}
void output_node_participations(const spa_network_t & network, std::ostream& os) {
  for (id_t node = 0; node < network.size_in_nodes(); ++node) 
    output_multiset_id_content(network.get_participations(node),os," ");
  return;
}
void output_node_memberships(const spa_network_t & network, std::ostream& os)  {
  for (id_t node = 0; node < network.size_in_nodes(); ++node) 
    output_multiset_id_size(network.get_participations(node),os);
  return;
}
void output_internal_degrees(const spa_network_t & network, std::ostream& os) {
  for (id_t cluster = 0; cluster < network.size_in_clusters(); ++cluster)
    output_vector_uint_content(network.get_subgraph(cluster).get_internal_degrees(),os," ");
  return;
}