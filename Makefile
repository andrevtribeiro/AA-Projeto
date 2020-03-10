g=graph

all:
	g++ -g -o karger karger.cpp

run:
	./karger $(g)