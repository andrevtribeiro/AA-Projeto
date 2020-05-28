CC      := g++
CFLAGS  := -std=c++17 -O3 -Ofast -Wall -Wextra -Wunused -Wno-missing-declarations -fopenmp

BINARIES :=  main
UTILS := utils.cpp

all: 
	bc
	apl
    # +$(MAKE) -C structures/graph
	# $(CC) $(CFLAGS) ./bin/utils.o ./bin/main.o -o program

./bin/utils.o: ./structures/graph/utils.cpp
	$(CC) -c $(CFLAGS) $< -o $@

./bin/bc.o: ./bc.cpp
	$(CC) -c $(CFLAGS) $< -o $@

./bin/apl.o: ./apl.cpp
	$(CC) -c $(CFLAGS) $< -o $@

bc: ./bin/utils.o ./bin/bc.o
	$(CC) $(CFLAGS) ./bin/utils.o ./bin/bc.o -o $@

apl: ./bin/utils.o ./bin/apl.o
	$(CC) $(CFLAGS) ./bin/utils.o ./bin/apl.o -o $@

# $(BINARIES): % : %.cpp
# 	$(CC) $(CFLAGS) $< -o $@

clean:
	$(RM) bin/* ./apl ./bc 
