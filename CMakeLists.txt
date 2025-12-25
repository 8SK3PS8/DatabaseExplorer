cmake_minimum_required(VERSION 3.16)
project(cpp_database_explorer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find packages
find_package(PkgConfig REQUIRED)
find_package(CURL REQUIRED)

# MongoDB C++ Driver
pkg_check_modules(MONGOCXX REQUIRED libmongocxx)
pkg_check_modules(BSONCXX REQUIRED libbsoncxx)

# nlohmann/json for Neo4j
find_package(nlohmann_json REQUIRED)

# Cassandra - find manually since no .pc file on Mac
find_path(CASSANDRA_INCLUDE_DIR cassandra.h
    HINTS /usr/local/opt/cassandra-cpp-driver/include
          /opt/homebrew/include
          /usr/local/include)

find_library(CASSANDRA_LIBRARY cassandra
    HINTS /usr/local/opt/cassandra-cpp-driver/lib
          /opt/homebrew/lib
          /usr/local/lib)

if(NOT CASSANDRA_INCLUDE_DIR OR NOT CASSANDRA_LIBRARY)
    message(FATAL_ERROR "Cassandra driver not found")
endif()

# Source files
set(SOURCES
    src/main.cpp
    src/mongodb_demo.cpp
    src/cassandra_demo.cpp
    src/neo4j_demo.cpp
)

# Create executable
add_executable(db_explorer ${SOURCES})

# Include directories
target_include_directories(db_explorer PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${MONGOCXX_INCLUDE_DIRS}
    ${BSONCXX_INCLUDE_DIRS}
    ${CASSANDRA_INCLUDE_DIR}
    ${CURL_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(db_explorer
    ${MONGOCXX_LIBRARIES}
    ${BSONCXX_LIBRARIES}
    ${CASSANDRA_LIBRARY}
    ${CURL_LIBRARIES}
    nlohmann_json::nlohmann_json
)