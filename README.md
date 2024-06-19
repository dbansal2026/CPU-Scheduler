# CPU-Scheduler

## 1. First-Come, First-Served (FCFS)

Type: Non-preemptive

Description: Processes are executed in the order they arrive in the ready queue.

Implementation: Simple to implement using a FIFO queue.

### Characteristics

Advantages:

1.Simple and easy to understand.

2.No starvation since every process gets a turn.

Disadvantages:

1.Can cause long waiting times, especially for processes arriving later (convoy effect).

2.Not suitable for time-sharing systems.

### Example

Process Arrival: P1 (0ms), P2 (2ms), P3 (4ms)

Burst Time: P1 (5ms), P2 (3ms), P3 (2ms)

Order of Execution: P1 → P2 → P3

#### Performance Metrics

Average Waiting Time: (0+5+8)/3=4.33ms

Average Turnaround Time: (5+8+10)/3=7.67ms

## 2. Shortest Job First (SJF)

Type: Can be preemptive (Shortest Remaining Time First) or non-preemptive.

Description: Processes with the shortest burst time are executed first.

Implementation: Requires knowledge of burst times in advance.

### Characteristics

Advantages:

1.Minimizes average waiting time.

Disadvantages:

1.Possible starvation for longer processes.

2.Not practical without knowing burst times beforehand.

### Example

Process Arrival: P1 (0ms), P2 (2ms), P3 (4ms)

Burst Time: P1 (5ms), P2 (3ms), P3 (2ms)

Order of Execution: P3 → P2 → P1 (Non-preemptive)

### Performance Metrics

Average Waiting Time: (0+2+5)/3=2.33ms

Average Turnaround Time: (2+5+10)/3=5.67ms

## 3. Priority Scheduling

Type: Can be preemptive or non-preemptive.

Description: Processes are executed based on priority levels; higher priority processes are executed first.

Implementation: Can be complex due to priority assignment.

### Characteristics

Advantages:

1.Suitable for real-time systems.

Disadvantages:

1.Potential starvation for lower-priority processes.

2.Priority inversion can occur.

### Example

Process Arrival: P1 (0ms), P2 (1ms), P3 (2ms)

Burst Time: P1 (4ms), P2 (3ms), P3 (1ms)

Priority: P1 (2), P2 (1), P3 (3) [Higher number = higher priority]

Order of Execution: P3 → P1 → P2

### Performance Metrics

Average Waiting Time: (0+1+4)/3=1.67ms

Average Turnaround Time:(1+5+8)/3=4.67ms

## 4. Round Robin (RR)

Type: Preemptive

Description: Processes are executed in a cyclic order, with each process getting a fixed time slice (quantum).

Implementation: Requires a timer for interrupting processes.

### Characteristics:

 Advantages:
 
1.Fair allocation of CPU time.

2.Suitable for time-sharing systems.

Disadvantages:

1.High context-switching overhead.

2.Performance depends on the choice of time quantum.

### Example

Process Arrival: P1 (0ms), P2 (1ms), P3 (2ms)

Burst Time: P1 (4ms), P2 (3ms), P3 (2ms)

Time Quantum: 2ms

Order of Execution: P1 (2ms) → P2 (2ms) → P3 (2ms) → P1 (2ms) → P2 (1ms)

### Performance Metrics

Average Waiting Time: (0+4+6)/3=3.33ms

Average Turnaround Time: (8+9+8)/3=8.33ms

## Conclusion:

Each CPU scheduling algorithm has its own advantages and disadvantages, making them suitable for different types of operating systems and applications. The choice of a scheduling algorithm depends on the specific requirements such as minimizing waiting time, ensuring fairness, or prioritizing critical processes.

This report provides a comprehensive comparison of FCFS, SJF, Priority, and Round Robin scheduling algorithms, highlighting their characteristics, advantages, disadvantages, and performance metrics with examples.
