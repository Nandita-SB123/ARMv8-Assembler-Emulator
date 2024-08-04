#ifndef HELPER_H
#define HELPER_H

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#define ASCII_0 48
#define NO_OF_BITS 32
#define MAX_STRINGS (((uint64_t)1) << 21)
#define LABEL_LENGTH 50
#define MAX_LABELS 50
#define SIZE (((uint64_t)1) << 21)
#define REG_BITS 5

extern char *label;
extern char *opcode;
extern char *operand1;
extern char *operand2;
extern char *operand3;
extern char *operand4;
extern char *operand5;
extern char *subs;
extern char *ands;
extern char *adds;
extern char *madd;
extern char *msub;
extern char *sub;
extern char *orn;
extern char *orr;
extern char *zero_reg;
extern int displaying_index;
extern char *all_instructions[MAX_STRINGS];
extern int displaying_array[SIZE];

// struct that represents each key-value pair in the symbol table hash map
typedef struct
{
  char key_label[LABEL_LENGTH];
  int value_memA;
} ST;

// the symbol table hash map
extern ST symbol_table[MAX_LABELS];
extern void initialize_zero_reg(void);
extern void reg_to_bin(char *str, char *bin);
extern void imm_to_bin(char *val, char *bin, int len);
extern uint64_t bin_to_int(const char *str);
extern int64_t decimalToBinary(int num);

#endif