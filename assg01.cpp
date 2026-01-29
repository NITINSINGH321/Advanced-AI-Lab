#include <bits/stdc++.h>
using namespace std;

/*
 Generic Assignment Scheduler
 Enumerates all valid schedules within M days
 using DFS + backtracking over a dependency DAG.
*/

static const int PRINT_LIMIT = 200;

struct Task
{
    int id;
    int cost;
    vector<int> out; // dependents
};

int N, K, M;
vector<Task> tasks;
vector<int> indeg;
vector<bool> done;

long long totalSolutions = 0;

// Check if all tasks are completed
bool allDone()
{
    for (bool x : done)
        if (!x)
            return false;
    return true;
}

// DFS search
void dfs(int day,
         vector<int> &promptsLeft,
         vector<vector<pair<int, int>>> &schedule)
{
    if (allDone())
    {
        totalSolutions++;
        if (totalSolutions <= PRINT_LIMIT)
        {
            cout << "\n===== VALID SCHEDULE " << totalSolutions << " =====\n";
            for (int d = 0; d < (int)schedule.size(); d++)
            {
                cout << "Day " << d + 1 << ":\n";
                for (auto &p : schedule[d])
                {
                    cout << "  Student " << p.first + 1
                         << " -> Assignment " << p.second << "\n";
                }
            }
        }
        return;
    }

    if (day > M)
        return;

    bool workedToday = false;
    int T = tasks.size();

    // Try assigning any ready task to any student
    for (int s = 0; s < N; s++)
    {

        // symmetry breaking
        if (s > 0 && promptsLeft[s - 1] == K)
            break;

        for (int t = 0; t < T; t++)
        {

            if (done[t])
                continue;
            if (indeg[t] != 0)
                continue;
            if (promptsLeft[s] < tasks[t].cost)
                continue;

            workedToday = true;

            // apply
            done[t] = true;
            promptsLeft[s] -= tasks[t].cost;
            for (int v : tasks[t].out)
                indeg[v]--;

            schedule.back().push_back({s, tasks[t].id});

            dfs(day, promptsLeft, schedule);

            // undo
            schedule.back().pop_back();
            for (int v : tasks[t].out)
                indeg[v]++;
            promptsLeft[s] += tasks[t].cost;
            done[t] = false;
        }
    }

    // Advance day if at least one task done
    if (workedToday)
    {
        vector<int> old = promptsLeft;
        fill(promptsLeft.begin(), promptsLeft.end(), K);

        schedule.push_back({});
        dfs(day + 1, promptsLeft, schedule);
        schedule.pop_back();

        promptsLeft = old;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cerr << "Usage: ./schedule <input-file> <number-of-days>\n";
        return 1;
    }

    ifstream fin(argv[1]);
    M = stoi(argv[2]);

    if (!fin)
    {
        cerr << "Cannot open input file\n";
        return 1;
    }

    unordered_map<int, int> idIndex;
    vector<vector<int>> deps;

    string line;

    while (getline(fin, line))
    {
        if (line.empty() || line[0] == '%')
            continue;

        stringstream ss(line);
        char c;
        ss >> c;

        if (c == 'N')
            ss >> N;
        else if (c == 'K')
            ss >> K;
        else if (c == 'A')
        {
            int id, cost;
            ss >> id >> cost;
            idIndex[id] = tasks.size();
            tasks.push_back({id, cost, {}});
            deps.push_back({});
            int d;
            while (ss >> d && d != 0)
                deps.back().push_back(d);
        }
    }

    int T = tasks.size();
    indeg.assign(T, 0);
    done.assign(T, false);

    // Build DAG
    for (int i = 0; i < T; i++)
    {
        for (int d : deps[i])
        {
            int u = idIndex[d];
            tasks[u].out.push_back(i);
            indeg[i]++;
        }
    }

    vector<int> promptsLeft(N, K);
    vector<vector<pair<int, int>>> schedule;
    schedule.push_back({});

    cout << "Solving...\n";
    dfs(1, promptsLeft, schedule);

    cout << "\nTotal valid schedules: " << totalSolutions << "\n";
    if (totalSolutions > PRINT_LIMIT)
        cout << "(Only first " << PRINT_LIMIT << " printed)\n";

    return 0;
}
