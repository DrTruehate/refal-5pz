#define DEFINE_EXTERNALS 1

#include <stdlib.h>
#include "rasl.h"
#include "cdecl.h"


static void rc_rsee2(void);
static char* rasl_code(int);


int main(int argc, char **argv)
{
  if(argc < 2) {
    printf("Usage: see2a <filename>\n");
    exit(EXIT_FAILURE);
  }

  if( ( fdtmpr = fopen(argv[1], "rb") ) == NULL ) {
    printf("Can't open %s\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  rc_rsee2();
  printf("\n\n That's it, folks\n");
  exit(EXIT_SUCCESS);
}


/* RSEE2: Print the Refal Interpretation file 2. Feb. 21 1987. */
static void rc_rsee2(void)
{
  unsigned char opcode, c, d;
  char lname[MAXWS], title[MAXWS];
  int err, i, j;
  long n, k, xt, nt, cs, bt, z;

  // Read the title
  for(i = 0; i < MAXWS; i++) read_byte(title[i]);
  printf("TITLE %s\n", title);

  // Read the size
  read_long(n); read_long(nt); read_long(xt);
  read_long(cs); read_long(bt);
  printf( "The size of code is %ld\n"
          "size of entry table = %ld\n"
          "size of external table = %ld\n"
          "size of compound symbol table = %ld\n"
          "size of local table = %ld\n",
          n, nt, xt, cs, bt );

  // Get the ENTRY table.
  printf("\nThe ENTRY table.\n");
  for(i = 0; i < nt; i++) {
    for(j = 0; j < MAXWS; j++) { read_byte(c); lname[j] = c; }
    read_long(k); printf("%d: %s %ld\n", i, lname, k);
  }

  printf("\nThe EXTERNAL table.\n");
  for(i = 0; i < xt; i++) {
    for(j = 0; j < MAXWS; j++) { read_byte(c); lname[j] = c; }
    printf("%d: %s\n", i, lname);
  }

  printf("\nThe Compound Symbol table.\n");
  for(i = 0; i < cs; i++) {
    for(j = 0; j < MAXWS; j++) { read_byte(c); lname[j] = c; }
    printf("%d: %s\n", i, lname);
  }
  putchar('\n');

  printf("The Local table.\n");
  for(i = 0; i < bt; i++) { read_long(k); printf(" %ld\n", k); }
  err = z = 0;

  while(read_byte(opcode) == 1) {
    printf( "%4ld: %s ", z, rasl_code(opcode) );

    switch(opcode) {
      // This RASL instruction takes an address of a function as an argument.
      case ACT_EXTRN:
        printf(" External Function: ");
        for(i = 0; i < MAXWS; i++) { read_byte(c); if(c) putchar(c); }
        z += 5;
        break;
      case ACT1:
      read_long(k); printf(" Function offset %ld", k);
        z += 5;
        break;
      // These RASL operators take a compound symbol as an argument.
      case CSYM: case CSYMR: case NCS:
        read_long(k); printf(" Compound symbol %ld", k);
        z += 5;
        break;
      // These RASL operators require a (long) number as a parameter
      case NSYM: case NSYMR: case NNS:
        read_long(k); printf(" Long Number %ld", k);
        z += 5;
        break;
      // This RASL operator require a (long) number as a parameter
      case BUILT_IN:// Builtin function call
        read_long(k); printf(" Long Number %ld", k);
        z += 5;
        break;
      // This RASL operator require a (long) number as a parameter
      case BUILT_IN1: // Builtin function call
        read_long(k); printf(" Long Numbers %ld", k);
        read_long(k); printf(" and %ld", k);
        z += 9;
        break;
      // No arguments for these RASL operators.
      case BL: case BLR: case BR: case CL: case EMP: case EST: case PLEN:
      case PLENS: case PLENP: case PS: case PSR: case TERM: case TERMR:
      case LEN: case LENP: case VSYM: case VSYMR: case OUTEST: case POPVF:
      case PUSHVF: case STLEN:
        z++;
        break;
      // These RASL operators require a byte (character) as parameter.
      case SYM: case SYMR: case LENS: case NS:
        read_byte(c); printf(" Character %d %c", c, c);
        z += 2;
        break;
      // These RASL operators take one operand of size 1 byte.
      //case 12:// ELEN ???
      case MULE: case MULS: case OEXP: case OEXPR: case OVSYM: case OVSYMR:
      case RDY: case LENOS: case TPLE: case TPLS:
        read_byte(c); printf(" TE pointer %d", c);
        z += 2;
        break;

      // This RASL operator require two operands of size 1 byte.
      case SETB:
        read_byte(c); read_byte(d); printf(" TE pointers %d %d", c, d);
        z += 3;
        break;
      // These RASL operators take 1 byte and a variable number of bytes
      //  as parameters.
      case SYMS: case SYMSR: case TEXT:
        read_byte(d); printf(" Takes %d arguments:\n", d);
        for(i = 0; i < d; i++) {
          read_byte(c); printf("\t\tCharacter %d %c\n", c, c); }
        z += d+2;
        break;
      // These RASL operators take as argument a label of form FUNNAME$NUMBER,
      //  where FUNNAME is the current function name, and NUMBER is a number.
      case TRAN: case ECOND:
        read_long(k); printf(" Label offset %ld", k);
        z += 5;
        break;
      // These RASL operators define a label of form FUNNAME$NUMBER,
      //  where FUNNAME is the current function name, and NUMBER is a number.
      case LBL: case LABEL:
      case L: case E: // These operators define labels.
        printf(" Function defined: ");
        for(i = 0; i < MAXWS; i++) { read_byte(c); if(c) putchar(c); }
        z += MAXWS; printf(" Offset = %ld", z);
        break;
      default:
      printf(": Strange Opcode"); err++;
    } // end of switch(opcode)

    printf("\n");
  } // end of while()

  printf("size = %ld, z = %ld.\n""errors = %d\n", n, z, err);
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
