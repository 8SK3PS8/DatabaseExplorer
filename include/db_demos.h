#ifndef DB_DEMOS_H
#define DB_DEMOS_H

#include <string>
#include <iostream>

void run_mongodb_demo();
void run_cassandra_demo();
void run_neo4j_demo();

inline void print_header(const std::string& db_name) {
    std::cout << "\n--- " << db_name << " ---\n";
}

inline void print_ok(const std::string& msg) {
    std::cout << "[OK] " << msg << "\n";
}

inline void print_err(const std::string& msg) {
    std::cerr << "[ERR] " << msg << "\n";
}

#endif