from os import listdir
from os.path import isfile, join
import numpy as np
import matplotlib.pyplot as plt

GPROF_OUTPUT = "results/x86/gprof/"
PLOT_OUTPUT = "results/x86/plot/"

DPI = 300

def read_gprof_time_per_call(file):
    naive = 0.0
    tiled = 0.0
    with open(join(GPROF_OUTPUT, file), "r", encoding="utf-8") as gprof_f:
        for _ in range(4):
            gprof_f.readline()

        uses_ms = gprof_f.readline().find("ms/call") != -1
        for line in gprof_f:
            if line.find("tiled_multiply") != -1:
                tiled_line_tokens = [token for token in line.split(" ") if len(token) != 0]
                tiled = float(tiled_line_tokens[-2])
            
            if line.find("naive_multiply") != -1:
                naive_line_tokens = [token for token in line.split(" ") if len(token) != 0]
                naive = float(naive_line_tokens[-2])

        if uses_ms:
            naive /= 1000.0
            tiled /= 1000.0

    return naive, tiled

def plot_exec_time_by_matrix_size(gprof_out_files, tile_size, n, opt_level="novect"):
    files_for_tile_size = [f for f in gprof_out_files if f.find(f"b{tile_size}") != -1]
    
    files_for_opt_level = []
    for f in files_for_tile_size:
        optimization = f.split("_")[2].split(".")[0].strip()
        if optimization == opt_level:
            files_for_opt_level.append(f)

    print(files_for_opt_level)

    fig, ax = plt.subplots()

    naive_time = []
    tiled_time = []
    for prof_data_f in sorted(files_for_opt_level):
        naive, tiled = read_gprof_time_per_call(prof_data_f)
        naive_time.append(naive)
        tiled_time.append(tiled)
        
    ax.plot(n, naive_time, ".-", linewidth=2, label=f"naive")
    ax.plot(n, tiled_time, ".-", linewidth=2, label=f"tiled (tile = {tile_size})")

    opt = opt_level
    if opt == "novect":
        opt = "none"

    ax.legend()
    ax.set_xlabel(r"$n$")
    ax.set_ylabel("Execution time / call (s)")
    ax.set_title(f"Square matrix multiplication average execution time per call (opt = {opt})")

    fig.savefig(join(PLOT_OUTPUT, f"b{tile_size}_{opt}_matrix_dims.png"), dpi=DPI)

def plot_exec_time_by_tile_size(gprof_out_files, n):
    tile_sizes = range(20, 76, 4)
    files_for_matrix_size = [f for f in gprof_out_files if f.find(f"n{n}") != -1]

    files_for_matrix_size_tile_size = []
    for f in files_for_matrix_size:
        tile_size_pos = f.find("b")
        if tile_size_pos != -1:
            tile_size = int(f[tile_size_pos+1:tile_size_pos+3])
            if tile_size in tile_sizes:
                files_for_matrix_size_tile_size.append(f)
            
    optimizations = ["novect"] # TODO: Add O2 optimization
    files_by_optimization = {optimization: [] for optimization in optimizations}
    for f in files_for_matrix_size_tile_size:
        optimization = f.split("_")[2].split(".")[0].strip()
        if optimization in files_by_optimization:
            files_by_optimization[optimization].append(f)

    print(files_by_optimization)

    fig, ax = plt.subplots()

    for opt, runs in files_by_optimization.items():
        tiled_time = []
        
        for run_f in sorted(runs):
            _, tiled = read_gprof_time_per_call(run_f)
            tiled_time.append(tiled)

        opt_level = opt
        if opt_level == "novect":
            opt_level = "o0"
        
        ax.plot(tile_sizes, tiled_time, ".-", linewidth=2, label=f"tiled, {opt_level} (n = {n})")

    ax.legend()
    ax.set_xlabel("tile size")
    ax.set_ylabel("Execution time / call (s)")
    ax.set_title(f"Tiled square matrix multiplication average execution time per call")

    fig.savefig(join(PLOT_OUTPUT, "n500_tile_sizes.png"), dpi=DPI)

if __name__ == "__main__":
    print(f"Read gprof flat profiles in {GPROF_OUTPUT}")
    gprof_out_files = [f for f in listdir(GPROF_OUTPUT) if isfile(join(GPROF_OUTPUT, f))]

    plot_exec_time_by_matrix_size(gprof_out_files, tile_size=32, n=np.linspace(50, 500, 10), opt_level="novect")
    plot_exec_time_by_matrix_size(gprof_out_files, tile_size=68, n=np.linspace(50, 500, 10), opt_level="novect")

    plot_exec_time_by_matrix_size(gprof_out_files, tile_size=68, n=np.linspace(50, 1000, 20), opt_level="o2")

    plot_exec_time_by_tile_size(gprof_out_files, n=500)

    print(f"Saved plots to {PLOT_OUTPUT}")
