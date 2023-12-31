#include "bif_lex.h"

struct bitab bi[] =
  {
      /* These numbers are hard-coded in some places (e.g. in pass2.c)
          so change them carefully. */
    {"" ,  0, 0},
    {"Mu", 1, BI_FADDR},
    {"Add", 2, 0},
    {"Arg", 3, 0},
    {"Br", 4, 0},
    {"Card", 5, 0},
    {"Chr", 6, 0},
    {"Cp", 7, 0},
    {"Dg", 8, 0},
    {"Dgall", 9, 0},
    {"Div", 10, 0},
    {"Divmod", 11, 0},
    {"Explode", 12, 0},
    {"First", 13, 0},
    {"Get", 14, 0},
    {"Implode", 15, 0},
    {"Last", 16, 0},
    {"Lenw", 17, 0},
    {"Lower", 18, 0},
    {"Mod", 19, 0},
    {"Mul", 20, 0},
    {"Numb", 21, 0},
    {"Open", 22, 0},
    {"Ord", 23, 0},
    {"Print", 24, 0},
    {"Prout", 25, 0},
    {"Put", 26, 0},
    {"Putout", 27, 0},
    {"Rp", 28, 0},
    {"Step", 29, 0},
    {"Sub", 30, 0},
    {"Symb", 31, 0},
    {"Time", 32, 0},
    {"Type", 33, 0},
    {"Upper", 34,  0},
    {"Sysfun", 35, 0},
    {"Imp$$", 42, 0},
    {"Stop$$", 43, 0},
    {"", 44, 0},
      /* These numbers are hard-coded in some places (e.g. in pass2.c)
          so change them carefully. */
    {"Freeze", 45, 0},
    {"Freezer", 46, 0},
    {"Dn", 47, 0},
    {"Up", 48, BI_FADDR},
    {"Ev-met", 49, BI_FADDR},
    {"Residue", 50, BI_FADDR},
    {"GetEnv", 51, 0},
    {"System", 52, 0},
    {"Exit", 53, 0},
    {"Close", 54, 0},
    {"ExistFile", 55, 0},
    {"GetCurrentDirectory", 56, 0},
    {"RemoveFile", 57, 0},
    {"Implode_Ext", 58, 0},
    {"Explode_Ext", 59, 0},
    {"TimeElapsed", 60, 0},
    {"Compare", 61, 0},
    {"DeSysfun", 62, 0},
    {"XMLParse", 63, 0},
    {"Random", 64, 0},
    {"RandomDigit", 65, 0},
    {"Write", 66, 0},
    {"ListOfBuiltin", 67, 0},
    {"SizeOf", 68, 0},
    {"GetPID", 69, 0},

/*** This comments is needed for Graph- to C- code of transformation ***/
    {"int4fab_1", 70, 0},

    {"GetPPID", 71, 0}
  };

long nbi = (long) (sizeof(bi) / sizeof(struct bitab));
