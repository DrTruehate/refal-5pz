/* FILE   I/O.    */

#define FILE_LIMIT  40
#ifdef DEFINE_EXTERNALS
  FILE *file_table[FILE_LIMIT];
#else
  extern FILE *file_table[FILE_LIMIT];
#endif
