#ifndef CONFIG_H
#define CONFIG_H

#include <cmath>

// Page size alignment
#define PAGE_SIZE 4096

//Virtual Memory
#define VIRTUAL_MEMORY 128ULL*1024*1024*1024

//Main Memory
#define MAIN_MEMORY 128ULL*1024*1024*1024

//Multi-Level properties
#define TOTAL_ADDR_SPACE 32
#define OFFSET_BITS static_cast<int>(log2(PAGE_SIZE))
#define REMAINING_BITS_FOR_PAGE_NO (TOTAL_ADDR_SPACE - OFFSET_BITS)
#define LEVEL_1_BITS static_cast<int>(ceil(static_cast<double>(REMAINING_BITS_FOR_PAGE_NO) / 2))
#define LEVEL_2_BITS (REMAINING_BITS_FOR_PAGE_NO - LEVEL_1_BITS)

#define LEVEL_1_SIZE (1U << LEVEL_1_BITS)
#define LEVEL_2_SIZE (1U << LEVEL_2_BITS)

//For trace generation

#define TEXT_SECTION_ADDRESS 0x00005730

#define DATA_SECTION_ADDRESS 0x0005c380

#define BSS_SECTION_ADDRESS 0x0005c7c0

#define HEAP_SECTION_ADDRESS 0x00061a80

#define STACK_SECTION_ADDRESS 0xbfc00000

#endif