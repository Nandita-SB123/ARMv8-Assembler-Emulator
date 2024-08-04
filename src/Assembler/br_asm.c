#include "br_asm.h"

#include <stdbool.h>

int num_labels(int start, int end)
{
  bool neg = false;
  int labels = 0;
  if (start > end)
  {
    int temp = end;
    end = start;
    start = temp;
    neg = true;
  }
  for (int i = start + 1; i < end; i++)
  {
    if (strstr(all_instructions[i], ":") != NULL)
    {
      labels++;
    }
  }

  if (neg)
  {
    labels *= (-1);
  }
  return labels;
}

void branch_asm(int instr_address)
{

  static char final_instruction[65];

  if (strcmp(opcode, "b") == 0)
  {
    strcpy(final_instruction, "\0");
    char label[10];
    sprintf(label, "%s: ", operand1);
    int offset;
    for (int i = 0; i < MAX_LABELS; i++)
    {
      if (strcmp(symbol_table[i].key_label, label) == 0 || strcmp(symbol_table[i].key_label, label) == -32)
      {

        offset = symbol_table[i].value_memA - instr_address;
      }
    }
    int no_labels = num_labels(instr_address, offset + instr_address);
    offset -= no_labels;

    char first_bits[] = "000101";

    char address_as_bin[10];
    sprintf(address_as_bin, "#%d", offset);

    char binary_str[27];
    imm_to_bin(address_as_bin, binary_str, 26);
    strcat(final_instruction, first_bits);
    strcat(final_instruction, binary_str);
  }
  else if (strcmp(opcode, "br") == 0)
  {

    char first_bits[] = "1101011000011111000000";
    int num = atoi(operand1 + 1);
    int string_to_cat = decimalToBinary(num);
    char as_a_string[100];
    sprintf(as_a_string, "%5.5d", string_to_cat);
    strcat(final_instruction, first_bits);
    strcat(final_instruction, as_a_string);
    strcat(final_instruction, "00000");
  }
  else if (strncmp(opcode, "b.", 2) == 0)
  {
    char label[10];
    sprintf(label, "%s:", operand1);
    int offset;
    for (int i = 0; i < MAX_LABELS; i++)
    {

      if (strcmp(symbol_table[i].key_label, label) == 0 || strcmp(symbol_table[i].key_label, label) == -32)
      {
        offset = symbol_table[i].value_memA - instr_address;
      }
    }

    int no_labels = num_labels(instr_address, offset + instr_address);
    offset -= no_labels;

    char first_bits[] = "01010100";
    char address_as_bin[10];
    sprintf(address_as_bin, "#%d", offset);

    char binary_str[20];
    imm_to_bin(address_as_bin, binary_str, 19);

    char cond[5];

    if (strcmp(opcode, "b.eq") == 0)
    {
      strcpy(cond, "0000");
    }
    else if (strcmp(opcode, "b.ne") == 0)
    {
      strcpy(cond, "0001");
    }
    else if (strcmp(opcode, "b.ge") == 0)
    {
      strcpy(cond, "1010");
    }
    else if (strcmp(opcode, "b.lt") == 0)
    {
      strcpy(cond, "1011");
    }
    else if (strcmp(opcode, "b.gt") == 0)
    {
      strcpy(cond, "1100");
    }
    else if (strcmp(opcode, "b.le") == 0)
    {
      strcpy(cond, "1101");
    }
    else if (strcmp(opcode, "b.al") == 0)
    {
      strcpy(cond, "1110");
    }

    strcat(final_instruction, first_bits);
    strcat(final_instruction, binary_str);
    strcat(final_instruction, "0");
    strcat(final_instruction, cond);
  }

  int as_decimal = bin_to_int(final_instruction);
  displaying_array[displaying_index] = as_decimal;
}