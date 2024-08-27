#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <iomanip>
#include <algorithm>
#include <string>

using namespace std;

struct TraceEntry {
    int task;
    uint64_t address;
    int size;
};

vector<TraceEntry> generateTrace(int numTasks, int numLines, int minSize, int maxSize, uint64_t minAddress, uint64_t maxAddress) {
    vector<TraceEntry> trace;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> taskDist(1, numTasks);
    uniform_int_distribution<> sizeDist(minSize, maxSize);
    uniform_int_distribution<uint64_t> addressDist(minAddress, maxAddress);

    for (int i = 0; i < numLines; ++i) {
        TraceEntry entry;
        entry.task = taskDist(gen);
        entry.size = sizeDist(gen);
        entry.address = addressDist(gen) & ~0x3FF; // Align to 1KB boundary
        
        // Ensure the address doesn't go out of bounds
        if (entry.address + entry.size * 1024 > maxAddress) {
            entry.address = maxAddress - entry.size * 1024;
        }
        
        trace.push_back(entry);
    }

    return trace;
}

void writeTraceToFile(const vector<TraceEntry>& trace, const string& filename) {
    ofstream outFile(filename);
    if (!outFile) {
        cerr << "Error: Unable to open file for writing." << endl;
        return;
    }

    for (const auto& entry : trace) {
        outFile << "T" << entry.task << ":0x"
                << setfill('0') << setw(8) << hex << entry.address
                << ":" << dec << entry.size << "KB" << endl;
    }

    outFile.close();
}

int main() {
    int numTasks, numLines, minSize, maxSize;
    uint64_t minAddress, maxAddress;
    int pageSize, virtualMemory, mainMemory;

    cout << "Enter the number of tasks: ";
    cin >> numTasks;

    cout << "Enter the number of lines to generate: ";
    cin >> numLines;

    cout << "Enter the minimum allocation size (in KB): ";
    cin >> minSize;

    cout << "Enter the maximum allocation size (in KB): ";
    cin >> maxSize;

    cout << "Enter the minimum address (in hex, e.g., 0x00000000): ";
    cin >> hex >> minAddress;

    cout << "Enter the maximum address (in hex, e.g., 0x040FFFFF): ";
    cin >> hex >> maxAddress;

    cout << "Enter the PAGE_SIZE (in KB): ";
    cin >> dec >> pageSize;

    cout << "Enter the Virtual Memory size (in GB): ";
    cin >> virtualMemory;

    cout << "Enter the Main Memory size (in GB): ";
    cin >> mainMemory;

    vector<TraceEntry> trace = generateTrace(numTasks, numLines, minSize, maxSize, minAddress, maxAddress);

    string filename = "tracefile_" + to_string(pageSize) + "KB_" + to_string(virtualMemory) + "GB_" + to_string(mainMemory) + "GB.txt";
    writeTraceToFile(trace, filename);

    cout << "Trace file '" << filename << "' has been generated." << endl;

    return 0;
}