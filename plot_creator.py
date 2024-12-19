import subprocess
import matplotlib.pyplot as plt
import statistics as stat

node_number = [100, 200, 300, 400, 500]
node_number_genetic = [100, 120, 140, 160, 180]
node_number_zykov = [10, 12, 14, 16, 18, 20]
algorithms = ['exhaustive', 'greedy', 'backtracking', 'zykov', 'welsh-powell', 'genetic']
all_algorithms = {}

for algorithm in algorithms:
    print(f'{algorithm}')
    execution_times = []
    std_devs = []

    if algorithm == 'zykov':
        nodes_to_iterate = node_number_zykov
    elif algorithm == 'genetic':
        nodes_to_iterate = node_number_genetic
    else:
        nodes_to_iterate = node_number

    for nodes in nodes_to_iterate:
        times = []
        for i in range(10):
            result = subprocess.run(
                ['./grafy', str(nodes), algorithm],
                capture_output=True,
                text=True
            )
            execution_time = float(result.stdout.strip().split(": ")[-1])
            times.append(execution_time)
        std_devs.append(stat.stdev(times))
        execution_times.append(stat.mean(times))
        print(execution_times[-1])

    all_algorithms[algorithm] = {
        "nodes": nodes_to_iterate,
        "mean_times": execution_times,
        "std_devs": std_devs,
    }

    plt.figure()
    plt.errorbar(nodes_to_iterate, execution_times, yerr=std_devs, fmt='o-', label=f'{algorithm}')
    plt.title(f"Execution Time vs Number of Nodes ({algorithm})")
    plt.xlabel("Number of Nodes")
    plt.ylabel("Execution Time (s)")
    plt.legend()
    plt.grid(True)

    plt.savefig(f"{algorithm}_execution_time_plot.png")
    plt.close()


fig, ax1 = plt.subplots()

ax1.set_xlabel("Number of Nodes")
ax1.set_ylabel("Execution Time (s)", color="tab:blue")
for algorithm, data in all_algorithms.items():
    if algorithm != 'zykov' and algorithm != 'genetic':
        ax1.plot(
            data["nodes"], data["mean_times"], marker='o', label=f"{algorithm}"
        )
ax1.tick_params(axis='y', labelcolor="tab:blue")
ax1.legend(loc='upper left')
ax1.grid(True)

ax2 = ax1.twinx()
ax2.set_ylabel("Execution Time (s) [Zykov, genetic]", color="tab:red")
zykov_data = all_algorithms.get('zykov', None)
genetic_data = all_algorithms.get('genetic', None)
if zykov_data and genetic_data:
    ax2.plot(
        zykov_data["nodes"], zykov_data["mean_times"], marker='s', color="tab:red", label="zykov"
    )
    ax2.plot(
        genetic_data["nodes"], genetic_data["mean_times"], marker='x', color="tab:pink", label="genetic"
    )
    ax2.tick_params(axis='y', labelcolor="tab:red")
    ax2.legend(loc='upper right')

plt.title("Execution Time vs Number of Nodes (All Algorithms)")
plt.savefig("combined_execution_time_plot.png")
plt.close()
