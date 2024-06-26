#ifndef MEMORY_H
#define MEMORY_H

extern bool is_power_of_2(unsigned int value);

extern int get_physical_address(unsigned int virtual_address, unsigned int offset_bits, unsigned int page_table_loc, int* physical_memory);

extern int read_value(int virtual_address, int page_mask, int page_table_loc, int* physical_memory);

extern void write_value(int value, int virtual_address, int page_mask, int page_table_loc, int* physical_memory);

#endif