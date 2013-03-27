//
//  avantvm.h
//  Avant
//
//  Created by Tiago Rezende on 3/24/13.
//  Copyright (c) 2013 Tiago Rezende. All rights reserved.
//

#ifndef Avant_avantvm_h
#define Avant_avantvm_h

/*
 * Avant is an experiment with different stack-machine mechanics:
 * DUP, DROP, SWAP, ROTATE and other opcodes give way to stack swizzling.
 * Each opcode can consume or produce a number of stack elements
 */

#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <memory.h>


typedef enum _argtype {
    A_SYMBOL = 0, // pointer to constant string
    A_INT = 1, // integer number
    A_FLOAT, // floating-point number
    A_LIST, // cons list
    A_CLOSURE, // isolated stack object with attached function
    A_CODE, // single opcode
    A_STRING,
    A_POINTER,
} t_argtype;

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

typedef struct _atom {
    union {
        struct _atom *prototype;
        struct {
            char atype;
            char flags;
            unsigned padding:(sizeof(struct _atom*)-sizeof(char)-sizeof(char));
        };
    };
} t_atom; // struct for boxed values

typedef struct {
    t_atom atom;
    int i;
} t_boxed_int;

typedef struct {
    t_atom atom;
    float f;
} t_boxed_float;

typedef struct {
    t_atom atom;
    bool constant;
    char *str;
} t_boxed_string;

typedef struct _symbol {
    t_atom atom;
    char *name;
    struct _symbol *next;
} t_symbol;

typedef struct _cell {
    t_atom atom;
    t_atom *first;
    t_atom *rest;
} t_cell;

typedef union _atom_types {
    t_atom *atom;
    t_boxed_float *boxed_float;
    t_boxed_int *boxed_int;
    t_boxed_string *boxed_string;
    t_symbol *symbol;
    t_cell *cell;
} t_u_atomptr;

typedef union _atom_union {
    int i;
    t_u_atomptr a;
} u_atomptr_int;

typedef struct _av_context {
    u_atomptr_int *stack;
    u_atomptr_int *stack_end;
    size_t stack_size;
} t_av_context;

typedef struct _av_op_ctx {
    t_atom *args[256];
} t_av_op_ctx;

typedef void(*t_op_func)(t_av_op_ctx *ctx);

/*
typedef struct _av_op_func_spec {
    t_op_func op_func;
    void *pseudoclosure;
    t_argtype *consumes;
    t_argtype *produces;
} t_av_op_func_spec;
*/



#endif
