#include <bits/stdc++.h>
#include "config.h"
#include "task.h"
#include "memory_manager.h"
#include <chrono>

using namespace std;
unordered_map<uint64_t, Task> tasks;
unordered_map<uint64_t, TaskWSLP> tasksSLP;
unordered_map<uint64_t, TaskWMLP> tasksMLP;
MemoryManager memoryManager(MAIN_MEMORY / PAGE_SIZE);

void processInfoTask(uint64_t tid, uint64_t lAddr, uint64_t size)
{
    // cout << "Processing - Type: " << tid << ", Address: " << lAddr << ", Size: " << size << endl;

    auto itr = tasks.find(tid);
    if (itr == tasks.end())
    {
        tasks.emplace(tid, Task(tid));
    }

    tasks[tid].requestMemory(lAddr, size, memoryManager);
}

void processInfoSLP(uint64_t tid, uint64_t lAddr, uint64_t size)
{
    try
    {
        auto itr = tasksSLP.find(tid);
        if (itr == tasksSLP.end())
        {
            tasksSLP.emplace(tid, TaskWSLP(tid));
        }
        tasksSLP[tid].requestMemory(lAddr, size, memoryManager);
    }
    catch (const bad_alloc &e)
    {
        cerr << "Memory allocation failed for task " << tid << ": " << e.what() << endl;
        throw runtime_error("Memory Full...");
    }
}

void processInfoMLP(uint64_t tid, uint64_t lAddr, uint64_t size)
{
    try
    {
        // cout << "Task:" << tid << ",Logical Address: " << lAddr << ",Size To Be Allocated :" << size << endl;
        auto itr = tasksMLP.find(tid);
        if (itr == tasksMLP.end())
        {
            tasksMLP.emplace(tid, TaskWMLP(tid));
        }
        tasksMLP[tid].requestMemory(lAddr, size, memoryManager);
    }
    catch (const bad_alloc &e)
    {
        cerr << "Memory allocation failed for task " << tid << ": " << e.what() << endl;
        throw runtime_error("Memory Full...");
    }
}

void readAndProcessTraceFile(const string filename, int choice)
{
    ifstream file(filename);
    string line;
    regex pattern(R"(T?(\d+):\s*(0x[0-9A-Fa-f]+):(\d+[KMG]B))");
    smatch match;

    while (getline(file, line))
    {
        if (line.empty())
        {
            break;
        }

        if (regex_search(line, match, pattern))
        {
            uint64_t type = stoul(match[1]);
            uint64_t lAddr = stoul(match[2], nullptr, 16);
            string size = match[3];

            regex pattern("(\\d+)(KB|MB)");
            uint64_t sizeToAllocate;
            smatch match2;
            if (regex_match(size, match2, pattern))
            {
                uint64_t value = stoul(match2[1]);
                string unit = match2[2];
                if (unit == "KB")
                {
                    sizeToAllocate = value * 1024;
                }
                else if (unit == "MB")
                {
                    sizeToAllocate = value * 1024 * 1024;
                }
            }
            switch (choice)
            {
            case 1:
                processInfoTask(type, lAddr, sizeToAllocate);
                break;

            case 2:
                processInfoSLP(type, lAddr, sizeToAllocate);
                break;

            case 3:
                processInfoMLP(type, lAddr, sizeToAllocate);
                break;

            case 4:
                processInfoTask(type, lAddr, sizeToAllocate);
                processInfoSLP(type, lAddr, sizeToAllocate);
                processInfoMLP(type, lAddr, sizeToAllocate);
                break;

            default:
                cerr << "Wrong Choice" << endl;
                break;
            }
        }
    }
}
void printTask()
{
    cout << "Map Implementation" << endl;
    uint64_t pageTableEntries = 0;
    uint64_t totalPageHits = 0;
    uint64_t totalPagefaults = 0;
    uint64_t totalMemAllocated = 0;
    for (auto &itr : tasks)
    {
        pageTableEntries += itr.second.pageTable.size();
        totalPageHits += itr.second.pageHit;
        totalPagefaults += itr.second.pageFault;
        totalMemAllocated += itr.second.memoryAllocated;
        cout << "Task" << itr.first << " ->Page Hits: " << itr.second.pageHit << " ,Page Faults:" << itr.second.pageFault << " ,Total memory allocated for this task: " << itr.second.memoryAllocated << endl;
    }
    cout << "Total Page Hits :" << totalPageHits << endl;
    cout << "Total Page faults :" << totalPagefaults << endl;
    cout << "Total Memory Allocated :" << totalMemAllocated << endl;
    cout << "Memory Used by PageTable:" << pageTableEntries*32 << endl;
}

void printTaskSLP()
{
    cout << "Single Level Page Table Implementation" << endl;
    uint64_t pageTableEntries = 0;
    uint64_t totalPageHits = 0;
    uint64_t totalPagefaults = 0;
    uint64_t totalMemAllocated = 0;
    for (auto &itr : tasksSLP)
    {
        pageTableEntries += itr.second.pageTable.size();
        totalPageHits += itr.second.pageHit;
        totalPagefaults += itr.second.pageFault;
        totalMemAllocated += itr.second.memoryAllocated;
        cout << "Task" << itr.first << " ->Page Hits: " << itr.second.pageHit << " ,Page Faults:" << itr.second.pageFault << " ,Total memory allocated for this task: " << itr.second.memoryAllocated << endl;
    }
    cout << "Total Page Hits :" << totalPageHits << endl;
    cout << "Total Page faults :" << totalPagefaults << endl;
    cout << "Total Memory Allocated :" << totalMemAllocated << endl;
    cout << "Memory Used by PageTable:" << pageTableEntries*32 << endl;
}
void printTaskMLP()
{
    cout << "Multi-Level Page Table Implementation" << endl;
    uint64_t pageTableEntries1 = 0;
    uint64_t pageTableEntries2 = 0;
    uint64_t l1,l2;
    uint64_t totalPageHits = 0;
    uint64_t totalPagefaults = 0;
    uint64_t totalMemAllocated = 0;
    for (auto &itr : tasksMLP)
    {
        pageTableEntries1 += (1 << itr.second.level1Bits);
        pageTableEntries2 += (1 << itr.second.level2Bits);
        l1 = itr.second.level1Bits;
        l2 = itr.second.level2Bits;
        totalPageHits += itr.second.pageHit;
        totalPagefaults += itr.second.pageFault;
        totalMemAllocated += itr.second.memoryAllocated;
        cout << "Task" << itr.first << " ->Page Hits: " << itr.second.pageHit << " ,Page Faults:" << itr.second.pageFault << " ,Total memory allocated for this task: " << itr.second.memoryAllocated << endl;
    }
    cout << "Total Page Hits :" << totalPageHits << endl;
    cout << "Total Page faults :" << totalPagefaults << endl;
    cout << "Total Memory Allocated :" << totalMemAllocated << endl;
    cout << "Memory Used by PageTable(Level1):" << pageTableEntries1*l1 << endl;
    cout << "Memory Used by PageTable(Level2):" << pageTableEntries2*l2 << endl;
}
int main()
{
    string filename;
    int implementationType;

    cout << "Enter the name of the input file: ";
    cin >> filename;

    cout << "Choose the page table implementation:" << endl;
    cout << "1. Basic (unordered_map)" << endl;
    cout << "2. Single-Level Page Table" << endl;
    cout << "3. Multi-Level Page Table" << endl;
    cout << "4. Run All" << endl;
    cout << "Enter your choice (1-4): ";
    cin >> implementationType;

    if (implementationType < 1 || implementationType > 4)
    {
        cout << "Invalid choice. Exiting." << endl;
        return 1;
    }
    auto start = chrono::high_resolution_clock::now();
    readAndProcessTraceFile(filename.c_str(), implementationType);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    //Performance metrics;
    switch (implementationType)
    {
    case 1:
        printTask();
        break;

    case 2:
        printTaskSLP();
        break;

    case 3:
        printTaskMLP();
        break;

    case 4:
        printTask();
        printTaskSLP();
        printTaskMLP();
        break;

    default:
        cerr << "Wrong Choice" << endl;
        break;
    }
    cout << "Execution Time: " << duration.count() << " seconds" << endl;
    cout << "Remaining Main Memory: " << memoryManager.getRemainingFrames()*PAGE_SIZE << endl;
    return 0;
}