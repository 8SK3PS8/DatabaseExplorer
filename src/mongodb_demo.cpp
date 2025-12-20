#include "db_demos.h"
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/json.hpp>

const char* MONGO_URI = "mongodb://explorer:explorer123@localhost:27017";
// This is the connection string - explorer is the username, expl123 is the password, localhost:27017 is connect to that port

void run_mongodb_demo()
{
    print_header("MongoDB");

    static mongocxx::instance instance{};

    try {
        mongocxx::client client{mongocxx::uri{MONGO_URI}};
        std::cout << "Conected" << std::endl;

        //in mongo if it doesnt exist it will create one of the cluster or the db
        auto db = client["testdb"]; // database
        auto users = db["users"]; // this is the collection

        users.delete_many({}); // delete all of the current data

        //inserting - writing a json directly
        users.insert_one(bsoncxx::from_json(R"({
            "name": "AJ",
            "age": 21,
            "school": "Texas A&M",
            "internships": [
                {"company": "Amazon", "year": 2023},
                {"company": "Meta", "year": 2024}
            ]
        })"));
        std::cout << "Inserted AJ" << std::endl;

        users.insert_one(bsoncxx::from_json(R"({
            "name": "Bob",
            "age": 25,
            "school": "UT Austin"
        })"));
        std::cout << "Inserted Bob" << std::endl;

        users.insert_one(bsoncxx::from_json(R"({
            "name": "Carol",
            "age": 23,
            "school": "Texas A&M",
            "internships": [
                {"company": "Google", "year": 2024}
            ]
        })"));
        std::cout << "Inserted Carol" << std::endl;

        // Queries

        //Find AJ
        std::cout << "1. Find AJ:" << std::endl;
        auto result = users.find_one(bsoncxx::from_json(R"({"name": "AJ"})"));
        if (result) {
            std::cout << "  " << bsoncxx::to_json(*result) << std::endl;
        }

        //Find all users
        std::cout << " 2. All users:" << std::endl;
        for (auto&& doc : users.find({})) {
            std::cout << "  " << bsoncxx::to_json(doc) << std::endl;
        }

        //Find users at Texas A&M
        std::cout << "\n3. Users at Texas A&M:\n";
        for (auto&& doc : users.find(bsoncxx::from_json(R"({"school": "Texas A&M"})"))) {
            std::cout << "  " << bsoncxx::to_json(doc) << std::endl;
        }

        //Find users older than 22
        std::cout << "\n4. Users older than 22:\n";
        for (auto&& doc : users.find(bsoncxx::from_json(R"({"age": {"$gt": 22}})"))) {
            std::cout << "  " << bsoncxx::to_json(doc) << std::endl;
        }

        //Find users who interned at Meta
        std::cout << "\n5. Users who interned at Meta:\n";
        for (auto&& doc : users.find(bsoncxx::from_json(R"({"internships.company": "Meta"})"))) {
            std::cout << "  " << bsoncxx::to_json(doc) << std::endl;
        }


        //update 

        std::cout << "\n6. Update AJ's age to 22:\n";
        users.update_one(
            bsoncxx::from_json(R"({"name": "AJ"})"),
            bsoncxx::from_json(R"({"$set": {"age": 22}})")
        ); // first arg is which doc to update, second arg is what change to make $set is what you use to make that change

    }
}