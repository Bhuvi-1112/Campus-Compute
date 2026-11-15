
# CampusCompute – Technical Documentation

## 1. Project Purpose
CampusCompute demonstrates operating-system concepts through
a modular C application running on Linux.

## 2. Modules
- CPU Scheduling: FCFS, Round Robin and non-preemptive Priority.
- Concurrent Activity Logger: POSIX threads, bounded queue,
  mutex and condition variables.
- IPC: POSIX shared memory and named semaphore.
- Banker's Algorithm: Need calculation and safety checking.
- Worker Management: fork() and waitpid().
- Resource Limits: getrlimit() and setrlimit().

## 3. Source Organization
- src/: C implementation and module test programs.
- include/: Header files and declarations.
- logs/: Generated logger output.
- docs/: Project documentation.

## 4. Build
Run `make clean` followed by `make`.

## 5. Execute
Run `./campuscompute` and select a module from the menu.

## 6. Testing
Each module has an independent test executable.
The tests cover scheduling, logging, IPC, safe and unsafe
resource states, worker execution and CPU limits.

## 7. Resource Cleanup
The application releases its synchronization objects,
shared memory, semaphore and process/thread resources.
