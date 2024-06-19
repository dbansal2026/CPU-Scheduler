# CPU Scheduling Algorithms in C++

## Introduction

This document provides a comprehensive explanation of four CPU scheduling algorithms implemented in C++: First-Come-First-Serve (FCFS), Shortest Job First (SJF), Priority Scheduling, and Round Robin. These algorithms are fundamental in operating systems and are used to manage and optimize the execution of tasks (or processes) on a CPU.

## Task Structure

Each task is represented by a Task structure containing essential attributes such as id, arrivalTime, burstTime, priority, waitingTime, turnaroundTime, and remainingTime. These attributes are crucial for simulating the behavior and performance metrics of different scheduling algorithms.

```cpp
struct Task {
    int id;               // Identifier for the task
    int arrivalTime;      // Time at which the task arrives
    int burstTime;        // Total CPU time required by the task
    int priority;         // Priority of the task
    int waitingTime;      // Total waiting time of the task
    int turnaroundTime;   // Total time from submission to completion of the task
    int remainingTime;    // Remaining time needed by the task to complete
};
```
The Task structure encapsulates all necessary information about each task, facilitating the simulation of different scheduling algorithms and the computation of performance metrics.

## Helper Functions

This function computes the average waiting time and average turnaround time for a given list of tasks. These metrics are essential for evaluating the efficiency and performance of scheduling algorithms.

```cpp
void calculateAverageTimes(const vector<Task>& tasks, float& avgWaitingTime, float& avgTurnaroundTime) {
    int totalWaitingTime = 0;
    int totalTurnaroundTime = 0;

    for (const auto& task : tasks) {
        totalWaitingTime += task.waitingTime;
        totalTurnaroundTime += task.turnaroundTime;
    }

    avgWaitingTime = (float)totalWaitingTime / tasks.size();
    avgTurnaroundTime = (float)totalTurnaroundTime / tasks.size();
}
```
The calculateAverageTimes function iterates through a vector of tasks to accumulate their waiting times and turnaround times. It then computes the average values by dividing the accumulated totals by the number of tasks.

## Scheduling Algorithms

### First-Come-First-Serve (FCFS)

First-Come-First-Serve (FCFS) is one of the simplest CPU scheduling algorithms, where tasks are processed in the order they arrive. In this approach, the CPU executes the tasks sequentially based on their arrival time, without considering their execution time or priority. FCFS ensures fairness, as tasks are serviced in the order they request the CPU. However, it may lead to inefficiency if long-running tasks (with high burst times) arrive first, causing shorter tasks to wait excessively. This phenomenon is known as the "convoy effect," where shorter tasks get delayed behind longer ones. FCFS scheduling is suitable for scenarios where task deadlines are less critical and simplicity outweighs potential performance optimizations.
```cpp
void fcfs(vector<Task>& tasks) {
    int currentTime = 0;
    for (auto& task : tasks) {
        if (currentTime < task.arrivalTime) {
            currentTime = task.arrivalTime;
        }
        task.waitingTime = currentTime - task.arrivalTime;
        task.turnaroundTime = task.waitingTime + task.burstTime;
        currentTime += task.burstTime;
    }
}
```
### Shortest Job First (SJF)

Shortest Job First (SJF) scheduling selects the task with the smallest burst time (execution time) next for CPU execution. This algorithm aims to minimize average waiting time and improve system throughput by prioritizing shorter tasks. SJF can be either preemptive or non-preemptive: in preemptive SJF, a new shorter task arriving during execution of a longer task can interrupt and take over; in non-preemptive SJF, once a task starts executing, it continues until completion. The main advantage of SJF is its ability to reduce waiting times for tasks, leading to improved overall system performance and faster response times for shorter jobs. However, predicting exact burst times accurately in real-world scenarios can be challenging, and SJF may suffer from starvation for longer tasks if a continuous stream of short tasks keeps arriving.

```cpp
void sjf(vector<Task>& tasks) {
    int currentTime = 0;
    int completedTasks = 0;
    while (completedTasks < tasks.size()) {
        vector<Task*> availableTasks;
        for (auto& task : tasks) {
            if (task.arrivalTime <= currentTime && task.remainingTime > 0) {
                availableTasks.push_back(&task);
            }
        }
        if (!availableTasks.empty()) {
            auto shortestTask = min_element(availableTasks.begin(), availableTasks.end(), [](Task* a, Task* b) {
                return a->remainingTime < b->remainingTime;
            });
            Task* task = *shortestTask;
            currentTime += task->remainingTime;
            task->remainingTime = 0;
            task->turnaroundTime = currentTime - task->arrivalTime;
            task->waitingTime = task->turnaroundTime - task->burstTime;
            completedTasks++;
        } else {
            currentTime++;
        }
    }
}
```
### Priority Scheduling

Priority Scheduling assigns priorities to tasks and selects the task with the highest priority for execution. In this algorithm, tasks with higher priority (lower numeric value) are executed before tasks with lower priority. Priority scheduling can be preemptive (where tasks can be interrupted and rescheduled based on changing priorities) or non-preemptive (where a task continues to execute until completion). This approach allows for the implementation of different scheduling policies, such as preemptive priority (where new higher-priority tasks preempt currently running tasks) or non-preemptive priority (where tasks run to completion without interruption). Priority scheduling ensures that tasks critical to the system's operation or user interaction are handled promptly, but it requires careful management of priorities to avoid starvation of lower-priority tasks. Balancing task priorities effectively is crucial to optimizing system performance and responsiveness.

```cpp
void priorityScheduling(vector<Task>& tasks) {
    int currentTime = 0;
    int completedTasks = 0;
    while (completedTasks < tasks.size()) {
        vector<Task*> availableTasks;
        for (auto& task : tasks) {
            if (task.arrivalTime <= currentTime && task.remainingTime > 0) {
                availableTasks.push_back(&task);
            }
        }
        if (!availableTasks.empty()) {
            auto highestPriorityTask = min_element(availableTasks.begin(), availableTasks.end(), [](Task* a, Task* b) {
                return a->priority < b->priority;
            });
            Task* task = *highestPriorityTask;
            currentTime += task->remainingTime;
            task->remainingTime = 0;
            task->turnaroundTime = currentTime - task->arrivalTime;
            task->waitingTime = task->turnaroundTime - task->burstTime;
            completedTasks++;
        } else {
            currentTime++;
        }
    }
}
```
### Round Robin

Round Robin (RR) scheduling allocates a fixed time quantum (slice) to each task in a cyclic manner. Tasks are executed for a predefined time slice, and if they do not finish within that time, they are preempted and put back into the ready queue. Round Robin ensures fair CPU time distribution among tasks, preventing any single task from monopolizing the CPU for an extended period. This approach is effective for time-sharing systems and environments where tasks have similar execution requirements. Round Robin scheduling is simple to implement and guarantees that every task gets an equal opportunity to execute, promoting system responsiveness and fairness. However, choosing an appropriate time quantum is crucial: a too short quantum may lead to frequent context switches and overhead, while a too long quantum may reduce the benefits of fairness and responsiveness.

```cpp
void roundRobin(vector<Task>& tasks, int timeQuantum) {
    queue<Task*> taskQueue;
    int currentTime = 0;
    int completedTasks = 0;

    // Push tasks into the queue based on their arrival time
    auto taskIter = tasks.begin();
    while (taskIter != tasks.end() && taskIter->arrivalTime <= currentTime) {
        taskQueue.push(&(*taskIter));
        ++taskIter;
    }

    while (completedTasks < tasks.size()) {
        if (!taskQueue.empty()) {
            Task* currentTask = taskQueue.front();
            taskQueue.pop();

            if (currentTask->remainingTime > timeQuantum) {
                currentTime += timeQuantum;
                currentTask->remainingTime -= timeQuantum;
            } else {
                currentTime += currentTask->remainingTime;
                currentTask->remainingTime = 0;
                currentTask->turnaroundTime = currentTime - currentTask->arrivalTime;
                currentTask->waitingTime = currentTask->turnaroundTime - currentTask->burstTime;
                completedTasks++;
            }

            // Push newly arrived tasks into the queue
            while (taskIter != tasks.end() && taskIter->arrivalTime <= currentTime) {
                taskQueue.push(&(*taskIter));
                ++taskIter;
            }

            // Push the current task back into the queue if it's not completed
            if (currentTask->remainingTime > 0) {
                taskQueue.push(currentTask);
            }
        } else {
            currentTime++;
            while (taskIter != tasks.end() && taskIter->arrivalTime <= currentTime) {
                taskQueue.push(&(*taskIter));
                ++taskIter;
            }
        }
    }
}
```
## Main Function

The main function initializes tasks based on user input, allows the user to choose the scheduling algorithm, and displays detailed results including average waiting time and average turnaround time.

```cpp

int main() {
    int n, timeQuantum, choice;
    cout << "Enter the number of tasks (max 50): ";
    cin >> n;

    if (n > 50) {
        cout << "Number of tasks should not exceed 50." << endl;
        return 1;
    }

    vector<Task> tasks(n);
    for (int i = 0; i < n; ++i) {
        cout << "Enter arrival time, burst time, and priority for task " << i + 1 << ": ";
        cin >> tasks[i].arrivalTime >> tasks[i].burstTime >> tasks[i].priority;
        tasks[i].id = i + 1;
        tasks[i].remainingTime = tasks[i].burstTime;
    }

    cout << "Choose the scheduling algorithm:\n";
    cout << "1. FCFS\n2. SJF\n3. Priority\n4. Round Robin\n";
    cin >> choice;

    if (choice == 4) {
        cout << "Enter time quantum for Round Robin scheduling: ";
        cin >> timeQuantum;
    }

    vector<Task> selectedTasks = tasks;

    switch (choice) {
        case 1:
            fcfs(selectedTasks);
            printResults(selectedTasks, "FCFS");
            break;
        case 2:
            sjf(selectedTasks);
            printResults(selectedTasks, "SJF");
            break;
        case 3:
            priorityScheduling(selectedTasks);
            printResults(selectedTasks, "Priority");
            break;
        case 4:
            roundRobin(selectedTasks, timeQuantum);
            printResults(selectedTasks, "Round Robin");
            break;
        default:
            cout << "Invalid choice!" << endl;
            return 1;
    }

    float avgWaitingTime, avgTurnaroundTime;
    calculateAverageTimes(selectedTasks, avgWaitingTime, avgTurnaroundTime);
    cout << "\nAverage Waiting Time: " << avgWaitingTime << endl;
    cout << "Average Turnaround Time: " << avgTurnaroundTime << endl;

    return 0;
}
```
The main function facilitates interaction with the user, allowing them to input task details and select a scheduling algorithm. It then executes the chosen algorithm on the task set and displays comprehensive results, including average waiting time and average turnaround time.

## Conclusion

In conclusion, this document has provided an in-depth explanation of four CPU scheduling algorithms: FCFS, SJF, Priority Scheduling, and Round Robin. Each algorithm has been implemented in C++, allowing for simulation and evaluation of task scheduling strategies based on arrival time, burst time, and priority. Understanding these algorithms is essential for optimizing CPU utilization, improving system performance, and meeting application-specific requirements.


