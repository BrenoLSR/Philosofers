<div align="center">

# 🍝 Philosophers

### *I never thought philosophy would be so deadly*

<img src="https://img.shields.io/badge/42-SP-000000?style=for-the-badge&logo=42&logoColor=white"/>
<img src="https://img.shields.io/badge/Language-C-00599C?style=for-the-badge&logo=c&logoColor=white"/>
<img src="https://img.shields.io/badge/Threads-pthreads-FF6B6B?style=for-the-badge"/>
<img src="https://img.shields.io/badge/Mutex-Synchronization-4ECDC4?style=for-the-badge"/>
<img src="https://img.shields.io/badge/94.8%25-C-blue?style=for-the-badge"/>

</div>

---

## 📖 Table of Contents

- [About](#-about)
- [The Dining Philosophers Problem](#-the-dining-philosophers-problem)
- [Rules](#-rules)
- [Arguments](#-arguments)
- [Project Structure](#-project-structure)
- [Concepts Covered](#-concepts-covered)
- [Installation & Compilation](#-installation--compilation)
- [Usage & Output](#-usage--output)
- [Test Cases](#-test-cases)
- [Implementation Details](#-implementation-details)
- [Allowed Functions](#-allowed-functions)
- [Author](#-author)

---

## 📚 About

**Philosophers** is a project from the **42 School** curriculum that implements the classic **Dining Philosophers Problem**, originally formulated by **Edsger W. Dijkstra in 1965**. It introduces concurrent programming using **POSIX threads (pthreads)** and **mutexes** in C.

The challenge is to safely manage shared resources among multiple threads while preventing **deadlocks**, **data races**, and **starvation** — using no external libraries.

---

## 🍽️ The Dining Philosophers Problem

> *One or more philosophers sit at a round table. There is a large bowl of spaghetti in the middle.*

- A number of philosophers sit around a **circular table**.
- There is **one fork between each pair of adjacent philosophers** — so N philosophers = N forks.
- A philosopher needs **both the left and right fork** to eat.
- Philosophers cycle through three states: **thinking 🤔**, **eating 🍝**, and **sleeping 😴**.
- If a philosopher doesn't start eating within `time_to_die` milliseconds of their last meal (or the start), they **die 💀**.
- The simulation stops when **a philosopher dies** or when **all philosophers have eaten** enough times (if specified).

---

## 📏 Rules

- Philosophers **do not communicate** with each other.
- Philosophers **do not know** if another is about to die.
- A philosopher's death must be reported **within 10ms** of the actual death time.
- The program must have **zero data races**.
- Each philosopher should **never starve** if the simulation parameters allow it.

---

## 🔢 Arguments

```bash
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]
```

| Argument | Type | Description |
|---|---|---|
| `number_of_philosophers` | `int` | Number of philosophers (and forks) at the table |
| `time_to_die` | `int (ms)` | Milliseconds a philosopher can go without starting to eat before dying |
| `time_to_eat` | `int (ms)` | Milliseconds it takes for a philosopher to eat |
| `time_to_sleep` | `int (ms)` | Milliseconds a philosopher spends sleeping |
| `number_of_times_each_philosopher_must_eat` | `int` *(optional)* | If all philosophers eat this many times, the simulation ends |

> All time values are in **milliseconds**.

---

## 🗂️ Project Structure

```
Philosofers/
│
├── main.c            # Entry point — argument validation and simulation launch
├── parsing.c         # Argument parsing and input sanitization (uses ft_atol)
├── ft_atol.c         # Custom ft_atol for safe long integer conversion
├── table_init.c      # Table and philosopher struct initialization, mutex setup
├── simulation.c      # Thread creation and simulation start/stop logic
├── philo.c           # Philosopher lifecycle (think → eat → sleep loop)
├── routine.c         # Fork-grabbing and eating routine per philosopher
├── monitor.c         # Death and meal-count monitoring thread
├── time.c            # Time utility — gettimeofday-based ms timestamp
├── utils.c           # Shared helpers (mutex-protected print, etc.)
├── clean.c           # Mutex destruction and memory cleanup
├── philo.h           # All structs, defines, and function prototypes
├── Makefile          # Build system
└── estudo/           # Study notes and references used during development
```

---

## 🧠 Concepts Covered

### Threads (pthreads)
Each philosopher runs as an **independent POSIX thread** via `simulation.c`. All threads share the same memory space, which is what makes synchronization critical.

```c
pthread_create(&philo->thread, NULL, philosopher_routine, philo);
pthread_join(philo->thread, NULL);
```

### Mutexes
Each fork on the table is represented as a **mutex** (initialized in `table_init.c`). A philosopher must **lock** both adjacent mutexes before eating and **unlock** them afterward.

```c
pthread_mutex_lock(&table->forks[philo->left_fork]);
pthread_mutex_lock(&table->forks[philo->right_fork]);
// eating...
pthread_mutex_unlock(&table->forks[philo->right_fork]);
pthread_mutex_unlock(&table->forks[philo->left_fork]);
```

### Monitor Thread (`monitor.c`)
A dedicated thread runs in parallel and continuously checks:
- Whether any philosopher has exceeded `time_to_die` without eating.
- Whether all philosophers have completed the required number of meals.

Once either condition is met, a shared death flag is set and all threads stop gracefully.

### Data Race Prevention
All shared state (last meal timestamp, meal count, death flag) is protected by **dedicated mutexes**. The print function in `utils.c` uses its own mutex to prevent interleaved console output.

### Custom `ft_atol` (`ft_atol.c`)
Instead of using `atoi`, the project implements a custom `ft_atol` that handles edge cases like leading whitespace, signs, and overflow — ensuring robust argument parsing.

---

## ⚙️ Installation & Compilation

### Prerequisites

- GCC or Clang
- GNU Make
- Linux or macOS (POSIX threads required)

### Clone & Build

```bash
git clone https://github.com/BrenoLSR/Philosofers.git
cd Philosofers
make
```

### Makefile Targets

| Target | Description |
|---|---|
| `make` / `make all` | Compiles the project |
| `make clean` | Removes object files |
| `make fclean` | Removes object files and the binary |
| `make re` | Full recompile from scratch |

---

## 🚀 Usage & Output

```bash
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [meal_count]
```

Each action is printed in the format:

```
<timestamp_ms> <philosopher_id> <action>
```

**Possible actions:**
```
has taken a fork
is eating
is sleeping
is thinking
died
```

**Example run (`./philo 5 800 200 200`):**
```
0      1 has taken a fork
0      1 has taken a fork
0      1 is eating
0      3 has taken a fork
0      3 has taken a fork
0      3 is eating
200    1 is sleeping
200    3 is sleeping
200    2 has taken a fork
...
```

---

## 🧪 Test Cases

| Command | Expected Behavior |
|---|---|
| `./philo 1 800 200 200` | Philosopher 1 takes one fork and dies — only 1 fork available |
| `./philo 5 800 200 200` | No philosopher should die |
| `./philo 5 800 200 200 7` | Stops cleanly after each philosopher eats 7 times |
| `./philo 4 410 200 200` | No philosopher should die |
| `./philo 4 310 200 100` | One philosopher should die |
| `./philo 2 400 200 200` | No philosopher should die |

### Race & Leak Detection

```bash
# Check for data races with helgrind
valgrind --tool=helgrind ./philo 4 410 200 200

# Or compile with ThreadSanitizer
gcc -fsanitize=thread -g *.c -o philo -lpthread
```

---

## 🔍 Implementation Details

### Philosopher State Machine

```
┌──────────┐   picks up forks   ┌─────────┐
│ THINKING │ ─────────────────► │ EATING  │
└──────────┘                    └─────────┘
     ▲                               │
     │         wakes up              │ finishes eating
     │                               ▼
     └──────────────────── ┌──────────────┐
                           │   SLEEPING   │
                           └──────────────┘
```

### Fork Acquisition Strategy (Deadlock Prevention)

To prevent circular wait (deadlock), philosophers acquire forks in different orders based on their ID:

- **Odd ID** → picks up **left fork first**, then right.
- **Even ID** → picks up **right fork first**, then left.

This asymmetry ensures no circular dependency can form between adjacent philosophers.

### Timestamp Precision (`time.c`)

```c
long    get_time_ms(void)
{
    struct timeval  tv;

    gettimeofday(&tv, NULL);
    return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}
```

### Edge Case: 1 Philosopher

When there is only **1 philosopher**, they pick up their only fork but can never acquire a second — so they die after `time_to_die` milliseconds. `philo.c` and `routine.c` handle this correctly without hanging or crashing.

---

## 📋 Allowed Functions

| Function | Header | Purpose |
|---|---|---|
| `memset` | `<string.h>` | Memory initialization |
| `printf` | `<stdio.h>` | Output |
| `malloc` / `free` | `<stdlib.h>` | Dynamic memory |
| `write` | `<unistd.h>` | Low-level output |
| `usleep` | `<unistd.h>` | Microsecond sleep |
| `gettimeofday` | `<sys/time.h>` | High-precision timestamp |
| `pthread_create` | `<pthread.h>` | Create a thread |
| `pthread_detach` | `<pthread.h>` | Detach a thread |
| `pthread_join` | `<pthread.h>` | Wait for thread completion |
| `pthread_mutex_init` | `<pthread.h>` | Initialize a mutex |
| `pthread_mutex_destroy` | `<pthread.h>` | Destroy a mutex |
| `pthread_mutex_lock` | `<pthread.h>` | Acquire lock |
| `pthread_mutex_unlock` | `<pthread.h>` | Release lock |

---

## 👤 Author

<div align="center">

**Breno LSR**
42 São Paulo

[![GitHub](https://img.shields.io/badge/GitHub-BrenoLSR-181717?style=for-the-badge&logo=github)](https://github.com/BrenoLSR)
[![42](https://img.shields.io/badge/42-São_Paulo-000000?style=for-the-badge&logo=42&logoColor=white)](https://www.42sp.org.br/)

</div>

---

<div align="center">

*"The unexamined life is not worth living." — Socrates*

</div>
