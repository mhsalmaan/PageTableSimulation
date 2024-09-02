#include "config.h"
#include "task.h"
#include "memory_manager.h"
#include <iostream>
#include <algorithm>

using namespace std;

// Map Implementation(Logical page number is the key and physical page number is value)
void Task::requestMemory(uint64_t lAddr, uint64_t size, MemoryManager &memoryManager)
{
    uint64_t pagesNeeded = ceil(size / PAGE_SIZE); // Total pages needed for asked memory allocation
    uint32_t startPage = lAddr / PAGE_SIZE;        // address of starting page

    // check for each page required  for requested memory
    for (uint64_t i = 0; i < pagesNeeded; i++)
    {
        // check and increment page hit counter if current page is already in the page table
        if (pageTable.find(startPage + i) != pageTable.end())
        {
            pageHit++;
        }
        // else increment the page fault counter if page is not found
        else
        {
            pageFault++;
            uint64_t *physicalPage = memoryManager.allocateFrame(); // allocate new physical frame
            pageTable[startPage + i] = physicalPage;                // map the logical page number to newly allocated physical page
            memoryAllocated += PAGE_SIZE;                           // update the amount of allocated memory
        }
    }
}

// Single-Level Page Table Implementation
void TaskWSLP::requestMemory(uint64_t lAddr, uint64_t size, MemoryManager &memoryManager)
{
    uint64_t startPage = lAddr >> OFFSET_BITS;
    uint64_t endPage = startPage + (uint64_t)ceil((double)size / PAGE_SIZE);

    for (uint64_t pageNumber = startPage; pageNumber < endPage; pageNumber++)
    {
        if (pageNumber >= pageTableSize)
        {
            throw out_of_range("Page number exceeds virtual address space");
        }
        if (pageTable[pageNumber] != nullptr)
        {
            pageHit++;
        }
        else
        {
            pageFault++;
            uint64_t *physicalPageAddr = memoryManager.allocateFrame();
            pageTable[pageNumber] = physicalPageAddr;
            memoryAllocated += PAGE_SIZE;
        }
    }
}

// // Two-Level Page Table Implementation
void TaskWMLP::requestMemory(uint64_t lAddr, uint64_t size, MemoryManager &memoryManager)
{
    uint64_t startPage = lAddr >> OFFSET_BITS;                               // Starting page
    uint64_t endPage = startPage + (uint64_t)ceil((double)size / PAGE_SIZE); // lastPage

    for (uint64_t pageNumber = startPage; pageNumber < endPage; pageNumber++)
    {
        uint64_t level1Index = pageNumber >> LEVEL_2_BITS;
        uint64_t level2Index = pageNumber & ((1ULL << LEVEL_2_BITS) - 1);

        // Check if level1 entry exists, if not create it
        if (pageTable[level1Index] == nullptr)
        {
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
            }
        }
        else
        {
            pageHit++;
        }
    }
}
