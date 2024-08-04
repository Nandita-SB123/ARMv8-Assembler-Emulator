#ifndef HELP_H
#define HELP_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#define NUM_REGS 31
#define MAX_VALUE_SIZE 64
#define HALTING "10001010000000000000000000000000" // binary equivalent of 0x8a000000, the hex of halting.
#define MEMORY_SIZE 1024
#define MEM_SIZE (((uint64_t)1) << 21)
#define regW 0x00000000ffffffff
#define nflagW (((uint64_t)1) << 31)
#define nflagX (((uint64_t)1) << 63)
#define regX 0xffffffffffffffff
#define BYTE 8

extern char finalString[];
extern char registers[10000];
extern char pccontents[33];
extern char flags[30];
extern char nonZeroMem[10000];
extern int8_t mem[MEM_SIZE];
extern uint32_t memory[MEMORY_SIZE];

// Processor State
typedef struct
{
  int N;
  int Z;
  int C;
  int V;
} PSTATE;

// Special Purpose Registers
typedef struct
{
  int64_t gen_reg[NUM_REGS];
  uint64_t ZR;
  uint64_t PC;
} SPR;

extern PSTATE pstate;
extern SPR regs;

extern uint64_t bin_to_int(const char *str);

// helper function to reverse the twos complement of a function.
extern int64_t reverse_complement(char val[], int len);

// decimal to binary converting function
extern int64_t decimalToBinary(int num);

#endif
