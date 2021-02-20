CXX=g++
CXXFLAGS+=-std=c++14
LDFLAGS+=-lopendht

all: sync async

async: async.cpp
	$(CXX) $^ -o $@ $(CXXFLAGS) $(LDFLAGS)

sync: sync.cpp
	$(CXX) $^ -o $@ $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -f sync async