#include "helper.h"

char *label;
char *opcode;
char *operand1;
char *operand2;
char *operand3;
char *operand4;
char *operand5;

char *subs = "subs";
char *ands = "ands";
char *adds = "adds";
char *madd = "madd";
char *msub = "msub";
char *sub = "sub";
char *orn = "orn";
char *orr = "orr";

char *zero_reg;
void initialize_zero_reg(void)
{
  if (operand1[0] == 'w')
  {
    zero_reg = "wzr";
  }
  else
  {
    zero_reg = "rzr";
  }
}
int displaying_array[SIZE];
ST symbol_table[MAX_LABELS];

static void complement(int no, char *bin, int len);

// convert xn to 5 bit equivalent
// input xn or wn and you'll get a 5 bit result you can input into your instruction.
void reg_to_bin(char *str, char *bin)
{
  // 0 reg accounted for.
  if (!strcmp(str + 1, "zr"))
  {
    strcpy(bin, "11111");
    return;
  }
  int no = atoi((str + 1));
  memset(bin, '0', REG_BITS);
  bin[5] = '\0';
  int count = REG_BITS - 1;
  while (no != 0)
  {
    bin[count] = (char)((no % 2) + ASCII_0);
    no /= 2;
    count--;
  }
}

// convert int value to binary of required length
// input #imm, string to put into and strlen(string).
// Example if I want 19 bits, input #val, string and 19 (not 20)
// string should be of size len + 1. '\0' will be accounted for.
void imm_to_bin(char *val, char *bin, int len)
{
  int no;
  if (!strncmp(val + 1, "0x", 2))
  {
    no = (int)strtol(val + 1, NULL, 16);
  }
  else
  {
    no = atoi(val + 1);
  }
  int temp = no;
  memset(bin, '0', len);
  bin[len] = '\0';
  int count = len - 1;
  while (no != 0)
  {
    bin[count] = (char)(abs(no % 2) + ASCII_0);
    no /= 2;
    count--;
  }

  if (temp < 0)
  {
    complement(temp, bin, len);
  }
}

// convert binary string value to integer
uint64_t bin_to_int(const char *str)
{
  uint64_t result = strtoull(str, NULL, 2);
  return result;
}

// convert decimal value to binary
int64_t decimalToBinary(int num)
{
  int64_t binaryNum = 0;
  int64_t place = 1;

  while (num > 0)
  {
    int remainder = num % 2;
    num = num / 2;
    binaryNum += remainder * place;
    place *= 10;
  }
  return binaryNum;
}

static void complement(int no, char *bin, int len)
{
  // flip, add 1, reconvert.
  for (int i = 0; i < strlen(bin); i++)
  {
    bin[i] = bin[i] == '1' ? '0' : '1';
  }

  int compl_no = bin_to_int(bin) + 1;
  int count = len - 1;
  while (compl_no != 0)
  {
    bin[count] = (char)(abs(compl_no % 2) + ASCII_0);
    compl_no /= 2;
    count--;
  }
}