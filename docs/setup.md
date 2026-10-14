# CONFIGCORE Setup & Demonstration Guide

## System Requirements
- Windows 10/11
- MinGW GCC 6.3+ (or standard C11 compiler)
- Modern Web Browser (Chrome, Edge, Firefox)

## Quick Start
1. Double-click `run.bat` or execute in PowerShell:
   ```powershell
   .\run.bat
   ```
2. `run.bat` will automatically:
   - Compile `configcore.exe` using GCC with `-lws2_32`
   - Compile and execute unit tests `test_runner.exe`
   - Launch default web browser at `http://localhost:8080`
   - Start the C HTTP Server

## Manual Compilation Commands
- **Build Backend Server**:
  ```cmd
  gcc -Iinclude src/*.c -lws2_32 -o configcore.exe
  ```
- **Build Unit Tests**:
  ```cmd
  gcc -Iinclude src/*.c tests/test_main.c -o test_runner.exe
  ```
- **Run Server**:
  ```cmd
  configcore.exe 8080
  ```
