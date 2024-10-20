#include <iostream>
#include <algorithm>
#include <iomanip>
#include <cstring>
#include <stack>
#include <vector>
#include <queue>

using namespace std;

const int maxn = 1001;
int n;
pair<char, pair<int, int>> p[maxn];

struct Proccess {
    char name;
    int arrivalTime, burstTime, priority;
};

Proccess pp[maxn];

bool cmp(pair<char, pair<int, int>> a, pair<char, pair<int, int>> b) {
    if (a.second.first == b.second.first) {
        return a.second.second < b.second.second;
    }
    return a.second.first < b.second.first;
}

bool cmpp(Proccess a, Proccess b) {
    if (a.arrivalTime == b.arrivalTime) {
        return a.priority < b.priority;
    }
    return a.arrivalTime < b.arrivalTime;
}

void InputAB() {
    cin >> n;
    for (int i = 1; i <= n; i++) {
        char c;
        int x, y;
        cin >> c >> x >> y;
        p[i].first = c;
        p[i].second.first = x;
        p[i].second.second = y;
    }
}

void InputABP() {
    cin >> n;
    for (int i = 1; i <= n; i++) {
        char name;
        int arrivalTime, burstTime, priority;
        cin >> name >> arrivalTime >> burstTime >> priority;
        pp[i].name = name;
        pp[i].arrivalTime = arrivalTime;
        pp[i].burstTime = burstTime;
        pp[i].priority = priority;
    }
}

void SolveWithFCFS() {
    sort(p, p + n + 1, cmp);
    int mm[maxn];
    for (int i = 1; i <= n; i++) {
        mm[i] = p[i].second.first + p[i].second.second;
    }
    int tt[maxn];
    tt[1] = p[1].second.first + p[1].second.second;
    for (int i = 2; i <= n; i++) {
        if (p[i].second.first == p[i - 1].second.first) {
            tt[i] = tt[i - 1] + p[i].second.second;
        } else {
            if (p[i].second.first <= tt[i - 1]) {
                tt[i] = tt[i - 1] + p[i].second.second;
            } else {
                tt[i] = p[i].second.first + p[i].second.second;
            }
        }
    }
    int wt[maxn];
    double awt = 0;
    for (int i = 1; i <= n; i++) {
        wt[i] = tt[i] - mm[i];
        awt += wt[i];
    }
    awt /= n;
    cout << "#FCFS:" <<endl;
    cout << "Gantt (FCFS): ";
    cout << "|" << p[1].second.first << "|---" << p[1].first << "---|" << tt[1];
    for (int i = 2; i <= n; i++) {
        cout << "|---" << p[i].first << "---|" << tt[i];
    }
    cout << "|" << endl;
    cout << "P | Mong muon | Thuc Te | Waiting Time" << endl;
    for (int i = 1; i <= n; i++) {
        cout << p[i].first << " | " << mm[i] << " | " << tt[i] << " | " << wt[i] << endl;
    }
    cout << "AWT (FCFS): " << fixed << setprecision(3) << awt << endl;
}

void SolveWithSJF() {
    sort(p, p + n + 1, cmp);
    int mm[maxn];
    for (int i = 1; i <= n; i++) {
        mm[i] = p[i].second.first + p[i].second.second;
    }
    int tt[maxn];
    memset(tt, -1, sizeof(tt));
    stack<pair<char, int>> current;
    tt[1] = p[1].second.first + p[1].second.second;
    current.push({p[1].first, tt[1]});
    while (current.size() < n) {
        int minBurstTime = INT_MAX;
        int nextProcess = -1;
        for (int i = 2; i <= n; i++) {
            if (tt[i] == -1 && p[i].second.first <= current.top().second && p[i].second.second < minBurstTime) {
                minBurstTime = p[i].second.second;
                nextProcess = i;
            }
        }
        if (nextProcess != -1) {
            tt[nextProcess] = current.top().second + p[nextProcess].second.second;
            current.push({p[nextProcess].first, tt[nextProcess]});
        } else {
            for (int i = 2; i <= n; i++) {
                if (tt[i] == -1) {
                    tt[i] = p[i].second.first + p[i].second.second;
                    current.push({p[i].first, tt[i]});
                    break;
                }
            }
        }
    }
    int wt[maxn];
    double awt = 0;
    double att = 0;
    for (int i = 1; i <= n; i++) {
        wt[i] = tt[i] - mm[i];
        awt += wt[i];
        att += wt[i] + p[i].second.second;
    }
    awt /= n;
    att /= n;
    vector<pair<char, int>> v;
    while (!current.empty()) {
        pair<char, int> top = current.top();
        current.pop();
        v.push_back(top);
    }
    reverse(v.begin(), v.end());
    cout << "#SJF:" <<endl;
    cout << "Gantt (SJF): ";
    cout << "|" << p[1].second.first << "|---" << p[1].first << "---|" << tt[1];
    for (int i = 1; i < n; i++) {
        cout << "|---" << v[i].first << "---|" << v[i].second;
    }
    cout << "|" << endl;
    cout << "P | Mong muon | Thuc Te | Waiting Time" << endl;
    for (int i = 1; i <= n; i++) {
        cout << p[i].first << " | " << mm[i] << " | " << tt[i] << " | " << wt[i] << endl;
    }
    cout << "AWT (SJF): " << fixed << setprecision(3) << awt << endl;
    cout << "ATT (SJF): " << fixed << setprecision(3) << att << endl;
}

void SolveWithSRTF() {
    sort(p, p + n + 1, cmp);
    int mm[maxn];
    int remainingTime[maxn];
    for (int i = 1; i <= n; i++) {
        mm[i] = p[i].second.first + p[i].second.second;
        remainingTime[i] = p[i].second.second;
    }
    queue<pair<char, int>> gantt;
    int tt[maxn];
    int currentTime = 0, completed = 0, oldProcess = 1;
    while (completed != n) {
        int nextProcess = -1;
        int minRemainingTime = INT_MAX;
        for (int i = 1; i <= n; i++) {
            if (remainingTime[i] > 0 && p[i].second.first <= currentTime) {
                if (remainingTime[i] < minRemainingTime) {
                    minRemainingTime = remainingTime[i];
                    nextProcess = i;
                }
            }
        }
        if (nextProcess == -1) {
            ++currentTime;
            continue;
        }
        if (nextProcess != oldProcess) {
            char c = p[oldProcess].first;
            gantt.push({c, currentTime});
            oldProcess = nextProcess;
        }
        --remainingTime[nextProcess];
        ++currentTime;
        if (remainingTime[nextProcess] == 0) {
            ++completed;
            tt[nextProcess] = currentTime;
        }
    }
    gantt.push({p[oldProcess].first, currentTime});
    int wt[maxn];
    double awt = 0;
    for (int i = 1; i <= n; i++) {
        wt[i] = tt[i] - mm[i];
        awt += wt[i];
    }
    awt /= n;
    cout << "#SRTF:" <<endl;
    cout << "Gantt (SRTF): ";
    cout << "|" << p[1].second.first << "|";
    while (!gantt.empty()) {
        pair<char, int> tmp = gantt.front();
        gantt.pop();
        cout << "--" << tmp.first << "--|" << tmp.second << "|";
    }
    cout << endl;
    cout << "P | Mong muon | Thuc Te | Waiting Time" << endl;
    for (int i = 1; i <= n; i++) {
        cout << p[i].first << " | " << mm[i] << " | " << tt[i] << " | " << wt[i] << endl;
    }
    cout << "AWT (SRTF): " << fixed << setprecision(3) << awt << endl;
}

void SolveWithRR() {
    int timeQuantum;
    cout << "Enter time quantum to solve with Round Robin: ";
    cin >> timeQuantum;
    sort(p, p + n + 1, cmp);
    int mm[maxn];
    bool ever[maxn];
    int remainingTime[maxn];
    for (int i = 1; i <= n; i++) {
        mm[i] = p[i].second.first + p[i].second.second;
        remainingTime[i] = p[i].second.second;
        ever[i] = false;
    }
    queue<pair<char, int>> gantt;
    queue<int> ready;
    int tt[maxn];
    int currentTime = 0, completed = 0;
    bool isAllProccessInQueue = false;
    int proccessInQuantum = 0;
    remainingTime[0] = -1;
    while (completed != n) {
        if (!isAllProccessInQueue) {
            for (int i = 1; i <= n; i++) {
                if (!ever[i] && remainingTime[i] > 0 && p[i].second.first <= currentTime) {
                    ready.push(i);
                    ever[i] = true;
                    if (i == n) {
                        isAllProccessInQueue = true;
                    }
                }
            }
        }
        if (remainingTime[proccessInQuantum] == 0) {
            ready.pop();
        } else if (remainingTime[proccessInQuantum] != -1) {
            ready.pop();
            ready.push(proccessInQuantum);
        }
        proccessInQuantum = ready.front();
        if (remainingTime[proccessInQuantum] <= timeQuantum) {
            currentTime += remainingTime[proccessInQuantum];
            remainingTime[proccessInQuantum] = 0;
            tt[proccessInQuantum] = currentTime;
            completed++;
        } else {
            remainingTime[proccessInQuantum] -= timeQuantum;
            currentTime += timeQuantum;
        }
        gantt.push({p[proccessInQuantum].first, currentTime});
    }
    int wt[maxn];
    double awt = 0;
    for (int i = 1; i <= n; i++) {
        wt[i] = tt[i] - mm[i];
        awt += wt[i];
    }
    awt /= n;
    cout << "#RR:" <<endl;
    cout << "Gantt (RR): ";
    cout << "|" << p[1].second.first << "|";
    while (!gantt.empty()) {
        pair<char, int> tmp = gantt.front();
        gantt.pop();
        cout << "--" << tmp.first << "--|" << tmp.second << "|";
    }
    cout << endl;
    cout << "P | Mong muon | Thuc Te | Waiting Time" << endl;
    for (int i = 1; i <= n; i++) {
        cout << p[i].first << " | " << mm[i] << " | " << tt[i] << " | " << wt[i] << endl;
    }
    cout << "AWT (RR): " << fixed << setprecision(3) << awt << endl;
}

void SolveWithNPP() {
    sort(pp, pp + n + 1, cmpp);
    int mm[maxn];
    for (int i = 1; i <= n; i++) {
        mm[i] = pp[i].arrivalTime + pp[i].burstTime;
    }
    int tt[maxn];
    memset(tt, -1, sizeof(tt));
    stack<pair<char, int>> current;
    tt[1] = pp[1].arrivalTime + pp[1].burstTime;
    current.push({pp[1].name, tt[1]});
    while (current.size() < n) {
        int minPriority = INT_MAX;
        int nextProcess = -1;
        for (int i = 2; i <= n; i++) {
            if (tt[i] == -1 && pp[i].arrivalTime <= current.top().second && pp[i].priority < minPriority) {
                minPriority = pp[i].priority;
                nextProcess = i;
            }
        }
        if (nextProcess != -1) {
            tt[nextProcess] = current.top().second + pp[nextProcess].burstTime;
            current.push({pp[nextProcess].name, tt[nextProcess]});
        } else {
            for (int i = 2; i <= n; i++) {
                if (tt[i] == -1) {
                    tt[i] = pp[i].arrivalTime + pp[i].burstTime;
                    current.push({pp[i].name, tt[i]});
                    break;
                }
            }
        }
    }
    int wt[maxn];
    double awt = 0;
    double att = 0;
    for (int i = 1; i <= n; i++) {
        wt[i] = tt[i] - mm[i];
        awt += wt[i];
        att += wt[i] + pp[i].burstTime;
    }
    awt /= n;
    att /= n;
    vector<pair<char, int>> v;
    while (!current.empty()) {
        pair<char, int> top = current.top();
        current.pop();
        v.push_back(top);
    }
    reverse(v.begin(), v.end());
    cout << "#NPP:" <<endl;
    cout << "Gantt (NPP): ";
    cout << "|" << pp[1].arrivalTime << "|---" << pp[1].name << "---|" << tt[1];
    for (int i = 1; i < n; i++) {
        cout << "|---" << v[i].first << "---|" << v[i].second;
    }
    cout << "|" << endl;
    cout << "P | Mong muon | Thuc Te | Waiting Time" << endl;
    for (int i = 1; i <= n; i++) {
        cout << pp[i].name << " | " << mm[i] << " | " << tt[i] << " | " << wt[i] << endl;
    }
    cout << "AWT (NPP): " << fixed << setprecision(3) << awt << endl;
    cout << "ATT (NPP): " << fixed << setprecision(3) << att << endl;
}

void SolveWithPP() {
    sort(pp, pp + n + 1, cmpp);
    int mm[maxn];
    int remainingTime[maxn];
    for (int i = 1; i <= n; i++) {
        mm[i] = pp[i].arrivalTime + pp[i].burstTime;
        remainingTime[i] = pp[i].burstTime;
    }
    queue<pair<char, int>> gantt;
    int tt[maxn];
    int currentTime = 0, completed = 0, oldProcess = 1;
    while (completed != n) {
        int nextProcess = -1;
        int minPriority = INT_MAX;
        for (int i = 1; i <= n; i++) {
            if (remainingTime[i] > 0 && pp[i].arrivalTime <= currentTime) {
                if (pp[i].priority < minPriority) {
                    minPriority = pp[i].priority;
                    nextProcess = i;
                }
            }
        }
        if (nextProcess == -1) {
            ++currentTime;
            continue;
        }
        if (nextProcess != oldProcess) {
            char c = pp[oldProcess].name;
            gantt.push({c, currentTime});
            oldProcess = nextProcess;
        }
        --remainingTime[nextProcess];
        ++currentTime;
        if (remainingTime[nextProcess] == 0) {
            ++completed;
            tt[nextProcess] = currentTime;
        }
    }
    gantt.push({pp[oldProcess].name, currentTime});
    int wt[maxn];
    double awt = 0;
    for (int i = 1; i <= n; i++) {
        wt[i] = tt[i] - mm[i];
        awt += wt[i];
    }
    awt /= n;
    cout << "#PP:" <<endl;
    cout << "Gantt (PP): ";
    cout << "|" << pp[1].arrivalTime << "|";
    while (!gantt.empty()) {
        pair<char, int> tmp = gantt.front();
        gantt.pop();
        cout << "--" << tmp.first << "--|" << tmp.second << "|";
    }
    cout << endl;
    cout << "P | Mong muon | Thuc Te | Waiting Time" << endl;
    for (int i = 1; i <= n; i++) {
        cout << pp[i].name << " | " << mm[i] << " | " << tt[i] << " | " << wt[i] << endl;
    }
    cout << "AWT (PP): " << fixed << setprecision(3) << awt << endl;
}

int main() {
    cout << "Enter 1 to solve without priority (FCFS, SJF, SRTF, RR)" << endl;
    cout << "Enter 2 to solve with priority (NPP, PP)" << endl;
    int choose = 0;
    cin >> choose;
    while (choose != 1 && choose != 2) {
        cin >> choose;
    }
    if (choose == 1) {
        InputAB();
        SolveWithFCFS();
        SolveWithSJF();
        SolveWithSRTF();
        SolveWithRR();
    } else {
        InputABP();
        SolveWithNPP();
        SolveWithPP();
    }
    return 0;
}

// #TEST CASE CHO ENTER = 1 (FCFS, SJF, SRTF, RR)

//Test case 1 đây nha mọi người!
//1
//7
//A 0 3
//B 2 2
//C 4 4
//D 6 6
//E 3 3
//F 5 6
//G 1 2

//Test case 2 đây nha mọi người!
//1
//6
//A 0 6
//B 2 2
//C 4 4
//D 1 6
//E 3 3
//F 5 6

//Test case 3 đây nha mọi người!
//1
//6
//A 1 6
//B 0 8
//C 12 7
//D 3 2
//E 2 5
//F 10 3

//Test case 4 đây nha mọi người!
//1
//6
//A 0 12
//B 1 10
//C 6 2
//D 3 3
//E 2 7
//F 9 1

//Test case 5 đây nha mọi người!
//1
//5
//A 0 8
//B 2 19
//C 4 3
//D 5 6
//E 7 10

//Test case 6 đây nha mọi người!
//1
//5
//A 1 6
//B 0 8
//C 12 7
//D 3 2
//E 2 5


// #TEST CASE CHO ENTER = 2 (NPP, PP)

//Test case 1 đây nha mọi người!
//2
//6
//A 0 3 6
//B 2 2 5
//C 4 4 3
//D 1 6 7
//E 3 3 2
//F 5 6 1

//Test case 2 đây nha mọi người!
//2
//6
//A 1 3 2
//B 5 4 5
//C 4 3 3
//D 0 5 4
//E 3 4 6
//F 2 6 1