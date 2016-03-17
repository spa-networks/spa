#include "spa_algorithm.hpp"

void spa_star (double p, double q, double r, spa_network_t & network, std::mt19937& engine) {
  bool new_individual;
  std::uniform_real_distribution<double> rand_real(0,1);
  unsigned int quiet_size = 0;
  if (r*(1-p) >= 1) {
    LOGGER_WRITE(Logger::DEBUG,VAR_PRINT(r*(1-p))+" >= 1 : Links are occuring on a faster time-scale.")
    /** In this case, link occur on a fast time scale. 
      * One must set epsilon such that dL/dT is smaller than one and interpretable as a probability, i.e.
      *
      *  dL     n
      *  -- =  --- 
      *  dT     N
      *
      * with n the effective size of an average cluster, and N the total sizes.
      */
    double epsilon = 1/(r*(1-p));
    /** By definition,  a fraction epsilon \in [0,1] of events lead to SPA steps.
      * Since epsilon can get quite small, we sample from the distribution that the  
      * first non-link creation event occurs on the nth event (geometric distribution)
      * instead of throwing a dice for birth/growth event at each time step.
      *
      * The effective size ratio is respected if we allow link creation events to fail
      * when a fully connected node is selected.
      */
    std::geometric_distribution<int> geo_real(epsilon);
    while (network.size_in_nodes()<network.target_size()) {
      // create links due to steps where no cluster related event happen.  
      unsigned int number_of_link_creation_steps = geo_real(engine); 
      LOGGER_WRITE(Logger::DEBUG, "Geometric distribution result: "+VAR_EVAL(number_of_link_creation_steps)+" additional links must be created.")
      for (unsigned int i = 0; i < number_of_link_creation_steps; ++i) {
        LOGGER_WRITE(Logger::DEBUG, "link_creation_step:")
        auto event_ret = network.link_creation_step(engine);
        LOGGER_WRITE(Logger::DEBUG,"\tSUCCESS ? "+VAR_EVAL(std::get<3>(event_ret))+" || "+VAR_EVAL(std::get<0>(event_ret))+"<->"+VAR_EVAL(std::get<1>(event_ret))+" in cluster "+VAR_EVAL(std::get<2>(event_ret)))
      }

      // test for cluster birth or growth
      if (rand_real(engine) < p) {
        rand_real(engine) < q ?  new_individual = true : new_individual = false;
        LOGGER_WRITE(Logger::DEBUG, "cluster_birth with"+VAR_PRINT(new_individual))
        id_t event_ret = network.cluster_birth(new_individual,engine);
        LOGGER_WRITE(Logger::DEBUG, "\tcluster="+VAR_EVAL(event_ret))
      } 
      else { 
        rand_real(engine) < q ?  new_individual = true : new_individual = false;
        LOGGER_WRITE(Logger::DEBUG, "cluster_growth with "+VAR_PRINT(new_individual))
        auto event_ret = network.cluster_growth(new_individual,engine);
        LOGGER_WRITE(Logger::DEBUG,"\tnode="+VAR_EVAL(std::get<0>(event_ret))+" cluster="+VAR_EVAL(std::get<1>(event_ret)))
      }
      // normal link creation step
      LOGGER_WRITE(Logger::DEBUG, "link_creation_step:")
      auto event_ret = network.link_creation_step(engine);
      LOGGER_WRITE(Logger::DEBUG,"\tSUCCESS ? "+VAR_EVAL(std::get<3>(event_ret))+" || "+VAR_EVAL(std::get<0>(event_ret))+"<->"+VAR_EVAL(std::get<1>(event_ret))+" in cluster "+VAR_EVAL(std::get<2>(event_ret)))

      LOGGER_WRITE(Logger::DEBUG,"===============================")
      LOGGER_WRITE(Logger::DEBUG, "Current size:"+VAR_PRINT(network.size_in_nodes()))
      #if LOGGER_LEVEL < 3 
        if (network.size_in_nodes()%100==0 && quiet_size != network.size_in_nodes()) {
          LOGGER_WRITE(Logger::INFO, VAR_PRINT(network.size_in_nodes()))
          quiet_size = network.size_in_nodes();
        }
      #endif
    }
  }
  else {
    LOGGER_WRITE(Logger::DEBUG,VAR_PRINT(r*(1-p))+" < 1 : SPA-events are occuring on a faster time-scale.")
    /** In this case, link creation occur on a slow time scale and we set epsilon = 1, such that
      *
      *  dL     n
      *  -- =  --- * r * (1-p)
      *  dT     N
      *
      * The effective size ratio is respected if we allow link creation events to fail
      * when a fully connected node is selected.
      */
    while (network.size_in_nodes()<network.target_size()) {
      // test for cluster birth/growth
      if (rand_real(engine) < p) {
        rand_real(engine) < q ?  new_individual = true : new_individual = false;
        LOGGER_WRITE(Logger::DEBUG, "cluster_birth with"+VAR_PRINT(new_individual))
        id_t event_ret = network.cluster_birth(new_individual,engine);
        LOGGER_WRITE(Logger::DEBUG, "\tcluster="+VAR_EVAL(event_ret))
      } 
      else {
        rand_real(engine) < q ?  new_individual = true : new_individual = false;
        LOGGER_WRITE(Logger::DEBUG, "cluster_growth with "+VAR_PRINT(new_individual))
        auto event_ret = network.cluster_growth(new_individual,engine);
        LOGGER_WRITE(Logger::DEBUG,"\tnode="+VAR_EVAL(std::get<0>(event_ret))+" cluster="+VAR_EVAL(std::get<1>(event_ret)))
      }
      // test for link creation
      if (rand_real(engine)<r*(1-p)) {
        LOGGER_WRITE(Logger::DEBUG, "link_creation_step:")
        auto event_ret = network.link_creation_step(engine);
        LOGGER_WRITE(Logger::DEBUG,"\tSUCCESS ? "+VAR_EVAL(std::get<3>(event_ret))+" || "+VAR_EVAL(std::get<0>(event_ret))+"<->"+VAR_EVAL(std::get<1>(event_ret))+" in cluster "+VAR_EVAL(std::get<2>(event_ret)))
      }

      LOGGER_WRITE(Logger::DEBUG,"===============================")
      LOGGER_WRITE(Logger::DEBUG, "Current size:"+VAR_PRINT(network.size_in_nodes()))
      #if LOGGER_LEVEL < 3 
        if (network.size_in_nodes()%100==0 && quiet_size != network.size_in_nodes()) {
          LOGGER_WRITE(Logger::INFO, VAR_PRINT(network.size_in_nodes()))
          quiet_size = network.size_in_nodes();
        }
      #endif
    }
  }
  return;
}
void spa_basic(double p, double q, spa_network_t & network, std::mt19937& engine) {
  bool new_individual;
  std::uniform_real_distribution<double> rand_real(0,1);
  unsigned int quiet_size = 0;
  // untill the desired size is reached...
  while (network.size_in_nodes()<network.target_size()) {
    // test for cluster birth or growth
    if (rand_real(engine) < p) {
      rand_real(engine) < q ?  new_individual = true : new_individual = false;
      LOGGER_WRITE(Logger::DEBUG, "cluster_birth with"+VAR_PRINT(new_individual))
      id_t event_ret = network.cluster_birth(new_individual,engine);
      LOGGER_WRITE(Logger::DEBUG, "\tcluster="+VAR_EVAL(event_ret))
    } 
    else { 
      rand_real(engine) < q ?  new_individual = true : new_individual = false;
      LOGGER_WRITE(Logger::DEBUG, "cluster_growth with "+VAR_PRINT(new_individual))
      auto event_ret = network.cluster_growth(new_individual,engine);
      LOGGER_WRITE(Logger::DEBUG,"\tnode="+VAR_EVAL(std::get<0>(event_ret))+" cluster="+VAR_EVAL(std::get<1>(event_ret)))
    }

    LOGGER_WRITE(Logger::DEBUG,"===============================")
    LOGGER_WRITE(Logger::DEBUG, "Current size:"+VAR_PRINT(network.size_in_nodes()))
    #if LOGGER_LEVEL < 3 
        if (network.size_in_nodes()%100==0 && quiet_size != network.size_in_nodes()) {
          LOGGER_WRITE(Logger::INFO, VAR_PRINT(network.size_in_nodes()))
          quiet_size = network.size_in_nodes();
        }
    #endif
  }
  return;
}