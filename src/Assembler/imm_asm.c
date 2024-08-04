#include "imm_asm.h"

#include "helper.h"

static char instr[NO_OF_BITS + 1];

void data_process_asm(void)
{
  if (!strcmp(opcode, "add") || !strcmp(opcode, "adds") ||
      !strcmp(opcode, "sub") || !strcmp(opcode, "subs"))
  {
    if (operand3[0] == 'x' || operand3[0] == 'w')
    {
      reg_arith();
    }
    else
    {
      imm_arith();
    }
  }
  else if (!strcmp(opcode, "movn") || !strcmp(opcode, "movz") ||
           !strcmp(opcode, "movk"))
  {
    imm_widemove();
  }
  else if (!strcmp(opcode, "madd") || !strcmp(opcode, "msub"))
  {
    reg_mult();
  }
  else
  {
    reg_logical();
  }
  displaying_array[displaying_index] = bin_to_int(instr);
  strcpy(instr, "");
}

void imm_arith(void)
{
  // Adding sf
  if (operand1[0] == 'w')
  {
    strcat(instr, "0");
  }
  else
  {
    strcat(instr, "1");
  }
  // Adding opc
  if (!strcmp(opcode, "add"))
  {
    strcat(instr, "00");
  }
  else if (!strcmp(opcode, "adds"))
  {
    strcat(instr, "01");
  }
  else if (!strcmp(opcode, "sub"))
  {
    strcat(instr, "10");
  }
  else
  {
    strcat(instr, "11");
  }

  // Adding 100
  strcat(instr, "100");
  // Adding opi
  strcat(instr, "010");
  // Adding sh
  if (operand4 == NULL)
  {
    strcat(instr, "0");
  }
  else
  {

    if ((!strcmp(operand4, "lsl")) && (strcmp(operand5, "#0") != 0))
    {

      strcat(instr, "1");
    }
    else
    {
      strcat(instr, "0");
    }
  }
  // Adding imm12
  char imm12[13];
  imm_to_bin(operand3, imm12, 12);
  strcat(instr, imm12);
  // Adding rn
  char rn[6];
  reg_to_bin(operand2, rn);
  strcat(instr, rn);
  // Adding rd
  char rd[6];
  reg_to_bin(operand1, rd);
  strcat(instr, rd);
}

void imm_widemove(void)
{
  // Adding sf
  if (operand1[0] == 'w')
  {
    strcat(instr, "0");
  }
  else
  {
    strcat(instr, "1");
  }
  // Adding opc
  if (!strcmp(opcode, "movn"))
  {
    strcat(instr, "00");
  }
  else if (!strcmp(opcode, "movz"))
  {
    strcat(instr, "10");
  }
  else
  {
    strcat(instr, "11");
  }
  // Adding 100
  strcat(instr, "100");
  // Adding opi
  strcat(instr, "101");
  // Adding hw
  if (operand4 == NULL)
  {
    strcat(instr, "00");
  }
  else
  {
    int c = (atoi(operand4 + 1)) / 16;
    if (c == 0)
    {
      strcat(instr, "00");
    }
    else if (c == 1)
    {
      strcat(instr, "01");
    }
    else if (c == 2)
    {
      strcat(instr, "10");
    }
    else
    {
      strcat(instr, "11");
    }
  }
  // Adding imm16
  char imm16[17];
  imm_to_bin(operand2, imm16, 16);
  strcat(instr, imm16);
  // Adding rd
  char rd[6];
  reg_to_bin(operand1, rd);
  strcat(instr, rd);
}

void reg_mult(void)
{
  // Adding sf
  if (operand1[0] == 'w')
  {
    strcat(instr, "0");
  }
  else
  {
    strcat(instr, "1");
  }
  // Adding bits
  strcat(instr, "0011011000");
  // Adding rm
  char rm[6];
  reg_to_bin(operand3, rm);
  strcat(instr, rm);
  // Adding x
  if (!strcmp(opcode, "madd"))
  {
    strcat(instr, "0");
  }
  else
  {
    strcat(instr, "1");
  }
  // Adding ra
  char ra[6];
  reg_to_bin(operand4, ra);
  strcat(instr, ra);
  // Adding rn
  char rn[6];
  reg_to_bin(operand2, rn);
  strcat(instr, rn);
  // Adding rd
  char rd[6];
  reg_to_bin(operand1, rd);
  strcat(instr, rd);
}

void reg_arith(void)
{
  // Adding sf
  if (operand1[0] == 'w')
  {
    strcat(instr, "0");
  }
  else
  {
    strcat(instr, "1");
  }
  // Adding opc
  if (!strcmp(opcode, "add"))
  {
    strcat(instr, "00");
  }
  else if (!strcmp(opcode, "adds"))
  {
    strcat(instr, "01");
  }
  else if (!strcmp(opcode, "sub"))
  {
    strcat(instr, "10");
  }
  else
  {
    strcat(instr, "11");
  }
  // Adding M and 3 other bits
  strcat(instr, "0101");
  // Adding opr
  if (operand4 == NULL)
  {
    strcat(instr, "1000");
  }
  else
  {
    if (!strcmp(operand4, "lsl"))
    {
      strcat(instr, "1000");
    }
    else if (!strcmp(operand4, "lsr"))
    {
      strcat(instr, "1010");
    }
    else if (!strcmp(operand4, "asr"))
    {
      strcat(instr, "1100");
    }
  }
  // Adding rm
  char rm[10];
  reg_to_bin(operand3, rm);
  strcat(instr, rm);
  // Adding operand
  char operand[10];
  if (operand5 == NULL)
  {
    strcat(operand, "000000");
  }
  else
  {

    imm_to_bin(operand5, operand, 6);
  }
  strcat(instr, operand);
  // Adding rn
  char rn[6];
  reg_to_bin(operand2, rn);
  strcat(instr, rn);
  // Adding rd
  char rd[6];
  reg_to_bin(operand1, rd);
  strcat(instr, rd);
}

void reg_logical(void)
{
  // Adding sf
  if (operand1[0] == 'w')
  {
    strcat(instr, "0");
  }
  else
  {
    strcat(instr, "1");
  }
  // Adding opc
  if (!strcmp(opcode, "and") || !strcmp(opcode, "bic"))
  {
    strcat(instr, "00");
  }
  else if (!strcmp(opcode, "orr") || !strcmp(opcode, "orn"))
  {
    strcat(instr, "01");
  }
  else if (!strcmp(opcode, "eor") || !strcmp(opcode, "eon"))
  {
    strcat(instr, "10");
  }
  else
  {
    strcat(instr, "11");
  }
  // Adding bits
  strcat(instr, "0101");
  // Adding shift bits
  if (operand4 == NULL)
  {
    strcat(instr, "000");
  }
  else
  {
    if (!strcmp(operand4, "lsl"))
    {
      strcat(instr, "000");
    }
    else if (!strcmp(operand4, "lsr"))
    {
      strcat(instr, "001");
    }
    else if (!strcmp(operand4, "asr"))
    {
      strcat(instr, "010");
    }
    else
    {
      strcat(instr, "011");
    }
  }
  // Adding N
  if (!strcmp(opcode, "and") || !strcmp(opcode, "orr") || !strcmp(opcode, "eor") || !strcmp(opcode, "ands"))
  {
    strcat(instr, "0");
  }
  else
  {
    strcat(instr, "1");
  }
  // Adding rm
  char rm[6];
  reg_to_bin(operand3, rm);
  strcat(instr, rm);
  // Adding operand
  char operand[10];
  if (operand5 == NULL)
  {
    strcat(operand, "000000");
  }
  else
  {
    imm_to_bin(operand5, operand, 6);
  }
  strcat(instr, operand);
  // Adding rn
  char rn[6];
  reg_to_bin(operand2, rn);
  strcat(instr, rn);
  // Adding rd
  char rd[6];
  reg_to_bin(operand1, rd);
  strcat(instr, rd);
}

void aliases(void)
{
  initialize_zero_reg();

  if (!strcmp(opcode, "cmp") || !strcmp(opcode, "cmn") || !strcmp(opcode, "tst"))
  {
    if (!strcmp(opcode, "cmp"))
    {
      opcode = subs;
    }
    else if (!strcmp(opcode, "cmn"))
    {
      opcode = adds;
    }
    else
    {
      opcode = ands;
    }
    if (operand3 != NULL && operand4 != NULL)
    {
      operand5 = operand4;
      operand4 = operand3;
    }
    operand3 = operand2;
    operand2 = operand1;
    operand1 = zero_reg;
  }
  else if (!strcmp(opcode, "neg") || !strcmp(opcode, "negs") || !strcmp(opcode, "mvn") || !strcmp(opcode, "mov"))
  {
    if (!strcmp(opcode, "neg"))
    {
      opcode = sub;
    }
    else if (!strcmp(opcode, "negs"))
    {
      opcode = subs;
    }
    else if (!strcmp(opcode, "mvn"))
    {
      opcode = orn;
    }
    else
    {
      opcode = orr;
    }
    operand3 = operand2;
    operand2 = zero_reg;
  }
  else
  {
    if (!strcmp(opcode, "mul"))
    {
      opcode = madd;
    }
    else
    {
      opcode = msub;
    }
    operand4 = zero_reg;
  }
  data_process_asm();
}