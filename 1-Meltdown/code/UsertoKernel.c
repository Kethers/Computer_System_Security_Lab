#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <emmintrin.h>
#include <x86intrin.h>

int main() 
{
  char *kernel_data_addr = (char*)0xf93fb000;
  char kernel_data = *kernel_data_addr;
  printf("I have reached here.\n");
  return 0;
}
