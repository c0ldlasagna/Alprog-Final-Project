CXX = g++
CXXFLAGS = -std=c++23 -Wall

CLIENT_SRC = client/main.cpp $(wildcard client/src/*.cpp) $(wildcard shared/src/*.cpp)
CLIENT_INC = -I ./client/include -I ./shared/include -I ./thirdparty/FTXUI/include/ -L ./thirdparty/FTXUI/lib/

SERVER_SRC = server/main.cpp $(wildcard server/src/*.cpp) $(wildcard shared/src/*.cpp)
SERVER_INC = -I ./shared/include -I ./server/include

all: build/client build/server

build/client: $(CLIENT_SRC)
	mkdir -p build
	$(CXX) $(CXXFLAGS) $(CLIENT_INC) -o $@ $(CLIENT_SRC) -lws2_32 -lftxui-component -lftxui-dom -lftxui-screen

build/server: $(SERVER_SRC)
	mkdir -p build
	$(CXX) $(CXXFLAGS) $(SERVER_INC) -o $@ $(SERVER_SRC) -lws2_32

clean:
	rm -rf build
