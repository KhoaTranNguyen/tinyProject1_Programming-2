CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -IC:/msys64/ucrt64/include/eigen3

SRC = cpu_prediction.cpp
TARGET = cpu_prediction

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	@echo "Cleaning up..."
	-del /Q $(TARGET).exe 2>nul || rm -f $(TARGET)