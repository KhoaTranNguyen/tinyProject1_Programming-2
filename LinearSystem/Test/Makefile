CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -IC:/msys64/ucrt64/include/eigen3

# You can add more test files here
TESTS = test1 test2

.PHONY: all clean $(TESTS)

all: $(TESTS)

$(TESTS):
	$(CXX) $(CXXFLAGS) $@.cpp -o $@

clean:
	@echo "Cleaning up..."
	-del /Q $(addsuffix .exe, $(TESTS)) 2>nul || rm -f $(TESTS)
# If you want to run all tests, you can use the following command
# make all
# If you want to clean up the generated files, you can use the following command
# make clean