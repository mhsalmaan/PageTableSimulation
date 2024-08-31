#include "config.h"
#include <iostream>
#include <pthread.h>
#include <fstream>
#include <vector>
#include <sstream>
#include <random>
#include <iomanip>

using namespace std;

pthread_mutex_t fileMutex = PTHREAD_MUTEX_INITIALIZER;

struct ThreadArgs
{
    int tid;
    int linesPerTask;
    ofstream *outfile;
};

struct MemorySection {
    uintptr_t start;
    uintptr_t end;
    const char* name;
};

const MemorySection memorySections[] = {
    {TEXT_SECTION_ADDRESS, DATA_SECTION_ADDRESS - 1, "Text"},
    {DATA_SECTION_ADDRESS, BSS_SECTION_ADDRESS - 1, "Data"},
    {BSS_SECTION_ADDRESS, HEAP_SECTION_ADDRESS - 1, "BSS"},
    {HEAP_SECTION_ADDRESS, STACK_SECTION_ADDRESS - 1, "Heap"}
};

const int numSections = sizeof(memorySections) / sizeof(MemorySection);

uintptr_t alignToPageSize(uintptr_t address) {
    return address & ~(PAGE_SIZE - 1);
}   

bool isAddressInSection(uintptr_t address, uintptr_t size, const MemorySection& section) {
    return (address >= section.start) && (address + size <= section.end);
}

void* generateTrace(void* arg) {
    ThreadArgs* args = static_cast<ThreadArgs*>(arg);
    int tid = args->tid;
    int linesPerTask = args->linesPerTask;
    ofstream& outFile = *(args->outfile);

     random_device rd;
     mt19937 gen(rd());
     uniform_int_distribution<> sizeDist(1, 1000);  // Size from 1 to 1000 KB

    for (int i = 0; i < linesPerTask; ++i) {
        uintptr_t address;
        uintptr_t size;
        const MemorySection* selectedSection;

        do {
            // Randomly select a memory section
            int sectionIndex =  uniform_int_distribution<>(0, numSections - 1)(gen);
            selectedSection = &memorySections[sectionIndex];

            // Generate a random address within the selected section
             uniform_int_distribution<uintptr_t> addressDist(selectedSection->start, selectedSection->end);
            address = alignToPageSize(addressDist(gen));

            // Generate a random size
            size = sizeDist(gen) * 1024;  // Convert KB to bytes
        } while (!isAddressInSection(address, size, *selectedSection));

         stringstream ss;
        ss << "T" << tid << ":0x" <<  setfill('0') <<  setw(16) <<  hex << address
           << ":" <<  dec << (size / 1024) << "KB\n";

        pthread_mutex_lock(&fileMutex);
        outFile << ss.str();
        pthread_mutex_unlock(&fileMutex);
    }

    delete args;
    return nullptr;
}
int main()
{
    int noOfTasks, noOfLines;
    cout << "Please Enter the no of tasks in the trace and no. of lines the tracefile should contain:" << endl;
    cin >> noOfTasks >> noOfLines;

    int linesPerTask = noOfLines / noOfTasks;

    ofstream outfile("output.txt");
    vector<pthread_t> threads(noOfTasks);

    srand(time(nullptr));

    for (int i = 0; i < noOfTasks; i++)
    {
        ThreadArgs *args = new ThreadArgs;
        args->tid = i + 1;
        args->linesPerTask = linesPerTask;
        args->outfile = &outfile;
        pthread_create(&threads[i], nullptr, generateTrace, args);
    }

    for (int i = 0; i < noOfTasks; i++)
    {
        pthread_join(threads[i], nullptr);
    }

    outfile.close();
    pthread_mutex_destroy(&fileMutex);

    return 0;
}