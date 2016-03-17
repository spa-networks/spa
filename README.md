# Structural Preferential Attachment Simulation
[![Build Status](https://travis-ci.org/spa-networks/spa.svg?branch=master)](https://travis-ci.org/spa-networks/spa) 
[![DOI](https://zenodo.org/badge/18575/spa-networks/spa.svg)](https://zenodo.org/badge/latestdoi/18575/spa-networks/spa)

## Table of Contents

1. [About](#about)
2. [Quick Start](#quick-start)
  1. [Compilation](#compilation)
    1. [Using CMake](#using-cmake)
    2. [Using GCC](#using-gcc)
  2. [Execution](#execution)
  3. [Output](#output)
3. [Papers](#papers)
  1. [Growth at the Level of Communities](#growth-at-the-level-of-communities)
  2. [Growth Within Communities](#growth-within-communities)
  3. [Hierarchical Growth](#hierarchical-growth)

## About:

This is a C++ implementation of the stochastic growth process dubbed Structural Preferential Attachment (SPA).
See [our official website](http://www.spa-networks.org/) for more informations.

## Quick Start

### Compilation


`spa_networks` uses features from C++11 and must therefore be compiled with `gcc-4.6` or `clang-3.7` (or a newer version).

#### Using CMake

Compiling with cmake should be straightforward:

    cmake .
    make

The available options for `cmake` are

* `OPTMEM_MODE`
* `HPC_MODE`
* `FORCE_NO_BOOST`

`OPTMEM_MODE` favors a small memory footprint over speed.

`HPC_MODE` silences all output and compiles assertions out.

The options can be passed to the usual GUI or through the command line interface:

    cmake -DOPTMEM_MODE=ON .
    make

The build [has been tested](https://travis-ci.org/spa-networks/spa) with `cmake-2.8` and `cmake-3.4`, on OSx and GNU/Linux, with `gcc` and `clang`.

#### Using GCC

The explicit compilation call below should work on most *nix systems

    g++ -o3 -W -Wall -Wextra -pedantic -std=c++0x  spa_main.cpp modular_structure.cpp spa_network.cpp subgraph.cpp spa_algorithm.cpp interface.cpp output_functions.cpp misc_functions.cpp includes/logger.cpp -lboost_program_options -o SPA

Compilations options can be manually adjusted in the [src/config.h](src/config.h) file.

### Execution

Multiple parameters must be set for each simulation of the SPA process.
These parameters can be passed through a configuration file 

    ./spa_networks -c path/to/config/file.txt

Note that `./spa_networks -g` generates an empty configuration file template.
If `boost::program_options` is installed and properly linked to the binary, parameters will also be accepted directly from the command line. See

    ./spa_networks -h 

for more information about each flags.

### Output

The results of a simulation can be found in the directory from which `spa_networks` is called.
This default behavior can be modified with the `-B / --base_path=` flag (by specifying a new absolute or relative path).
Note that **outputs are not computed unless specifically requested**.
So, for example, there won't be an edge list file, unless the user specifies a file name for the edge_list. e.g. `--edge_list=TestEdgeList.txt`.

## Papers:

More information about the SPA process can be found in the following papers.

### Growth at the Level of Communities:

* [Structural preferential attachment: Network organization beyond the link](http://arxiv.org/abs/1105.5980)
* [Structural preferential attachment: Stochastic process for the growth of scale-free, modular and self-similar systems](http://arxiv.org/abs/1109.0034)

### Growth Within Communities:

* A preferential attachment approach to community structure and the structure of communities (In submission)

### Hierarchical Growth:

* [Complex networks as an emerging property of hierarchical preferential attachment](http://arxiv.org/abs/1312.0171)
