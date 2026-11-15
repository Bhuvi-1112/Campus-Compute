
# CampusCompute
## Intelligent Job Scheduling and Resource Management

### Project Overview
CampusCompute is a Linux-based system programming project
implemented in C. It demonstrates CPU scheduling,
concurrent logging, inter-process communication, resource
allocation safety, worker process management and resource
limits.

### Technologies
- C programming language
- GCC compiler
- Linux Ubuntu
- POSIX threads
- POSIX shared memory
- POSIX named semaphores
- fork() and waitpid()
- setrlimit()

### Modules
1. CPU Scheduling
   - First Come First Served (FCFS)
   - Round Robin
   - Non-preemptive Priority Scheduling
   - CT, TAT, WT and averages

2. Concurrent Activity Logger
   - Producer-consumer queue
   - POSIX threads
   - Mutex and condition variables

3. Inter-Process Communication
   - POSIX shared memory
   - Named semaphore
   - Resource cleanup

4. Banker's Safety Algorithm
   - Available, Allocation and Maximum matrices
   - Need matrix
   - Safe sequence and unsafe-state detection

5. Worker Process Management
   - fork()
   - waitpid()
   - Normal execution and simulated failure

6. Resource Limits
   - getrlimit()
   - setrlimit()
   - CPU time limit demonstration

### Project Structure
- src/       Source files
- include/   Header files
- logs/      Log directory
- docs/      Documentation
- Makefile   Build configuration

### Requirements
- Ubuntu Linux
- GCC
- GNU Make
- POSIX threads and system libraries

### Build Instructions
Run:

    make clean
    make

### Run the Application
Run:

    ./campuscompute

### Run Individual Module Tests
    ./test_scheduler
    ./test_logger
    ./test_ipc
    ./test_banker
    ./test_worker
    ./test_resource

### Clean Build Files
    make clean

### Team Details
Student SRN: PES1UG26CS811
Team Member SRN: PES1UG26CS846

### Note
Worker failure is simulated intentionally for testing.
