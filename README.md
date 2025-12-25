This was a simple C++ mini-project to learn more about different database types and when to use them (more details on this can be found on my [blog post](https://adeobam.com/blog/nosql-databases-cpp)). This project gave me hands on experience with Wide-Column Databases, Document Store Databases, and Graph Databases.

## Briefly what I learned

**Cassandra (Wide-Column Store)** Wide-Column have high-write throughoput amd are great for time-series data. The data in a wide-column store is distributed across nodes (machines with Cassandra installed) by a partition key with clustering keys used to sort the data within each partition. The combined use of partition key for querying and clustering keys for sorting the data makes it a very good option for sensor reading and stock tick data.

**MongoDB (Document Database)** - MongoDB is a document database and documnet databases are very flexible and hierarchical. These databases store JSON documents directly without creating or defining schemas. This is generally good for user profiles and their data, product catalogs and generally records of data.

**Neo4j (Graph Database)** - Graph Databases are exactly as they sound, they are graphs. These Databases are very good for highly connected data where relationships are the main thing that you query. Instead of JOINs, you traverse relationships directly by following pointers. The building block for this graph are nodes, that have properties and are connected to eachother via defined relationships. These are good for social networks, recommendation engines, and fraud detection.

## Running the Project

```bash
#Start the databases
docker-compose up -d

#Build
mkdir build && cd build
cmake ..
make

#Run
./db_explorer
```