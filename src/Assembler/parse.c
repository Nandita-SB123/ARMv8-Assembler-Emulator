#include "parse.h"

#include <ctype.h>

int operand_index;

// tokenises the given line by splitting it apart into opcode, operand label etc.
void tokenise(char *line)
{
  char *rest = line;
  char *token;
  char *ptr;
  long num;

  label = NULL;
  opcode = NULL;
  operand1 = NULL;
  operand2 = NULL;
  operand3 = NULL;
  operand4 = NULL;
  operand5 = NULL;

  // Tokenize label (if exists)
  token = strtok_r(rest, " ", &rest);
  if ((token && token[strlen(token) - 1] == ':'))
  {
    label = token;
    token = strtok_r(NULL, " ", &rest);
  }

  // Opcode
  if (token)
  {
    opcode = token;
  }

  int operand_index = 1;

  // Operands
  while ((token = strtok_r(NULL, ", ", &rest)))
  {
    if (operand_index == 1)
    {
      operand1 = token;
      operand_index++;
    }
    else if (operand_index == 2)
    {
      operand2 = token;
      operand_index++;
    }
    else if (operand_index == 3)
    {
      operand3 = token;
      operand_index++;
    }
    else if (operand_index == 4)
    {
      operand4 = token;
      operand_index++;
    }
    else if (operand_index == 5)
    {
      operand5 = token;
      operand_index++;
    }
  }
}

void parse_file_into_instructions(const char *filename)
{
  FILE *in = fopen(filename, "r");
  printf("the file has been opened\n");

  if (in == NULL)
  {
    fprintf(stderr, "Can't open input file\n");
    exit(EXIT_FAILURE);
  }

  char *line = NULL;
  size_t len = 0;
  ssize_t nread;
  int i = 0;
  int skip_line = 0;
  int label_count = 0;

  while ((nread = getline(&line, &len, in)) != -1)
  {

    // the if statement check if the line is empty, if the line is empty
    // then we do not put that line inside the all_instructions (it is empty so it is of no use)

    if (strcmp(line, "\n") == 0)
    {
      continue;
    }

    line[strcspn(line, "\n")] = '\0';

    if (strchr(line, ':'))
    {
      if (isspace(line[strlen(line) - 1]))
        line[strlen(line) - 1] = '\0';
      // Store the label and its address
      strncpy(symbol_table[label_count].key_label, line, LABEL_LENGTH);
      symbol_table[label_count].value_memA = i + 1;
      label_count++;
    }

    all_instructions[i] = strdup(line);
    if (all_instructions[i] == NULL)
    {
      fprintf(stderr, "Memory allocation failed\n");
      exit(EXIT_FAILURE);
    }
    i++;
  }
  free(line);
  fclose(in);
  int j = 0;

  while (all_instructions[j] != NULL)
  {
    tokenise(all_instructions[j]);

    if (opcode == NULL)
    {
      j++;
      continue;
    }
    else if ((strcmp(opcode, "and") == 0) && (strcmp(operand1, "x0") == 0) && (strcmp(operand2, "x0") == 0) && (strcmp(operand3, "x0") == 0))
    {
      data_process_asm();
    }
    else if ((strcmp(opcode, "b") == 0) || (strcmp(opcode, "br") == 0) || (strncmp(opcode, "b.", 2) == 0))
    {
      branch_asm(j);
    }
    else if (((strcmp(opcode, "ldr") == 0)) || ((strcmp(opcode, "str") == 0)))
    {
      ldrstr_asm(j);
    }
    else if (!strcmp(opcode, "cmp") ||
             !strcmp(opcode, "cmn") ||
             !strncmp(opcode, "neg", 3) ||
             !strcmp(opcode, "tst") ||
             !strcmp(opcode, "mvn") ||
             !strcmp(opcode, "mov") ||
             !strcmp(opcode, "mul") ||
             !strcmp(opcode, "mneg"))
    {
      aliases();
    }
    else if (!strcmp(opcode, ".int"))
    {
      directive_asm();
    }
    else
    {
      data_process_asm();
    }
    j++;
    displaying_index++;
  }
}