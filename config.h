#ifndef CONFIG_H
#define CONFIG_H

#include <cmath>

// Page size alignment
#define PAGE_SIZE 1024  // 2main KB

//Virtual Memory
#define VIRTUAL_MEMORY 8ULL*1024*1024*1024

//Main Memory
#define MAIN_MEMORY 16ULL*1024*1024*1024

//Multi-Level properties
#define TOTAL_ADDR_SPACE 32
#define OFFSET_BITS static_cast<int>(log2(PAGE_SIZE))
#define REMAINING_BITS_FOR_PAGE_NO (TOTAL_ADDR_SPACE - OFFSET_BITS)
#define LEVEL_1_BITS static_cast<int>(ceil(static_cast<double>(REMAINING_BITS_FOR_PAGE_NO) / 2))
#define LEVEL_2_BITS (REMAINING_BITS_FOR_PAGE_NO - LEVEL_1_BITS)

#define LEVEL_1_SIZE (1U << LEVEL_1_BITS)
#define LEVEL_2_SIZE (1U << LEVEL_2_BITS)

#endif