#ifndef BR_EMU_H
#define BR_EMU_H
#include "help.h"

#include <stdint.h>
#include <string.h>

// unconditional branch instructions
extern void unconditional_branch_instr(char simm26[]);

// register branch instructions
extern void register_branch_instr(char xn[]);

// conditional branch instructions
extern void cond_branch_instr(char simm19[], char cond[]);

#endif
