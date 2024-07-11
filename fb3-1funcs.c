#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fb3-1.h"

struct ast {
    int nodetype;
    struct ast *l;
    struct ast *r;
};

struct numval {
    int nodetype;
    double number;
};

struct symref {
    int nodetype;
    struct symbol *s;
};

struct symasgn {
    int nodetype;
    struct symbol *s;
    struct ast *v;
};

struct symbol {
    char *name;
    double value;
};

#define NHASH 9997
struct symbol symtab[NHASH];

struct ast *newast(int nodetype, struct ast *l, struct ast *r) {
    struct ast *a = malloc(sizeof(struct ast));
    if (!a) {
        yyerror("out of space");
        exit(0);
    }
    a->nodetype = nodetype;
    a->l = l;
    a->r = r;
    return a;
}

struct ast *newnum(double d) {
    struct numval *a = malloc(sizeof(struct numval));
    if (!a) {
        yyerror("out of space");
        exit(0);
    }
    a->nodetype = 'K';
    a->number = d;
    return (struct ast *)a;
}

struct ast *newref(char *s) {
    struct symref *a = malloc(sizeof(struct symref));
    if (!a) {
        yyerror("out of space");
        exit(0);
    }
    a->nodetype = 'N';
    a->s = lookup(s);
    return (struct ast *)a;
}

struct ast *newasgn(char *s, struct ast *v) {
    struct symasgn *a = malloc(sizeof(struct symasgn));
    if (!a) {
        yyerror("out of space");
        exit(0);
    }
    a->nodetype = '=';
    a->s = lookup(s);
    a->v = v;
    return (struct ast *)a;
}

double eval(struct ast *a) {
    double v;

    switch (a->nodetype) {
        case 'K':
            v = ((struct numval *)a)->number;
            break;

        case 'N':
            v = ((struct symref *)a)->s->value;
            break;

        case '=':
            v = eval(((struct symasgn *)a)->v);
            ((struct symasgn *)a)->s->value = v;
            break;

        case '+':
            v = eval(a->l) + eval(a->r);
            break;

        case '-':
            v = eval(a->l) - eval(a->r);
            break;

        case '*':
            v = eval(a->l) * eval(a->r);
            break;

        case '/':
            v = eval(a->l) / eval(a->r);
            break;

        default:
            printf("internal error: bad node %c\n", a->nodetype);
            break;
    }
    return v;
}

void treefree(struct ast *a) {
    switch (a->nodetype) {
        case '+':
        case '-':
        case '*':
        case '/':
            treefree(a->l);
            treefree(a->r);
            break;

        case 'K':
            free(a);
            break;

        case 'N':
            free(a);
            break;

        case '=':
            treefree(((struct symasgn *)a)->v);
            free(a);
            break;

        default:
            printf("internal error: free bad node %c\n", a->nodetype);
            break;
    }
}

void yyerror(const char *s) {
    fprintf(stderr, "error: %s\n", s);
}

struct symbol *lookup(char *sym) {
    struct symbol *sp = &symtab[hash(sym) % NHASH];
    int scount = NHASH;

    while (--scount >= 0) {
        if (sp->name && !strcmp(sp->name, sym)) {
            return sp;
        }

        if (!sp->name) {
            sp->name = strdup(sym);
            sp->value = 0;
            return sp;
        }

        if (++sp >= symtab + NHASH) sp = symtab;
    }
    yyerror("symbol table overflow\n");
    abort();
}

unsigned int hash(char *sym) {
    unsigned int hash = 0;
    unsigned c;

    while ((c = *sym++)) hash = hash * 9 ^ c;

    return hash;
}

