#include "stdio.h"
#include "clib.h"

#include "instructions.h"

#define NULL ((void *)0)

insn_type insn_table[] = {
  // Arithmetic instructions
  { "add", "d,s,t", 0x0, 0x0, R_TYPE },
  { "addi", "d,s,i", 0x1, 0x0, I_TYPE },
  { "addu", "d,s,t", 0x0, 0x1, R_TYPE },
  { "addui", "d,s,i", 0x1, 0x1, I_TYPE },

  { "sub", "d,s,t", 0x0, 0x2, R_TYPE },
  { "subi", "d,s,i", 0x1, 0x2, I_TYPE },
  { "subu", "d,s,t", 0x0, 0x3, R_TYPE },
  { "subui", "d,s,i", 0x1, 0x3, I_TYPE },

  { "mult", "d,s,t", 0x0, 0x4, R_TYPE },
  { "multi", "d,s,i", 0x1, 0x4, I_TYPE },
  { "multu", "d,s,t", 0x0, 0x5, R_TYPE },
  { "multui", "d,s,i", 0x1, 0x5, I_TYPE },

  { "div", "d,s,t", 0x0, 0x6, R_TYPE },
  { "divi", "d,s,i", 0x1, 0x6, I_TYPE },
  { "divu", "d,s,t", 0x0, 0x7, R_TYPE },
  { "divui", "d,s,i", 0x1, 0x7, I_TYPE },

  { "rem", "d,s,t", 0x0, 0x8, R_TYPE },
  { "remi", "d,s,i", 0x1, 0x8, I_TYPE },
  { "remu", "d,s,t", 0x0, 0x9, R_TYPE },
  { "remui", "d,s,i", 0x1, 0x9, I_TYPE },

  { "lhi", "d,i", 0x3, 0xe, I_TYPE },
  { "la", "d,j", 0xc, 0x0, J_TYPE },

  // Bitwise instructions
  { "and", "d,s,t", 0x0, 0xb, R_TYPE },
  { "andi", "d,s,i", 0x1, 0xb, I_TYPE },

  { "or", "d,s,t", 0x0, 0xd, R_TYPE },
  { "ori", "d,s,i", 0x1, 0xd, I_TYPE },

  { "xor", "d,s,t", 0x0, 0xf, R_TYPE },
  { "xori", "d,s,i", 0x1, 0xf, I_TYPE },

  { "sll", "d,s,t", 0x0, 0xa, R_TYPE },
  { "slli", "d,s,i", 0x1, 0xa, I_TYPE },

  { "srl", "d,s,t", 0x0, 0xc, R_TYPE },
  { "srli", "d,s,i", 0x1, 0xc, I_TYPE },

  { "sra", "d,s,t", 0x0, 0xe, R_TYPE },
  { "srai", "d,s,i", 0x1, 0xe, I_TYPE },

  // Test instructions
  { "slt", "d,s,t", 0x2, 0x0, R_TYPE },
  { "slti", "d,s,i", 0x3, 0x0, I_TYPE },
  { "sltu", "d,s,t", 0x2, 0x1, R_TYPE },
  { "sltui", "d,s,i", 0x3, 0x1, I_TYPE },

  { "sgt", "d,s,t", 0x2, 0x2, R_TYPE },
  { "sgti", "d,s,i", 0x3, 0x2, I_TYPE },
  { "sgtu", "d,s,t", 0x2, 0x3, R_TYPE },
  { "sgtui", "d,s,i", 0x3, 0x3, I_TYPE },

  { "sle", "d,s,t", 0x2, 0x4, R_TYPE },
  { "slei", "d,s,i", 0x3, 0x4, I_TYPE },
  { "sleu", "d,s,t", 0x2, 0x5, R_TYPE },
  { "sleui", "d,s,i", 0x3, 0x5, I_TYPE },

  { "sge", "d,s,t", 0x2, 0x6, R_TYPE },
  { "sgei", "d,s,i", 0x3, 0x6, I_TYPE },
  { "sgeu", "d,s,t", 0x2, 0x7, R_TYPE },
  { "sgeui", "d,s,i", 0x3, 0x7, I_TYPE },

  { "seq", "d,s,t", 0x2, 0x8, R_TYPE },
  { "seqi", "d,s,i", 0x3, 0x8, I_TYPE },
  { "sequ", "d,s,t", 0x2, 0x9, R_TYPE },
  { "sequi", "d,s,i", 0x3, 0x9, I_TYPE },

  { "sne", "d,s,t", 0x2, 0xa, R_TYPE },
  { "snei", "d,s,i", 0x3, 0xa, I_TYPE },
  { "sneu", "d,s,t", 0x2, 0xb, R_TYPE },
  { "sneui", "d,s,i", 0x3, 0xb, I_TYPE },
  
  // Branch instructions
  { "j", "j", 0x4, 0x0, J_TYPE },
  { "jr", "s", 0x5, 0x0, J_TYPE },
  { "jal", "j", 0x6, 0x0, J_TYPE },
  { "jalr", "s", 0x7, 0x0, J_TYPE },
  { "beqz", "s,b", 0xa, 0x0, J_TYPE },
  { "bnez", "s,b", 0xb, 0x0, J_TYPE },

  // Memory instructions
  { "lw", "d,o(s)", 0x8, 0x0, J_TYPE },
  { "sw", "d,o(s)", 0x9, 0x0, J_TYPE },

  // Special instructions
  { "movgs", "D,s", 0x3, 0xc, I_TYPE },
  { "movsg", "d,S", 0x3, 0xd, I_TYPE },
  { "break", "", 0x2, 0xc, I_TYPE },
  { "syscall", "", 0x2, 0xd, I_TYPE },
  { "rfe", "", 0x2, 0xe, I_TYPE },

  { ".word", NULL, 0xfff, 0xfff, DIRECTIVE },
  { ".ascii", NULL, 0xfff, 0xfff, DIRECTIVE },
  { ".asciiz", NULL, 0xfff, 0xfff, DIRECTIVE },
  { ".space", NULL, 0xfff, 0xfff, DIRECTIVE },
  { ".equ", NULL, 0xfff, 0xfff, DIRECTIVE },
  { ".global", NULL, 0xfff, 0xfff, DIRECTIVE },
  { ".data", NULL, 0xfff, 0xfff, DIRECTIVE },
  { ".text", NULL, 0xfff, 0xfff, DIRECTIVE },
  { ".bss", NULL, 0xfff, 0xfff, DIRECTIVE },
  { ".frame", NULL, 0xfff, 0xfff, DIRECTIVE },
  { ".mask", NULL, 0xfff, 0xfff, DIRECTIVE },
  { NULL, NULL, 0, 0, OTHER }
};

// GPR table
char *GPR_name[] = {
  "$0", "$1", "$2", "$3",
  "$4", "$5", "$6", "$7",
  "$8", "$9", "$10", "$11",
  "$12", "$13", "$sp", "$ra" };

// SPR table
char *SPR_name[] = {
  "$spr0", "$spr1", "$spr2", "$spr3",
  "$cctrl", "$estat", "$icount", "$ccount",
  "$evec", "$ear", "$esp", "$ers",
  "$spr12", "$spr13", "$spr14", "$spr15" };
/*
void disassemble(unsigned int insn_address, unsigned int instruction)
{
  // First we scan through to find the mnemonic
  unsigned int OPCode = (instruction >> 28) & 0xf;
  unsigned int func = (instruction >> 16) & 0xf;
  unsigned int Rd = (instruction >> 24) & 0xf;
  unsigned int Rs = (instruction >> 20) & 0xf;
  unsigned int Rt = (instruction & 0xf);
  unsigned int immediate = instruction & 0xffff;
  //  int signed_immediate = ((immediate & 0x8000) ? (0xffff0000 | immediate) : immediate);
  unsigned int address = instruction & 0xfffff;
  int signed_address = ((address & 0x80000) ? (0xfff00000 | address) : address);


  int insn_num = 0, i;

  // Here we look up the mnemonic in our table.
  while (insn_table[insn_num].mnemonic != NULL) {
    if (insn_table[insn_num].OPCode == OPCode
	&& (insn_table[insn_num].type == J_TYPE || insn_table[insn_num].func == func))
      break; // We have a match
    insn_num++;
  }

  // If we couldn't match an instruction
  if (insn_table[insn_num].mnemonic == NULL) {
    printf("???");
    return;
  }


  // Output the mnemonic
  printf("%s\t", insn_table[insn_num].mnemonic);

  // Now the parameters
  

  // Scan through the operand format string
  for (i = 0 ; (unsigned int)i < strlen(insn_table[insn_num].operands) ; i++) {
    if (insn_table[insn_num].operands[i] == 'd') {
      printf("%s", GPR_name[Rd]);
    }
    if (insn_table[insn_num].operands[i] == 's') {
      printf("%s", GPR_name[Rs]);
    }
    if (insn_table[insn_num].operands[i] == 'D') {
      printf("%s", SPR_name[Rd]);
    }
    if (insn_table[insn_num].operands[i] == 'S') {
      printf("%s", SPR_name[Rs]);
    }
    if (insn_table[insn_num].operands[i] == 't') {
      printf("%s", GPR_name[Rt]);
    }
    if (insn_table[insn_num].operands[i] == 'o') { // Twenty bit offset
      if (address == 0)
	printf("0");
      else if (Rs != 0) {
	printf("%d", signed_address);
      }
      else
	printf("0x%05x", address);
    }
    if (insn_table[insn_num].operands[i] == 'b') {
      printf("0x%05x", (((unsigned)((signed int)insn_address + signed_address) & 0xfffff) + 1));
    }
    if (insn_table[insn_num].operands[i] == 'i') { // 16 bit immediate value
      // We should check if the instruction sign extends or not, and if it does then
      // We should print a signed integer
      printf("0x%04x", immediate);
    }
    if (insn_table[insn_num].operands[i] == 'j') {
      printf("0x%05x", address);
    }

    printf("%s", insn_table[insn_num].operands[i]);
  }
}
*/
