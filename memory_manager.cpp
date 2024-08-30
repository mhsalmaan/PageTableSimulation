#include "memory_manager.h"
#include <iostream>

uint64_t* MemoryManager::allocateFrame()
{
    // if (freeFrames.empty())
    // {
    //     throw runtime_error("Memory Full..");
    // }
    // uint64_t frame = freeFrames.front();
    // freeFrames.pop();
    // allocatedFrames++;
    if(allocatedFrames < totalFrames && freeFrames[allocatedFrames] == 0)
    {
        freeFrames[allocatedFrames] = 1;
        uint64_t *addr = (uint64_t*)&freeFrames[allocatedFrames];
        allocatedFrames++;
        return addr;
    }

    throw runtime_error("Memory Full..");
    return nullptr;
}

uint64_t MemoryManager::getRemainingFrames()
{
    return totalFrames - allocatedFrames;
}