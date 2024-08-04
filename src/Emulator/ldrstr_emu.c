#include "ldrstr_emu.h" 
 
uint32_t compute_addr(int sf, int u, int i, char imm12[], char Xn[], int simm9)
{
  uint32_t transfer_address;
  if (u == 1)
  { // unsigned imm offset
    int uoffset;
    if (sf == 1)
    {
      uoffset = bin_to_int(imm12) * 8;
    }
    else
    {
      uoffset = bin_to_int(imm12) * 4;
    }
    transfer_address = regs.gen_reg[bin_to_int(Xn)] + (uint64_t) uoffset;
  }
  else
  {
    if (i == 1)
    {
      // address = Xn + simm9
      transfer_address = regs.gen_reg[bin_to_int(Xn)] + simm9;
      // Xn = Xn + simm9
      regs.gen_reg[bin_to_int(Xn)] = transfer_address;
    }
    else
    {
      transfer_address = regs.gen_reg[bin_to_int(Xn)];
      regs.gen_reg[bin_to_int(Xn)] = regs.gen_reg[bin_to_int(Xn)] + simm9;
    }
  }
  return transfer_address;
}

void load(uint32_t addr, int sf, char Rt[])
{
  int64_t word = 0;
  if (sf == 0)
  {
    uint32_t mem_addresses[4];
    for (uint32_t a = addr; a <= addr + 3; a++)
    {
      mem_addresses[a - addr] = mem[a];
    }
    uint8_t maskmem = pow(2, 8) - 1;

    for (int i = 3; i > 0; i--)
    {
      mem_addresses[i] = mem_addresses[i] & maskmem;
      word += mem_addresses[i];
      word = word << 8;
    }

    word = word + (mem_addresses[0] & maskmem);
  }
  else
  {
    uint32_t mem_addresses[8];
    for (uint32_t a = addr; a <= addr + 7; a++)
    {
      mem_addresses[a - addr] = mem[a];
    }

    uint8_t maskmem = pow(2, 8) - 1;

    for (int i = 7; i > 0; i--)
    {
      mem_addresses[i] = mem_addresses[i] & maskmem;
      word += mem_addresses[i];
      word = word << 8;
    }
    word = word + (mem_addresses[0] & maskmem);
  }

  regs.gen_reg[bin_to_int(Rt)] = word;
}

uint32_t reg_offset_load(char Xn[], char Xm[])
{
  return regs.gen_reg[bin_to_int(Xn)] + regs.gen_reg[bin_to_int(Xm)];
}

void store(uint32_t addr, int sf, char Rt[])
{
  if (sf == 0)
  {
    int8_t mem_values[4];
    uint64_t mask32 = pow(2, 32) - 1;
    int32_t lower_32rt = regs.gen_reg[bin_to_int(Rt)] & mask32;

    int64_t mask_counter = pow(2, 8) - 1;

    for (int i = 0; i < 4; i++)
    {
      mem_values[i] = (uint8_t)((((lower_32rt & mask_counter) >> (i * 8))) & 0b11111111);
      mask_counter = mask_counter << 8;
    }

    for (int i = 0; i < 4; i++)
    {
      mem[addr + i] = mem_values[i] & 0xff;
    }
  }
  else
  {
    int16_t mem_values[8];
    int64_t rt = regs.gen_reg[bin_to_int(Rt)];

    int64_t mask_counter = pow(2, 8) - 1;
    for (int i = 0; i < 8; i++)
    {
      // rt & mask_counter clears up rt to extract the byte required.
      mem_values[i] = (uint8_t)((((rt & mask_counter) >> (i * 8))) & 0b11111111);
      mask_counter = mask_counter << 8;
    }

    for (int i = 0; i < 8; i++)
    {
      mem[addr + i] = mem_values[i] & 0xff;
    }
  }
}

uint32_t literal_load_only(char simm19[])
{
  char complement[20];
  int64_t offset = bin_to_int(simm19);
  strcpy(complement, simm19);
  if(simm19[0] == '1') {
    int64_t minus1 = bin_to_int(simm19) - 1;
    memset(complement, '1', 19);
    complement[19] = '\0';
    int count = 18;
    while(minus1 != 0) {
      complement[count] = minus1 % 2 == 0? '1' : '0';
      minus1 /= 2;
      count--;
    }
    offset = -1 * bin_to_int(complement);
  }
  return regs.PC + (int64_t)(offset * 4);
}
