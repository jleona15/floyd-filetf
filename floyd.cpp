#include <time.h>
#include <iostream>
#include <fstream>
#include "graph_gen.hpp"

#define NUM_GRAPHS 100
#define START_SIZE 3
#define DBG_PRINT 1
#define TIME_PRINT 0
#define TEST 1

void bf_recur(Graph *g, int **arr, int orig_index, int curr_index, int graph_size, int curr_weight);

int **brute_force(Graph *g, int graph_size) {
	int **ret = (int **)malloc(sizeof(int *) * graph_size * graph_size);
	for(int i = 0; i < graph_size; ++i) {
		for(int j = 0; j < graph_size; ++j) {
			if(g->at(i, j) != nullptr) {
				ret[i * graph_size + j] = new int(*g->at(i, j));
			} else {
				ret[i * graph_size + j] = nullptr;
			}
		}
	}

	for(int i = 0; i < graph_size; ++i) {
		bf_recur(g, ret, i, i, graph_size, 0);
	}

	return ret;
}

void bf_recur(Graph *g, int **arr, int orig_index, int curr_index, int graph_size, int curr_weight) {
	if((arr[orig_index * graph_size + curr_index] == nullptr) || (*arr[orig_index * graph_size + curr_index] >= curr_weight)) {
		if(arr[orig_index * graph_size + curr_index] == nullptr) {
			arr[orig_index * graph_size + curr_index] = new int(curr_weight);
		} else {
			*arr[orig_index * graph_size + curr_index] = curr_weight;
		}

		for(int i = 0; i < graph_size; ++i) {
			if((g->at(curr_index, i) != nullptr) && (i != curr_index) && (i != orig_index)) {
				bf_recur(g, arr, orig_index, i, graph_size, curr_weight + *g->at(curr_index, i));
			}
		}
	}
}

int **floyd(Graph *g, int graph_size) {
	int **ret = (int **)malloc(sizeof(int *) * graph_size * graph_size);
	for(int i = 0; i < graph_size; ++i) {
		for(int j = 0; j < graph_size; ++j) {
			if(g->at(i, j) != nullptr) {
				ret[i * graph_size + j] = new int(*g->at(i, j));
			} else {
				ret[i * graph_size + j] = nullptr;
			}
		}
	}

	for(int k = 0; k < graph_size; ++k) {
		for(int i = 0; i < graph_size; ++i) {
			for(int j = 0; j < graph_size; ++j) {
				if(!((ret[k * graph_size + j] == nullptr) || (ret[i * graph_size + k] == nullptr))) {
					if((ret[i * graph_size + j] == nullptr) || (*ret[k * graph_size + j] + *ret[i * graph_size + k] < *ret[i * graph_size + j])) {
						if(ret[i * graph_size + j] == nullptr) {
							ret[i * graph_size + j] = new int(*ret[k * graph_size + j] + *ret[i * graph_size + k]);
						} else {
							*ret[i * graph_size + j] = *ret[k * graph_size + j] + *ret[i * graph_size + k];
						}
					}
				}
			}
		}
	}

	return ret;
}

int **dijkstras(Graph *g, int graph_size) {
	int **ret = (int **)malloc(sizeof(int *) * graph_size * graph_size);
	bool *visited = (bool *)malloc(sizeof(bool *) * graph_size);
	for(int i = 0; i < graph_size; ++i) {
		for(int j = 0; j < graph_size; ++j) {
			if(i == j) {
				ret[i * graph_size + j] = new int(0);
			} else {
				ret[i * graph_size + j] = nullptr;
			}
		}
	}

	for(int i = 0; i < graph_size; ++i) {
		for(int j = 0; j < graph_size; ++j) {
			visited[j] = false;
		}

		bool all_visited = false;

		int *min = nullptr;
		int min_index = -1;

		while(!all_visited) {
			min_index = -1;
			min = nullptr;
			all_visited = true;
			for(int j = 0; j < graph_size; ++j) {
				if(!visited[j] && ret[i * graph_size + j] != nullptr) {
					if(min == nullptr || *ret[i * graph_size + j] < *min) {
						min = ret[i * graph_size + j];
						min_index = j;
					}
					all_visited = false;
				}
			}

			if(!all_visited) {
				visited[min_index] = true;

				for(int j = 0; j < graph_size; ++j) {
					if(g->at(min_index, j) != nullptr && (ret[i * graph_size + j] == nullptr || *g->at(min_index, j) + *ret[i * graph_size + min_index] < *ret[i * graph_size + j])) {
						if(ret[i * graph_size + j] == nullptr) {
							ret[i * graph_size + j] = new int(*g->at(min_index, j) + *ret[i * graph_size + min_index]);
						} else {
							*ret[i * graph_size + j] = *g->at(min_index, j) + *ret[i * graph_size + min_index];
						}
					}
				}
			}
		}
	}

	free(visited);

	return ret;
}

void print_matrix(int **arr, int graph_size, std::ofstream &fout) {
	for(int i = 0; i < graph_size; ++i) {
		for(int j = 0; j < graph_size; ++j) {
			if(arr[i * graph_size + j] == nullptr) {
				fout << "NULL\t";
			} else {
				fout << *arr[i * graph_size + j] << '\t';
			}
		}

		fout << '\n';
	} 
}

int main(void) {
	//srand(time(NULL));
	Graph *g;
	clock_t begin;
	clock_t end;

	int **arr;

	std::ofstream fout;

	if(TEST == 0) {
		fout.open("brute_force.txt");
	} else if (TEST == 1) {
		fout.open("floyd.txt");
	} else {
		fout.open("dijkstras.txt");
	}

	for(int i = START_SIZE; (i < NUM_GRAPHS + START_SIZE) || (NUM_GRAPHS == -1); ++i) {
		g = new Graph(i);

		begin = clock();

		if(TEST == 0) {
			arr = brute_force(g, i);
		} else if (TEST == 1) {
			arr = floyd(g, i);
		} else {
			arr = dijkstras(g, i);
		}

		end = clock();

		for(int j = 0; j < i * i; ++j) {
			free(arr[j]);
		}

		free(arr);
			
		fout << i << ", " << 1000 * (double)(end - begin) / CLOCKS_PER_SEC << "\n";
	}
}
