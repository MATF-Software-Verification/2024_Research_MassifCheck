# System Description


## 🧾 Problem Description

Modern `C` and `C++` applications often exhibit inefficient memory usage that is difficult to detect without specialized profiling tools. Although Valgrind’s Massif provides detailed heap analysis, its output is complex and not user-friendly.

This project addresses the need for a graphical application that simplifies running Massif, facilitates configuration, and assists in interpreting results. It also incorporates automated analysis to identify memory issues and support optimization efforts.

## 🏗️ System Architecture

The application is built as a modular Qt-based C++ desktop program designed for analyzing heap memory usage using Valgrind's Massif tool. The system is logically divided into several key modules, each responsible for a specific responsibility within the workflow of loading, generating, parsing, analyzing, and displaying memory profiling data.

### 🔧 Core Components
1. **Main Application** (`main.cpp`, `mainwindow.cpp/.h/.ui`)

    - This is the entry point and central GUI controller of the application. It manages the primary user interface and coordinates interactions between UI components and backend logic modules. It integrates file selection, threshold settings, and visualization features.

2. **MassifRunner Module** (`massifrunner.cpp/.h`)

     - Handles the full execution logic for memory profiling.  
   - If a **source file** and '`Compile and run...`' option is selected, it compiles it using `gcc` or `g++`, then runs the resulting binary with Valgrind’s Massif tool.  
   - If a **binary** and '`Load binary and run...`' is selected, it directly runs it under Massif.  
   - If the '`Show results`' button is clicked and a valid `massif.out.x` file is selected, it can invoke the `Parser` to read `massif.out.x` output and trigger analysis via the `MassifAnalyzer` module.

3. **Parser Module** (`Parser.cpp/.h`, `Snapshot.h`, `AllocationEntry.h`)

    - Handles parsing of the raw massif.out.x files. It converts the output into structured data formats (snapshots and entries) that can be used for visualization and analysis.

4. **MassifAnalyzer Module** (`massifanalyzer.cpp/.h`)

    - Implements logic for detecting memory usage patterns, such as heap growth, peak allocations, and stabilization. It evaluates snapshots based on heuristics and flags potential inefficiencies.
    - Generates detailed reports summarizing memory allocation behavior both overall and per-function.
    - Provides heuristic warnings and informational messages to guide users in identifying memory inefficiencies.

5. **Threshold Configuration** (`massifanalyzerthresholds.cpp/.h/.ui`, `MassifAnalyzerThresholds.h`)

    - Provides a UI and logic to allow users to customize analysis thresholds (e.g., stabilization sensitivity, growth rate limits). These thresholds directly influence the analyzer’s results.


6. **Execution Options** (`massifoptions.cpp/.h/.ui`)

    - Enables users to customize how Valgrind is executed (e.g., specifying additional command-line options or working directories). These options are passed to the MassifRunner module.

7. **File Selector Component** (`fileselector.cpp/.h`)

    - Provides a dialog interface for selecting files based on the mode:
        - **COMPILE** mode: lets the user select C/C++ source files (`*.c`, `*.cpp`) for compilation.
        - **BINARY** mode: lets the user select executable binary files.
        - **OUTPUT** mode: lets the user select existing Massif output files (`massif.out.*`).
   - Automatically generates output file names and directories for compiled binaries.
   - Includes utility functions for path handling, such as converting Windows paths to WSL format.
   - Supports setting file selection programmatically and clearing selected files.

8. **Results Display** (`resultdialog.cpp/.h/.ui`)

    - Displays parsed and analyzed results to the user. It includes snapshot summaries and automatically highlights potential memory issues detected by the analyzer.

9. **Enums and Constants** (`ModeEnum.h, TimeUnit.h, functionallocsummary.h`)

    - Contains helper types and constants used across the system for mode tracking, time unit formatting, and structuring function-level summaries.

### 🧪 Testing Subsystem

A dedicated `test/` directory contains example `.c` and `.cpp` source files that can be used to quickly test the Massif analysis pipeline. These files simulate various memory allocation behaviors.

A helper script `run_massif_on_tests.sh` automates the following steps:

- Compiles each test source file using `gcc` or `g++`

- Runs the compiled binary through Valgrind's Massif tool

- Outputs `massif.out.x` files into the `massif_files/` directory for later analysis in the application

### 📁 Resource and UI Integration

- `.ui` files (e.g., `mainwindow.ui`, `massifanalyzerthresholdswindow.ui`) are created using Qt Designer and define the layout and structure of GUI components.

- The `Resources/` directory contains icons or other assets required for visual presentation.

## 📊 Basic Ideas, Key Decisions, and Algorithm Description

### 💡 Basic Ideas

The project is designed to provide a flexible tool for memory profiling using Valgrind’s Massif tool. It supports two main use cases:
- Running Massif on a selected executable binary or on a binary generated from compiling a selected source file.
- Loading and analyzing existing massif.out files to visualize and understand memory usage patterns.

### 🎯 Key Decisions

- The architecture separates file management (**FileSelector**), execution (**MassifRunner**), and analysis (**MassifAnalyzer**). This modularity improves maintainability and testability.

- The parser is designed to convert raw Massif output into structured snapshots and allocation entries, filtering out irrelevant or standard library functions to focus the analysis on user code. This increases the signal-to-noise ratio in reports.

-  The analysis uses heuristic rules rather than strict formulas to identify potential memory issues such as memory jumps, leaks, fragmentation, and large allocations. This approach balances accuracy with noise reduction, allowing configurable thresholds to fine-tune sensitivity.

- Memory allocations are aggregated per function, allowing identification of functions responsible for large or numerous allocations, which might require optimization.

### ⚙️ Algorithm Descripiton

1. **Memory Jump Detection:**  
   Iterate through the list of `Snapshot` instances representing memory states at discrete times.  
   For each snapshot, calculate the relative increase (`heapJump`) in heap memory compared to the previous snapshot.  
   Similarly, calculate `stackJump` for stack memory.  
   If these jumps exceed the configured thresholds (e.g., 50%), record an informational message about the jump.

2. **Memory Stabilization Check:**  
   Upon detecting a memory jump, use a stabilization window (e.g., 5 snapshots) to verify if memory usage remains stable or continues growing.  
   Stability is defined as no increase in heap memory over all snapshots in the window.  
   If memory is stable, output a note indicating stabilization; otherwise, output a warning about continued growth.

3. **Large Memory Warnings:**  
   If heap or stack memory in any snapshot exceeds a large memory threshold (e.g., 1GB), output a warning message.

4. **Fragmentation Detection:**  
   For each snapshot, calculate the fragmentation ratio as extra heap memory divided by total heap memory.  
   If this ratio exceeds a threshold (e.g., 10%), warn about possible heap fragmentation.

5. **Memory Freeing Verification:**  
   Check the last snapshot to see if heap memory has been freed below a minimal threshold.  
   Warn if memory is still allocated, or confirm that memory has been fully freed.

6. **Function Allocation Report:**  
   Summarize allocation data per function, including total bytes allocated and allocation counts.  
   Warn about functions responsible for very large allocations (e.g., >100MB).  
   Note functions with many small allocations, suggesting potential for optimization via pooling or preallocation.

## 👤 Authors

- [Stefan Drljević](https://github.com/Stefan-Dr)
- [Vukašin Marković](https://github.com/sntntn)