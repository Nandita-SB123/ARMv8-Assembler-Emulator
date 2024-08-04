#ifndef REG_EMU_H
#define REG_EMU_H

#include <stdint.h>
#include <string.h>
#include "help.h"

extern void arithmetic_reg(
    int sf, char opc[], char opr[],
    char rm[], char operand[], char rn[], char rd[]);
extern void bitlogic_reg(
    int sf, char opc[], char opr[],
    char rm[], char operand[], char rn[], char rd[]);
extern void multiply_reg(int sf, char rm[], char x, char ra[], char rn[], char rd[]);
extern uint64_t lsl(int shift_amount, char thing_shifted[]);
extern uint64_t lsr(int shift_amount, char thing_shifted[]);
extern uint64_t asr(int shift_amount, char thing_shifted[]);
extern uint64_t ror(int shift_amount, char thing_shifted[]);
extern int64_t lsl_int(int shift_amount, int64_t thing_shifted);
extern int64_t lsr_int(int shift_amount, int64_t thing_shifted);
extern int64_t rorw_int(int shift_amount, int64_t thing_shifted);
extern uint64_t rorx_int(int shift_amount, int64_t thing_shifted);
extern uint64_t asr_int(int shift_amount, int64_t thing_shifted, int sf);

#endif