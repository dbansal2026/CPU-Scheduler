#include <bits/stdc++.h>
using namespace std;

struct TASK
{
    int id;
    int arrival_time;
    int burst_time;
    int priority;
    int remaining_time;
};

void FCFS_scheduling(vector<TASK> &TASKS, const int &num_TASKS);

void SJF_scheduling(vector<TASK> &TASKS, const int &num_TASKS);

void P_scheduling(vector<TASK> &TASKS, const int &num_TASKS);

void RR_scheduling(vector<TASK> &TASKS, const int &num_TASKS, const int &time_quantum);

int main()
{
    int num_TASKS;
    cout << "Enter the number of TASKS: ";
    cin >> num_TASKS;

    vector<TASK> TASKS(num_TASKS);
    cout << "Considering high no. represent highest Priority\n";
    cout << "Enter TASK details (arrival time, burst time, priority):\n";
    for (int i = 0; i < num_TASKS; ++i)
    {
        TASKS[i].id = i + 1;
        cout << "TASK " << TASKS[i].id << ": ";
        cin >> TASKS[i].arrival_time >> TASKS[i].burst_time >> TASKS[i].priority;
        TASKS[i].remaining_time = TASKS[i].burst_time;
    }

    cout << "Select the scheduling algorithm:\n"
         << "1. First Come First Served Scheduling Algorithm\n"
         << "2. Shortest Job First Preemptive Scheduling Algorithm\n"
         << "3. Priority Preemptive Scheduling Algorithm\n"
         << "4. Round Robin Scheduling Algorithm\n\n";

    int algo_select;
    cin >> algo_select;
    if (algo_select == 1)
    {
        FCFS_scheduling(TASKS, num_TASKS);
    }
    else if (algo_select == 2)
    {
        SJF_scheduling(TASKS, num_TASKS);
    }
    else if (algo_select == 3)
    {
        P_scheduling(TASKS, num_TASKS);
    }
    else if (algo_select == 4)
    {
        int time_quantum;
        cout << "\nEnter the time quantum for Round Robin scheduling: ";
        cin >> time_quantum;
        RR_scheduling(TASKS, num_TASKS, time_quantum);
    }
    else
    {
        cout << "Invalid Input\n";
    }

    return 0;
}

void FCFS_scheduling(vector<TASK> &TASKS, const int &num_TASKS)
{
    // Sort TASKS by arrival time
    sort(TASKS.begin(), TASKS.end(), [](const TASK &a, const TASK &b)
         { return a.arrival_time < b.arrival_time; });

    int execution_lower[num_TASKS], execution_higher[num_TASKS], waiting[num_TASKS], turnaround[num_TASKS];

    cout << "\nFCFS Scheduling:\n\n";
    int recent_time = 0;
    int total_turnaround_time = 0;
    int total_waiting_time = 0;

    for (const TASK &TASK : TASKS)
    {
        // Wait if the TASK hasn't arrived yet
        if (recent_time < TASK.arrival_time)
        {
            recent_time = TASK.arrival_time;
        }

        // Execute the TASK
        execution_lower[TASK.id - 1] = recent_time;
        recent_time += TASK.burst_time;
        execution_higher[TASK.id - 1] = recent_time;
        turnaround[TASK.id - 1] = recent_time - TASK.arrival_time;
        waiting[TASK.id - 1] = turnaround[TASK.id - 1] - TASK.burst_time;
        total_turnaround_time += turnaround[TASK.id - 1];
        total_waiting_time += waiting[TASK.id - 1];
    }

    sort(TASKS.begin(), TASKS.end(), [](const TASK &a, const TASK &b)
         { return a.id < b.id; });

    for (const TASK &TASK : TASKS)
    {
        cout << "TASK " << TASK.id << " executed during time " << execution_lower[TASK.id - 1];
        cout << " to " << execution_higher[TASK.id - 1] << " unit" << endl;
        cout << "Turn Around Time for TASK " << TASK.id << " is " << turnaround[TASK.id - 1] << " unit" << endl;
        cout << "Waiting Time for TASK " << TASK.id << " is " << waiting[TASK.id - 1] << " unit\n"
             << endl;
    }

    double average_waiting_time = static_cast<double>(total_waiting_time) / num_TASKS;
    double average_turnaround_time = static_cast<double>(total_turnaround_time) / num_TASKS;

    cout << "Average Turn Around Time: " << average_turnaround_time << " unit" << endl;
    cout << "Average Waiting Time: " << average_waiting_time << " unit" << endl;
}

void SJF_scheduling(vector<TASK> &TASKS, const int &num_TASKS)
{
    //Considering this algo to be Preemptive  
    int bursttime[num_TASKS];
    for (int i = 0; i < num_TASKS; i++)
    {
        bursttime[TASKS[i].id - 1] = TASKS[i].burst_time;
    }
    // Sort TASKS primarily by arrival time, secondarily by burst time
    sort(TASKS.begin(), TASKS.end(), [](const TASK &a, const TASK &b)
         {
        if (a.arrival_time == b.arrival_time) {
            return a.burst_time < b.burst_time;
        }
        return a.arrival_time < b.arrival_time; });

    vector<vector<int>> execution(num_TASKS);
    cout << "\nSJF Scheduling:\n"
         << endl;
    int recent_time = 0;
    int total_turnaround_time = 0;
    int total_waiting_time = 0;
    int TASKS_completed = 0;

    while (TASKS_completed < num_TASKS)
    {
        int shortest_job_index = -1;
        for (int i = 0; i < num_TASKS; ++i)
        {
            if (TASKS[i].arrival_time <= recent_time && TASKS[i].burst_time > 0)
            {
                if (shortest_job_index == -1 || TASKS[i].burst_time < TASKS[shortest_job_index].burst_time)
                {
                    shortest_job_index = i;
                }
            }
        }

        if (shortest_job_index == -1)
        {
            // No TASK is ready to execute, move to the next available TASK's arrival time
            recent_time++;
            continue;
        }

        TASK &TASK = TASKS[shortest_job_index];
        TASK.burst_time--;
        execution[TASK.id - 1].push_back(recent_time);

        if (TASK.burst_time == 0)
        {
            TASKS_completed++;
        }

        recent_time++;
    }

    // Sort TASKS back by their IDs for output consistency
    sort(TASKS.begin(), TASKS.end(), [](const TASK &a, const TASK &b)
         { return a.id < b.id; });

    // Print the execution times for each TASK
    for (const TASK &TASK : TASKS)
    {
        cout << "TASK " << TASK.id << " executed during time ";
        if (execution[TASK.id - 1].empty())
        {
            cout << "no time";
        }
        else
        {
            int start = execution[TASK.id - 1][0], end = execution[TASK.id - 1][0];
            for (size_t i = 1; i < execution[TASK.id - 1].size(); ++i)
            {
                if (execution[TASK.id - 1][i] == execution[TASK.id - 1][i - 1] + 1)
                {
                    end = execution[TASK.id - 1][i];
                }
                else
                {
                    if (start == end)
                    {
                        cout << start << " to " << start + 1;
                    }
                    else
                    {
                        cout << start << " to " << end + 1;
                    }
                    cout << ", ";
                    start = end = execution[TASK.id - 1][i];
                }
            }
            int completion_time = 0;
            if (start == end)
            {
                cout << start << " to " << start + 1 << " unit";
                completion_time = start + 1;
            }
            else
            {
                cout << start << " to " << end + 1 << " unit";
                completion_time = end + 1;
            }
            int turnaroundtime = completion_time - TASK.arrival_time;
            int waitingtime = turnaroundtime - bursttime[TASK.id - 1];
            cout << "\nTurn Around Time for TASK " << TASK.id << " is " << turnaroundtime << " unit" << endl;
            cout << "Waiting Time for TASK " << TASK.id << " is " << waitingtime << " unit"
                 << endl;
            total_waiting_time += waitingtime;
            total_turnaround_time += turnaroundtime;
        }
        cout << endl;
    }

    double average_waiting_time = static_cast<double>(total_waiting_time) / num_TASKS;
    double average_turnaround_time = static_cast<double>(total_turnaround_time) / num_TASKS;

    cout << "Average Turnaround Time: " << average_turnaround_time << " units" << endl;
    cout << "Average Waiting Time: " << average_waiting_time << " units" << endl;
}

void P_scheduling(vector<TASK> &TASKS, const int &num_TASKS)
{
    // Considering this algo to be Preemptive
    int bursttime[num_TASKS];
    for (int i = 0; i < num_TASKS; i++)
    {
        bursttime[TASKS[i].id - 1] = TASKS[i].burst_time;
    }
    // Sort TASKS primarily by arrival time, secondarily by burst time
    sort(TASKS.begin(), TASKS.end(), [](const TASK &a, const TASK &b)
         {
        if (a.arrival_time == b.arrival_time) {
            return a.priority > b.priority;
        }
        return a.arrival_time < b.arrival_time; });

    vector<vector<int>> execution(num_TASKS);
    cout << "\nPriority Scheduling:\n"
         << endl;
    int recent_time = 0;
    int total_turnaround_time = 0;
    int total_waiting_time = 0;
    int TASKS_completed = 0;

    while (TASKS_completed < num_TASKS)
    {
        int highest_priority_index = -1;
        for (int i = 0; i < num_TASKS; ++i)
        {
            if (TASKS[i].arrival_time <= recent_time && TASKS[i].burst_time > 0)
            {
                if (highest_priority_index == -1 || TASKS[i].priority > TASKS[highest_priority_index].priority)
                {
                    highest_priority_index = i;
                }
            }
        }

        if (highest_priority_index == -1)
        {
            // No TASK is ready to execute, move to the next available TASK's arrival time
            recent_time++;
            continue;
        }

        TASK &TASK = TASKS[highest_priority_index];
        TASK.burst_time--;
        execution[TASK.id - 1].push_back(recent_time);

        if (TASK.burst_time == 0)
        {
            TASKS_completed++;
        }

        recent_time++;
    }

    // Sort TASKS back by their IDs for output consistency
    sort(TASKS.begin(), TASKS.end(), [](const TASK &a, const TASK &b)
         { return a.id < b.id; });

    // Print the execution times for each TASK
    for (const TASK &TASK : TASKS)
    {
        cout << "TASK " << TASK.id << " executed during time ";
        if (execution[TASK.id - 1].empty())
        {
            cout << "no time";
        }
        else
        {
            int start = execution[TASK.id - 1][0], end = execution[TASK.id - 1][0];
            for (size_t i = 1; i < execution[TASK.id - 1].size(); ++i)
            {
                if (execution[TASK.id - 1][i] == execution[TASK.id - 1][i - 1] + 1)
                {
                    end = execution[TASK.id - 1][i];
                }
                else
                {
                    if (start == end)
                    {
                        cout << start << " to " << start + 1;
                    }
                    else
                    {
                        cout << start << " to " << end + 1;
                    }
                    cout << ", ";
                    start = end = execution[TASK.id - 1][i];
                }
            }
            int completion_time = 0;
            if (start == end)
            {
                cout << start << " to " << start + 1 << " unit";
                completion_time = start + 1;
            }
            else
            {
                cout << start << " to " << end + 1 << " unit";
                completion_time = end + 1;
            }
            int turnaroundtime = completion_time - TASK.arrival_time;
            int waitingtime = turnaroundtime - bursttime[TASK.id - 1];
            cout << "\nTurn Around Time for TASK " << TASK.id << " is " << turnaroundtime << " unit" << endl;
            cout << "Waiting Time for TASK " << TASK.id << " is " << waitingtime << " unit"
                 << endl;
            total_waiting_time += waitingtime;
            total_turnaround_time += turnaroundtime;
        }
        cout << endl;
    }

    double average_waiting_time = static_cast<double>(total_waiting_time) / num_TASKS;
    double average_turnaround_time = static_cast<double>(total_turnaround_time) / num_TASKS;

    cout << "Average Turnaround Time: " << average_turnaround_time << " units" << endl;
    cout << "Average Waiting Time: " << average_waiting_time << " units" << endl;
}

void RR_scheduling(vector<TASK> &TASKS, const int &num_TASKS, const int &time_quantum)
{
    queue<TASK *> TASK_queue;
    int recent_time = 0;
    int total_turnaround_time = 0;
    int total_waiting_time = 0;
    int TASKS_completed = 0;
    cout << "\nRound Robin Scheduling:\n"
         << endl;

    vector<pair<int, int>> execution[num_TASKS];

    // Sort TASKS by arrival time
    sort(TASKS.begin(), TASKS.end(), [](const TASK &a, const TASK &b)
         { return a.arrival_time < b.arrival_time; });

    // Add initial TASKS to the queue
    auto TASK_it = TASKS.begin();
    while (TASK_it != TASKS.end() && TASK_it->arrival_time <= recent_time)
    {
        TASK_queue.push(&(*TASK_it));
        TASK_it++;
    }

    while (TASKS_completed < num_TASKS)
    {
        if (!TASK_queue.empty())
        {
            TASK *TASK = TASK_queue.front();
            TASK_queue.pop();

            int exec_time = min(time_quantum, TASK->remaining_time);

            execution[TASK->id - 1].emplace_back(recent_time, recent_time + exec_time);
            TASK->remaining_time -= exec_time;
            recent_time += exec_time;

            // Add new TASKS to the queue that have arrived
            while (TASK_it != TASKS.end() && TASK_it->arrival_time <= recent_time)
            {
                TASK_queue.push(&(*TASK_it));
                TASK_it++;
            }

            if (TASK->remaining_time > 0)
            {
                TASK_queue.push(TASK);
            }
            else
            {
                int turnaround_time = recent_time - TASK->arrival_time;
                int waiting_time = turnaround_time - TASK->burst_time;

                total_turnaround_time += turnaround_time;
                total_waiting_time += waiting_time;
                TASKS_completed++;
            }
        }
        else
        {
            recent_time++;
        }
    }
    sort(TASKS.begin(), TASKS.end(), [](const TASK &a, const TASK &b)
         { return a.id < b.id; });

    for (const TASK &TASK : TASKS)
    {
        cout << "TASK " << TASK.id << " executed during time ";
        for (const auto &period : execution[TASK.id - 1])
        {
            cout << period.first << " to " << period.second << (period == execution[TASK.id - 1].back() ? " unit" : ", ");
        }
        cout << endl;
        int turnaround_time = execution[TASK.id - 1].back().second - TASK.arrival_time;
        int waiting_time = turnaround_time - TASK.burst_time;
        cout << "Turn Around Time for TASK " << TASK.id << " is " << turnaround_time << " unit" << endl;
        cout << "Waiting Time for TASK " << TASK.id << " is " << waiting_time << " unit\n"
             << endl;
    }

    double average_waiting_time = static_cast<double>(total_waiting_time) / num_TASKS;
    double average_turnaround_time = static_cast<double>(total_turnaround_time) / num_TASKS;
    cout << "\nAverage Turnaround Time: " << average_turnaround_time << " unit" << endl;
    cout << "Average Waiting Time: " << average_waiting_time << " unit" << endl;
}
