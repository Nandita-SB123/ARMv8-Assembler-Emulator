
#include "emulate.h"

char finalString[] = "";

char registers[10000] = "REGISTERS:\n";
char pccontents[33] = "PC = ";
char flags[30] = "PSTATE: ";
char nonZeroMem[10000] = "Non-zero Memory:\n";

// Global variable for storing current instr.
static char curr_instr[33] = "";

// Initializing the structs
static void initialize(void)
{
  for (int i = 0; i < NUM_REGS; i++)
  {
    regs.gen_reg[i] = 0;
  }

  regs.ZR = 0;
  regs.PC = 0;

  pstate.C = 0;
  pstate.N = 0;
  pstate.V = 0;
  pstate.Z = 1;

  // initializing all memory locations to 0
  for (int i = 0; i < MEM_SIZE; i++)
  {
    mem[i] = 0;
  }
}

int size;
char *all_instructions[MEM_SIZE];
int all_instructions_size;

// Function to fetch a 4-byte instruction from memory
void fetch(int address)
{
  strcpy(curr_instr, all_instructions[address]);
}

int main(int argc, char **argv)
{
  initialize();

  FILE *file_in = fopen(argv[1], "rb");
  if (file_in == NULL)
  {
    fprintf(stderr, "Can't open input file\n");
    exit(1);
  }

  fseek(file_in, 0, SEEK_END); // Move the file pointer to the end of the file
  size = ftell(file_in);
  rewind(file_in);

  int ch;
  int arr[size];
  int pos = 0;

  while ((ch = getc(file_in)) != EOF)
  {
    mem[pos] = ch;
    arr[pos++] = ch;
  }

  pos = 0;

  for (int j = 0; j < size; j += 4)
  {
    char current_instruction[33] = "";
    for (int i = 0; i < 4; i++)
    {
      char current_byte[9];
      sprintf(current_byte, "%8.8ld", decimalToBinary(arr[j + 3 - i]));
      strcat(current_instruction, current_byte);
    }
    all_instructions[pos] = malloc(33);
    strcpy(all_instructions[pos++], current_instruction);
    all_instructions_size++;
  }

  fclose(file_in);

  int i;

  for (i = 0; i < size / 4; i++)
  {
    if (regs.PC / 4 >= all_instructions_size)
    {
      break;
    }

    fetch(regs.PC / 4);

    if (strcmp(curr_instr, HALTING) == 0)
    {
      printf("termination condition achieved\n");
      break;
    }

    decode(curr_instr);
  }

  if (argc == 2)
  {
    display();
    write_to_stdout();
  }
  else if (argc == 3)
  {
    display();
    write_to_file(argv[2]);
  }

  return EXIT_SUCCESS;
}

void display()
{
  // registers
  for (int i = 0; i < NUM_REGS; i++)
  {
    char regNum[10] = "";
    sprintf(regNum, "X%2.2d = ", i);
    strcat(registers, regNum);
    char regValue[20] = "";
    sprintf(regValue, "%16.16lx\n", regs.gen_reg[i]);
    strcat(registers, regValue);
  }

  // PC
  char pcValue[20] = "";
  sprintf(pcValue, "%16.16lx\n", regs.PC);
  strcat(pccontents, pcValue);

  // PCSTATE
  if (pstate.N == 0)
  {
    strcat(flags, "-");
  }
  else
  {
    strcat(flags, "N");
  }

  if (pstate.Z == 0)
  {
    strcat(flags, "-");
  }
  else
  {
    strcat(flags, "Z");
  }

  if (pstate.C == 0)
  {
    strcat(flags, "-");
  }
  else
  {
    strcat(flags, "C");
  }

  if (pstate.V == 0)
  {
    strcat(flags, "-");
  }
  else
  {
    strcat(flags, "V");
  }
  strcat(flags, "\n");

  // Non-zero memory

  for (int i = 0; i < MEM_SIZE; i += 4)
  {
    char memAddress[30] = "";
    sprintf(memAddress, "0x%8.8x:  ", i);
    char memValue[9] = "";

    for (int j = 0; j < 4; j++)
    {
      char byteValue[20] = "";
      sprintf(byteValue, "%2.2x", mem[i + (3 - j)] & 0xff);
      strcat(memValue, byteValue);
    }
    if (!(strcmp(memValue, "00000000") == 0))
    {
      strcat(nonZeroMem, memAddress);
      strcat(nonZeroMem, memValue);
      strcat(nonZeroMem, "\n");
    }
  }
}

void write_to_stdout()
{

  printf("%s", registers);
  printf("%s", pccontents);
  printf("%s", flags);
  printf("%s", nonZeroMem);
}

void write_to_file(char *filename)
{

  FILE *file_out = fopen(filename, "w");

  if (file_out == NULL)
  {
    fprintf(stderr, "Can't open file\n");
    exit(1);
  }

  if (ferror(file_out))
  {
    fprintf(stderr, "Error occurred writing to output.txt\n");
    exit(1);
  }

  fprintf(file_out, "%s", registers);
  fprintf(file_out, "%s", pccontents);
  fprintf(file_out, "%s", flags);
  fprintf(file_out, "%s", nonZeroMem);

  fclose(file_out);
}
