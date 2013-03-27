//
//  main.m
//  Avant
//
//  Created by Tiago Rezende on 3/22/13.
//  Copyright (c) 2013 Tiago Rezende. All rights reserved.
//

#include "SDL.h"
#import <Cocoa/Cocoa.h>
#import <OpenGL/gl3.h>

static char * resource_file_path(const char *filename)
{
    @autoreleasepool {
        NSString *str = [[NSBundle mainBundle] pathForResource:[NSString stringWithCString:filename encoding:NSUTF8StringEncoding]
                                                        ofType:nil];
        if (str) {
            return strdup([str cStringUsingEncoding:NSUTF8StringEncoding]);
        } else {
            return NULL;
        }
    }
}

typedef struct _atom {
    int type;
    union {
        int ival;
        float fval;
        char *strval;
        void *pval;
        struct _list *lval;
    };
} t_atom;
typedef struct _list {
    int refcount;
    t_atom *first;
    t_atom *rest;
} t_list;

/*
 * One parameter stack for each overarching type of argument
 * I want to test this idea of multiple parameter stacks, it looks like
 * it can allow for good things.
 */
enum PARAM_STACKS {
    INT_STACK = 0,
    FLOAT_STACK,
    COMPLEX_STACK,
    STRING_STACK,
    POINTER_STACK,
    LIST_STACK,
    WORD_STACK,
    FP_ATTRIB_STACK,
    INT_ATTRIB_STACK,
    MATRIX_STACK,
    BITMAP_STACK,
    NOTE_STACK,
    SAMPLE_BUFFER_STACK,
    FIRST_USER_STACK
};

typedef struct {
    int numstacks;
    t_list **param_stacks;
    t_list *return_stack;
} t_context;

int main(int argc, char *argv[]) {
    //return NSApplicationMain(argc, (const char **)argv);
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_Window *window = SDL_CreateWindow("Avant", SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED, 800, 600,
                                          SDL_WINDOW_RESIZABLE |
                                          SDL_WINDOW_SHOWN |
                                          SDL_WINDOW_OPENGL);
    SDL_GLContext *glctx = SDL_GL_CreateContext(window);
    
    GLint vtxarr = 0;
    glGenVertexArrays(1, &vtxarr);
    GLint arrbuf = 0;
    glGenBuffers(1, &arrbuf);
    glBindVertexArray(vtxarr);
    glBindBuffer(GL_ARRAY_BUFFER, arrbuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*4, NULL, GL_DYNAMIC_DRAW);
    float *ptmap = glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(float)*4, GL_MAP_WRITE_BIT|GL_MAP_INVALIDATE_RANGE_BIT);
    printf("map ptr: 0x%X",ptmap);
    SDL_Event e;
    
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_WINDOWEVENT:
                    
                    break;
                case SDL_KEYDOWN:
                case SDL_KEYUP:
                    
                    break;
                case SDL_MOUSEMOTION:
                    
                    break;
                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEBUTTONUP:
                    
                    break;
                case SDL_MOUSEWHEEL:
                    
                    break;
                    
                default:
                    break;
            }
        }
        
        SDL_GL_SwapWindow(window);
    }
    SDL_GL_DeleteContext(glctx);
    SDL_DestroyWindow(window);
    return 0;
}
