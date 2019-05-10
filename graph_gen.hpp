
#include <stdlib.h>

#define MAX_WEIGHT 15
#define MIN_WEIGHT 3

#define NEW_EDGE_CHANCE 60

class Graph {
	public:
	int **adj;
	int num_nodes;

	bool cycle_helper(int, int, int, int);

	public:
	Graph(int);
	Graph() = delete;

	~Graph() {
		for(int i = 0; i < num_nodes * num_nodes; ++i) {
			if(adj[i] != nullptr) {
				delete adj[i];
			}
		}

		delete adj;
	}

	int *&at(int, int);
	void randomize();
	//bool find_negative_cycle();
};
