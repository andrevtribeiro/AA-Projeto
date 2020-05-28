CC      := g++
CFLAGS  := -std=c++17 -O3 -Ofast -Wall -Wextra -Wunused -Wno-missing-declarations -fopenmp

BINARIES := utils.cpp

all: 
    $(BINARIES)

$(BINARIES): % : %.cpp
	$(CC) $(CFLAGS) $< -o $@