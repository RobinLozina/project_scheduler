# Project Scheduler

## Overview

**Project Scheduler** is a command-line application built in C to calculate the **Critical Path** of a project. The program reads a list of tasks and their dependencies from a CSV file, then computes the earliest and latest possible start times for each task, as well as the critical path of the project. This is particularly useful for project managers to identify tasks that cannot be delayed without affecting the overall project timeline.

### Key Features

- **Critical Path Calculation**: Identifies the tasks that are crucial for maintaining the project’s timeline.
- **Earliest and Latest Start Times**: Computes the earliest and latest possible start times for each task, considering dependencies.
- **Graphical Representation**: Provides a textual representation of the timeline, showing when each task can start, its duration, and any possible delays (slack).

## How It Works

1. **Input File**: The program reads tasks from a CSV file where each line represents a task with the following fields:

   - Task ID
   - Duration (in arbitrary time units)
   - Predecessors (a comma-separated list of task IDs that must be completed before this task can begin)

2. **Calculations**:

   - **Earliest Start Times**: Calculates the earliest time each task can start based on the completion of its predecessor tasks.
   - **Latest Start Times**: Determines the latest time each task can start without delaying the entire project.
   - **Critical Path**: Identifies the sequence of tasks that directly impact the project completion time.

3. **Output**:
   - Displays a breakdown of task durations, dependencies, and start times.
   - Shows the critical path, which helps in identifying tasks that should be prioritized.
   - Optionally outputs a visual representation of the project timeline.

## Running the Project

### Prerequisites

- **C Compiler**: Ensure you have `gcc` or another C compiler installed.
- **CSV Input File**: A file in the same directory named `tasks.csv` with the following format:
  ```plaintext
  TaskID, Duration, PredecessorIDs
  1, 2, 0
  2, 4, 0
  3, 4, 1
  4, 5, 1,2
  5, 6, 3,4
  ```
  where 0 as a predecessor means no dependencies for the task

1. **Open a Terminal**:

   ```sh
   cd project_scheduler

   ```

2. **Compile the Code**:

   ```sh
   gcc project_scheduler.c -o project_scheduler

   ```

3. **Run the Program**:

   ```sh
   ./project_scheduler

   ```
