# 2024_Research_MassifCheck
**Qt** desktop application for running and analyzing **Valgrind Massif** heap memory profiling output.

---

## ✨ Current Features

- Select a ``C`` or ``C++`` source file (.c or .cpp) to automatically compile it and analyze with Valgrind Massif
- Select a compiled binary to analyze
- Run Valgrind's Massif tool with custom options
- Parse Massif output
- Provide memory usage analysis with function-level insights, including detection of heap growth and stabilization  
- Generate suggestions based on customizable thresholds to help identify abnormal memory behavior


📘 For detailed instructions, see the [Usage Guide](https://github.com/MATF-Software-Verification/2024_Research_MassifCheck/wiki/Usage-Guide).
---

## 🚀 Getting Started
### 1. Prerequisites

- [Qt Creator](https://www.qt.io/download) (6.0+)
- WSL (Windows Subsystem for Linux)
- Ubuntu distribution inside **WSL**
- `g++` and `gcc` compilers installed inside **WSL**
- Valgrind installed in **WSL**

---
### 2. Installing Qt Creator

Download and install from [https://www.qt.io/download](https://www.qt.io/download)

> ✅ Recommended: Use **MSVC** or **MinGW** kits for Desktop development

---
### 3. Setting up WSL and Valgrind

#### a. Enable WSL

Open PowerShell as Administrator and run:

```powershell
wsl --install
```



#### b. Set Ubuntu as the Default WSL Distribution if you have multiple

```powershell
wsl -l -v
```

This will list all installed distros, for example:

```powershell
  NAME            STATE           VERSION
* Ubuntu-22.04    Running         2
  Debian          Stopped         2
```

Then set Ubuntu as default:

```powershell
wsl --set-default Ubuntu-22.04
```

#### c. Install build-essential tools in Ubuntu

Open powershell and launch wsl

```powershell
wsl
```

Then inside wsl run 

```powershell
sudo apt update
sudo apt install build-essential
```

The `build-essential` package includes `gcc`, `g++`, and other tools required for compiling C/C++ programs.


#### d. Install Valgrind in Ubuntu

Open powershell and launch wsl

```powershell
wsl
```

Then inside wsl run

```powershell
sudo apt update
sudo apt install valgrind
```

Verify installation:

```powershell
valgrind --version
```

---

## 🛠️ Building the Project

- Open the project folder containing `CMakeLists.txt` in Qt Creator

- Configure with a Desktop kit

- Click the Run button ▶️

💡 Make sure your Valgrind-executed binaries are accessible from WSL paths if running through Windows UI.

## 🧪 Testing with Example Inputs

The `./test/` directory contains several example C and C++ source files that you can use to test the application.

When you run these test files through the application, the generated `massif.out.x` files will be saved by default in the `./massif_files/` directory.

Alternatively, you can use the provided `run_massif_on_tests.sh` script to automatically compile and generate `massif.out.x` files in the `./massif_files/` directory, which you can then load and analyze within the application.

---

## 🤖  Automated Massif Profiling for C and C++ Tests with `run_massif_on_tests.sh` (Optional)

The `run_massif_on_tests.sh` helper script is optional and automates the process of compiling and running C and C++ tests with Valgrind's Massif tool to generate profiling data.
This script can be used as a faster way to generate Massif output files, but all the same functionality is available through the Qt application itself.

### 🔍 **What it does:**

- Searches for `.c` and `.cpp` files listed in the `test/` directory.
    
- Compiles them using the appropriate compiler:
    
    - `gcc` for `.c` files
    - `g++` for `.cpp` files
        
- Automatically creates the `massif_files/` directory if it doesn't exist.
    
- Runs each compiled test through `valgrind --tool=massif`.
    
- Saves the output as `massif.out.<n>` files inside the `massif_files/` directory.

📂 Example file structure:
```
├── QT_Massif_App/
├── test/
│   ├── test1.c
│   ├── test2.c
│   └── test3.cpp
├── massif_files/
│   ├── massif.out.1
│   ├── massif.out.2
│   └── massif.out.3
└── run_massif_on_tests.sh
```
💡 This script is helpful for quickly generating Massif profiling data for test binaries outside the Qt application interface.

### ⚙️ **How to use:**

1. **(Optional)** Predefined Test Files:
    - The script is currently hardcoded to run the following test files: `test1.c`, `test2.c`, `test3.cpp`, `test4.cpp`, `test5.cpp`.
    - To add your own test files, simply edit the `TEST_FILES` array in the script. For example:

    ```bash
    TEST_FILES=("test1.c" "test2.c" "test3.cpp" "my_new_test4.c")
    ```
    - The script will automatically detect the correct compiler based on the file extension (`gcc` for `.c` and `g++` for `.cpp`).
    > **Note:** This step is optional. If you don't need to modify the test files, you can skip it and use the default ones.

    
2. Make sure `run_massif_on_tests.sh` is executable:

    ```bash
    chmod +x run_massif_on_tests.sh
    ```
3. Run the script:

    ```bash
    ./run_massif_on_tests.sh
    ```

The script will automatically compile and run each test, saving the profiling results in the `massif_files/` directory.

---

## 📌 Notes

- 🐧 **Valgrind** runs only on Linux systems. On Windows, it uses WSL to execute Linux binaries.

---

## 👤 Contributors

- [Stefan Drljević](https://github.com/Stefan-Dr)
- [Vukašin Marković](https://github.com/sntntn)
