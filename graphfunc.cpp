#include "graph.cpp"

Graph::Graph(int n)
{
    adjacency_matrix = new int* [n];
    sequence_array = new int* [n];
    vertices = n;

    for (int i = 0; i < n; i++)
    {
        adjacency_matrix[i] = new int[n];
        sequence_array[i] = new int[1];
        sequence_array[i][0] = 0; //vertex degree

        for (int j = 0; j < n; j++)
        {
            adjacency_matrix[i][j] = 0;
        }
    }
}

Graph::Graph* clone() 
{
    Graph* newGraph = new Graph(vertices);

    for (int i = 0; i < vertices; i++) 
    {
        for (int j = 0; j < vertices; j++) 
        {
            newGraph->adjacency_matrix[i][j] = adjacency_matrix[i][j];
        }
    }

    return newGraph;
}

void Graph::generate_graph(double saturation)
{
    if (saturation > 1 || saturation < 0)
    {
        printf("Wrong saturation value\n");
        exit(0);
    }

    int random_int_array[vertices];
    int size;
    
    for (int i = 0; i < vertices; i++)
    {
        random_int_array[i] = i;
    }

    size = sizeof(random_int_array)/sizeof(random_int_array[0]);

    for (int i = 0; i < vertices; i++)
    {
        shuffle(random_int_array, random_int_array+size, engine);

        for (int j = 0; j < (int)(saturation*vertices); j++)
        {
            if (random_int_array[j] != i)
            {
                this->add_edge(i, random_int_array[j]);
            }
        } 
    }
}

void Graph::check_bridges()
{
    /*lets assert that bridge exists only at the distance of two vertices*/

    for (int i = 0; i < vertices; i++)
    {
        for (int j = 0; j < vertices; j++)
        {
            if (adjacency_matrix[i][j] == 1)
            {
                for (int k = 0; k < vertices; k++)
                {
                    if (adjacency_matrix[j][k] == 1 && adjacency_matrix[i][k] == 0 && i != k)
                    {
                        adjacency_matrix[i][k] = 2;
                    }
                }
            }
        }
    }
}
