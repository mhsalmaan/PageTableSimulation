#include "memory_manager.h"
#include <iostream>

uint64_t MemoryManager::allocateFrame()
{
    if (freeFrames.empty())
    {
        throw runtime_error("Memory Full..");
    }
    uint64_t frame = freeFrames.front();
    freeFrames.pop();
    allocatedFrames++;
    return frame;
}

uint64_t MemoryManager::getRemainingFrames()
{
    return totalFrames - allocatedFrames;
}