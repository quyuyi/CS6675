#include <opendht.h>
#include <opendht/peer_discovery.h>
#include <vector>
#include <mutex>
#include <memory>
#include <string>

// Using msgpack serialization
struct ExampleServiceData {
    in_port_t service_port;
    uint32_t some_id;
    std::string some_param;
    MSGPACK_DEFINE(service_port, some_id, some_param)
};
static const std::string MY_SERVICE_NAME = "my_service";
static constexpr in_port_t MULTICAST_PORT = 2222;


int main() {
    auto mtx = std::make_shared<std::mutex>();
    auto cv = std::make_shared<std::condition_variable>();
    auto ready = std::make_shared<bool>(false);

    /*** Sync heler function ***/
    auto wait = [=] {
        *ready = true;
        std::unique_lock<std::mutex> lk(*mtx);
        cv->wait(lk);
        *ready = false;
    };
    auto done_cb = [=](bool success) {
        if (success) {
            std::cout << "success!" << std::endl;
        } else {
            std::cout << "failed..." << std::endl;
        }
        std::unique_lock<std::mutex> lk(*mtx);
        cv->wait(lk, [=]{ return *ready; });
        cv->notify_one();
    };
    /*** End sync heler function ***/


    // Launch a dht node on a new thread, using a
    // generated RSA key pair, and listen on port 4222.
    dht::DhtRunner node;
    node.run(4222, dht::crypto::generateIdentity(), true);
    dht::DhtRunner node2;
    node2.run(4223, dht::crypto::generateIdentity(), true);
    // Join the network through any running node,
    // here using a known bootstrap node.
    node.bootstrap("bootstrap.ring.cx", "4222");
    node2.bootstrap("bootstrap.ring.cx", "4223");

    // peer discovery
    // A single instance & port can be used for both advertising and discovering of multiple different services
    // dht::PeerDiscovery discovery(MULTICAST_PORT);
    // ExampleServiceData my_data {4222, 1, "1"};
    // discovery.startPublish(MY_SERVICE_NAME, my_data);

    // dht::PeerDiscovery discovery2(MULTICAST_PORT);
    // ExampleServiceData my_data2 {4223, 2, "1"};
    // discovery2.startPublish(MY_SERVICE_NAME, my_data2);

    // discovery.startDiscovery<ExampleServiceData>(MY_SERVICE_NAME,
    // [](ExampleServiceData&& d, dht::SockAddr&& addr) {
    //     addr.setPort(d.service_port);
    //     std::cout << "Discovered some peer for our service at " << addr.toString() << std::endl;
    // });


    // sleep(10);

    // put some data on the dht
    std::vector<uint8_t> some_data(5, 10);

    node.put("unique_key", some_data, [=] (bool success) {
        std::cout << "Put: ";
        done_cb(success);
    });

    // blocking to see the result of put
    wait();

    // put some data on the dht, signed with our generated private key
    node.putSigned("unique_key_42", some_data, [=] (bool success) {
        std::cout << "Put: ";
        done_cb(success);
    });

    // blocking to see the result of put
    wait();

    // get data from the dht
    node.get("unique_key",
    [](const std::vector<std::shared_ptr<dht::Value>>& values) {
        // Callback called when values are found
        for (const auto& value : values)
            std::cout << "Found value: " << *value << std::endl;
        return true; // return false to stop the search
    },
    [=] (bool success) {
        std::cout << "Get: ";
        done_cb(success);
    });
    wait();

    // get data from the dht
    node.get("unique_key_42",
    [](const std::vector<std::shared_ptr<dht::Value>>& values) {
        // Callback called when values are found
        for (const auto& value : values)
            std::cout << "Found value: " << *value << std::endl;
        return true; // return false to stop the search
    },
    [=] (bool success) {
        std::cout << "Get: ";
        done_cb(success);
    });
    wait();

    while (true) {}

    // wait for dht threads to end
    node.join();
    return 0;
}