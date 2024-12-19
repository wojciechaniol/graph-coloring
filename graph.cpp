#include "graph.hpp"

random_device rd;
mt19937 engine(rd());

bool compare(int a[2], int b[2])
{
    return a[1] > b[1];
}

Graph::Graph(int n)
{
    adjacency_matrix = new int* [n];
    vertices = n;

    for (int i = 0; i < n; i++)
    {
        adjacency_matrix[i] = new int[n];

        for (int j = 0; j < n; j++)
        {
            adjacency_matrix[i][j] = 0;
        }
    }
}

Graph* Graph::clone() 
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
    
    for (int i = 0; i < vertices; i++)
    {
        random_int_array[i] = i;
    }

    for (int i = 0; i < vertices; i++)
    {
        shuffle(random_int_array, random_int_array+vertices, engine);
        int j = 0;
        int cnt = 0;

        while (cnt < (int)(saturation*vertices))
        {
            if (random_int_array[j] != i)
            {
                if (adjacency_matrix[i][random_int_array[j]] != 1 && random_int_array[j] > i)
                {
                    this->add_edge(i, random_int_array[j]);
                }

                cnt++;
            }

            j++;

            if (j == vertices)
            {
                j = 0;
            }
        } 
    }
}

void Graph::check_bridges()
{
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

bool Graph::active_vertex(int v) 
{
    for (int i = 0; i < vertices; i++) 
    {
        if (adjacency_matrix[v][i] != 0 || adjacency_matrix[i][v] != 0) 
        {
            return true;
        }
    }

    return false;
}

bool Graph::is_clique() 
{
    for (int i = 0; i < vertices; i++) 
    {
        if (!active_vertex(i))
        {
            continue;
        }

        for (int j = i + 1; j < vertices; j++) 
        {
            if (!active_vertex(j))
            {
                continue;
            }

            if (adjacency_matrix[i][j] == 0) 
            {
                return false;
            }
        }
    }

    return true;
}

void Graph::merge_vertices(int u, int v) 
{
    for (int i = 0; i < vertices; i++) 
    {
        if (i != u && i != v) 
        {
            adjacency_matrix[u][i] |= adjacency_matrix[v][i];
            adjacency_matrix[i][u] = adjacency_matrix[u][i];
        }
    }

    for (int i = 0; i < vertices; i++) 
    {
        adjacency_matrix[v][i] = 0;
        adjacency_matrix[i][v] = 0;
    }
} 

void Graph::add_edge(int u, int v)
{
    adjacency_matrix[u][v] = 1;
    adjacency_matrix[v][u] = 1;
}

void Graph::printing()
{
    printf("Matrix for the graph: \n");

    for (int i = 0; i < vertices; i++)
    {
        for (int j = 0; j < vertices; j++)
        {
            printf("----");
        }
        printf("\n");
        for (int j = 0; j < vertices; j++)
        {
            printf("| %d ", adjacency_matrix[i][j]);
        }
        printf("|\n");
    }
}

void Graph::coloring_print(int* chromatic_value)
{
    int colors = 0;

    for (int i = 0; i < vertices; i++)
    {
        printf("Color of vertex %d: %d\n", i, chromatic_value[i]);

        if (chromatic_value[i] > colors)
        {
            colors = chromatic_value[i];
        }
    }

    printf("Number of colors used: %d \n", ++colors);
}

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
            for (int j = 0; j < vertices; j++)
            {
                if (chromatic_value[i] == chromatic_value[j] && adjacency_matrix[i][j] > 0)
                {
                    chromatic_value[j]++;
                    over = false;
                }
            }
        }
    }

    // coloring_print(chromatic_value);
}

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

        for (int j = 0; j < vertices; j++)
        {
            if (chromatic_value[j] != -1 && adjacency_matrix[i][j] > 0)
            {
                color_availability[chromatic_value[j]] = false;
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

    //coloring_print(chromatic_value);
    delete[] chromatic_value;
    delete[] color_availability;
}

bool Graph::is_safe(int* colors, int node, int color)
{
    for (int i = 0; i < vertices; i++)
    {
        if (adjacency_matrix[node][i] > 0 && colors[i] == color)
        {
            return false;
        }
    }

    return true;
}

void Graph::backtracking(int* colors, int node, int numColors)
{   
    if (node == vertices)
    {
    //     printf("Colors assigned to each vertex: \n");

    //     for (int i = 0; i < vertices; i++)
    //     {
    //         printf("vertex: %d -> color: %d\n", i, colors[i]);
    //     }

        return;
    }

    for (int color = 0; color < numColors; color++)
    {
        if (is_safe(colors, node, color))
        {
            colors[node] = color;                    
            return backtracking(colors, node + 1, numColors);
        }
    }
}

void Graph::welsh_powell()
{
    int* vertex_degree[vertices];
    int chromatic_value[vertices];
    int current_color = 0;

    for (int i = 0; i < vertices; i++)
    {
        vertex_degree[i] = new int[2];
        vertex_degree[i][1] = 0;
        vertex_degree[i][0] = i;
        chromatic_value[i] = -1;
    }

    for (int i = 0; i < vertices; i++)
    {
        for (int j = i + 1; j < vertices; j++)
        {
            if (adjacency_matrix[i][j] > 0)
            {
                vertex_degree[i][1] += 1;
                vertex_degree[j][1] += 1;
            }
        }
    }

    sort(vertex_degree, vertex_degree + vertices, compare);

    for (int i = 0; i < vertices; i++)
    {
        if (chromatic_value[vertex_degree[i][0]] == -1)
        {
            chromatic_value[vertex_degree[i][0]] = current_color;

            for (int j = i + 1; j < vertices; j++)
            {
                if (chromatic_value[vertex_degree[j][0]] == -1)
                {
                    if (is_safe(chromatic_value, vertex_degree[j][0], current_color))
                    {
                        chromatic_value[vertex_degree[j][0]] = current_color;
                    }
                }
            }
        }

        current_color++;
    }

    // coloring_print(chromatic_value);

    for (int i = 0; i < vertices; i++)
    {
        delete[] vertex_degree[i];
    }
}

void Graph::partition(int** color_array, int available_colors)
{
    queue<int> vertexes;
    int front_v;
    int* chromatic_value = new int [vertices];
    int* every_node_numerator = new int [vertices];

    chromatic_value[0] = color_array[0][0];

    for (int i = 0; i < vertices; i++)
    {
        every_node_numerator[i] = 0;
    }

    for (int i = 0; i < vertices; i++)
    {
        for (int j = 0; j < vertices; j++)
        {
            if (adjacency_matrix[i][j] > 0)
            {
                vertexes.push(j);
            }
        }

        while (!vertexes.empty())
        {
            front_v = vertexes.front();

            while (chromatic_value[front_v] == chromatic_value[i] && every_node_numerator[front_v] < available_colors)
            {
                chromatic_value[front_v] = color_array[front_v][every_node_numerator[front_v]];
                every_node_numerator[front_v]++;
            }

            if (every_node_numerator[front_v] > available_colors)
            {
                printf("not able to do this coloring, too few colors available\n");
                return;
            }

            vertexes.pop();
        }
    }

    coloring_print(chromatic_value);

    delete[] chromatic_value;
    delete[] every_node_numerator;
}

void Graph::palette_restriciton(int available_colors)
{
    uniform_int_distribution<int> int_rand(0, available_colors-1);
    int** vertex_colors = new int* [vertices];
    int banned_color;

    for (int i = 0; i < vertices; i++)
    {
        vertex_colors[i] = new int[available_colors-1];
        banned_color = int_rand(engine);

        for (int j = 0; j < available_colors-1; j++)
        {
            vertex_colors[i][j] = (j < banned_color) ? j : j+1;
        }
    }

    partition(vertex_colors, available_colors);

    for (int i = 0; i < vertices; i++)
    {
        delete[] vertex_colors[i];
    }
    delete[] vertex_colors;
}

int Graph::zykov(Graph& graph) 
{
    if (graph.is_clique()) 
    {
        return graph.count_vertices();
    }

    for (int u = 0; u < vertices; u++) 
    {
        if (!active_vertex(u))
        {
            continue;
        }

        graph.check_bridges();

        for (int v = u + 1; v < vertices; v++) 
        {
            if (!active_vertex(v))
            {
                continue;
            }

            if (adjacency_matrix[u][v] == 0) 
            {
                Graph* branch1 = graph.clone();
                branch1->add_edge(u, v);

                Graph* branch2 = graph.clone();
                branch2->merge_vertices(u, v);

                int result = min(branch1->zykov(*branch1), branch2->zykov(*branch2));

                delete branch1;
                delete branch2;

                return result;
            }
        }
    
    }

    return INT_MAX;
}

int Graph::do_zykov()
{
    return zykov(*this);
}

int Graph::count_vertices() 
{
    int count = 0;
    for (int i = 0; i < vertices; i++) 
    {
        if (active_vertex(i))
        {
            count++;
        }
    }
    return count;
}

void Graph::genetic(int population_size)
{
    int max_degree = 0, max_color;
    int degree = 0;
    int generation = 0;
    int lowest_penalty = INT_MAX;
    int* best_individual;
    int** population = new int* [population_size];

    for (int i = 0; i < vertices; i++)
    {
        for (int j = 0; j < vertices; j++)
        {
            if (adjacency_matrix[i][j] > 0)
            {
                degree++;
            }
        }

        if (degree > max_degree)
        {
            max_degree = degree;
            max_color = max_degree;
        }

        degree = 0;
    }

    for (int i = 0; i < population_size; i++)
    {
        population[i] = new int [vertices];
        create_individual(population[i], max_color);
    }

    while(lowest_penalty != 0 && generation < 10000)
    {
        generation++;

        int** new_population = selection(population, population_size);

        for (int i = 0; i < population_size-1; i+=2)
        {
            crossover(new_population[i], new_population[i+1]);
        }
        
        for (int i = 0; i < population_size; i++)
        {
            if (generation < 200)
            {
                mutation(new_population[i], max_color, 0.3);
            }
            else
            {
                mutation(new_population[i], max_color, 0.5);
            }
        }

        for (int i = 0; i < population_size; i++)
        {
            delete[] population[i];
            population[i] = new int [vertices];
            copy(new_population[i], new_population[i]+vertices, population[i]);
        }

        for (int i = 0; i < population_size; i++)
        {
            int penalty = calculate_penalty(population[i]);
            if (penalty < lowest_penalty)
            {
                lowest_penalty = penalty;
                best_individual = population[i];
            }
        }

        // if (generation%10 == 0)
        // {
        //     printf("generation: %d best fittnes: %d\n", generation, lowest_penalty);
        //     printf("Best individual: \n");

        //     for (int i = 0; i < vertices; i++)
        //     {
        //         printf("%d ", best_individual[i]);
        //     }

        //     printf("\n"); 
        // }

        // if (lowest_penalty == 0)
        // {
        //     printf("generation: %d best fittnes: %d\n", generation, lowest_penalty);
        //     printf("Best individual: \n");

        //     for (int i = 0; i < vertices; i++)
        //     {
        //         printf("%d ", best_individual[i]);
        //     }

        //     printf("\n"); 
        // }
    }
    
    delete[] population;
    delete[] best_individual;
}

void Graph::create_individual(int* colors, int max_color)
{
    uniform_int_distribution<int> int_rand(0, max_color);
    int color;

    for (int i = 0; i < vertices; i++)
    {
        color = int_rand(engine);
        colors[i] = color;
    }
}

int Graph::calculate_penalty(int* individual)
{
    int penalty = 0;

    for (int i = 0; i < vertices; i++)
    {
        for (int j = i; j < vertices; j++)
        {
            if (individual[i] == individual[j] && adjacency_matrix[i][j] > 0)
            {
                penalty++;
            }
        }
    }

    return penalty;
}

void Graph::crossover(int* parent_1, int* parent_2)
{
    uniform_int_distribution<int> int_rand(2, vertices-3);
    int index = int_rand(engine);
    int cnt = 0;

    for (int i = index; i < vertices; i++)
    {
        parent_1[i] = parent_2[i];
        if (cnt < index)
        {
            parent_2[cnt] = parent_1[cnt];
            cnt++;
        }
    }
}

void Graph::mutation(int* individual, int max_color, double probability)
{
    uniform_real_distribution<double> double_rand(0, 1);
    uniform_int_distribution<int> int_rand(0, vertices-1);
    double var = double_rand(engine);

    if (var <= probability)
    {   
        int position = int_rand(engine);
        uniform_int_distribution<int> int_rand(0, max_color);
        individual[position] = int_rand(engine);
    }
}

int** Graph::selection(int** population, int population_size)
{
    uniform_int_distribution<int> int_rand(0, population_size-1);
    int** new_population = new int*[population_size];
    int first_individual, second_individual;

    shuffle(population, population + population_size, engine);

    for (int i = 0; i < population_size; i++)
    {
        new_population[i] = new int[vertices];
        first_individual = int_rand(engine);
        second_individual = int_rand(engine);

        if (calculate_penalty(population[first_individual]) > calculate_penalty(population[second_individual]))
        {
            copy(population[second_individual], population[second_individual] + vertices, new_population[i]);
        }
        else
        {
            copy(population[first_individual], population[first_individual] + vertices, new_population[i]);
        }
    }

    return new_population;
}
