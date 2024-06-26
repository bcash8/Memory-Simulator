#include <stdio.h>
#include <stdbool.h>
#include "memory.h"

bool is_power_of_2(unsigned int value)
{
  return (value & (value - 1)) == 0 && value != 0;
}

int get_physical_address(unsigned int virtual_address, unsigned int offset_bits, unsigned int page_table_loc, int* physical_memory) {
  unsigned int mask = (1 << offset_bits) - 1; // shift 1 left by offset amount, then subtract 1 to create a mask like 0xFFF
  unsigned int offset = virtual_address & mask; // extract the offset amount from the address
  unsigned int page_number = virtual_address >> offset_bits; // extract the page_number by shifting the offset bits out
  unsigned int physical_page_number = physical_memory[page_table_loc + page_number];
  unsigned int physical_address = physical_page_number + offset; //add the offset to the physical_page_number to get the full address

  return physical_address;
}

int read_value(int virtual_address, int page_mask, int page_table_loc, int* physical_memory) {
  return physical_memory[get_physical_address(virtual_address, page_mask, page_table_loc, physical_memory)];
}

void write_value(int value, int virtual_address, int page_mask, int page_table_loc, int* physical_memory) {
  physical_memory[get_physical_address(virtual_address, page_mask, page_table_loc, physical_memory)] = value;
}
