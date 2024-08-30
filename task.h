#ifndef TASK_H
#define TASK_H

#include "memory_manager.h"
#include <unordered_map>

using namespace std;

// Task with Page table as a map(Logical page number is the key and physical page number is value)
class Task
{
public:
    uint64_t tid;
    unordered_map<uint64_t, uint64_t*> pageTable;
    uint64_t pageHit = 0;
    uint64_t pageFault = 0;
    uint64_t memoryAllocated = 0;
    Task() : tid(0) {}
    Task(uint64_t x) : tid(x) {}
    void requestMemory(uint64_t logicalAddress, uint64_t size, MemoryManager &memmoryManager);
};

// Single-Level Page Table(SLP)
class TaskWSLP
{
public:
    vector<uint64_t*> pageTable;
    uint64_t tid;
    const uint64_t pageTableSize = VIRTUAL_MEMORY / PAGE_SIZE;
    uint64_t pageHit = 0;
    uint64_t pageFault = 0;
    uint64_t memoryAllocated = 0;
    TaskWSLP() : tid(0), pageTable(pageTableSize, nullptr) {}
    TaskWSLP(uint64_t tid) {
        tid = tid;
        pageTable.resize(pageTableSize,nullptr);
     }
    void requestMemory(uint64_t logicalAddress, uint64_t size, MemoryManager &memoryManager);
};
// Task with Multi-Level Page Table (Two-Level)
// class TaskWMLP
// {
// private:
//     uint64_t tid;
//     vector<vector<uint64_t>> level1Table; // Level 1 Page Table
//     const uint64_t offsetBits;
//     const uint64_t level1Bits;
//     const uint64_t level2Bits;

// public:
//     uint64_t pageHit = 0;
//     uint64_t pageFault = 0;
//     uint64_t memoryAllocated = 0;

//     TaskWMLP() : tid(0), offsetBits(0), level1Bits(0), level2Bits(0) {}

//     TaskWMLP(uint64_t tid)
//         : tid(tid),
//           offsetBits((uint64_t)(log2(PAGE_SIZE))),
//           level1Bits((uint64_t)(ceil((double)(REMAINING_BITS_FOR_PAGE_NO) / 2))),
//           level2Bits(REMAINING_BITS_FOR_PAGE_NO - level1Bits),
//           level1Table(1 << level1Bits, vector<uint64_t>(1 << level2Bits, -1)) {}

//     void requestMemory(uint64_t logicalAddress, uint64_t size, MemoryManager &memoryManager);
// };

// Task with Multi-Level Page Table (Two-Level)
class TaskWMLP
{
public:
    uint64_t tid;
    vector<vector<uint64_t*>*> pageTable;
    uint64_t pageHit = 0;
    uint64_t pageFault = 0;
    uint64_t memoryAllocated = 0;

    TaskWMLP() : tid(0) {}

    TaskWMLP(uint64_t tid) {
        tid = tid;
        pageTable.resize(LEVEL_1_SIZE, nullptr);
    }
    void requestMemory(uint64_t logicalAddress, uint64_t size, MemoryManager &memoryManager);
};

#endif