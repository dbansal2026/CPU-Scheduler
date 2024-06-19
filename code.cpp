#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

struct Task {
    int id;
    int arrivalTime;
    int burstTime;
    int priority;
    int waitingTime;
    int turnaroundTime;
    int remainingTime;
};

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

void printResults(const vector<Task>& tasks, const string& algorithm) {
    cout << "\n" << algorithm << " Scheduling Results:\n";
    cout << "Task ID\tArrival Time\tBurst Time\tPriority\tWaiting Time\tTurnaround Time\n";
    for (const auto& task : tasks) {
        cout << task.id << "\t" << task.arrivalTime << "\t\t" << task.burstTime << "\t\t" << task.priority << "\t\t" 
             << task.waitingTime << "\t\t" << task.turnaroundTime << "\n";
    }
}

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

            // Add new arriving tasks to the queue
            while (taskIter != tasks.end() && taskIter->arrivalTime <= currentTime) {
                taskQueue.push(&(*taskIter));
                ++taskIter;
            }

            // Requeue the current task if it's not yet complete
            if (currentTask->remainingTime > 0) {
                taskQueue.push(currentTask);
            }
        } else {
            // If the queue is empty, move time forward to the next task arrival
            if (taskIter != tasks.end()) {
                currentTime = taskIter->arrivalTime;
                taskQueue.push(&(*taskIter));
                ++taskIter;
            }
        }
    }
}

int main() {
    int n, timeQuantum, choice;
    cout << "Enter the number of tasks : ";
    cin >> n;

    vector<Task> tasks(n);
    for (int i = 0; i < n; ++i) {
        cout << "Enter arrival time, burst time and priority for task " << i + 1 << ": ";
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
    cout << "\n" << (choice == 1 ? "FCFS" : choice == 2 ? "SJF" : choice == 3 ? "Priority" : "Round Robin") 
         << " - Average Waiting Time: " << avgWaitingTime << ", Average Turnaround Time: " << avgTurnaroundTime << "\n";

    return 0;
}
