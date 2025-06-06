CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -O2
INCLUDES := -Iinclude
SRC := $(wildcard src/*.cpp)
LIB_OBJ := $(SRC:src/%.cpp=src/%.o)

all: app_1.exe app_2.exe

app_1.exe: $(LIB_OBJ) main1.o
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

app_2.exe: $(LIB_OBJ) main2.o
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@echo "Cleaning..."
	@rm -f $(LIB_OBJ) main1.o main2.o app_1.exe app_2.exe

