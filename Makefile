CXX = g++
CXXFLAGS = -std=c++23 -Wall

CLIENT_SRC = client/main.cpp $(wildcard client/src/*.cpp) $(wildcard shared/src/*.cpp)
CLIENT_INC = -I ./client/include -I ./shared/include

SERVER_SRC = server/main.cpp $(wildcard server/src/*.cpp) $(wildcard shared/src/*.cpp)
SERVER_INC = -I ./shared/include -I ./server/include

all: client server

client: $(CLIENT_SRC)
	mkdir -p build
	$(CXX) $(CXXFLAGS) $(CLIENT_INC) -o build/client $(CLIENT_SRC) -lws2_32

server: $(SERVER_SRC)
	mkdir -p build
	$(CXX) $(CXXFLAGS) $(SERVER_INC) -o build/server $(SERVER_SRC) -lws2_32

clean:
	rm -rf build