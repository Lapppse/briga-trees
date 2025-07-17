.DEFAULT_GOAL = all
PERFFLAGS := -O3
MEMFLAGS := -g -O1
ERRFLAGS := # -Wall -Wextra

all: tests gentest

tests: rb treap

treap: treapperf treapmem

treapperf:
	g++ ./src/treap.cpp -o ./bin/treap $(PERFFLAGS)

treapmem:
	g++ ./src/treap.cpp -o ./bin/treapmemtest $(MEMFLAGS)

rb: rbperf rbmem

rbperf:
	g++ ./src/rb.cpp -o ./bin/rb $(PERFFLAGS)

rbmem:
	g++ ./src/rb.cpp -o ./bin/rbmemtest $(MEMFLAGS)

gentest:
	g++ ./src/testgen.cpp -o ./bin/testgen -O3
