# ────────────────────────────────────────────────────────────────
#  Makefile for TPCH Query 5  (C++17 + POSIX threads)
# ────────────────────────────────────────────────────────────────
#  • Compiles every .cpp file in the current folder
#  • Produces a single executable called “tpch_q5”
#  • Optimised with -O2 and linked with -pthread
#  • `make clean` removes the binary and any .o files
# ────────────────────────────────────────────────────────────────

CXX       = g++
CXXFLAGS  = -std=c++17 -O2 -Wall -Wextra -pthread
SRC       = $(wildcard *.cpp)
OUT       = tpch_q5

all: $(OUT)

# Link all .cpp files in one step
$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Optional target to build object files separately
objs: $(SRC:%.cpp=%.o)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OUT) *.o
