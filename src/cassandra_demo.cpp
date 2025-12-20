#include "db_demos.h"
#include <cassandra.h>

void run_cassandra_demo() {
    print_header("Cassandra");

    //First we need to create the Cluster
    // The Cluster is like a warehouse where all of the Cassandra Nodes live
    CassCluster* cluster = cass_cluster_new();

    //The session for a cluster is the pool of tcp connections to nodes - kinda like the rds proxy - it knows the topology of the nodes (whats in what node), handles load balancing, and handles tcp connections to each node
    CassSession* session = cass_session_new();

    // This tells our code where the cassandra nodes are (the nodes are running from our docker-copmpose file)
    cass_cluster_set_contact_points(cluster, "127.0.0.1");
    cass_cluster_set_port(cluster, 9042);

    //Connect to the cluster
    CassFuture* connect = cass_session_connect(session, cluster); // this is async and can take some seconds so there are waits to wait until it finishes

    //quick check to see if the connection was successful or if it failed
    if(cass_future_error_code(connect) != CASS_OK)
    {
        const char* msg;
        size_t len;
        cass_future_error_message(connect, &msg, &len);
        print_err("Connect failed: " + std::string(msg, len));
    }
    cass_future_free(connect); // have to free cassFuture connect bc thge session_connect call actually connects it, the object just has the status of whether it connected or not
    std::cout << "Connected!" << std::endl;

    // Now its time to create a Keyspace (Database) on our Node
    const char* create_keyspace =  "CREATE KEYSPACE IF NOT EXISTS trading "
        "WITH replication = {'class': 'SimpleStrategy', 'replication_factor': 1}"; // this is the statement for creation, class SImple Strategy means [] replication factor is just the # of other nodes that have your data
    
    CassStatement* statement = cass_statement_new(create_keyspace, 0);
    CassFuture* executeStatus = cass_session_execute(session, statement); // this executes the statement and returns status of execution - session executes it and applies it to the entire cluster
    cass_statement_free(statement); // free the statement memory
    cass_future_free(executeStatus); // free the status memory
    std::cout << "Created the Keyspace (Database)" << std::endl;


    // Now lets create the table - symbol is the partition key - ts is the clustering key
    const char* create_table_statement =
        "CREATE TABLE IF NOT EXISTS trading.ticks ("
        "  symbol TEXT,"
        "  ts TIMESTAMP,"
        "  price DOUBLE,"
        "  volume INT,"
        "  PRIMARY KEY (symbol, ts)"
        ") WITH CLUSTERING ORDER BY (ts DESC)"; // this is really one string cpp just concatenates them together automatically

    statement = cass_statement_new(create_table_statement, 0);
    executeStatus = cass_session_execute(session, statement);
    cass_statement_free(statement);
    cass_future_free(executeStatus);
    std::cout << "Created the Table!" << std::endl;



    // Now we have to insert data into the table
    const char* insert = 
        "INSERT INTO trading.ticks (symbol, ts, price, volume) "
        "VALUES (?, toTimestamp(now()), ?, ?)";
    
    // Insert one row: AAPL, $150.50, 1000 shares
    statement = cass_statement_new(insert, 3);  // 3 = three placeholders
    cass_statement_bind_string(statement, 0, "AAPL");   // First ? = symbol
    cass_statement_bind_double(statement, 1, 150.50);   // Second ? = price
    cass_statement_bind_int32(statement, 2, 1000);      // Third ? = volume
    
    executeStatus = cass_session_execute(session, statement);
    cass_statement_free(statement);
    cass_future_free(executeStatus);
    print_ok("Inserted AAPL tick");

    // Insert another: GOOGL
    statement = cass_statement_new(insert, 3);
    cass_statement_bind_string(statement, 0, "GOOGL");
    cass_statement_bind_double(statement, 1, 140.25);
    cass_statement_bind_int32(statement, 2, 500);
    
    executeStatus = cass_session_execute(session, statement);
    cass_statement_free(statement);
    cass_future_free(executeStatus);
    print_ok("Inserted GOOGL tick");


    //Time to query data - cassandra does this by partition key
    const char* select = "SELECT symbol, price, volume FROM trading.ticks WHERE symbol = ?";
    
    statement = cass_statement_new(select, 1);  // 1 placeholder
    cass_statement_bind_string(statement, 0, "AAPL");
    
    executeStatus = cass_session_execute(session, statement);
    if (cass_future_error_code(executeStatus) == CASS_OK) 
    {
        std::cout << "ERROR" << std::endl;
    }
    cass_statement_free(statement);
    cass_future_free(executeStatus);

    //Final Cleanup
    
    // Drop table
    statement = cass_statement_new("DROP TABLE trading.ticks", 0);
    executeStatus = cass_session_execute(session, statement);
    cass_statement_free(statement);
    cass_future_free(executeStatus);
    print_ok("Dropped table");

    // Close session
    CassFuture* close_future = cass_session_close(session);
    cass_future_wait(close_future);
    cass_future_free(close_future);
    
    cass_session_free(session);
    cass_cluster_free(cluster);
    print_ok("Disconnected");



}