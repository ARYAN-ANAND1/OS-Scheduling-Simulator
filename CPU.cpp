#include <bits/stdc++.h>
using namespace std;

struct Process {
    int id;         // Process ID
    int at;         // Arrival Time
    int bt;         // Burst Time
    int prio;       // Priority
    int ct, tat, wt; // Completion, Turnaround, Waiting Time
    int rt;         // Remaining Time (for preemptive)
};

// Utility function to print results
void printResults(vector<Process> &p) {
    cout << "\nPID\tAT\tBT\tPR\tCT\tTAT\tWT\n";
    for (auto &pr : p) {
        cout << pr.id << "\t" << pr.at << "\t" << pr.bt << "\t" << pr.prio << "\t"
             << pr.ct << "\t" << pr.tat << "\t" << pr.wt << "\n";
    }
    double avgTAT = 0, avgWT = 0;
    for (auto &pr : p) {
        avgTAT += pr.tat;
        avgWT += pr.wt;
    }
    cout << "Average TAT = " << avgTAT / p.size() << endl;
    cout << "Average WT  = " << avgWT / p.size() << endl;
}

//  SJF Non-Preemptive
void SJF_NP(vector<Process> p) {
    cout << "\n--- SJF (Non-Preemptive) ---\n";
    int time = 0, completed = 0;
    vector<bool> done(p.size(), false);

    while (completed < p.size()) {
        int idx = -1, mn = INT_MAX;
        for (int i = 0; i < p.size(); i++) {
            if (!done[i] && p[i].at <= time) {
                if (p[i].bt < mn) {
                    mn = p[i].bt;
                    idx = i;
                }
            }
        }
        if (idx == -1) {
            time++;
        } else {
            time += p[idx].bt;
            p[idx].ct = time;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;
            done[idx] = true;
            completed++;
        }
    }
    printResults(p);
}

//  SJF Preemptive (SRTF)
void SJF_P(vector<Process> p) {
    cout << "\n--- SJF (Preemptive / SRTF) ---\n";
    int n = p.size();
    for (auto &pr : p) pr.rt = pr.bt;

    int time = 0, completed = 0;
    while (completed < n) {
        int idx = -1, mn = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (p[i].at <= time && p[i].rt > 0) {
                if (p[i].rt < mn) {
                    mn = p[i].rt;
                    idx = i;
                }
            }
        }
        if (idx == -1) {
            time++;
        } else {
            p[idx].rt--;
            time++;
            if (p[idx].rt == 0) {
                p[idx].ct = time;
                p[idx].tat = p[idx].ct - p[idx].at;
                p[idx].wt = p[idx].tat - p[idx].bt;
                completed++;
            }
        }
    }
    printResults(p);
}

//  Priority Scheduling (Non-Preemptive)
void Priority_NP(vector<Process> p) {
    cout << "\n--- Priority Scheduling (Non-Preemptive) ---\n";
    int time = 0, completed = 0;
    vector<bool> done(p.size(), false);

    while (completed < p.size()) {
        int idx = -1, highPrio = INT_MAX;
        for (int i = 0; i < p.size(); i++) {
            if (!done[i] && p[i].at <= time) {
                if (p[i].prio < highPrio) {
                    highPrio = p[i].prio;
                    idx = i;
                }
            }
        }
        if (idx == -1) {
            time++;
        } else {
            time += p[idx].bt;
            p[idx].ct = time;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;
            done[idx] = true;
            completed++;
        }
    }
    printResults(p);
}

//Priority Scheduling (Preemptive)
void Priority_P(vector<Process> p) {
    cout << "\n--- Priority Scheduling (Preemptive) ---\n";
    int n = p.size();
    for (auto &pr : p) pr.rt = pr.bt;

    int time = 0, completed = 0;
    while (completed < n) {
        int idx = -1, highPrio = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (p[i].at <= time && p[i].rt > 0) {
                if (p[i].prio < highPrio) {
                    highPrio = p[i].prio;
                    idx = i;
                }
            }
        }
        if (idx == -1) {
            time++;
        } else {
            p[idx].rt--;
            time++;
            if (p[idx].rt == 0) {
                p[idx].ct = time;
                p[idx].tat = p[idx].ct - p[idx].at;
                p[idx].wt = p[idx].tat - p[idx].bt;
                completed++;
            }
        }
    }
    printResults(p);
}

// Round Robin
void RoundRobin(vector<Process> p, int tq) {
    cout << "\n--- Round Robin (TQ=" << tq << ") ---\n";
    int n = p.size();
    for (auto &pr : p) pr.rt = pr.bt;

    queue<int> q;
    vector<bool> inQ(n, false);
    int time = 0, completed = 0;

    while (completed < n) {
        for (int i = 0; i < n; i++) {
            if (p[i].at <= time && !inQ[i] && p[i].rt > 0) {
                q.push(i);
                inQ[i] = true;
            }
        }

        if (q.empty()) {
            time++;
            continue;
        }

        int idx = q.front(); q.pop();
        int execTime = min(tq, p[idx].rt);
        time += execTime;
        p[idx].rt -= execTime;

        for (int i = 0; i < n; i++) {
            if (p[i].at <= time && !inQ[i] && p[i].rt > 0) {
                q.push(i);
                inQ[i] = true;
            }
        }

        if (p[idx].rt == 0) {
            p[idx].ct = time;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;
            completed++;
        } else {
            q.push(idx);
        }
    }
    printResults(p);
}

// ------------------ MAIN ------------------
int main() {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;
    vector<Process> p(n);
    cout << "Enter AT, BT, Priority for each process:\n";
    for (int i = 0; i < n; i++) {
        p[i].id = i + 1;
        cin >> p[i].at >> p[i].bt >> p[i].prio;
    }

    SJF_NP(p);
    SJF_P(p);
    Priority_NP(p);
    Priority_P(p);
    RoundRobin(p,1); // Time Quantum = 1

    return 0;
}
