#include "graph.hpp"

int main(int argc, char** argv)
{   
    int n = stoi(argv[1]);
    int* colors = new int[n];
    double saturation = 0.2;

    for (int i = 0; i < n; i++)
    {
        colors[i] = -1;
    }

    Graph g(n);

    g.generate_graph(saturation);
    g.check_bridges();

    auto start = chrono::high_resolution_clock::now();

    if (argc > 2)
    {
        string algorithm = argv[2];
        if (algorithm == "exhaustive") g.exhaustive_search();
        else if (algorithm == "greedy") g.greedy();
        else if (algorithm == "backtracking") g.backtracking(colors, 0, (int)(n*saturation)+1);
        else if (algorithm == "zykov") g.do_zykov();
        else if (algorithm == "welsh-powell") g.welsh_powell();
        else if (algorithm == "genetic")
        {
            g.genetic(200);
        }
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration<double>(end - start);

    printf("Time taken: %.8lf\n", duration.count());

    delete[] colors;

    return 0;
}
