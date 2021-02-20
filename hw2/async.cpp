#include <opendht.h>
#include <vector>
#include <iostream>
#include <stdio.h>
using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        perror("Usage: ./main <num_nodes>");
        exit(1);
    }
    int num_nodes = atoi(argv[1]);
    // vector<dht::DhtRunner*> nodes;
    // for (int i = 0; i < num_nodes; i++) {
    //     dht::DhtRunner *node = new dht::DhtRunner();
    //     node.run(4222, dht::crypto::generateIdentity(), true);
    //     nodes.push_back(node);
    // }


    dht::DhtRunner node;
    dht::DhtRunner node2;
    // launch a dht node on a new thread, using a generated RSA key pair, and listen on port 4222
    node.run(4222, dht::crypto::generateIdentity(), true);
    node2.run(4223, dht::crypto::generateIdentity(), true);

    // join the network through nay running node, here using a known bootstrap node
    node.bootstrap("bootstrap.jami.net", "4222");
    node2.bootstrap("bootstrap.jami.net", "4223");
    
    // put some data on the dht
    std::cout << "Putting data...\n";
    // std::vector<uint8_t> some_data(5, 10);
    // node.put("unique_key", some_data);
    node.put("key1", "hello from node 1");
    node2.put("key1", "hello from node 2");

    sleep(10);

    // put some data on the dht, signed with our generated private key
    // node.putSigned("unique_key_42", some_data);

    // get data from the dht
    node.get("key1", [](const std::vector<std::shared_ptr<dht::Value>>& values) {
        // callback called when values are found
        for (const auto& value : values) {
            std::string v {value->data.begin(), value->data.end()}; 
            std::cout << "Found value: " << v << std::endl;
        }
        return true; // return false to stop the search
    });

    while (true) {
        sleep(5);
    }
    // wait for dht threads to end
    node.join();
    node2.join();
    return 0;
}