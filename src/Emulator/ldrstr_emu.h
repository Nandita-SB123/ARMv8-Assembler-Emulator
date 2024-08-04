#ifndef LDRSTR_EMU_H
#define LDRSTR_EMU_H

#include <stdint.h>
#include "help.h"

// implementation of functions for loadstore in emulator

extern uint32_t compute_addr(int sf, int u, int i, char imm12[], char Xn[], int simm9);
extern void load(uint32_t addr, int sf, char Rt[]);
extern uint32_t reg_offset_load(char Xn[], char Xm[]);
extern void store(uint32_t addr, int sf, char Rt[]);
extern uint32_t literal_load_only(char simm19[]);
#endif
