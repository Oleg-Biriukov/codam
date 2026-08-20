*This project has been created as part of the 42 curriculum by obirukov.*

# Codexion

## Description

Codexion is a multithreaded simulation of coders competing for shared dongles. Each coder repeatedly performs three stages: compiling, debugging, and refactoring. To compile, a coder must acquire both adjacent dongles. The simulation ends when a coder burns out or when every coder has completed the requested number of compilations.

The project focuses on concurrent resource management, scheduling, precise timing, starvation prevention, and safe communication between threads. Two scheduling policies are available:

- `fifo`: grants a dongle to the coder whose request arrived first.
- `edf`: grants a dongle to the coder with the earliest burnout deadline.

## Features

- One thread per coder and a separate burnout-monitor thread.
- Custom binary heaps for dongle request queues; no standard-library priority queue is used.
- FIFO and earliest-deadline-first arbitration.
- Dongle cooldown handling after compilation.
- Serialized action and burnout logging.
- Input validation, clean shutdown, and resource cleanup.

## Instructions

### Requirements

- A C compiler compatible with the C11/POSIX APIs.
- POSIX threads (`pthread`).
- `make`.

### Compilation

From the repository root:

```sh
make
```

The executable is named `codexion`. To remove object files and the executable:

```sh
make clean
make fclean
```

### Execution

Run the program with eight arguments:

```text
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

All time values are expressed in milliseconds. `scheduler` must be either `fifo` or `edf`.

Example:

```sh
./codexion 5 800 200 200 70 3 10 edf
```

The program prints timestamped actions, including dongle acquisition, compiling, debugging, refactoring, and burnout events. The included test script exercises invalid arguments, normal completion, burnout, scheduling, cooldown, and edge cases:

```sh
make
./tests.sh
```

## Blocking cases handled

### Deadlock prevention and Coffman conditions

Coders request their two adjacent dongles through the central scheduler. A coder only receives the condition-variable notification to begin compiling after both dongles have been assigned, so it does not hold one dongle while waiting indefinitely for the other. The scheduler grants dongles from the per-dongle priority queues and releases both resources when compilation ends. This breaks circular-wait behavior and prevents a deadlock caused by the ring-shaped resource layout.

### Starvation prevention

FIFO preserves request arrival order. EDF prioritizes the earliest burnout interval and uses the coder ID as a deterministic tie-breaker. Every request is inserted into both relevant dongle heaps, so a coder cannot be skipped by one side of its resource pair.

### Cooldown handling

After a compilation, each released dongle records its release time. The scheduler does not grant a dongle until the configured cooldown has elapsed. This prevents immediate reuse while still allowing the resource to become available automatically.

### Precise burnout detection

A dedicated monitor thread continuously checks every unfinished coder burnout interval. It sets the shared burnout state and prints the burnout message under the logging mutex, while coder threads regularly check the same termination state and stop promptly.

### Log serialization

Every log message is protected by the print mutex. This keeps a timestamp and its associated action on one line even when several coder threads are active.

### Clean termination and memory ownership

The simulation stops on the first burnout or after all required compilations finish. Coder threads are joined before shutdown, synchronization primitives are destroyed, and allocated coder, dongle, list, workspace, and simulation objects are freed.

## Thread synchronization mechanisms

The implementation uses POSIX threads and the following synchronization objects:

- `pthread_mutex_t mut`: protects global termination and failure flags such as `is_over`, `is_failed`, and `is_burnout`.
- `pthread_mutex_t mut_prnt`: serializes all output from coder and monitor threads.
- `pthread_mutex_t mut_time`: protects shared timestamps used for burnout, request ordering, deadlines, and cooldown calculations.
- `pthread_mutex_t mut_array`: protects coder and dongle state, heap access, resource ownership, compilation counters, and condition-variable waits.
- `pthread_cond_t`: each coder has a condition variable that the scheduler broadcasts after both required dongles have been assigned. The coder waits with a timed wait and rechecks shared termination state, avoiding missed wakeups and allowing shutdown to be observed.
- Custom scheduling events: the scheduler repeatedly examines the heap head of each dongle and assigns resources when the highest-priority waiting coder is eligible. The global condition variable is also broadcast when a coder submits new dongle requests.

For example, the scheduler locks `mut_array` while checking and changing a dongle `is_active` flag, while the coder locks the same mutex before reading its connections. The monitor updates `is_burnout` under `mut`, and all worker threads check that flag before continuing. These lock-protected reads and writes prevent races on ownership, completion state, and simulation shutdown.

## Resources

- `man pthreads` and `man pthread_mutex_lock`: POSIX thread and mutex APIs.
- [POSIX Threads Programming](https://computing.llnl.gov/tutorials/pthreads/): thread creation, synchronization, and condition variables.
- [The Open Group Base Specifications](https://pubs.opengroup.org/onlinepubs/9699919799/): reference for POSIX timing and threading behavior.
- [Binary heap](https://en.wikipedia.org/wiki/Binary_heap): background on the priority-queue structure used by the scheduler.
- The 42 Codexion subject: project rules, timing requirements, and scheduling policies.

### AI usage

AI assistance was used to help organize and proofread this README, summarize the implemented synchronization and scheduling behavior from the source, and check that the documented build and test commands match the repository. The C implementation, design decisions, and tests remain part of the project code and were reviewed against the subject requirements.
