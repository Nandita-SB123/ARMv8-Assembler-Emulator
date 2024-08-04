#ifndef ASSEMBLE_H
#define ASSEMBLE_H

#include <string.h>

#include <stdlib.h>

#include <unistd.h>

#include <stdint.h>

#include <math.h>

#include <stdio.h>

#include "helper.h"

#include "parse.h"

#include "imm_asm.h"

#include "ldrstr_asm.h"

#include "br_asm.h"

// function to write the instruction into the binary file
extern int write_to_bin_file(char *filename);

#endif