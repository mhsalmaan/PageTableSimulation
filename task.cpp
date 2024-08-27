#include "config.h"
#include "task.h"
#include "memory_manager.h"
#include <iostream>
#include <algorithm>

using namespace std;

//Map Implementation(Logical page number is the key and physical page number is value)
void Task::requestMemory(uint64_t lAddr, uint64_t size, MemoryManager &memoryManager)
{
    uint64_t pagesNeeded = ceil(size / PAGE_SIZE); //Total pages needed for asked memory allocation
    uint32_t startPage = lAddr / PAGE_SIZE; //addr of starting page

    for (uint64_t i = 0; i < pagesNeeded; i++)
    {
        if (pageTable.find(startPage + i) != pageTable.end())
        {
            pageHit++;  
        }
        else
        {
            pageFault++;
            uint64_t physicalPage = memoryManager.allocateFrame();
            pageTable[startPage + i] = physicalPage;
            memoryAllocated += PAGE_SIZE;
        }
    }

    // cout << "Memory Allocated...." << endl;
}

// Single-Level Page Table Implementation
void TaskWSLP::requestMemory(uint64_t lAddr, uint64_t size, MemoryManager &memoryManager)
{
    uint64_t startPage = lAddr >> offsetBits;
    uint64_t endPage = startPage + (uint64_t)ceil((double)size / PAGE_SIZE);

    // cout << "Requesting memory: Logical Address = " << lAddr
    //      << ", Size = " << size
    //      << ", Start Page = " << startPage
    //      << ", End Page = " << endPage << endl;

    for (uint64_t pageNumber = startPage; pageNumber < endPage; pageNumber++)
    {
        if (pageNumber >= pageTableSize)
        {
            throw out_of_range("Page number exceeds virtual address space");
        }
        auto itr = find(pageTable.begin(), pageTable.end(), pageNumber);
        if (itr != pageTable.end())
        {
            pageHit++;
            // cout << "Page Table allocated for logical addr :" << pageNumber << endl;
        }
        else
        {
            pageFault++;
            uint64_t physicalPage = memoryManager.allocateFrame();
            // cout << "Allocated Physical Page = " << physicalPage
            //      << " for Logical Page = " << pageNumber << endl;
            pageTable.push_back(pageNumber);
            memoryAllocated += PAGE_SIZE;
        }
    }

    cout << endl;
}

// // Two-Level Page Table Implementation
void TaskWMLP::requestMemory(uint64_t lAddr, uint64_t size, MemoryManager &memoryManager)
{
    uint64_t startPage = lAddr >> offsetBits; //Starting page
    uint64_t endPage = startPage + (uint64_t)ceil((double)size / PAGE_SIZE); //lastPage

    // cout << "Requesting memory: Logical Address = " << lAddr
    //      << ", Size = " << size
    //      << ", Start Page = " << startPage
    //      << ", End Page = " << endPage << endl;

    for (uint64_t pageNumber = startPage; pageNumber < endPage; pageNumber++)
    {
        uint64_t level1Index = pageNumber >> level2Bits;
        uint64_t level2Index = pageNumber & ((1ULL << level2Bits) - 1);

        // cout << "Individual Page Number: " << pageNumber << endl;
        // cout << "Level 1 Index:" << level1Index << " ,Level 2 Index:" << level2Index << endl;

        // Check if level1 entry exists, if not create it
        if (pageTable.find(level1Index) == pageTable.end())
        {
            pageTable[level1Index] = vector<uint64_t>();
        }

        // Check if page is already allocated
        auto &level2Table = pageTable[level1Index];
        auto itr = find(level2Table.begin(), level2Table.end(), level2Index);

        if (itr != level2Table.end())
        {
            pageHit++;
            // cout << "Page Hit..." << endl;
        }
        else
        {
            pageFault++;
            uint64_t physicalPage = memoryManager.allocateFrame();
            // cout << "Page fault..." << endl;
            // cout << "Allocated Physical Page = " << physicalPage
            //      << " for Logical Page = " << pageNumber << endl;
            level2Table.push_back(level2Index);
            memoryAllocated += PAGE_SIZE;
        }
    }

    // cout << "Multi-Level Page Table Memory Allocated...." << endl;
    // printPageTableStatus();
}

//For debugging print page table status
void TaskWMLP::printPageTableStatus() const
{
    cout << "Multi-Level Page Table Status:" << endl;
    for (const auto &level1Entry : pageTable)
    {
        for (const auto &level2Index : level1Entry.second)
        {
            uint64_t pageNumber = (level1Entry.first << level2Bits) | level2Index;
            cout << "Page Table allocated for logical addr: " << pageNumber << endl;
        }
    }

    cout << "Task" << tid << " ->Page Hits: " << pageHit
         << " ,Page Faults:" << pageFault
         << " ,Total memory allocated for this task: " << memoryAllocated << endl;
}


// void TaskWMLP::requestMemory(uint64_t lAddr, uint64_t size, MemoryManager &memoryManager)
// {
//     uint64_t startPage = lAddr >> offsetBits;
//     uint64_t endPage = (lAddr + size - 1) >> offsetBits;

//     cout << "Requesting memory: Logical Address = " << lAddr
//               << ", Size = " << size
//               << ", Start Page = " << startPage
//               << ", End Page = " << endPage << endl;

//     for (uint64_t pageNumber = startPage; pageNumber <= endPage; pageNumber++)
//     {
//         uint64_t level1Index = pageNumber >> level2Bits;
//         uint64_t level2Index = pageNumber & ((1 << level2Bits) - 1);

//         cout << "Individual Page Number: " << pageNumber << endl;
//         cout << "Level 1 Index:" << level1Index << " ,Level 2 Index:" << level2Index << endl;

//         // Ensure level1Table has enough entries
//         if (level1Index >= level1Table.size())
//         {
//             level1Table.resize(level1Index + 1, vector<uint64_t>(1 << level2Bits, -1));
//         }

//         // Reference to the level 2 table
//         vector<uint64_t> &level2Table = level1Table[level1Index];

//         if (level2Table[level2Index] != -1)
//         {
//             pageHit++;
//             cout << "Page Hit..." << endl;
//         }
//         else
//         {
//             pageFault++;
//             uint64_t physicalPage = memoryManager.allocateFrame();
//             level2Table[level2Index] = physicalPage;
//             memoryAllocated += PAGE_SIZE;
//             cout << "Page fault..." << endl;
//         }
//     }

//     cout << "Multi-Level Page Table Memory Allocated...." << endl;
// }
