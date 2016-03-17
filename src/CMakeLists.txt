add_subdirectory(includes) 

if (Boost_FOUND)
    include_directories(${BOOST_INCLUDEDIR})
endif (Boost_FOUND)

add_executable(spa_networks  spa_main.cpp modular_structure.cpp spa_network.cpp subgraph.cpp spa_algorithm.cpp interface.cpp output_functions.cpp misc_functions.cpp)
target_link_libraries (spa_networks logger)
set_target_properties(spa_networks PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../ )

if (Boost_FOUND)
    target_link_libraries(spa_networks ${Boost_LIBRARIES})
endif (Boost_FOUND)

