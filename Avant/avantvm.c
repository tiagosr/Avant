//
//  avantvm.c
//  Avant
//
//  Created by Tiago Rezende on 3/24/13.
//  Copyright (c) 2013 Tiago Rezende. All rights reserved.
//

#include "avantvm.h"
#include <stdio.h>



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
#define ST_IS_INT(x) (ST(x).i&1)
#define ST_IS_ATOM(x) ((ST(x).i&1)==0)
#define ST_AS_ATOM(x) (ST(x).a)
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
                if (ST_IS_ATOM(1)) {
                    if (ST_IS_ATOM(2)) {
                        // unreference 2, then copy atom
                    } else {
                        // copy atom
                    }
                } else {
                    if (ST_IS_ATOM(2)) {
                        // unreference 2, then assign
                    }
                    ST(2).i = ST(1).i;
                }
                code += 1+2;
            }
                break;
                
            case STORE:
            {
                
                code += 1+2;
            }
            default:
                return -2; // invalid opcode
        }
    }
#undef ST_IS_INT
#undef ST_IS_ATOM
#undef ST_AS_ATOM
#undef ST
    return 0;
}
