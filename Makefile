CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Wconversion
CP_LIBRARY = $(shell find -type f -name "*.hpp")

build/%: %.cpp $(CP_LIBRARY)
	mkdir -p build
	$(CXX) $(CXXFLAGS) -I. $< -o $@

help:
	@echo "To build xyz.cpp, enter 'make build/xyz'"

clean:
	rm -rf build
