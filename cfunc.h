/* files marked with # are used in the Tracer as well. */

/* rc5.c -- main functions of compiler. */
int ri_inquire(char *, char *, int);
int rc_gchar(void);

/* lex.c# -- lexical scan functions. */
int rc_gettoken(void);
int rc_serror(int, char *);
int rc_swarn(int);

/* parser.c -- top down recursive descent parser */
struct node* rc_parser(void);

/* pass2.c -- second pass resolving references. */
int rc_initcom (int, char * *);
int rc_getbeginfile (char **);
int rc_getnextfile (int, char **);
int rc_sbtable (struct functab *);
int rc_sftable (struct functab *);
int rc_sltable (struct functab *);
int rc_pass2 (void);
int rc_end (void);

/* rc.c -- Refal compiler: general and right side translation. */
int refcom (struct node *);
int tr_rtail (struct node *, int *);
int refc_out (struct rasl_instruction * *, int);
int transl_right (struct element *, int);
int get_var_index (int, unsigned char *);
int is_bit_checked (unsigned char *, int);
int check_bit (unsigned char *, int);

/* rcaux.c# -- expression parsing and auxiliary functions. */
int rc_initrp (void);
char *rc_allmem (int);
char *rc_memral (int);
int rc_post_opt (struct rasl_instruction * *, int);
int rc_post (struct rasl_instruction *);
int merge_string_instr (int, int, struct rasl_instruction *);
struct element *refal_expression (int);
char *rc_getact (void);
char *getcoms (void);
int rc_getvar (int, char, struct element *);
int getconst (int, struct element *);
int searchv (char, char *);
struct functab *searchf (char *, struct functab *);
int insert_instruction (struct rasl_instruction *, int, union param);
int rc_options(int argc, char * argv []);
int rc_help(void);



/* rcleft.c# -- Refal compiler: left parts compilation */
int transl_left (struct element *, int *);
int match (struct element *, int, int, int *, int *, int, int *);
struct HOLES *delete_hole (struct HOLES *, struct HOLES *);
struct HOLES *select_hole (struct HOLES *, struct element *, int *, struct HOLES **);
struct HOLES *add_hole (struct HOLES *, struct HOLES *, int, int, int, int);
int free_holes (struct HOLES *);
int check_var (int);
int no_lengthening (struct element *);
int rc_out (int, union param);

/* rcopt.c -- Refal optimizer */
int refc_opt (struct rasl_instruction * *, int, int *);
int comp_inst (struct rasl_instruction *, struct rasl_instruction *);
int comp_sym_inst (struct rasl_instruction *, struct rasl_instruction *);
int split_string (struct rasl_instruction *, int, int, int);
int ch2sym (struct rasl_instruction *);
int chop_tail (struct rasl_instruction *);
int is_left_part (struct rasl_instruction *);
int comp_next_ins (struct rasl_instruction *, struct rasl_instruction *,
  struct rasl_instruction **);
int delete_to_ptr (struct rasl_instruction *, struct rasl_instruction *);
int save_label (int, struct rasl_instruction *);
struct rasl_instruction *find_def_label (int);

/* sem.c -- some semantic processing during parsing. */
struct node *rc_mknode (int, branch_t, branch_t, branch_t);
char *rc_deffn (void);
char *rc_mkextrn (char *);
char *rc_mkentry (char *);
int free_tree (struct node *);

/* vyvod.c -- saving Refal translation in a file. */
char *rc_deflabel(char *, struct functab *);
long rc_getcsn (char *);
int rc_vyvod (struct rasl_instruction * *, int);
