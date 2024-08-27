#ifndef CONFIG_H
#define CONFIG_H

#include <cmath>

// Page size alignment
#define PAGE_SIZE 4096  // 2main KB

//Virtual Memory
#define VIRTUAL_MEMORY 4ULL*1024*1024*1024

//Main Memory
#define MAIN_MEMORY 4ULL*1024*1024*1024

//Multi-Level properties
#define TOTAL_ADDR_SPACE 32
#define REMAINING_BITS_FOR_PAGE_NO TOTAL_ADDR_SPACE-(log2(PAGE_SIZE))

#endif