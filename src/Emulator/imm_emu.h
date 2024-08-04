#ifndef IMM_EMU_H
#define IMM_EMU_H

#include <stdint.h>
#include "help.h"

// implementation of functions for immediate in emulator

extern int64_t add(int sf, char rd[], char rn[], int op2);
extern int64_t sub(int sf, char rd[], char rn[], int op2);
extern void add_wf(int sf, char rd[], char rn[], int op2);
extern void sub_wf(int sf, char rd[], char rn[], int op2);
extern void wide_move(int sf, char hw[], char imm16[], char rd[], char opc[]);

#endif