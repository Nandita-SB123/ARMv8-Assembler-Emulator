
#include "decode.h"

void decode_imm(char instr[])
{
  char opi[4];
  char opc[3];
  int sf = (instr[0] == '0' ? 0 : 1);
  char rd[6];
  int sh = (instr[9] == '0' ? 0 : 1);
  char imm12[25];
  char rn[6];
  char hw[3];
  char imm16[17];

  strncpy(opi, instr + 6, 3);
  strncpy(opc, instr + 1, 2);
  strncpy(rd, instr + 27, 5);
  strncpy(imm12, instr + 10, 12);
  strncpy(rn, instr + 22, 5);
  strncpy(hw, instr + 9, 2);
  strncpy(imm16, instr + 11, 16);
  opi[3] = '\0';
  opc[2] = '\0';
  rd[5] = '\0';
  imm12[12] = '\0';
  rn[5] = '\0';
  hw[2] = '\0';
  imm16[16] = '\0';
  int opi_int = bin_to_int(opi);
  int opc_int = bin_to_int(opc);

  if (opi_int == 2)
  {

    int64_t shifted_imm = bin_to_int(imm12);
    if (sh == 1)
    {
      char str[65];
      int64_t res = lsl_int(12, shifted_imm);
      shifted_imm = res;
    }
    switch (opc_int)
    {
    case 0:
      add(sf, rd, rn, shifted_imm);
      break;
    case 1:
      add_wf(sf, rd, rn, shifted_imm);
      break;
    case 2:
      sub(sf, rd, rn, shifted_imm);
      break;
    case 3:
      sub_wf(sf, rd, rn, shifted_imm);
      break;
    }
  }
  else
  {
    wide_move(sf, hw, imm16, rd, opc);
  }
}

void decode_reg(char instr[])
{
  char opr[5];
  char opc[3];
  int sf = (instr[0] == '0' ? 0 : 1);
  char rd[6];
  int M = (instr[3] == '0' ? 0 : 1);
  char operand[7];
  char rn[6];
  char rm[6];

  char ra[6];
  char x = instr[16];

  strncpy(opr, instr + 7, 4);
  strncpy(opc, instr + 1, 2);
  strncpy(rd, instr + 27, 5);
  strncpy(operand, instr + 16, 6);
  strncpy(rn, instr + 22, 5);
  strncpy(rm, instr + 11, 5);
  strncpy(ra, instr + 17, 5);
  opr[4] = '\0';
  opc[2] = '\0';
  rd[5] = '\0';
  operand[6] = '\0';
  rn[5] = '\0';
  rm[5] = '\0';
  ra[5] = '\0';

  if (M == 1)
  {
    multiply_reg(sf, rm, x, ra, rn, rd);
  }
  else
  {
    if (opr[0] == '1')
    {
      arithmetic_reg(sf, opc, opr, rm, operand, rn, rd);
    }
    else
    {
      bitlogic_reg(sf, opc, opr, rm, operand, rn, rd);
    }
  }
}

void decode_loadstore(char instr[])
{
  int sf = (instr[1] == '0' ? 0 : 1);
  int U = (instr[7] == '0' ? 0 : 1);
  int L = (instr[9] == '0' ? 0 : 1);
  int I = (instr[20] == '0' ? 0 : 1);
  char offset[13];
  char xn[6];
  char rt[6];
  char simm19[20];
  char xm[6];
  char imm12[13];
  char simm9[10];

  strncpy(offset, instr + 10, 12);
  strncpy(xn, instr + 22, 5);
  strncpy(rt, instr + 27, 5);
  strncpy(simm19, instr + 8, 19);
  strncpy(xm, instr + 11, 5);
  strncpy(imm12, instr + 10, 12);
  strncpy(simm9, instr + 11, 9);

  offset[12] = '\0';
  xn[5] = '\0';
  rt[5] = '\0';
  simm19[19] = '\0';
  xm[5] = '\0';
  imm12[12] = '\0';
  simm9[9] = '\0';

  int32_t addr = 0;
  char chk_reg_offset[7];
  strncpy(chk_reg_offset, instr + 16, 6);
  chk_reg_offset[6] = '\0';
  if ((instr[10] == '1') && strcmp(chk_reg_offset, "011010") == 0)
  {
    addr = reg_offset_load(xn, xm);
  }
  else
  {
    addr = compute_addr(sf, U, I, imm12, xn, reverse_complement(simm9, strlen(simm9)));
  }

  // checking for load literal
  char chk_load_literal[7];
  strncpy(chk_load_literal, instr + 2, 6);
  chk_load_literal[6] = '\0';

  if (instr[0] == '0' && strcmp(chk_load_literal, "011000") == 0)
  {
    addr = literal_load_only(simm19);
    load(addr, sf, rt);
    return;
  }

  if (L == 1)
  {
    load(addr, sf, rt);
  }
  else
  {
    store(addr, sf, rt);
  }
}

void decode_branch(char instr[])
{
  char check_uncond[6];
  char check_reg1[23];
  char check_reg2[6];
  char check_cond1[9];
  char simm26[27];
  char simm19[20];
  char cond[5];
  char xn[6];

  strncpy(check_uncond, instr, 6);
  strncpy(check_reg1, instr, 22);
  strncpy(check_reg2, instr + 27, 6);
  strncpy(check_cond1, instr, 9);
  strncpy(simm19, instr + 8, 19);
  strncpy(simm26, instr + 6, 26);
  strncpy(cond, instr + 28, 4);
  strncpy(xn, instr + 22, 6);

  check_uncond[6] = '\0';
  check_reg1[22] = '\0';
  check_reg2[5] = '\0';
  check_cond1[8] = '\0';
  simm26[26] = '\0';
  simm19[19] = '\0';
  cond[4] = '\0';
  xn[5] = '\0';

  char xn_value[6];
  strncpy(xn_value, instr + 22, 5);
  xn[5] = '\0';

  if (strcmp(check_uncond, "000101") == 0)
  {
    unconditional_branch_instr(simm26);
  }

  if (strcmp(check_reg1, "1101011000011111000000") == 0 && strcmp((instr + 27), "00000") == 0)
  {
    register_branch_instr(xn);
  }

  if (strcmp(check_cond1, "01010100") == 0 && instr[27] == '0')
  {
    cond_branch_instr(simm19, cond);
  }
}

void decode(char instr[])
{
  char op0[5];
  strncpy(op0, instr + 3, 4);
  op0[4] = '\0';

  if (op0[0] == '1' && op0[1] == '0' && op0[2] == '0')
  {
    decode_imm(instr);
    regs.PC += 4;
  }
  else if (op0[0] == '1' && op0[1] == '0' && op0[2] == '1')
  {
    decode_branch(instr);
  }
  else if (op0[1] == '1' && op0[2] == '0' && op0[3] == '1')
  {
    decode_reg(instr);
    regs.PC += 4;
  }
  else if (op0[1] == '1' && op0[3] == '0')
  {
    decode_loadstore(instr);
    regs.PC += 4;
  }
}
