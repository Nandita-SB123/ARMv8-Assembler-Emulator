
#include "reg_emu.h"

void arithmetic_reg(
    int sf, char opc[], char opr[],
    char rm[], char operand[], char rn[], char rd[])
{
  int64_t op2;

  // lsl
  if (opr[1] == '0' && opr[2] == '0')
  {
    op2 = lsl_int(bin_to_int(operand), regs.gen_reg[bin_to_int(rm)]);
  }

  // lsr
  if (opr[1] == '0' && opr[2] == '1')
  {
    op2 = (int64_t)(lsr_int(bin_to_int(operand), regs.gen_reg[bin_to_int(rm)]));
    if (sf == 1)
    {
      int power = 64 - bin_to_int(operand);
      op2 = op2 & ((uint64_t)(pow(2, power) - 1));
    }
    else
    {
      int power = 32 - bin_to_int(operand);
      op2 = op2 & ((uint64_t)(pow(2, power) - 1));
    }
  }

  // asr
  if (opr[1] == '1' && opr[2] == '0')
  {
    op2 = asr_int(bin_to_int(operand), regs.gen_reg[bin_to_int(rm)], sf);
    if (sf == 0)
    {
      op2 = op2 & regW;
    }
  }

  // addition
  if (strcmp(opc, "00") == 0)
  {
    if (sf == 1)
    {
      regs.gen_reg[bin_to_int(rd)] = regs.gen_reg[bin_to_int(rn)] + op2;
    }
    else
    {
      regs.gen_reg[bin_to_int(rd)] = (regs.gen_reg[bin_to_int(rn)] + op2) & regW;
    }
  }

  if (strcmp(opc, "01") == 0)
  {
    if (sf == 1)
    {
      regs.gen_reg[bin_to_int(rd)] = regs.gen_reg[bin_to_int(rn)] + op2;
    }
    else
    {
      regs.gen_reg[bin_to_int(rd)] = (regs.gen_reg[bin_to_int(rn)] + op2) & regW;
    }

    int64_t result = regs.gen_reg[bin_to_int(rd)];

    if (sf == 1)
    {
      pstate.N = (result & nflagX) >> 63;
    }
    else
    {
      pstate.N =( result & nflagW) >> 31;
    }

    if (result == 0)
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

    if (op2 > 0 && regs.gen_reg[bin_to_int(rn)] > 0 && result < 0)
    {
  
      pstate.V = 1;
    }
    else if (op2 < 0 && regs.gen_reg[bin_to_int(rn)] < 0 && result > 0)
    {
    
      pstate.V = 1;
    }

    int64_t rn_val = regs.gen_reg[bin_to_int(rn)];


    if (sf == 0) {
      op2 = op2 & regW;
      rn_val = rn_val & regW;
      result = result & regW;
    }

    uint64_t op2cast = (uint64_t) op2 ;
    uint64_t regcast = (uint64_t) rn_val;
    uint64_t resultcast = (uint64_t) result;

    if (resultcast < op2cast || resultcast < regcast) {
      pstate.C = 1;
    }

  }

  // subtraction
  if (strcmp(opc, "10") == 0) // sub without flags
  {
    if (sf == 1)
    {
      regs.gen_reg[bin_to_int(rd)] = regs.gen_reg[bin_to_int(rn)] - op2;
    }
    else
    {
      regs.gen_reg[bin_to_int(rd)] = (regs.gen_reg[bin_to_int(rn)] - op2) & regW;
    }
  }

  if (strcmp(opc, "11") == 0) // sub with flags
  {
    if (sf == 1)
    {
      regs.gen_reg[bin_to_int(rd)] = regs.gen_reg[bin_to_int(rn)] - op2;
    }
    else
    {
      regs.gen_reg[bin_to_int(rd)] = (regs.gen_reg[bin_to_int(rn)] - op2) & regW;
    }

    int64_t result = regs.gen_reg[bin_to_int(rd)];

    if (sf == 1)
    {
      pstate.N = (result & nflagX) >> 63;
    }
    else
    {
      pstate.N = (result & nflagW) >> 31;
    }

    if (result == 0)
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

    if (sf == 0) {
      op2 = op2 & regW;
      rn_val = rn_val & regW;
    }

    uint64_t op2cast = (uint64_t) op2 ;
    uint64_t regcast = (uint64_t) rn_val;


    if (op2cast <= regcast)

    {
      pstate.C = 1;
    }
  }
}

void bitlogic_reg(
    int sf, char opc[], char opr[],
    char rm[], char operand[], char rn[], char rd[])
{
  int64_t op2;

  // lsl
  if ((opr[1] == '0') && (opr[2] == '0'))
  {
    op2 = lsl_int(bin_to_int(operand), regs.gen_reg[bin_to_int(rm)]);
  }

  // lsr
  if (opr[1] == '0' && opr[2] == '1')
  {
    op2 = lsr_int(bin_to_int(operand), regs.gen_reg[bin_to_int(rm)]);
    if (sf == 1)
    {
      int power = 64 - bin_to_int(operand);
      op2 = op2 & ((uint64_t)(pow(2, power) - 1));
    }
    else
    {
      int power = 32 - bin_to_int(operand);
      op2 = op2 & ((uint64_t)(pow(2, power) - 1));
    }
  }

  // asr
  if (opr[1] == '1' && opr[2] == '0')
  {
    op2 = asr_int(bin_to_int(operand), regs.gen_reg[bin_to_int(rm)], sf);
    if (sf == 0)
    {
      op2 = op2 & regW;
    }
  }

  // ror
  if (opr[1] == '1' && opr[2] == '1')
  {
    if (sf == 0)
    {
      op2 = rorw_int(bin_to_int(operand), regs.gen_reg[bin_to_int(rm)] & regW);
    }
    else
    {
      op2 = rorx_int(bin_to_int(operand), regs.gen_reg[bin_to_int(rm)]);
    }
  }

  // checking N
  if (opr[3] == '1')
  {
    op2 = ~op2;
  }

  if (sf == 0)
  {
    op2 = op2 << 32;
    op2 = op2 >> 32;
  }
  else
  {
    op2 = op2 << 64;
    op2 = op2 >> 64;
  }

  // note: we have already negated op2 if necessary
  // so we dont have to have 8 cases, only 4 needed

  if (strcmp(opc, "00") == 0)
  {
    if (sf == 1)
    {
      regs.gen_reg[bin_to_int(rd)] = regs.gen_reg[bin_to_int(rn)] & op2;
    }
    else
    {
      regs.gen_reg[bin_to_int(rd)] = (regs.gen_reg[bin_to_int(rn)] & op2) & regW;
    }
  }

  if (strcmp(opc, "01") == 0)
  {
    if (sf == 1)
    {
      regs.gen_reg[bin_to_int(rd)] = regs.gen_reg[bin_to_int(rn)] | op2;
    }
    else
    {
      regs.gen_reg[bin_to_int(rd)] = (regs.gen_reg[bin_to_int(rn)] | op2) & regW;
    }
  }

  if (strcmp(opc, "10") == 0)
  {
    if (sf == 1)
    {
      regs.gen_reg[bin_to_int(rd)] = regs.gen_reg[bin_to_int(rn)] ^ op2;
    }
    else
    {
      regs.gen_reg[bin_to_int(rd)] = (regs.gen_reg[bin_to_int(rn)] ^ op2) & regW;
    }
  }

  if (strcmp(opc, "11") == 0)
  {
    if (sf == 1)
    {
      regs.gen_reg[bin_to_int(rd)] = regs.gen_reg[bin_to_int(rn)] & op2;
    }
    else
    {
      regs.gen_reg[bin_to_int(rd)] = (regs.gen_reg[bin_to_int(rn)] & op2) & regW;
    }

    // updating condition flags
    int res = bin_to_int(rd);
    uint64_t result = regs.gen_reg[res];

    if (sf == 1)
    {
      pstate.N = result & nflagX;
    }
    else
    {
      pstate.N = result & nflagW;
    }

    if (result == 0)
    {
      pstate.Z = 1;
    }
    else
    {
      pstate.Z = 0;
    }

    pstate.C = 0;
    pstate.V = 0;
  }
}

void multiply_reg(int sf, char rm[], char x, char ra[], char rn[], char rd[])
{
  int64_t product = regs.gen_reg[bin_to_int(rn)] * regs.gen_reg[bin_to_int(rm)];

  if (x == '0')
  {
    if (sf == 1)
    {
      regs.gen_reg[bin_to_int(rd)] = regs.gen_reg[bin_to_int(ra)] + product;
    }
    else
    {
      regs.gen_reg[bin_to_int(rd)] = (regs.gen_reg[bin_to_int(ra)] + product) & regW;
    }
  }
  else if (x == '1')
  {
    if (sf == 1)
    {
      regs.gen_reg[bin_to_int(rd)] = regs.gen_reg[bin_to_int(ra)] - product;
    }
    else
    {
      regs.gen_reg[bin_to_int(rd)] = (regs.gen_reg[bin_to_int(ra)] - product) & regW;
    }
  }
}

// shifting functions

uint64_t lsl(int shift_amount, char thing_shifted[])
{
  uint64_t result = bin_to_int(thing_shifted) << shift_amount;
  return result;
}

uint64_t lsr(int shift_amount, char thing_shifted[])
{
  uint64_t result = bin_to_int(thing_shifted) >> shift_amount;
  return result;
}

// asr shifting
uint64_t asr(int shift_amount, char thing_shifted[])
{
  char sign_bit = thing_shifted[0];

  uint64_t result;

  if (sign_bit == '0')
  {
    result = lsr(shift_amount, thing_shifted);
    return result;
  }

  else
  {

    char number_as_string[32];

    char sign[shift_amount + 1];

    for (int i = 0; i < shift_amount; i++)
    {
      strcat(sign, "1");
    }

    result = lsr(shift_amount, thing_shifted);

    //  converts the number into a string
    sprintf(number_as_string, "%lu", decimalToBinary(result));

    strcat(sign, number_as_string);

    return bin_to_int(sign);
  }
}

uint64_t ror(int shift_amount, char thing_shifted[])
{
  uint64_t n = bin_to_int(thing_shifted);

  // Perform the rotation
  return (n >> shift_amount) | (n << (32 - shift_amount));
}

// integer shifting

int64_t lsl_int(int shift_amount, int64_t thing_shifted)
{
  int64_t result = thing_shifted << shift_amount;
  return result;
}

int64_t lsr_int(int shift_amount, int64_t thing_shifted)
{
  int64_t result = thing_shifted >> shift_amount;
  return result;
}

int64_t rorw_int(int shift_amount, int64_t thing_shifted)
{
  int64_t result = (thing_shifted >> shift_amount) | (thing_shifted << (32 - shift_amount));
  result = result & regW;
  return result;
}
uint64_t rorx_int(int shift_amount, int64_t thing_shifted)
{
  uint64_t rotateMask = 1;
  rotateMask = rotateMask << shift_amount;
  rotateMask--;

  uint64_t intermediate = (thing_shifted & regX) >> shift_amount;
  uint64_t rotatedBitsMovedLeft = (thing_shifted & rotateMask) << (64 - shift_amount);
  intermediate = intermediate | rotatedBitsMovedLeft;
  return intermediate;
}

uint64_t asr_int(int shift_amount, int64_t thing_shifted, int sf)
{
  uint64_t modeMask = sf ? regX : regW;
  uint64_t signMask = sf ? nflagX : nflagW;

  uint64_t sign = thing_shifted & signMask;
  uint64_t intermediate = thing_shifted >> shift_amount;
  for (int i = 0; i < shift_amount; i++)
  {
    intermediate = intermediate | sign;
    sign = sign >> 1;
  }
  return intermediate & modeMask;
}