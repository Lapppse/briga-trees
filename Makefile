.DEFAULT_GOAL = all
CC = g++
WFLAGS = -Wall -Wextra
CXXFLAGS = -std=c++20 -I ./include
PERFFLAGS := -O3
MEMFLAGS := -g -O1

all: tests testgen

tests: rb treap avl splay set

treap: treapperf treapmem

treapperf:
	$(CC) $(CXXFLAGS) ./src/treap.cpp -o ./build/treap $(PERFFLAGS)

treapmem:
	$(CC) $(CXXFLAGS) ./src/treap.cpp -o ./build/treapmemtest $(MEMFLAGS)

rb: rbperf rbmem

rbperf:
	$(CC) $(CXXFLAGS) ./src/rb.cpp -o ./build/rb $(PERFFLAGS)

rbmem:
	$(CC) $(CXXFLAGS) ./src/rb.cpp -o ./build/rbmemtest $(MEMFLAGS)

avl: avlperf avlmem

avlperf:
	$(CC) $(CXXFLAGS) ./src/avl.cpp -o ./build/avl $(PERFFLAGS)

avlmem:
	$(CC) $(CXXFLAGS) ./src/avl.cpp -o ./build/avlmemtest $(MEMFLAGS)

splay: splayperf splaymem

splayperf:
	$(CC) $(CXXFLAGS) ./src/splay.cpp -o ./build/splay $(PERFFLAGS)

splaymem:
	$(CC) $(CXXFLAGS) ./src/splay.cpp -o ./build/splaymemtest $(MEMFLAGS)

testgen:
	# $(CC) $(CXXFLAGS) ./src/adaptivetestgen.cpp -o ./build/tg $(PERFFLAGS)
	$(CC) $(CXXFLAGS) ./src/testgen.cpp -o ./build/tg $(PERFFLAGS)
	
set: setperf setmem

setperf:
	$(CC) $(CXXFLAGS) ./src/set.cpp -o ./build/set $(PERFFLAGS)

setmem:
	$(CC) $(CXXFLAGS) ./src/set.cpp -o ./build/setmemtest $(MEMFLAGS)

