#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <stdio.h>
#include <algorithm>
#include <random>
#include <queue>

using namespace std;

random_device rd;
mt19937 engine(rd());

class Graph
{
    int** adjacency_matrix;
    int** sequence_array;
    int chromatic_number;
    int vertices;

    public:

        Graph(int n);
        Graph* clone();
        void generate_graph(int saturation);
        void check_bridges();
        bool active_vertex(int v);
        bool is_clique();
        void merge_vertices(int u, int v);
        void add_edge(int u, int v, bool sequence=true);
        void printing();
        void coloring_print(int* chromatic_value);
        void exhaustive_search();
        void greedy();
        bool is_safe(int* colors, int node, int color);
        void backtracking(int* colors, int node, int numColors);
        void welsh_powell();
        void partition(int** color_array);
        void palette_restriciton(int available_colors);
        int zykov(Graph& graph);
        int do_zykov();
        int countVertices();
        void genetic(int population_size);
        void create_individual(int* colors, int max_color);
        int calculate_penalty(int* individual);
        void crossover(int* parent_1, int* parent_2);
        void mutation(int* individual, int max_color, double probability);
        int** selection(int** population, int population_size)
};

#endif