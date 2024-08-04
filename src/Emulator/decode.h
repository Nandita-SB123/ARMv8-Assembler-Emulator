#ifndef DECODE_H
#define DECODE_H

#include "help.h"
#include "imm_emu.h"
#include "reg_emu.h"
#include "br_emu.h"
#include "ldrstr_emu.h"

#include <stdint.h>
#include <string.h>

// implementation of decode for immediate data transfer
extern void decode_imm(char instr[]);

// implementation of decode for register data transfer
extern void decode_reg(char instr[]);

// implementation of decode for loadstore data transfer
extern void decode_loadstore(char instr[]);

// implementation of decode for branch data transfer
extern void decode_branch(char instr[]);

// calls the appropriate decode function based on the instruction
extern void decode(char instr[]);

#endif