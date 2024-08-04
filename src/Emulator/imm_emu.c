

#include <stdint.h>
#include "imm_emu.h"

int64_t add(int sf, char rd[], char rn[], int op2)
{
  int64_t result;
  if (sf == 1)
  {
    result = regs.gen_reg[bin_to_int(rn)] + op2;
  }
  else
  {
    result = (regs.gen_reg[bin_to_int(rn)] + op2) & regW;
  }

  if (strcmp(rd, "11111") != 0)
  {
    regs.gen_reg[bin_to_int(rd)] = result;
  }
  return result;
}

int64_t sub(int sf, char rd[], char rn[], int op2)
{
  int64_t result;
  if (sf == 1)
  {
    result = regs.gen_reg[bin_to_int(rn)] - op2;
  }
  else
  {
    result = (regs.gen_reg[bin_to_int(rn)] - op2) & regW;
  }

  if (strcmp(rd, "11111") != 0)
  {
    regs.gen_reg[bin_to_int(rd)] = result;
  }

  return result;
}

void add_wf(int sf, char rd[], char rn[], int op2)
{
  int64_t sum = add(sf, rd, rn, op2);
  printf("sum %ld\n", sum);
  if (sf == 1)
  {
    pstate.N = (sum & nflagX) >> 63;
  }
  else
  {
    pstate.N = (sum & nflagW) >> 31;
  }
  if (sum == 0)
  {
    pstate.Z = 1;
  }
  else
  {
    pstate.Z = 0;
  }

  int op2_sign;
  if (sf == 1)
  {
    op2_sign = op2 & nflagX;
  }
  else
  {
    op2_sign = op2 & nflagW;
  }

  if (op2_sign == 0 && regs.gen_reg[bin_to_int(rn)] > 0 && pstate.N == 1)
  {
    pstate.V = 1;
  }
  else if (op2_sign == 1 && regs.gen_reg[bin_to_int(rn)] < 0 && pstate.N == 0)
  {
    pstate.V = 1;
  }

  if (sf == 0 && op2 > INT32_MAX - regs.gen_reg[bin_to_int(rn)])
  {
    pstate.C = 1;
  }
  else if (sf == 1 && op2 > INT64_MAX - regs.gen_reg[bin_to_int(rn)])
  {
    pstate.C = 1;
  }
}

void sub_wf(int sf, char rd[], char rn[], int op2)
{
  int64_t diff = sub(sf, rd, rn, op2);

  // getting difference in hexadecimal to check MSB = 1 for N.
  char diff_in_hexa[17];
  sprintf(diff_in_hexa, "%lx", diff);
  diff_in_hexa[16] = '\0';

  if (diff_in_hexa[0] >= '8')
  {
    pstate.N = 1;
  }
  if (diff == 0)
  {
    pstate.Z = 1;
  }
  else
  {
    pstate.Z = 0;
  }

  int op2_sign;
  if (sf == 1)
  {
    op2_sign = op2 & nflagX;
  }
  else
  {
    op2_sign = op2 & nflagW;
  }
  if (op2_sign == 1 && regs.gen_reg[bin_to_int(rn)] > 0 && pstate.N == 1)
  {
    pstate.V = 1;
  }
  else if (op2_sign == 0 && regs.gen_reg[bin_to_int(rn)] < 0 && pstate.N == 0)
  {
    pstate.V = 1;
  }

  int64_t rn_val = regs.gen_reg[bin_to_int(rn)];

  if (sf == 0)
  {
    op2 = op2 & regW;
    rn_val = rn_val & regW;
  }

  uint64_t op2cast = (uint64_t)op2;
  uint64_t regcast = (uint64_t)rn_val;
  printf("op2cast %lu \n", op2cast);
  printf("regcast %lu \n", regcast);

  if (op2cast <= regcast)
  {
    pstate.C = 1;
  }
}

void wide_move(int sf, char hw[], char imm16[], char rd[], char opc[])
{
  int hw_int = bin_to_int(hw);
  uint64_t op = bin_to_int(imm16) << (hw_int * 16);
  int opc_int = bin_to_int(opc);
  char result[64];

  if (opc_int == 0) // movn
  {
    for (int i = 63; i >= 0; i--)
    {
      result[63 - i] = ((~op) & (1ULL << i)) ? '1' : '0';
    }
  }
  else if (opc_int == 2) // movz
  {
    for (int i = 63; i >= 0; i--)
    {
      result[63 - i] = (op & (1ULL << i)) ? '1' : '0';
    }
  }
  else if (opc_int == 3) // movk
  {
    // Turn value at register to binary string representation (64 bits)
    for (int i = 63; i >= 0; i--)
    {
      result[63 - i] = (regs.gen_reg[bin_to_int(rd)] & (1ULL << i)) ? '1' : '0';
    }

    int lower = (3 - hw_int) * 16; // because little endian

    // insert imm16 into register value
    strncpy(result + lower, imm16, strlen(imm16));
  }
  if (sf == 0)
  {
    // clear first 32 bits as using wn - 32 bit mode
    memset(result, '0', 32);
  }

  regs.gen_reg[bin_to_int(rd)] = bin_to_int(result);
}
