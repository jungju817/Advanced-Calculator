extern int yylineno;
void yyerror(const char *s);
struct ast;
struct numval;
struct symbol;

struct ast *newast(int nodetype, struct ast *l, struct ast *r);
struct ast *newnum(double d);
struct ast *newref(char *s);
struct ast *newasgn(char *s, struct ast *v);
double eval(struct ast *);
void treefree(struct ast *);
struct symbol *lookup(char *sym);
unsigned int hash(char *sym);

