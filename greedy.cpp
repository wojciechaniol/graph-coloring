#include "graph.cpp"

void Graph::greedy()
{
    int* chromatic_value = new int[vertices];
    bool* color_availability = new bool[vertices];
    int color = 0;

    chromatic_value[0] = 0;
    color_availability[0] = true;

    for (int i = 1; i < vertices; i++)
    {
        chromatic_value[i] = -1;
        color_availability[i] = true;
    }

    for (int i = 1; i < vertices; i++)
    {
        color = 0;

        for (int j = 1; j <= sequence_array[i][0]; j++)
        {
            if (chromatic_value[sequence_array[i][j]] != -1)
            {
                color_availability[chromatic_value[sequence_array[i][j]]] = false;
            }

            for (int k = 1; k <= sequence_array[j][0]; k++)
            {
                if (chromatic_value[sequence_array[j][k]] != -1)
                {
                    color_availability[chromatic_value[sequence_array[j][k]] = false;
                }
            }
        }

        while (color < vertices)
        {
            if (color_availability[color] == true)
            {
                break;
            }
            color++;
        }

        chromatic_value[i] = color;

        for (int i = 0; i < vertices; i++)
        {
            color_availability[i] = true;
        }
    }

    coloring_print(chromatic_value);
    delete[] chromatic_value;
    delete[] color_availability;
}
