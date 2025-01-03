# Test Direktorijum

## Naming Rules for Massif Output Files

- Use the `--massif-out-file` option when running the Massif tool to explicitly name the output files.
- The file name format should be: `massif.out.<test_case_id>`
  - **Example:** `massif.out.1`, `massif.out.2`, etc.
- This ensures consistency and makes it easier to analyze and compare tests.

### Example Command to Run the Massif Tool:
```bash
valgrind --tool=massif --massif-out-file=massif.out.1 ./program

---
---


# Test Cases and Memory Usage Analysis

In this section, we describe different test cases that were used to analyze memory usage. The memory usage data is captured using Massif and visualized through graphs. Screenshots of these graphs are included below for clarity.

## Test Case 1: `massif.out.1`

**Purpose**: This test case is used to simulate a scenario where memory usage is relatively stable. The graph shows memory consumption over time, with specific points where the memory usage increases gradually, eventually stabilizing.

**Graph Explanation**:
The graph below shows memory usage (in KB) over the course of the program's execution. The X-axis represents time, and the Y-axis represents memory usage in kilobytes (KB). We can observe a gradual increase in memory consumption, with a peak around snapshot 29. After this point, the memory consumption remains relatively stable.


![image](https://github.com/user-attachments/assets/d96fcc2f-3942-4e25-bef2-795b7384f60b)


## Test Case 2: `massif.out.2`

**Purpose**: This test case is designed to simulate scenarios with sudden spikes in memory usage, allowing us to test the functionality of detecting abrupt jumps in memory consumption.

**Graph Explanation**:
In this graph, the vertical spikes represent sudden increases in memory usage. These spikes indicate significant moments of memory allocation, which we analyze for potential memory inefficiencies or leaks. The graph helps us to visualize how the tool tracks these sudden memory jumps effectively.


![image](https://github.com/user-attachments/assets/f1a3467c-46d4-4e34-b8ea-1d1a9a0fe0c4)
