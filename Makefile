CXX       = g++
CXXFLAGS  = -std=c++17 -O2 -Wall -Wextra -pthread
SRC       = tpch_q5.cpp
OUT       = tpch_q5

all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -f $(OUT)
