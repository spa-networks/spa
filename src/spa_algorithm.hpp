#ifndef SPA_ALGORITHM_HPP
#define SPA_ALGORITHM_HPP

// Configuration file
#include "config.h"
// STL
#include <random>   //std::uniform_real_distribution and std::geometric_distribution
#include <tuple>    // return type for event 
// Project files
#include "spa_network.hpp" // spa_network_t
// Typdef and helpers
#include "includes/logger.hpp" // LOGGER macros
#include "types.hpp" // id_t


void spa_star (double p, double q, double r, spa_network_t & network, std::mt19937& engine);
void spa_basic(double p, double q, spa_network_t & network, std::mt19937& engine);

#endif //SPA_ALGORITHM_HPP