#include "graph.cpp"

void Graph::exhaustive_search()
{
    int* chromatic_value = new int[vertices];
    bool over = false;

    for (int i = 0; i < vertices; i++)
    {
        chromatic_value[i] = 0;
    }

    while (!over)
    {           
        for (int i = 0; i < vertices; i++)
        {   
            over = true; 
            for (int j = 1; j <= sequence_array[i][0]; j++)
            {
                if (chromatic_value[i] == chromatic_value[sequence_array[i][j]])
                {
                    chromatic_value[j] = chromatic_value[i] + 1;
                    over = false;
                }

                for (int k = 1; k <= sequence_array[j][0]; k++)
                {
                    if (chromatic_value[k] == chromatic_value[i])
                    {
                        chromatic_value[k] = chromatic_value[i] + 1;
                        over = false;
                    }
                }
            }
        }
    }

    coloring_print(chromatic_value);
}
