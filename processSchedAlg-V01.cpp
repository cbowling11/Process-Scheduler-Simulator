

/*
Prject for Operating Systems class. 
Using the preemptive and non-premepted scheduling algorithms this project will simulate the given algrithm and 
either calculate or count the deliveriable expected in the assignment instructions.

***DELIVERABLES***
A- Generate a list of processes at arrival time ZERO.
B- Input the following fields.
a. Process Id
b. Arrival Time
c. Burst Time (Service Time)
d. Completion Time
C- Calculate the Turn Around Time for each process
D- Calculate the Waiting Time for each process
E- Calculate the Average Turn Around Time for all processes for each algorithm
F- Calculate the Average Waiting Time for all processes for each algorithm
G- Determine the algorithm that has the minimum Average Turn Around Time.
H- Determine the algorithm that has the minimum Average Waiting Time.
I- Use Menu to enable the user to choose the algorithms that he/she wants to simulate

***THE 5 ALGORITHMS TO TEST***
1- First-Come, First-Served FCFS Scheduling (None-Preemptive)
2- Shortest-Job-First SJF Scheduling (None-Preemptive)
3- Shortest Remaining Time (Preemptive)
4- Priority Scheduling (Preemptive)
5- Round Robin - RR Scheduling (Preemptive)

*/

#include <iostream>
#include <limits>
#include <vector>
#include <algorithm> 
#include <numeric>
using namespace std;



//**********CLASS TO HOLD VARIBLES WE NEED FOR EACH ALGORITHM************
class Process{
public:
    int processId;
    int arrivalTime;
    int burstTime;
    int completionTime;
    int turnAroundTime;
    int waitingTime;
    int priority;

    Process(int id, int arrival, int burst)
        : processId(id), arrivalTime(arrival), burstTime(burst), completionTime(0), turnAroundTime(0), waitingTime(0), priority(priority) {}
};


//********Function to Calculate 
void calculateMetrics(const vector<Process> &processes) {
    double totalTurnAroundTime = 0;
    double totalWaitingTime = 0;

    for (const auto &process : processes) {
        totalTurnAroundTime += process.turnAroundTime;
        totalWaitingTime += process.waitingTime;

        cout << "Process " << process.processId << ": "
             << "Turn Around Time = " << process.turnAroundTime << ", "
             << "Waiting Time = " << process.waitingTime << endl;
    }

    cout << "Average Turn Around Time: " << totalTurnAroundTime / processes.size() << endl;
    cout << "Average Waiting Time: " << totalWaitingTime / processes.size() << endl;
}
//************First come first serve algorithm*************************
void firstComeFirstServe(vector<Process> &processes) {
    int currentTime = 0;
  cout << "Running FCFS algorithm " << endl;
    for (auto &process : processes) {
        if (currentTime < process.arrivalTime) {
            currentTime = process.arrivalTime; // Wait for the process to arrive
        }
        process.completionTime = currentTime + process.burstTime;
        process.turnAroundTime = process.completionTime - process.arrivalTime;
        process.waitingTime = process.turnAroundTime - process.burstTime;

        currentTime = process.completionTime;
    
    }
    calculateMetrics(processes);
}

void shortestJobFirst(vector<Process> &processes) {
    cout << "Running SJF algorithm " << endl;
    int currentTime = 0;
    vector<bool> isCompleted(processes.size(), false);
    int completedProcesses = 0;

    while (completedProcesses < processes.size()) {
        int shortestJobIndex = -1;
        for (size_t i = 0; i < processes.size(); ++i) {
            if (!isCompleted[i] && processes[i].arrivalTime <= currentTime) {
                if (shortestJobIndex == -1 || processes[i].burstTime < processes[shortestJobIndex].burstTime) {
                    shortestJobIndex = i;
                }
            }
        }

        if (shortestJobIndex == -1) {
            currentTime++;
        } else {
            Process &p = processes[shortestJobIndex];
            currentTime += p.burstTime;
            p.completionTime = currentTime;
            p.turnAroundTime = p.completionTime - p.arrivalTime;
            p.waitingTime = p.turnAroundTime - p.burstTime;
            isCompleted[shortestJobIndex] = true;
            completedProcesses++;
        }
    }
    calculateMetrics(processes);
}

//****************Shortest Time Remaining Algorithm*********************
void shortestRemainingTime(vector<Process> &processes) {
    cout << "Running SRT algorithm " << endl;
    int currentTime = 0;
    int completedProcesses = 0;
    vector<int> remainingBurstTime(processes.size());

    for (size_t i = 0; i < processes.size(); ++i) {
        remainingBurstTime[i] = processes[i].burstTime;
    }

    while (completedProcesses < processes.size()) {
        int shortestRemainingIndex = -1;
        for (size_t i = 0; i < processes.size(); ++i) {
            if (processes[i].arrivalTime <= currentTime && remainingBurstTime[i] > 0) {
                if (shortestRemainingIndex == -1 || remainingBurstTime[i] < remainingBurstTime[shortestRemainingIndex]) {
                    shortestRemainingIndex = i;
                }
            }
        }

        if (shortestRemainingIndex == -1) {
            currentTime++;
        } else {
            Process &p = processes[shortestRemainingIndex];
            remainingBurstTime[shortestRemainingIndex]--;
            currentTime++;

            if (remainingBurstTime[shortestRemainingIndex] == 0) {
                p.completionTime = currentTime;
                p.turnAroundTime = p.completionTime - p.arrivalTime;
                p.waitingTime = p.turnAroundTime - p.burstTime;
                completedProcesses++;
            }
        }
    }
    calculateMetrics(processes);
}
void priorityScheduling(vector<Process> &processes){
    cout << "Running Priority Scheduling algorithm " << endl;
    int currentTime = 0;

    // Sort by arrival time initially to maintain order of arrival
    sort(processes.begin(), processes.end(), [](const Process &a, const Process &b) {
        return a.arrivalTime < b.arrivalTime;
    });

    vector<bool> isCompleted(processes.size(), false);
    int completedProcesses = 0;

    while (completedProcesses < processes.size()) {
        // Find the process with the highest priority that is ready to run
        int highestPriorityIndex = -1;
        for (size_t i = 0; i < processes.size(); ++i) {
            if (processes[i].arrivalTime <= currentTime && !isCompleted[i] && processes[i].burstTime > 0) {
                if (highestPriorityIndex == -1 || processes[i].priority < processes[highestPriorityIndex].priority) {
                    highestPriorityIndex = i;
                }
            }
        }

        if (highestPriorityIndex == -1) {
            // No process is ready, advance time
            currentTime++;
        } else {
            // Execute the process with the highest priority for 1 unit of time (preemption)
            Process &p = processes[highestPriorityIndex];
            currentTime++;
            p.burstTime--;

            if (p.burstTime == 0) {
                p.completionTime = currentTime;
                p.turnAroundTime = p.completionTime - p.arrivalTime;
                p.waitingTime = p.turnAroundTime - (p.completionTime - p.turnAroundTime);
                isCompleted[highestPriorityIndex] = true;
                completedProcesses++;

                
            }
        }
        
    }
    calculateMetrics(processes);
    

}

void roundRobin(vector<Process> &processes, int timeQuantum) {
    cout << "Running Round Robin algorithm " << endl;
    int currentTime = 0;
    vector<int> remainingBurstTime(processes.size());

    for (size_t i = 0; i < processes.size(); ++i) {
        remainingBurstTime[i] = processes[i].burstTime;
    }

    while (true) {
        bool allDone = true;
        for (size_t i = 0; i < processes.size(); ++i) {
            if (remainingBurstTime[i] > 0) {
                allDone = false;

                if (remainingBurstTime[i] > timeQuantum) {
                    currentTime += timeQuantum;
                    remainingBurstTime[i] -= timeQuantum;
                } else {
                    currentTime += remainingBurstTime[i];
                    remainingBurstTime[i] = 0;
                    processes[i].completionTime = currentTime;
                    processes[i].turnAroundTime = processes[i].completionTime - processes[i].arrivalTime;
                    processes[i].waitingTime = processes[i].turnAroundTime - processes[i].burstTime;
                }
            }
        }

        if (allDone) {
            break;
        }
    }
    calculateMetrics(processes);
}

void chooseAlgo(){
    void generateProcesses(vector<Process> &processes, int numProcesses, bool requiresPrior);
    void determineBestAlgorithm(const vector<double> &averageTurnAroundTimes, const vector<double> &averageWaitingTimes);
    vector<Process> processes;
    vector<int> choices;
    int choice, numProcesses;
    bool requiresPrior = false;
    vector<double> averageTurnAroundTimes;
    vector<double> averageWaitingTimes;

    cout << "Enter the number of processes to test: ";
    cin >> numProcesses;

    cout << "Please enter the number of the algorithms you would like to test (up to 5 selections)" << endl << "(enter -1 to finish selection):" << endl;
    cout << "First Come First Serve = 1" << endl << "Shortest Job First = 2" << endl << "Shortest Time Remaining = 3" << endl << "Priority Scheduling = 4" << endl << "Round Robin = 5" << endl;

    cout << "Available algorithms: 1, 2, 3, 4, 5" << endl;

    // Capture user choices
    while (true) {
        cin >> choice;

        if (choice == 4){
            requiresPrior = true;
        }

        if (choice == -1) {
            break;  // User finished selecting
        } else if (choice >= 1 && choice <= 5) {
            if (find(choices.begin(), choices.end(), choice) == choices.end()) {
                choices.push_back(choice);
            } else {
                cout << "Algorithm " << choice << " is already selected." << endl;
            }
        } else {
            cout << "Invalid choice. Please enter a number between 1 and 5 or -1 to finish." << endl;
        }
    }
    generateProcesses(processes, numProcesses, requiresPrior);

    cout << "\nRunning selected algorithms:\n" << endl;
    for (int selected : choices) {
        vector<Process> tempProcesses = processes;
        double avgTurnAroundTime = 0, avgWaitingTime = 0;

        switch (selected) {
            case 1:
                firstComeFirstServe(tempProcesses);
                avgTurnAroundTime = accumulate(tempProcesses.begin(), tempProcesses.end(), 0.0, [](double sum, const Process& p) { return sum + p.turnAroundTime; }) / tempProcesses.size();
                avgWaitingTime = accumulate(tempProcesses.begin(), tempProcesses.end(), 0.0, [](double sum, const Process& p) { return sum + p.waitingTime; }) / tempProcesses.size();
                break;
            case 2:
                shortestJobFirst(tempProcesses);
                avgTurnAroundTime = accumulate(tempProcesses.begin(), tempProcesses.end(), 0.0, [](double sum, const Process& p) { return sum + p.turnAroundTime; }) / tempProcesses.size();
                avgWaitingTime = accumulate(tempProcesses.begin(), tempProcesses.end(), 0.0, [](double sum, const Process& p) { return sum + p.waitingTime; }) / tempProcesses.size();
                break;
            case 3:
                shortestRemainingTime(tempProcesses);
                avgTurnAroundTime = accumulate(tempProcesses.begin(), tempProcesses.end(), 0.0, [](double sum, const Process& p) { return sum + p.turnAroundTime; }) / tempProcesses.size();
                avgWaitingTime = accumulate(tempProcesses.begin(), tempProcesses.end(), 0.0, [](double sum, const Process& p) { return sum + p.waitingTime; }) / tempProcesses.size();
                break;
            case 4:
                priorityScheduling(tempProcesses);
                avgTurnAroundTime = accumulate(tempProcesses.begin(), tempProcesses.end(), 0.0, [](double sum, const Process& p) { return sum + p.turnAroundTime; }) / tempProcesses.size();
                avgWaitingTime = accumulate(tempProcesses.begin(), tempProcesses.end(), 0.0, [](double sum, const Process& p) { return sum + p.waitingTime; }) / tempProcesses.size();
                break;
            case 5:
                roundRobin(tempProcesses, 4); // Assuming time quantum of 4
                avgTurnAroundTime = accumulate(tempProcesses.begin(), tempProcesses.end(), 0.0, [](double sum, const Process& p) { return sum + p.turnAroundTime; }) / tempProcesses.size();
                avgWaitingTime = accumulate(tempProcesses.begin(), tempProcesses.end(), 0.0, [](double sum, const Process& p) { return sum + p.waitingTime; }) / tempProcesses.size();
                break;
        }

        averageTurnAroundTimes.push_back(avgTurnAroundTime);
        averageWaitingTimes.push_back(avgWaitingTime);
    }

    determineBestAlgorithm(averageTurnAroundTimes, averageWaitingTimes);
}

void generateProcesses(vector<Process> &processes, int numProcesses, bool requiresPrior) {
    for (int i = 0; i < numProcesses; i++) {
        int arrivalTime, burstTime, priority;
        cout << "Enter arrival time for process " << i + 1 << ": ";
        cin >> arrivalTime;
        cout << "Enter burst time for process " << i + 1 << ": " << endl;
        cin >> burstTime;

        if(requiresPrior){
            cout << "Enter priority for process " << i + 1 << ":" << endl;
            cin >> priority;
        }

        processes.emplace_back(i + 1, arrivalTime, burstTime);
    }
}
void determineBestAlgorithm(const vector<double> &averageTurnAroundTimes, const vector<double> &averageWaitingTimes) {
    double minTurnAround = *min_element(averageTurnAroundTimes.begin(), averageTurnAroundTimes.end());
    double minWaiting = *min_element(averageWaitingTimes.begin(), averageWaitingTimes.end());

    cout << "Minimum Average Turn Around Time: " << minTurnAround << endl;
    cout << "Minimum Average Waiting Time: " << minWaiting << endl;
}

int main() {
    chooseAlgo();
    
    return 0;
}
