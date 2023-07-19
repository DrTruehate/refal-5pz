#define DEFINE_EXTERNALS 1

#include <stdlib.h>
#include "rasl.h"
#include "cdecl.h"


static void rc_rsee1(void);

int main(int argc, char **argv)
{
  if(argc < 2) {
    printf("Usage: see1 <filename>\n");
    exit(EXIT_FAILURE);
  }

  fdtmpr = fopen(argv[1], "rb");
  if(fdtmpr == NULL) {
    printf("Can't open %s\n", argv[1]);
    exit(EXIT_FAILURE);
  };

  rc_rsee1();
  fclose(fdtmpr);
  printf("\n\n That's it, folks\n");
  exit(EXIT_SUCCESS);
}


#define NUM_OF_RASL_INSTR 55

char *mnemonics[NUM_OF_RASL_INSTR] = {
  "ACT_EXTRN", "ACT1", "BL", "BLR", "BR", "CL", "SYM", "SYMR",
  "EMP", "EST", "MULE", "MULS", "PLEN", "PLENS", "PLENP", "PS",
  "PSR", "OEXP", "OEXPR", "OVSYM", "OVSYMR", "TERM", "TERMR", "RDY",
  "SETB", "LEN", "LENS", "LENP", "LENOS", "SYMS", "SYMSR", "TEXT",
  "NS", "TPLE", "TPLS", "TRAN", "VSYM", "VSYMR", "OUTEST", "ECOND",
  "POPVF", "PUSHVF", "STLEN", "CSYM", "CSYMR", "NSYM", "NSYMR", "NCS",
  "NNS", "LBL", "L", "E", "LABEL", "BUILT_IN1", "Special Mark B"
};

int rasl_numbers[NUM_OF_RASL_INSTR] = {
  ACT_EXTRN, ACT1, BL, BLR, BR, CL, SYM, SYMR,
  EMP, EST, MULE, MULS, PLEN, PLENS, PLENP, PS,
  PSR, OEXP, OEXPR, OVSYM, OVSYMR, TERM, TERMR, RDY,
  SETB, LEN, LENS, LENP, LENOS, SYMS, SYMSR, TEXT,
  NS, TPLE, TPLS, TRAN, VSYM, VSYMR, OUTEST, ECOND,
  POPVF, PUSHVF, STLEN, CSYM, CSYMR, NSYM, NSYMR, NCS,
  NNS, LBL, L, E, LABEL, BUILT_IN1, B
};


static char* rasl_code(int id)
{
  static char unknown[10];

  for(int i = 0; i < NUM_OF_RASL_INSTR; i++
  ) if(id == rasl_numbers[i]) return mnemonics[i];

  sprintf(unknown, "RASL-%d", id);
  return unknown;
}


/* Copies from fdtmpr to fdtmpw file replacing the
    references for their offsets. */
static void rc_rsee1(void)
{
  unsigned char opcode, c, d;
  char lname[MAXWS];
  int i;
  long k, n, z=0;

  while(read_byte(opcode) == 1) {
    printf( "%4ld: %s ", z, rasl_code(opcode) );

    switch(opcode) {
      // This RASL instruction takes an address of a function as an argument.
      case ACT1:
        for(i = 0; i < MAXWS; i++) { read_byte(c); lname[i] = c; }
        printf(" Function: %s\n", lname);
        z += MAXWS+1;
        break;
      // These RASL operators take a compound symbol as an argument.
      case CSYM: case CSYMR: case NCS:
      // These RASL operators require a (long) number as a parameter
      case NSYM: case NSYMR: case NNS: case BUILT_IN:
        read_long(k);
        printf(" Long Number %ld", k);
        z += 5;
        break;
      // Builtin function call with an argument
      case BUILT_IN1:
        read_long(n); //** First one is zero. **
        read_long(k);
        printf(" Long Numbers: %ld %ld\n", n, k);
        z += 9;
        break;
      // No arguments for these RASL operators.
      case BL: case BLR: case BR: case CL: case EMP: case EST: case PLEN:
      case PLENS: case PLENP: case PS: case PSR: case TERM: case TERMR:
      case LEN: case LENP: case VSYM: case VSYMR: case OUTEST:
      case POPVF: case PUSHVF: case STLEN:
        z++;
        break;
      // These RASL operators require a byte (character) as parameter.
      case SYM: case SYMR: case LENS: case NS: case MULE: case MULS:
      case OEXP: case OEXPR: case OVSYM: case OVSYMR: case RDY:
      case LENOS: case TPLE: case TPLS:
        read_byte(c);
        printf(" TE pointer %d", c);
        z += 2;
        break;
      // This RASL operator require two operands of size 1 byte.
      case SETB:
        read_byte(c);
        read_byte(d);
        printf(" TE pointers %d %d", c, d);
        z += 3;
        break;
      // These RASL operators take 1 byte and a variable number of bytes
      case SYMS: case SYMSR: case TEXT: // as parameters.
        read_byte(d);
        printf(" Takes %d arguments:\n", d);
        for(i = 0; i < d; i++) {
          read_byte(c);
          printf("\t\tCharacter %d %c\n", c, c);
        }
        z += d+2;
        break;
      // These RASL operators take as argument a label of form FUNNAME$NUMBER,
      //  where FUNNAME is the current function name, and NUMBER is a number.
      case TRAN: case ECOND:
        for(i = 0; i < MAXWS; i++) { read_byte(c); lname[i] = c; }
        printf(" Label: %s", lname);
        z += MAXWS+1;
        break;
      // These RASL operators define a label of form FUNNAME$NUMBER,
      //  where FUNNAME is the current function name, and NUMBER is a number.
      case LBL: case LABEL: case L: case E:
        for(i = 0; i < MAXWS; i++) { read_byte(c); lname[i] = c; }
        printf(" Function defined: %s", lname);
        z += MAXWS+1;
        break;
      default:
        fprintf(stderr, "PASS2 z = %ld,   %d: Strange Opcode\n", z, opcode);
        exit(EXIT_FAILURE);
    } // end of switch(opcode)

    printf("\n");
  } // end of while()

  printf("Size of the file is %ld bytes.\n", z);
}
