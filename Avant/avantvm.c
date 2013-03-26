//
//  avantvm.c
//  Avant
//
//  Created by Tiago Rezende on 3/24/13.
//  Copyright (c) 2013 Tiago Rezende. All rights reserved.
//

#include "avantvm.h"
#include <stdio.h>

typedef struct _op_spec {
    char size;
    char vars;
} t_op_spec;


enum _ops {
    _ABORT = 0,
    NOP,
    
    PUSH_INT,
    PUSH_FLOAT,
    PUSH_STR,
    PUSH_PTR,
    
    RESIZE,
    DUP,
    
    STORE,
    LOAD,
    
    F_ADD,
    F_SUB,
    F_MUL,
    F_DIV,
    
    I_ADD,
    I_SUB,
    I_MUL,
    I_DIV,
    I_MOD,
    
    I_ADD_C,
    I_ADD_C_C,
    I_SUB_C,
    I_SUB_C_C,
    
    I_DIVMOD,
    
    I_INC,
    I_DEC,
    
    EQUALS,
    EQUALS_0,
};


static inline void set_boxed_int(t_atom **p, int i) {
    if(*p) {
        if ((*p)->atype == A_INT) {
            goto is_already_int;
        }
        *p = realloc(*p,sizeof(t_boxed_int));
    } else {
        *p = malloc(sizeof(t_boxed_int));
    }
    ((t_boxed_int *)*p)->atom.atype = A_INT;
is_already_int:
    ((t_boxed_int *)*p)->i = i;
}

static inline void set_float(u_atomptr_int *p, float f) {
    if(p->i != 1) {
        if ((p->a).atom->atype == A_FLOAT) {
            goto is_already_float;
        }
        p->a.boxed_float = realloc(p->a.atom,sizeof(t_boxed_float));
    } else {
        p->a.boxed_float = malloc(sizeof(t_boxed_float));
    }
    (p->a.boxed_float)->atom.atype = A_FLOAT;
is_already_float:
    (p->a.boxed_float)->f = f;
}

static inline void set_conststring(u_atomptr_int *p, const char *str) {
    if (p->i != 1) {
        if (p->a.atom->atype == A_STRING) {
            if (p->a.boxed_string->constant) {
                goto is_already_string;
            }
            free(p->a.boxed_string->str);
            goto is_already_string;
        }
        p->a.boxed_string = realloc(p->a.atom, sizeof(t_boxed_string));
    } else {
        p->a.boxed_string = malloc(sizeof(t_boxed_string));
    }
    p->a.boxed_string->atom.atype = A_STRING;
is_already_string:
    p->a.boxed_string->constant = true;
    p->a.boxed_string->str = (char *)str;
}

static inline void set_copystring(u_atomptr_int *p, const char *str) {
    if (p->i != 1) {
        if (p->a.atom->atype == A_STRING) {
            if (p->a.boxed_string->constant) {
                goto is_already_string;
            }
            free(p->a.boxed_string->str);
            goto is_already_string;
        }
        p->a.boxed_string = realloc(p->a.atom, sizeof(t_boxed_string));
    } else {
        p->a.boxed_string = malloc(sizeof(t_boxed_string));
    }
    p->a.boxed_string->atom.atype = A_STRING;
is_already_string:
    p->a.boxed_string->constant = false;
    asprintf(&(p->a.boxed_string->str), "%s",str);
}





int execute(t_av_context *ctx, char* code) {
    t_av_op_ctx opctx;
#define ST(x) (ctx->stack[code[x]])
#define ST_AS_INT(x) (ST(x).i>>1)
#define ST_IS_ATOM(x) (ST(x).i&1)
#define ST_AS_ATOM(x) ((t_u_atomptr)(ST(x).a-1))
    while (1) {
        switch (code[0]) {
            case _ABORT:
                return -1;
            case NOP:
                // no operation.
                code++;
                break;
                
            case PUSH_INT:
            {
                int i = 0;
                memcpy(&i, code+1, sizeof(int));
                i <<= 1;
                u_atomptr_int ip;
                ip.i = i;
                ctx->stack[code[sizeof(int)+1]].i = i;
                //set_boxed_int(&(ST(sizeof(int)+1)->atom), i);
                code += 1+sizeof(int)+1;
            }
                break;
            case PUSH_FLOAT:
            {
                float f;
                memcpy(&f, code+1, sizeof(float));
                set_float(&ST(sizeof(float)+1), f);
                code += 1+sizeof(float)+1;
            }
                break;
            
            case PUSH_STR:
            {
                char *c;
                memcpy(&c, code+1, sizeof(char*));
                set_conststring(&ST(sizeof(char*)+1), c);
                code += 1+sizeof(char*)+1;
            }
                break;
                
            case RESIZE:
            {
                if (ST_IS_ATOM(1)) {
                    
                } else {
                    ctx->stack -= ST_AS_INT(1);
                    if (ctx->stack < ctx->stack_end) {
                        return -1; // stack overflow
                    }
                }
                code += 1+1;
            }
                break;
            
            case DUP:
            {
                
                code += 1+2;
            }
                break;
            default:
                break;
        }
    }
#undef ST_IS_INT
#undef ST_IS_ATOM
#undef ST_AS_ATOM
#undef ST
    return 0;
}
