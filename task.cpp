#include "config.h"
#include "task.h"
#include "memory_manager.h"
#include <iostream>
#include <algorithm>

using namespace std;

// Map Implementation(Logical page number is the key and physical page number is value)
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
            uint64_t* physicalPage = memoryManager.allocateFrame();
            pageTable[startPage + i] = physicalPage;
            memoryAllocated += PAGE_SIZE;
        }
    }

    // cout << "Memory Allocated...." << endl;
}

// Single-Level Page Table Implementation
void TaskWSLP::requestMemory(uint64_t lAddr, uint64_t size, MemoryManager &memoryManager)
{
    uint64_t startPage = lAddr >> OFFSET_BITS;
    uint64_t endPage = startPage + (uint64_t)ceil((double)size / PAGE_SIZE);

    cout << "Requesting memory: Logical Address = " << lAddr
         << ", Size = " << size
         << ", Start Page = " << startPage
         << ", End Page = " << endPage << endl;

    cout << pageTable.size() << endl;

    for (uint64_t pageNumber = startPage; pageNumber < endPage; pageNumber++)
    {
        if (pageNumber >= pageTableSize)
        {
            throw out_of_range("Page number exceeds virtual address space");
        }
        // auto itr = find(pageTable.begin(), pageTable.end(), pageNumber);
        // if (itr != pageTable.end())
        // {
        //     pageHit++;
        //     // cout << "Page Table allocated for logical addr :" << pageNumber << endl;
        // }
        if (pageTable[pageNumber] != nullptr)
        {
            pageHit++;
            cout << "Page Table Hit for entry at :" << pageNumber << " with addr :" << pageTable[pageNumber] << endl;
        }
        else
        {
            pageFault++;
            uint64_t *physicalPageAddr = memoryManager.allocateFrame();
            cout << "Allocated Physical Page Addr = " << physicalPageAddr << " for Logical Page = " << pageNumber << endl;
            pageTable[pageNumber] = physicalPageAddr;
            memoryAllocated += PAGE_SIZE;
        }
    }

    cout << endl;
}

// // Two-Level Page Table Implementation
void TaskWMLP::requestMemory(uint64_t lAddr, uint64_t size, MemoryManager &memoryManager)
{
    uint64_t startPage = lAddr >> OFFSET_BITS;                               // Starting page
    uint64_t endPage = startPage + (uint64_t)ceil((double)size / PAGE_SIZE); // lastPage

    cout << "Requesting memory: Logical Address = " << lAddr
         << ", Size = " << size
         << ", Start Page = " << startPage
         << ", End Page = " << endPage << endl;

    for (uint64_t pageNumber = startPage; pageNumber < endPage; pageNumber++)
    {
        uint64_t level1Index = pageNumber >> LEVEL_2_BITS;
        uint64_t level2Index = pageNumber & ((1ULL << LEVEL_2_BITS) - 1);

        cout << "Individual Page Number: " << pageNumber << endl;
        cout << "Level 1 Index:" << level1Index << " ,Level 2 Index:" << level2Index << endl;
        cout << "Level 1 Bits:" << LEVEL_1_BITS << " ,Level 2 Bits:" << LEVEL_2_BITS << endl;

        // Check if level1 entry exists, if not create it
        if (pageTable[level1Index] == nullptr)
        {
            // pageFault++;
            pageTable[level1Index] = new vector<uint64_t *>(LEVEL_2_SIZE, nullptr);
        }

        if ((*pageTable[level1Index])[level2Index] == nullptr)
        {
            pageFault++;
            uint64_t *frame = memoryManager.allocateFrame();
            if (frame != nullptr)
            {
                memoryAllocated += PAGE_SIZE;
                (*pageTable[level1Index])[level2Index] = frame;
                cout << "Page Fault!.. Address " << (*pageTable[level1Index])[level2Index] << " stored at index (" << level1Index << ", " << level2Index << ")" << endl;
            }
        }
        else
        {
            pageHit++;
            cout << "Page hit! Address " << (*pageTable[level1Index])[level2Index] << " stored at index (" << level1Index << ", " << level2Index << ")" << endl;
        }
    }

    // cout << "Multi-Level Page Table Memory Allocated...." << endl;
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
