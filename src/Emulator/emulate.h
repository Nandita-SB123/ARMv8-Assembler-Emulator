#ifndef EMULATE_H
#define EMULATE_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "help.h"
#include "imm_emu.h"
#include "ldrstr_emu.h"
#include "reg_emu.h"
#include "br_emu.h"
#include "decode.h"

// function to write the correct strings into the global variables
extern void display();

// function to write the output to the specified file
extern void write_to_file(char *filename);

// function to write the output to stdout
extern void write_to_stdout();

#endif