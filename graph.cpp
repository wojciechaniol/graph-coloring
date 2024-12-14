#include "graph.hpp"

bool compare(int a[2], int b[2])
{
    return a[1] > b[1];
}

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

            if (adjacency_matrix[i][j] != 1) 
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

void Graph::add_edge(int u, int v, bool sequence=true)
{
    if (sequence)
    {
        int* sequence_u= new int[sequence_array[u][0]+2];
        int* sequence_v = new int[sequence_array[v][0]+2];

        for (int i = 0; i < sequence_array[u][0]+1; i++)
        {
            sequence_u[i] = sequence_array[u][i];
        }

        for (int i = 0; i < sequence_array[v][0]+1; i++)
        {
            sequence_v[i] = sequence_array[v][i];
        }

        sequence_u[0]++;
        sequence_v[0]++;
        delete[] sequence_array[u];
        delete[] sequence_array[v];

        sequence_u[sequence_u[0]] = v;
        sequence_v[sequence_v[0]] = u;

        sequence_array[u] = sequence_u;
        sequence_array[v] = sequence_v;
    }

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

    printf("Sequence array for the graph: \n");

    for (int i = 0; i < vertices; i++)
    {
        printf("%d -> ", i);
        for (int j = 1; j <= sequence_array[i][0]; j++)
        {
            printf("%d ", sequence_array[i][j]);
        }
        printf("\n");
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
        printf("Colors assigned to each vertex: \n");

        for (int i = 0; i < vertices; i++)
        {
            printf("vertex: %d -> color: %d\n", i, colors[i]);
        }

        return;
    }

    for (int color = 0; color < numColors; color++)
    {
        if (is_safe(colors, node, color))
        {
            colors[node] = color;                    
            backtracking(colors, node + 1, numColors);
            colors[node] = -1;
        }
    }
}

/* in below algorithm we can add some adjustments and use sequence_array[i][0] */
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
            if (adjacency_matrix[i][j] == 1)
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
                    if (is_safe(chromatic_value, j, current_color))
                    {
                        chromatic_value[vertex_degree[j][0]] = current_color;
                    }
                }
            }
        }

        current_color++;
    }

    coloring_print(chromatic_value);
}

void Graph::partition(int** color_array, int available_colors)
{
    queue<int> vertexes;
    int front_v, numerator = 1;
    int* chromatic_value = new int [vertices];
    chromatic_value[0] = color_array[0][0];

    for (int i = 0; i < vertices; i++)
    {
        for (int j = 1; j <= sequence_array[i][0]; j++)
        {
            vertexes.push(sequence_array[i][j]);

            for (int k = 1; k <= sequence_array[j][0]; k++)
            {
                vertex.push(sequence_array[j][k]);
            }
        }

        while (!vertexes.empty())
        {
            numerator = 1;
            front_v = vertexes.front();
            chromatic_value[front_v] = color_array[front_v][0];

            while (chromatic_value[front_v] == chromatic_value[i] && numerator < available_colors)
            {
                chromatic_value[front_v] = color_array[front_v][numerator];
                numerator++;
            }

            if (numerator == available_colors)
            {
                printf("not able to do this coloring, too few colors available\n");
                return;
            }

            vertexes.pop();
        }
    }

    coloring_print(chromatic_value);
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
}

int Graph::zykov(Graph& graph) 
{
    if (graph.is_clique()) 
    {
        return graph.countVertices();
    }

    for (int u = 0; u < vertices; u++) 
    {
        if (!active_vertex(u))
        {
            continue;
        }

        for (int v = u + 1; v < vertices; v++) 
        {
            if (!active_vertex(v))
            {
                continue;
            }

            if (adjacency_matrix[u][v] == 0) 
            {
                Graph* branch1 = graph.clone();
                branch1->add_edge(u, v, false);

                Graph* branch2 = graph.clone();
                branch2->merge_vertices(u, v);

                int result = min(branch1->zykov(*branch1), branch2->zykov(*branch2));

                delete branch1;
                delete branch2;

                return result;
            }
        }
    
    }

    return 100000;
}

int Graph::do_zykov()
{
    return zykov(*this);
}

int Graph::countVertices() 
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

/*genetic algorithm*/
/* improve degree counting */

void Graph::genetic(int population_size)
{
    int max_degree = 0, max_color;
    int degree = 0;
    int generation = 0;
    int lowest_penalty = 0;
    int* best_individual;
    int** population = new int* [population_size];

    /*count degree*/
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

    lowest_penalty = calculate_penalty(population[0]);
    best_individual = population[0];

    while(lowest_penalty != 0 && generation < 10000)
    {
        generation++;
        population = selection(population, population_size);
        population_size /= 2;
        for (int i = 0; i < population_size-1; i++)
        {
            crossover(population[i], population[i+1]);
        }
        
        for (int i = 0; i < population_size; i++)
        {
            if (generation < 200)
            {
                mutation(population[i], max_color, 0.3);
            }
            else
            {
                mutation(population[i], max_color, 0.5);
            }
        }

        lowest_penalty = calculate_penalty(population[0]);
        best_individual = population[0];

        for (int i = 1; i < population_size; i++)
        {
            if (calculate_penalty(population[i]) < lowest_penalty)
            {
                lowest_penalty = calculate_penalty(population[i]);
                best_individual = population[i];
            }
        }

        printf("generation: %d best fittnes: %d\n", generation, lowest_penalty);
        printf("Best individual: \n");

        for (int i = 0; i < vertices; i++)
        {
            printf("%d ", best_individual[i]);
        }
    }
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
    int aux_parent[vertices - index], cnt = 0;

    for (int i = index; i < vertices; i++)
    {
        aux_parent[cnt] = parent_1[i];
        parent_1[i] = parent_2[i];
        parent_2[i] = aux_parent[cnt];
        cnt++;
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
    int** new_population = new int*[population_size/2];
    int size = sizeof(population) / sizeof(population[0]);

    shuffle(population, population + size, engine);

    for (int i = 0; i < population_size-1; i+=2)
    {
        if (calculate_penalty(population[i]) > calculate_penalty(population[i+1]))
        {
            new_population[i/2] = population[i+1];
        }
        else
        {
            new_population[i/2] = population[i];
        }
    }

    return new_population;
}

int main()
{   
    int n = 10;
    int* colors = new int[n];

    for (int i = 0; i < n; i++)
    {
        colors[i] = -1;
    }

    Graph g(n);

    g.generate_graph(0.4);

    /*
    g.printing();
    g.exhaustive_search();
    g.greedy();
    g.palette_restriciton(3);
    //printf("%d\n", g.do_zykov());
    //g.backtracking(colors, 0, 3);
    g.welsh_powell();
    g.genetic(20);
    */
    g.check_bridges();
    g.printing();

    delete[] colors;

    return 0;
}