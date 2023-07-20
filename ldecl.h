/* define loader macros.   */
#define write_byte_to_mem(BB) { *ldptr++ = (char)(BB); }
#ifndef FOR_OS_SUNLINUX
  #define write_long_to_mem(LL) \
    { *( (long *)ldptr) = (long)(LL); ldptr += sizeof(long); }
#else
/*** MAC, Sun (Linux) Versions ***/
  #define write_long_to_mem(LL) { \
    long TMP; TMP = (long)(LL); \
    memcpy( ldptr, (char *)&TMP, sizeof(long) ); \
    ldptr += sizeof (long);\
  }
#endif
#define read_byte_to_mem(X) fread( (char *)&X, sizeof(char), 1, fdtmpr)
#define read_long_to_mem(X) fread( (char *)&X, sizeof(long), 1, fdtmpr)

#ifndef MAXWS
  #define MAXWS 1024
#endif

/* Define the structure for holding module information. */
typedef struct {
  char *name, *addr;
} LOAD_TABLE;

typedef struct {
  char *name;
  long ident;
} COMPSYM_TABLE;

typedef struct module_descriptor {
  char module_name[FILENAME_MAX], *base;
  long size, cs_size, xt_size, np_size, lf_size;
  COMPSYM_TABLE *cs_table;
  LOAD_TABLE *ext_table, *entry_pts;
  char **local_table;
  struct module_descriptor *next;
  FILE *from;
} MODULE;

