#include "db_demos.h"
#include <iostream>
#include <limits>

void print_menu() {
    std::cout << "\n=== C++ Database Explorer ===\n"
              << "1. Cassandra\n"
              << "2. MongoDB\n"
              << "3. Neo4j\n"
              << "4. Run All\n"
              << "0. Exit\n"
              << "Choice: ";
}

void run_all_demos() {
    std::cout << "\nRunning all database demos...\n";
    run_cassandra_demo();
    run_mongodb_demo();
    run_neo4j_demo();
    std::cout << "\nAll demos completed!\n";
}

int main() {
    int choice;
    
    while (true) {
        print_menu();
        
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        
        switch (choice) {
            case 0:
                std::cout << "Goodbye!\n";
                return 0;
            case 1:
                run_cassandra_demo();
                break;
            case 2:
                run_mongodb_demo();
                break;
            case 3:
                run_neo4j_demo();
                break;
            case 4:
                run_all_demos();
                break;
            default:
                std::cout << "Invalid option.\n";
        }
    }
    
    return 0;
}