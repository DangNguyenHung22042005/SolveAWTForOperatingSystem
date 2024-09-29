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

bool cmp(pair<char, pair<int, int>> a, pair<char, pair<int, int>> b) {
    if (a.second.first == b.second.first) {
        return a.second.second < b.second.second;
    }
    return a.second.first < b.second.first;
}

void Input() {
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

void Display() {
    for (int i = 1; i <= n; i++) {
        cout << p[i].first << " " << p[i].second.first << " " << p[i].second.second << endl;
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
            tt[i] = p[i].second.first + p[i].second.second;
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

int main() {
    Input();
    SolveWithFCFS();
    SolveWithSJF();
    SolveWithSRTF();
    return 0;
}

//Test case 1 đây nha mọi người!
//7
//A 0 3
//B 2 2
//C 4 4
//D 6 6
//E 3 3
//F 5 6
//G 1 2

//Test case 2 đây nha mọi người!
//6
//A 0 6
//B 2 2
//C 4 4
//D 1 6
//E 3 3
//F 5 6

//Test case 3 đây nha mọi người!
//6
//A 1 6
//B 0 8
//C 12 7
//D 3 2
//E 2 5
//F 10 3

//Test case 4 đây nha mọi người!
//6
//A 0 12
//B 1 10
//C 6 2
//D 3 3
//E 2 7
//F 9 1