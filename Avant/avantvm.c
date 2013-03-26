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


static inline void set_int(t_atom **p, int i) {
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

static inline void set_float(t_atom **p, float f) {
    if(*p) {
        if ((*p)->atype == A_FLOAT) {
            goto is_already_float;
        }
        *p = realloc(*p,sizeof(t_boxed_float));
    } else {
        *p = malloc(sizeof(t_boxed_float));
    }
    ((t_boxed_float *)*p)->atom.atype = A_FLOAT;
is_already_float:
    ((t_boxed_float *)*p)->f = f;
}

static inline void set_conststring(t_atom **p, const char *str) {
    if (*p) {
        if ((*p)->atype == A_STRING) {
            if (((t_boxed_string *)*p)->constant) {
                goto is_already_string;
            }
            free(((t_boxed_string *)*p)->str);
            goto is_already_string;
        }
        *p = realloc(*p, sizeof(t_boxed_string));
    } else {
        *p = malloc(sizeof(t_boxed_string));
    }
    ((t_boxed_string *)*p)->atom.atype = A_STRING;
is_already_string:
    ((t_boxed_string *)*p)->constant = true;
    ((t_boxed_string *)*p)->str = str;
}

static inline void set_copystring(t_atom **p, const char *str) {
    if (*p) {
        if ((*p)->atype == A_STRING) {
            if (((t_boxed_string *)*p)->constant) {
                goto is_already_string;
            }
            free(((t_boxed_string *)*p)->str);
            goto is_already_string;
        }
        *p = realloc(*p, sizeof(t_boxed_string));
    } else {
        *p = malloc(sizeof(t_boxed_string));
    }
    ((t_boxed_string *)*p)->atom.atype = A_STRING;
is_already_string:
    ((t_boxed_string *)*p)->constant = false;
    ((t_boxed_string *)*p)->str = strcpy(str);
}




int execute(t_av_context *ctx, char* code) {
    t_av_op_ctx opctx;
#define ST(x) (ctx->stack+code[x])
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
                unsigned i = 0;
                memcpy(&i, code+1, sizeof(unsigned));
                set_int(ST(sizeof(unsigned)+1), i);
                code += 1+sizeof(unsigned)+1;
            }
                break;
            case PUSH_FLOAT:
            {
                float f;
                memcpy(&f, code+1, sizeof(float));
                set_float(ST(sizeof(float)+1), f);
                code += 1+sizeof(float)+1;
            }
                break;
            
            case PUSH_STR:
            {
                char *c;
                memcpy(&c, code+1, sizeof(char*));
                set_conststring(ST(sizeof(char*)+1), c);
                code += 1+sizeof(char*)+1;
            }
                break;
                
            case RESIZE:
            {
                ctx->stack -= ((t_boxed_int *)*ST(1))->i;
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
    return 0;
}
