#include <stdlib.h>
#include "rasl.h"
#include "cdecl.h"

FILE *fdtmpr; // .TMP read file
static void rc_rsee2(FILE *fp_out);

int main(int argc, char *argv[])
{
  FILE *fp_out;

  if(argc < 2) {
    printf("Usage: see2 in_file [out_file]\n");
    exit(EXIT_FAILURE);
  }

  fdtmpr = fopen (argv[1], "rb");
  if(fdtmpr == NULL) {
    printf("Can't open %s\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  if(argc > 2) {
    fp_out = fopen(argv[2], "wt");
    if(fp_out == NULL) {
      printf("unable to open file %s\n", argv[2]);
      exit(EXIT_FAILURE);
    }
  }
  else fp_out = stdout;

  rc_rsee2(fp_out);
  fprintf(fp_out, "\n\n That's it, folks\n");
  if(fp_out != stdout) fclose(fp_out);
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

/* RSEE2: Print the Refal Interpretation file 2. Feb. 21 1987. */
static void rc_rsee2(FILE *fp_out)
{
  unsigned char opcode, c, d;
  char lname[MAXWS], title[MAXWS];
  int err, i, j;
  long n, k, k1, xt, nt, cs, bt, z;

  // Read the title
  for (i = 0; i < MAXWS; i++) {
    read_byte(title[i]);
    if(title [i] == 0) break;
  }
  fprintf(fp_out, "TITLE %s\n", title);

  // Read the size
  read_long(n); read_long(nt); read_long(xt); read_long(cs); read_long(bt);
  fprintf(fp_out, "The size of code is %ld\n"
                  "size of entry table = %ld\n"
                  "size of external table = %ld\n"
                  "size of compound symbol table = %ld\n"
                  "size of local table = %ld\n", n, nt, xt, cs, bt);

  // Get the ENTRY table.
  fprintf(fp_out, "\nThe ENTRY table.\n");
  for(i = 0; i < nt; i++) {
    for(j = 0; j < MAXWS; j++) {
      read_byte(c); lname[j] = c;
      if(c == 0) break;
    }
    read_long(k);
    fprintf(fp_out, "%d: %s %ld\n", i, lname, k);
  }

  fprintf(fp_out, "\nThe EXTERNAL table.\n");
  for(i = 0; i < xt; i++) {
    for(j = 0; j < MAXWS; j++) {
      read_byte(c); lname[j] = c;
      if(c == 0) break;
    }
    fprintf(fp_out, "%d: %s\n", i, lname);
  }

  fprintf(fp_out, "\nThe Compound Symbol table.\n");
  for(i = 0; i < cs; i++) {
    for(j = 0; j < MAXWS; j++) {
      read_byte(c); lname[j] = c;
      if(c == 0) break;
    }
    fprintf(fp_out, "%ld: %s\n", cs-i-1, lname);
  }
  fprintf(fp_out, "\n");

  fprintf(fp_out, "The Local table.\n");
  for(i = 0; i < bt; i++) { read_long(k); fprintf(fp_out, " %ld\n", k); }
  err = z = 0;

  while(read_byte(opcode) == 1) {
    fprintf( fp_out, "%4ld: %s ", z, rasl_code(opcode) );

    switch(opcode) {
      // This RASL instruction takes an address of a function as an argument.
      case ACT_EXTRN:
        fprintf(fp_out, " External Function: ");
        for(i = 0; i < MAXWS; i++) {
          read_byte(c);
          if(c) putc(c, fp_out);
          else break;
        }
        z += 5;
        break;
      case ACT1:
        read_long(k);
        fprintf(fp_out, " Function offset %ld", k);
        z += 5;
        break;
      // These RASL operators take a compound symbol as an argument.
      case CSYM: case CSYMR: case NCS:
        read_long(k);
        fprintf(fp_out, " Compound symbol %ld", k);
        z += sizeof(long)+1;
        break;
      // These RASL operators require a (long) number as a parameter
      case NSYM: case NSYMR: case NNS:
        read_long(k);
        fprintf(fp_out, " Long Number %ld", k);
        z += 5;
        break;
      // This RASL operator require a (long) number as a parameter
      case BUILT_IN:// Builtin function call
        read_long(k);
        fprintf(fp_out, " Long Number %ld", k);
        z += 5;
        break;
      // This RASL operator require a (long) number as a parameter
      case BUILT_IN1: // Builtin function call
        read_long(k); fprintf(fp_out, " Long Numbers %ld", k);
        read_long(k); fprintf(fp_out, " and %ld", k);
        z += 9;
        break;
      // No arguments for these RASL operators.
      case BL: case BLR: case BR: case CL: case EMP: case EST: case PLEN:
      case PLENS: case PLENP: case PS: case PSR: case TERM: case TERMR:
      case LEN: case LENP: case VSYM: case VSYMR: case OUTEST: case POPVF:
      case PUSHVF: case STLEN: z++; break;
      // These RASL operators require a byte (character) as parameter.
      case SYM: case SYMR: case LENS: case NS:
        read_byte(c);
        fprintf(fp_out, " Character %d %c", c, c);
        z += 2;
        break;
      // These RASL operators take one operand of size 1 byte.
      //case 12: // ELEN ???
      case OEXP: case OEXPR: case OVSYM: case OVSYMR: case RDY: case LENOS:
        read_byte(c);
        fprintf(fp_out, " TE pointer %d", c);
        z += 2;
        break;

      case MULE: case MULS: case TPLE: case TPLS:
        read_long(k);
        fprintf(fp_out, "TE pointer %ld", k);
        z += 5;
        break;
      // This RASL operator require two operands of sizeof(long).
      case SETB:
        read_long(k1); read_long(k);
        fprintf(fp_out, " TE pointers %ld %ld", k1, k);
        z += 2*sizeof(long)+1;
        break;
      // These RASL operators take 1 byte and a variable number of bytes
      //  as parameters.
      case SYMS: case SYMSR: case TEXT:
        read_byte(d);
        fprintf(fp_out, " Takes %d arguments:\n", d);
        for(i = 0; i < d; i++) {
          read_byte(c);
          fprintf(fp_out, "\t\tCharacter %d %c\n", c, c);
        }
        z += d+2;
        break;
      // These RASL operators take as argument a label of form FUNNAME$NUMBER,
      //  where FUNNAME is the current function name, and NUMBER is a number.
      case TRAN: case ECOND:
        read_long(k);
        fprintf(fp_out, " Label offset %ld", k);
        z += 5;
        break;
      // These RASL operators define a label of form FUNNAME$NUMBER,
      //  where FUNNAME is the current function name, and NUMBER is a number.
      case LBL: case LABEL:
      // These operators define labels.
      case L: case E:
        fprintf(fp_out, " Function defined: ");
        read_byte(c);
        for(i = 0; i < MAXWS; i++) {
          read_byte(c);
          if(c) putc(c, fp_out);
          else break;
        }
        z += i+2;
        fprintf(fp_out, " Offset = %ld", z);
        break;
      default:
        fprintf(fp_out, ": Strange Opcode = %d", opcode);
        err++;
        break;
    } // end of switch(opcode)

    fprintf(fp_out, "\n");
  } // end of while()

  fprintf(fp_out, "size = %ld, z = %ld.\n""errors = %d\n", n, z, err);
}
