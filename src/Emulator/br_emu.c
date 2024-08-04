#include "br_emu.h"
// Branch instructions

void unconditional_branch_instr(char simm26[])
{
  uint64_t offset = bin_to_int(simm26) * 4;
  regs.PC = regs.PC + offset;
}

void register_branch_instr(char xn[])
{
  int register_number = bin_to_int(xn);
  regs.PC = regs.gen_reg[register_number];
}

void cond_branch_instr(char simm19[], char cond[])
{
  printf("IN COND BRANCH\n");
  printf("flags are:\n");
  printf("Z: %d\n", pstate.Z);
  printf("N: %d\n", pstate.N);
  printf("C: %d\n", pstate.C);
  printf("V: %d\n", pstate.V);
  printf("COND: %s\n", cond);

  // EQ
  if (strcmp(cond, "0000") == 0)
  {
    if (pstate.Z == 1)
    {
      printf("z flag is set yes\n");
      unconditional_branch_instr(simm19);
      printf("PC is now: %ld\n", regs.PC);
    }
    else
    {
      regs.PC += 4;
    }
  }

  // NE
  if (strcmp(cond, "0001") == 0)
  {
    if (pstate.Z == 0)
    {
      unconditional_branch_instr(simm19);
    }
    else
    {
      regs.PC += 4;
    }
  }

  // GE
  if (strcmp(cond, "1010") == 0)
  {
    if (pstate.N == pstate.V)
    {
      unconditional_branch_instr(simm19);
    }
  }

  // LT
  if (strcmp(cond, "1011") == 0)
  {
    if (pstate.N != pstate.V)
    {
      unconditional_branch_instr(simm19);
    }
  }

  // GT
  if (strcmp(cond, "1100") == 0)
  {
    if (pstate.Z == 0 && pstate.N == pstate.V)
    {
      unconditional_branch_instr(simm19);
    }
  }

  // LE
  if (strcmp(cond, "1101") == 0)
  {
    if (!(pstate.Z == 0 && pstate.N == pstate.V))
    {
      unconditional_branch_instr(simm19);
    }
  }

  // AL
  if (strcmp(cond, "1110") == 0)
  {
    unconditional_branch_instr(simm19);
  }
}
