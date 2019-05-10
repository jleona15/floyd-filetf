
floyd: graph_gen.o floyd.cpp
	g++ graph_gen.o floyd.cpp -g -o floyd

graph_gen.o: graph_gen.hpp graph_gen.cpp 
	g++ graph_gen.cpp -g -c -o graph_gen.o
