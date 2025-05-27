CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -IC:/msys64/ucrt64/include/eigen3

SRCS = Matrix/Matrix.cpp Vector/Vector.cpp test.cpp

test:
	$(CXX) $(CXXFLAGS) $(SRCS) -o $@

clean:
	rm -f test
