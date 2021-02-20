import opendht as dht
import time

node1 = dht.DhtRunner()
node1.run()
node2 = dht.DhtRunner()
node2.run()
node3 = dht.DhtRunner()
node3.run()
node4 = dht.DhtRunner()
node4.run()
node5 = dht.DhtRunner()
node5.run()

# Join the network through any running node,
# here using a known bootstrap node.
node1.bootstrap("bootstrap.jami.net", "4222")
node2.bootstrap("bootstrap.jami.net", "4223")
node3.bootstrap("bootstrap.jami.net", "4224")
node4.bootstrap("bootstrap.jami.net", "4225")
node5.bootstrap("bootstrap.jami.net", "4226")

# blocking call (provide callback arguments to make the call non-blocking)
for i in range(10):
    key_id = 1
    start = time.time()
    node1.put(dht.InfoHash.get("key{id}".format(id=key_id)), dht.Value(b'some binary data'))
    end = time.time()
    print("node1 put with key{id}, time elapsed: {t}".format(id=key_id, t=end-start))

for i in range(10):
    start = time.time()
    results = node2.get(dht.InfoHash.get("key{id}".format(id=key_id)))
    end = time.time()
    print("node2 get with key{id}, time elapsed: {t}".format(id=key_id, t=end-start))