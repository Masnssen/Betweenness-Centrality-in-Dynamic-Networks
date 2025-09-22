# Foremost Non-Stop Temporal Betweenness (C++ Implementation)

This project provides a C++ implementation for computing **foremost non-stop temporal betweenness centrality** on time-expanded graphs.  
It is designed for dynamic networks such as **public transportation timetables**, where paths must respect both causality (time ordering) and the **zero-wait constraint** (no waiting at intermediate nodes).

---

## 📂 Project Structure

```
.
├── fonction.h          # Core data structures (temporal graph, bitsets, edge arrays)
├── fonction_bb.h       # Alternative header (compatibility layer, version v1)
├── stat_reel_bc.cpp    # Main program: builds graph, computes BC, outputs results
├── Makefile            # Build instructions
```

---

## ⚙️ Compilation

Requires a modern **C++17** compiler (e.g., `g++`, `clang++`).  

To build:

```bash
make
```

This will produce an executable, usually named:

```bash
stat_reel_bc
```

---

## ▶️ Usage

The program expects a dataset describing temporal edges and lists of sources/targets.

### Input files

- **`<DATASET>.txt`**  
  Each line:  
  ```
  date  src  dst  cost
  ```
  - `date`: departure time (integer, e.g., minute of the day)  
  - `src`: source node ID  
  - `dst`: destination node ID  
  - `cost`: travel duration  

- **`<DATASET>.src`**  
  List of source node IDs (one per line).  

- **`<DATASET>.dst`**  
  List of target node IDs (one per line).  

By default, the program looks into:

```
../../Experiment/Paris/dataset_reel/network_temporal_day.*
```

Paths are configurable by editing `DATA_DIR` and `DATASET_NAME` macros at the top of `stat_reel_bc.cpp`.

---

### Running

```bash
./stat_reel_bc
```

---

## 📊 Output

Two result files are produced:

1. **`bc_<DATASET>_1.test`**  
   Contains per-source results:  
   ```
   node_id   out_degree   in_degree   betweenness   computation_time
   ```

2. **`stat_<DATASET>.result`** and **`stat_<DATASET>.moy`**  
   Auxiliary statistics (average times, path counts).

Example line in `bc_network_temporal_day_1.test`:

```
42   15   13   0.004312   0.002145
```
→ Node 42, out-degree 15, in-degree 13, betweenness ≈ 0.0043, computed in 0.002s.

---

## 🔍 Implementation Notes

- **Graph model:**  
  Time-expanded graph (`G'`) with temporal nodes `(vertex, time)` and edges linking feasible departures/arrivals.

- **Zero-wait constraint:**  
  Enforced by construction: edges chain only if arrival time = next departure time.

- **Bitset representation:**  
  Each path is represented as a bitset over physical nodes → efficient feasibility checks & aggregation.

- **Algorithms:**  
  - Depth-first exploration (`TemporalDFS`-like) with pruning.  
  - Label-setting routine (`TemporalDijkstra`-like) with dominance checks.  
  - Exact betweenness accumulation via source-to-all enumeration.

---

## 📈 Applications

- Analysis of **transportation networks** (e.g., Paris IDFM, Adelaide)  
- Identifying **critical nodes** with high centrality  
- Evaluating computation cost vs. centrality correlation  

---

## 🧹 Cleanup

Graphs can be memory-heavy. Use `freeGraphe(g);` at the end of `main()` if large instances must be released before exit.

---

## 📜 License

This code is provided for research and educational purposes. Adapt and extend as needed for your datasets.
