#include "ldrstr_asm.h"

static void pre_indexed(char *);

void ldrstr_asm(int instr_addr)
{
  char instrs[NO_OF_BITS + 1];
  memset(instrs, '0', NO_OF_BITS);
  instrs[NO_OF_BITS] = '\0';

  // sf
  if (operand1[0] == 'x')
  {
    instrs[1] = '1';
  }
  else
  {
    instrs[1] = '0';
  }
  // rt set
  char reg_bin[6];
  reg_to_bin(operand1, reg_bin);
  strncpy(instrs + 27, reg_bin, 5);
  // differentiating between load literal and the rest

  if (!strcmp("ldr", opcode) && (operand3 == NULL) && (operand2[0] != '['))
  {
    // load literal
    instrs[0] = '0';
    strncpy(instrs + 2, "011000", 6);
    char simm19[20];

    // to find the instruction address without labels.
    int label_count = 0;
    for (int i = 0; i < instr_addr; i++)
    {
      if (strstr(all_instructions[i], ":") != NULL)
      {
        label_count++;
      }
    }

    instr_addr -= label_count;

    // to detect if its a label
    for (int i = 0; i < MAX_LABELS; i++)
    {
      if (!strncmp(symbol_table[i].key_label, operand2, strlen(operand2)))
      {

        char *directive = all_instructions[symbol_table[i].value_memA];

        int og = 0; // this is what I need to check.

        for (int i = 0; i < MAX_STRINGS; i++)
        {

          if (strstr(all_instructions[i], ":") != NULL)
          {
            continue;
          }
          if (!strcmp(all_instructions[i], directive))
          {
            break;
          }
          og += 1;
        }

        char offset[20];

        sprintf(offset, "#%d\n", og - instr_addr);

        imm_to_bin(offset, simm19, 19);
        strncpy(instrs + 8, simm19, 19);
        displaying_array[displaying_index] = bin_to_int(instrs);
        return;
      }
    }

    // if it is not label
    char offset[strlen(operand2) + 2];
    if (operand2[0] != '#')
    {
      offset[0] = '#';
      strcpy(offset + 1, operand2);
    }
    else
    {
      strcpy(offset, operand2);
    }
    imm_to_bin(offset, simm19, 19);
    strncpy(instrs + 8, simm19, 19);
    return;
  }
  else
  {
    instrs[0] = '1';
    strncpy(instrs + 2, "11100", 5);
    instrs[8] = '0';
    // setting L
    if (!strcmp("ldr", opcode))
    {
      instrs[9] = '1';
    }
    else
    {
      instrs[9] = '0';
    }

    // storing Xn
    char *xn = operand2 + 1;
    reg_to_bin(xn, reg_bin);
    strncpy(instrs + 22, reg_bin, 5);
    if (operand3 == NULL)
    {
      // 0 offset
      // setting U
      instrs[7] = '1';
    }
    else
    {
      if (operand3[strlen(operand3) - 1] != '!' &&
          operand3[strlen(operand3) - 1] != ']')
      {

        // post index
        // setting I
        instrs[20] = '0';
        // setting other bits
        instrs[21] = '1';
        instrs[10] = '0';
        // setting simm9 using instr_toks[3]
        char simm9[10];
        imm_to_bin(operand3, simm9, 9);
        strncpy(instrs + 11, simm9, 9);
      }
      else if (operand3[strlen(operand3) - 1] == '!')
      {
        // pre indexed
        pre_indexed(instrs);
      }
      else if (operand3[0] == '#')
      {

        // unsigned offset
        // setting U
        instrs[7] = '1';
        // setting imm12
        char imm12[13];
        int operand;
        if (!strncmp(operand3 + 1, "0x", 2))
        {
          operand = (int)strtol(operand3 + 1, NULL, 16);
        }
        else
        {
          operand = atoi(operand3 + 1);
        }
        int imm_val = instrs[1] == '1' ? operand / 8 : operand / 4;
        char imm_calced[13];
        imm_calced[0] = '#';
        sprintf(imm_calced + 1, "%d", imm_val);
        imm_to_bin(imm_calced, imm12, 12);
        strncpy(instrs + 10, imm12, 12);
      }
      else
      {

        // register
        strncpy(instrs + 16, "011010", 6);
        instrs[10] = '1';
        // setting xm
        reg_to_bin(operand3, reg_bin);
        strncpy(instrs + 11, reg_bin, 5);
      }
    }
  }
  displaying_array[displaying_index] = bin_to_int(instrs);
}

void directive_asm()
{
  char instr[NO_OF_BITS + 1];
  char val[NO_OF_BITS + 1]; // to convert to instruction.
  val[0] = '#';
  strcpy(val + 1, operand1);
  imm_to_bin(val, instr, NO_OF_BITS);
  displaying_array[displaying_index] = bin_to_int(instr);
}

static void pre_indexed(char *instrs)
{
  // setting I
  instrs[20] = '1';
  // setting other bits
  instrs[21] = '1';
  instrs[10] = '0';
  // setting simm9 using instr_toks[3]
  char simm9[10];
  imm_to_bin(operand3, simm9, 9);
  strncpy(instrs + 11, simm9, 9);
}