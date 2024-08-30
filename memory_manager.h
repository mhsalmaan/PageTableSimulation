#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <queue>
#include <stdint.h>
using namespace std;


//MemoryManager Main memory mplememted as queue.
class MemoryManager {
private:
    vector<int> freeFrames;
public:
    uint64_t totalFrames;
    uint64_t allocatedFrames = 0;
    MemoryManager(uint64_t totalFrames) : totalFrames(totalFrames) {
        freeFrames.resize(totalFrames,0);
    }
    uint64_t* allocateFrame(); //function to allocate frames
    uint64_t getRemainingFrames(); //function to give frames
};

#endif