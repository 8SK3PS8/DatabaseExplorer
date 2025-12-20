cmake_minimum_required(VERSION 3.16)
project(cpp_database_explorer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find required packages
find_package(PkgConfig REQUIRED)

# PostgreSQL
find_package(PostgreSQL REQUIRED)

# MongoDB C++ Driver
pkg_check_modules(MONGOCXX REQUIRED libmongocxx)

# Cassandra C++ Driver
pkg_check_modules(CASSANDRA REQUIRED cassandra-cpp-driver)

# Redis (hiredis)
pkg_check_modules(HIREDIS REQUIRED hiredis)

# Neo4j - uses REST API via libcurl
find_package(CURL REQUIRED)

# nlohmann/json for JSON handling (Neo4j REST API responses)
find_package(nlohmann_json REQUIRED)

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PostgreSQL_INCLUDE_DIRS}
    ${MONGOCXX_INCLUDE_DIRS}
    ${CASSANDRA_INCLUDE_DIRS}
    ${HIREDIS_INCLUDE_DIRS}
    ${CURL_INCLUDE_DIRS}
)

# Source files
set(SOURCES
    src/main.cpp
    src/postgres_demo.cpp
    src/mongodb_demo.cpp
    src/cassandra_demo.cpp
    src/redis_demo.cpp
    src/neo4j_demo.cpp
)

# Create executable
add_executable(db_explorer ${SOURCES})

# Link libraries
target_link_libraries(db_explorer
    ${PostgreSQL_LIBRARIES}
    ${MONGOCXX_LIBRARIES}
    ${CASSANDRA_LIBRARIES}
    ${HIREDIS_LIBRARIES}
    ${CURL_LIBRARIES}
    nlohmann_json::nlohmann_json
)