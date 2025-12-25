#include "db_demos.h"
#include <curl/curl.h> // this is for the http headers and requests for neo4j
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Neo4j connection info - it doesnt have a native C/CPP driver so we have to run our commands via http
// 7474 is the HTTP API port
// db/neo4j is the name of the db - it is the default db created by the docker compose for neo4j
const char* NEO4J_URL = "http://localhost:7474/db/neo4j/tx/commit";
const char* NEO4J_USER = "neo4j";
const char* NEO4J_PASS = "explorer123";

// Callback for libcurl - it calls this function as data arrives
static size_t write_callback(void* contents, size_t size, size_t nmemb, std::string* out) {
    out->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Helper to run a Cypher query and get the response
bool run_cypher(const std::string& query, std::string& response) {
    CURL* curl = curl_easy_init();
    if (!curl) return false;

    // Neo4j HTTP API expects JSON like:
    // {"statements": [{"statement": "MY CYPHER QUERY"}]}
    json payload = {{"statements", {{{"statement", query}}}}};
    std::string body = payload.dump();

    //Set up HTTP headers
    struct curl_slist* headers = curl_slist_append(nullptr, "Content-Type: application/json");

    //Configure the request
    curl_easy_setopt(curl, CURLOPT_URL, NEO4J_URL);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
    curl_easy_setopt(curl, CURLOPT_USERNAME, NEO4J_USER);
    curl_easy_setopt(curl, CURLOPT_PASSWORD, NEO4J_PASS);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    //Send the request
    CURLcode res = curl_easy_perform(curl);

    // Cleanup
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    return res == CURLE_OK;
}

void print_results(const std::string& response) {
    json result = json::parse(response);
    for (auto& row : result["results"][0]["data"]) {
        std::cout << "  ";
        for (auto& val : row["row"]) {
            std::cout << val << " ";
        }
        std::cout << "\n";
    }
}

void run_neo4j_demo() {
    print_header("Neo4j");

    std::string response;

    //Test the connection just make sure its working
    if (!run_cypher("RETURN 1", response)) {
        std::cout << "Cannot connect to Neo4js!" << std::endl;
        return;
    }
    std::cout << "Connected!" << std::endl;

    //Create a couple of Nodes
    //pass in statements to run_cypher
    run_cypher("CREATE (aj:Person {name: 'AJ', company:'Meta'})", response); // Create AJ, AJ is a Person with attributes Name and Company
    std::cout << "Created an A.J Node" << std::endl;

    run_cypher("CREATE (bob:Person {name:'Bob', company:'Google'})", response); // Create Bob, Bob is a Person with attributes Name and Company
    std::cout << "Created a Bob Node" << std::endl;

    run_cypher("CREATE (carol:Person {name: 'Carol', company:'Meta'})", response); // Create Carol, Carol is a person with attribuest name and company
    std::cout << "Created a Carol Node" << std::endl;

    run_cypher("CREATE (meta:Company {name: 'Meta'})", response);
    std::cout << "Created a Meta Node" << std::endl;


    // CREATE THE RELATIONSHIPS

    //AJ is friends with Bob Relationship
    //Match 2 nodes, Create the relationship with direction
    run_cypher(
        "MATCH (a:Person {name:'AJ'}), (b:Person {name:'Bob'})"
        "CREATE (a)-[:FRIENDS_WITH]->(b)"
    , response);
    std::cout << "Created a friendship between AJ and Bob" << std::endl;

    //Created a friendship relationship between Bob and Carol
    run_cypher(
        "MATCH (b:Person {name: 'Bob'}), (c:Person {name: 'Carol'}) "
        "CREATE (b)-[:FRIENDS_WITH]->(c)",
        response);
    std::cout << "Created a friendship between Carol and Bob" << std::endl;

    //lets create a relationship that AJ works at Meta
    run_cypher(
        "MATCH (a:Person {name:'AJ'}), (b:Company {name:'Meta'})"
        "CREATE (a)-[:WORKS_AT{since: 2024}]->(m)"
    , response); // the since is bc relationships can have properties
    std::cout << "Created a working relation between AJ and Meta" << std::endl;


    //example queries - ai created

    // Simple: Find AJ
    std::cout << "\n1. Find AJ:\n";
    response.clear();
    run_cypher("MATCH (p:Person {name: 'AJ'}) RETURN p.name, p.company", response);
    print_results(response);

    // Simple: Find all people
    std::cout << "\n2. All people:\n";
    response.clear();
    run_cypher("MATCH (p:Person) RETURN p.name", response);
    print_results(response);

    // Simple: Who is AJ friends with?
    std::cout << "\n3. AJ's friends:\n";
    response.clear();
    run_cypher(
        "MATCH (aj:Person {name: 'AJ'})-[:FRIENDS_WITH]->(friend) "
        "RETURN friend.name",
        response);
    print_results(response);

    // Medium: Where does AJ work and since when?
    std::cout << "\n4. Where AJ works:\n";
    response.clear();
    run_cypher(
        "MATCH (aj:Person {name: 'AJ'})-[r:WORKS_AT]->(company) "
        "RETURN company.name, r.since",
        response);
    print_results(response);

    // Medium: Friends of friends (AJ -> Bob -> Carol)
    std::cout << "\n5. Friends of friends of AJ:\n";
    response.clear();
    run_cypher(
        "MATCH (aj:Person {name: 'AJ'})-[:FRIENDS_WITH]->()-[:FRIENDS_WITH]->(fof) "
        "RETURN fof.name",
        response);
    print_results(response);

    // Medium: Find everyone who works at Meta (using company property on Person)
    std::cout << "\n6. People who work at Meta:\n";
    response.clear();
    run_cypher(
        "MATCH (p:Person {company: 'Meta'}) "
        "RETURN p.name",
        response);
    print_results(response);

    //cleanup
    run_cypher("MATCH (n) DETACH DELETE n", response);
    print_ok("Cleaned up");

}