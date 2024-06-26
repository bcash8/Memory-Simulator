#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "memory.h"
#include <math.h>

bool read_file(char *file_name,
               unsigned int *virtual_memory_size,
               unsigned int *physical_memory_size,
               unsigned int *page_size,
               unsigned int *page_table_loc,
               int **memory_contents,
               unsigned int *memory_count)
{
  FILE *file = fopen(file_name, "r");
  if (!file)
  {
    perror("Failed to open file");
    return false;
  }

  if (fscanf(file, "%u", virtual_memory_size) != 1 || !is_power_of_2(*virtual_memory_size))
  {
    printf("Invalid virtual memory size\n");
    fclose(file);
    return false;
  }

  if (fscanf(file, "%u", physical_memory_size) != 1 || !is_power_of_2(*physical_memory_size))
  {
    printf("Invalid physical memory size\n");
    fclose(file);
    return false;
  }

  if (fscanf(file, "%u", page_size) != 1 || !is_power_of_2(*page_size))
  {
    printf("Invalid page size\n");
    fclose(file);
    return false;
  }

  if (fscanf(file, "%u", page_table_loc) != 1 || (*page_table_loc % *page_size) != 0)
  {
    printf("Invalid page table location\n");
    fclose(file);
    return false;
  }

  *memory_count = 0;
  int value;
  while (fscanf(file, "%d", &value) == 1)
  {
    (*memory_count)++;
  }
  rewind(file);

  // Skip the meta data at the top
  for (int i = 0; i < 4; i++)
  {
    fscanf(file, "%*d"); //%*d ignores the field
  }

  *memory_contents = (int *)malloc(*memory_count * sizeof(int));
  if (!*memory_contents)
  {
    perror("Failed to get memory");
    fclose(file);
    return false;
  }

  int i = 0;
  while (fscanf(file, "%d", &value) == 1)
  {
    (*memory_contents)[i] = value;
    i++;
  }

  fclose(file);
  return true;
}

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    printf("Invalid number of arguments. Expected 1 argument for file name.");
    return 1;
  }

  printf("Welcome to the memory simulator\n");

  char *file_name = argv[1];

  unsigned int virtual_memory_size, physical_memory_size, page_size, page_table_loc, memory_count;
  int *physical_memory;

  bool success = read_file(file_name, &virtual_memory_size, &physical_memory_size, &page_size, &page_table_loc, &physical_memory, &memory_count);
  if (!success)
  {
    return 1;
  }

  unsigned int address_size = (unsigned int)log2(virtual_memory_size);
  unsigned int offset_bits = (unsigned int)log2(page_size);
  unsigned int page_bits = (unsigned int)log2(virtual_memory_size / page_size);
  unsigned int frame_bits = (unsigned int)log2(physical_memory_size / page_size);

  char *input = NULL;
  size_t len = 0;
  char cmd;
  unsigned int address;
  int value;

  while (1)
  {
    printf(">");
    if (getline(&input, &len, stdin) == -1)
      break;
    sscanf(input, "%c %d %d", &cmd, &address, &value);

    if (cmd == 't')
    {
      printf("%d -> %d\n", address, get_physical_address(address, offset_bits, page_table_loc, physical_memory));
    }
    else if (cmd == 'r')
    {
      printf("%d: %d\n", address, read_value(address, offset_bits, page_table_loc, physical_memory));
    }
    else if (cmd == 'w')
    {
      write_value(value, address, offset_bits, page_table_loc, physical_memory);
      printf("Value: %d, written to VA: %d\n", value, address);
    }
    else if (cmd == 'q')
    {
      printf("Goodbye\n");
      return 0;
    }
    else if (cmd == 'h')
    {
      printf("help:\n\
        t  translate the virtual address\n\
        r  read the value at address\n\
        w   write the value to the memory address\n\
        h help\n\
        q quit\n"
      );
    }
    else
    {
      printf("Invalid input");
    }
  }

  free(physical_memory);
  return 0;
}
