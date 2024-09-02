#include <bits/stdc++.h>
#include "config.h"
#include "task.h"
#include "memory_manager.h"
#include <chrono>

using namespace std;
unordered_map<uint64_t, Task> tasks;                  // unordered map for map implementation
unordered_map<uint64_t, TaskWSLP> tasksSLP;           // unordered map for single level page implementation
unordered_map<uint64_t, TaskWMLP> tasksMLP;           // unordered map for multilevel page implementation
MemoryManager memoryManager(MAIN_MEMORY / PAGE_SIZE); // initialize a MemoryManager object

// function to allocate memory for tasks
void processInfoTask(uint64_t tid, uint64_t lAddr, uint64_t size)
{
    auto itr = tasks.find(tid);
    if (itr == tasks.end())
    {
        tasks.emplace(tid, Task(tid));
    }
    // try-catch blocks to check if memory allocation fails
    try
    {
        tasks[tid].requestMemory(lAddr, size, memoryManager);
    }
    catch (const bad_alloc &e)
    {
        cerr << "Memory allocation failed for task " << tid << ": " << e.what() << endl;
        throw runtime_error("Memory full...");
    }
}

// function to handle single level page table
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
        throw runtime_error("Memory Fusize;ll...");
    }
}

// function to handle multilevel page table
void processInfoMLP(uint64_t tid, uint64_t lAddr, uint64_t size)
{
    try
    {
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

// function to read and process the trace files - Input Module
void readAndProcessTraceFile(const string filename, int choice)
{
    ifstream file(filename);                                     // open an input file stream to read
    string line;                                                 // to hold each line read from the file
    regex pattern(R"(T?(\d+):\s*(0x[0-9A-Fa-f]+):(\d+[KMG]B))"); // regex pattern to match text format (T1:0x02052000:7KB)
    smatch match;                                                // to store match results from regex operations

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

            // Switch cases for user to ask the type of implementation
            switch (choice)
            {
            case 1: // for map implementation
                processInfoTask(type, lAddr, sizeToAllocate);
                break;

            case 2: // for single level page implementation
                processInfoSLP(type, lAddr, sizeToAllocate);
                break;

            case 3: // for multilevel page implementation
                processInfoMLP(type, lAddr, sizeToAllocate);
                break;

            case 4: // for running all three implemention at same time
                processInfoTask(type, lAddr, sizeToAllocate);
                processInfoSLP(type, lAddr, sizeToAllocate);
                processInfoMLP(type, lAddr, sizeToAllocate);
                break;

            default: // throw error if user other values other than 1-4
                cerr << "Wrong Choice" << endl;
                break;
            }
        }
    }
}

// function to print the tasks for map implementation
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
    // print the required information (total page hits, faults, memory allocated and used)
    cout << "Total Page Hits :" << totalPageHits << endl;
    cout << "Total Page faults :" << totalPagefaults << endl;
    cout << "Total Memory Allocated :" << totalMemAllocated << " Bytes" << endl;
    cout << "Memory Used by PageTable:" << pageTableEntries * 32 << " Bytes" << endl;
}

// function to print the tasks for single level page implementation
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
    cout << "Total Memory Allocated :" << totalMemAllocated << " Bytes" << endl;
    cout << "Memory Used by PageTable:" << pageTableEntries * 32 << " Bytes" << endl;
}

// function to print the tasks for multilevel page implementation
void printTaskMLP()
{
    cout << "Multi-Level Page Table Implementation" << endl;
    uint64_t totalPageHits = 0;
    uint64_t totalPageFaults = 0;
    uint64_t totalMemAllocated = 0;
    for (auto &task : tasksMLP)
    {
        totalPageHits += task.second.pageHit;
        totalPageFaults += task.second.pageFault;
        totalMemAllocated += task.second.memoryAllocated;
        cout << "Task" << task.first << " ->Page Hits: " << task.second.pageHit << " ,Page Faults:" << task.second.pageFault << " ,Total memory allocated for this task: " << task.second.memoryAllocated << endl;
    }
    cout << "Total Page Hits :" << totalPageHits << endl;
    cout << "Total Page Faults :" << totalPageFaults << endl;
    cout << "Total Memory Allocated :" << totalMemAllocated << " Bytes" << endl;
}

// main function 
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

    // Performance metrics;
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
    cout << "Remaining Main Memory: " << memoryManager.getRemainingFrames() * PAGE_SIZE  << " Bytes"<< endl;
    return 0;
}