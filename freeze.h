/* macros for freezer variables. */

/* some macros which deal with levels of variables. */
#define MAX_VAR_INDEX   0x0000FFFFL // 2^16-1 actually: maximum index.
#define MAX_VAR_ELEV    0xFFL       // 2^8-1 (or 255): maximum elevation.
#define MAX_VAR_LEVEL   0xFFL       // 2^8-1 (or 255): maximum level.
/* encoding of index, elevation and level of a variable:
  lower 16 bits: index,
  middle 8 bits: elevation,
  high 8 bits: level. */
#define INDEX_MASK  0x0000FFFFL
#define ELEV_MASK   0x00FF0000L
#define LEVEL_MASK  0xFF000000L

#define index_of(var)     ( (unsigned long)( INDEX_MASK&(var) ) )
#define elevation_of(var) ( (unsigned long)( (ELEV_MASK&(var) )>>16) )
#define level_of(var)     ( (unsigned long)( (LEVEL_MASK&(var) )>>24) )
#define make_level(level) ( ( (unsigned long)(level)<<24 )&LEVEL_MASK )
#define change_var_level(field, change) \
  ( index_of(field)|make_level( level_of(field)+(change) ) )

