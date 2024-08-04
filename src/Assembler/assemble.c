#include "assemble.h"

int displaying_index = 0;
char *all_instructions[MAX_STRINGS] = {
    NULL};

int write_to_bin_file(char *filename)
{

  // Open the file in binary write mode ("wb" mode)
  FILE *file = fopen(filename, "wb");
  if (file == NULL)
  {
    perror("Error opening file");
    return EXIT_FAILURE;
  }

  for (int i = 0; i < displaying_index; i++)
  {
    size_t elementsWritten = fwrite(&displaying_array[i], sizeof(int), 1, file);
    if (elementsWritten != 1)
    {
      perror("Error writing to file");
      fclose(file);
      return EXIT_FAILURE;
    }
  }

  // Close the file
  fclose(file);

  printf("Number written to the file successfully.\n");

  return EXIT_SUCCESS;
}

int main(int argc, char **argv)
{
  parse_file_into_instructions(argv[1]);
  write_to_bin_file(argv[2]);
  return 0;
}