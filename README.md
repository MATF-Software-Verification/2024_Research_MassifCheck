# 2024_Research_MassifCheck
Qt desktop application for running and analyzing Valgrind Massif heap memory profiling output.

---

## ✨ Current Features

- Select a C++ source file (.cpp) to automatically compile it and analyze with Valgrind Massif
- Select a compiled binary to analyze
- Run Valgrind's Massif tool with custom options
- Parse Massif output
- (In Progress) Visualize heap memory usage

---

## ## 🚀 Getting Started
### 1. Prerequisites

- [Qt Creator](https://www.qt.io/download) (6.0+)
- WSL (Windows Subsystem for Linux)
- Ubuntu distribution inside WSL
- Valgrind installed in WSL

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

#### c. Install Valgrind in Ubuntu

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

- Open massif-visual-analyzer.pro in Qt Creator

- Configure with a Desktop kit

- Click the Run button ▶️

💡 Make sure your Valgrind-executed binaries are accessible from WSL paths if running through Windows UI.

---

## 📌 Notes

- Valgrind runs only on Linux-based systems (including WSL).

- Visualization panel and full report view are currently in development.

- The project is modular and open to contributions.

---

## 👤 Contributors

- [Stefan Drljević](https://github.com/Stefan-Dr)
- [Vukašin Marković](https://github.com/sntntn)