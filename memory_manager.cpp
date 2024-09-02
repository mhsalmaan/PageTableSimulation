#include "memory_manager.h"
#include <iostream>

// function to allocate frame from the memory
uint64_t *MemoryManager::allocateFrame()
{
    // check if frames are available for allocation
    if (allocatedFrames < totalFrames && freeFrames[allocatedFrames] == 0)
    {
        // initially, mark the frame as allocated
        freeFrames[allocatedFrames] = 1;
        uint64_t *addr = (uint64_t *)&freeFrames[allocatedFrames];
        allocatedFrames++;
        return addr; // return the address of allocated frame
    }

    // throw runtime_error exception if no frames available
    throw runtime_error("Memory Full..");
    return nullptr;
}

// function to get the number of remaining frames that can be allocated
uint64_t MemoryManager::getRemainingFrames()
{
    return totalFrames - allocatedFrames; // return the remaining frames
}