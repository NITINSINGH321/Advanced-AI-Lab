Assignment Scheduler

This program enumerates all valid schedules for completing a set of assignments
subject to dependency constraints, student limits, and daily prompt capacity.

Each assignment requires a fixed number of prompts and may depend on other
assignments. An assignment can be started only after all its dependencies are
completed. Each student has at most K prompts per day.

The program prints all valid schedules (up to a print limit) and reports the
total number of valid schedules that complete within the given number of days.


Compilation:

g++ assg01.cpp -o assg01
OR
g++ -std=c++17 assg01.cpp -O2 -o assg01



Execution:
./assg01 <input-file> <number-of-days>

Example:
./assg01 input01.txt 3


Input Format:

N <number of students>
K <prompts per student per day>

A <id> <prompt-count> <dependency1> <dependency2> ... 0

Lines starting with % are treated as comments.


Output:

The program prints valid schedules (up to a fixed limit) and finally prints
the total number of valid schedules. If no schedule is possible within the
given number of days, the total will be zero.


Approach:

Assignments are modeled as nodes in a directed acyclic graph (DAG).
Depth-first search with backtracking is used to explore all feasible schedules.
At each step, any ready assignment (indegree zero) may be assigned to any student
with sufficient remaining prompts. Days advance once at least one assignment is
completed. Symmetric student schedules are avoided using canonical ordering.


Files Submitted:

1. assg01.cpp
2. input01.txt
3. input02.txt
4. input03.txt
5. README.txt
