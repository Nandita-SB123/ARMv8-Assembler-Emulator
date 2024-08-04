#include "help.h"

char curr_instr[33];
void initialize(void);

PSTATE pstate;
SPR regs;

int8_t mem[MEM_SIZE];

uint32_t memory[MEMORY_SIZE];

uint64_t bin_to_int(const char *str)
{
  // strtoull converts a string to an unsigned long long int.
  uint64_t result = strtoull(str, NULL, 2);
  return result;
}

int64_t reverse_complement(char val[], int len)
{

  if (val[0] == '0')
  {
    return bin_to_int(val);
  }
  int64_t val_in_int = (bin_to_int(val + 1) - 1);

  // result first stores the binary version of original val - 1
  char result[9];
  sprintf(result, "%08ld", decimalToBinary(val_in_int));
  result[8] = '\0';

  // switching values to reverse complement:
  for (int i = 0; i < 8; i++)
  {
    result[i] = (result[i] == '0' ? '1' : '0');
  }
  // multiplying by -1 since value is negative
  return (bin_to_int(result) * -1);
}

int64_t decimalToBinary(int num)
{
  int64_t binaryNum = 0;
  int64_t place = 1; // To place each binary digit in the correct position

  while (num > 0)
  {
    int remainder = num % 2;
    num = num / 2;
    binaryNum += remainder * place;
    place *= 10;
  }

  return binaryNum;
}
