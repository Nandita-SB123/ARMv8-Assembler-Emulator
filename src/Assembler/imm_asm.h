#ifndef IMM_ASM
#define IMM_ASM

#include <stdint.h>
#include <string.h>
#include "helper.h"

extern void data_process_asm(void);
extern void imm_arith(void);
extern void imm_widemove(void);
extern void reg_mult(void);
extern void reg_arith(void);
extern void aliases(void);
extern void reg_logical(void);

#endif
