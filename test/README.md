# Test Directory

## Naming Rules for Massif Output Files

- Use the `--massif-out-file` option when running the Massif tool to explicitly name the output files.
- The file name format should be: `massif.out.<test_case_id>`
  - **Example:** `massif.out.1`, `massif.out.2`, etc.
- This ensures consistency and makes it easier to analyze and compare tests.

### Example Command to Run the Massif Tool:
```bash
valgrind --tool=massif --massif-out-file=massif.out.1 ./program
```

# Test Cases and Memory Usage Analysis

In this section, we describe different test cases that were used to analyze memory usage. The memory usage data is captured using Massif and visualized through graphs. Screenshots of these graphs are included below for clarity.

## Test Case 1: `massif.out.1`

**Purpose**: This test case is used to simulate a scenario where memory usage is relatively stable. The graph shows memory consumption over time, with specific points where the memory usage increases gradually, eventually stabilizing.

**Graph Explanation**:
The graph below shows memory usage (in KB) over the course of the program's execution. The X-axis represents time, and the Y-axis represents memory usage in kilobytes (KB). We can observe a gradual increase in memory consumption, with a peak around snapshot 29. After this point, the memory consumption remains relatively stable.

![Screenshot from 2025-01-03 23-59-53](https://github.com/user-attachments/assets/9bd2a3f3-4b4f-4835-9c24-a7c6f5874d41)


## Test Case 2: `massif.out.2`

**Purpose**: This test case is designed to simulate scenarios with sudden spikes in memory usage, allowing us to test the functionality of detecting abrupt jumps in memory consumption.

**Graph Explanation**:
In this graph, the vertical spikes represent sudden increases in memory usage. These spikes indicate significant moments of memory allocation, which we analyze for potential memory inefficiencies or leaks. The graph helps us to visualize how the tool tracks these sudden memory jumps effectively.


![Screenshot from 2025-01-04 00-00-44](https://github.com/user-attachments/assets/adb07cd0-e2fd-47f9-b58c-ca72f99d3c04)

## Test Case 3: `massif.out.3`

**Purpose**: This test case illustrates a long-running program with a gradual and mostly steady increase in memory usage over an extended period. It is intended to test the tool’s ability to capture slow memory growth patterns, which can indicate subtle memory leaks or accumulation of retained data structures.

**Graph Explanation**:
The vertical axis represents memory usage in megabytes (MB), while the horizontal axis shows elapsed time in milliseconds (Mi). The graph shows a predominantly upward trend with some fluctuations but no significant drops, indicating persistent allocation of memory with minimal or no deallocation. The clustered peaks suggest phases where memory usage slightly spikes, possibly due to batch allocations or temporary buffers. This pattern helps identify programs where memory consumption accumulates steadily, potentially leading to performance degradation or resource exhaustion over time.

![Image](https://github.com/user-attachments/assets/9f185c9f-c0df-42af-a27a-851d8a18cd68)
