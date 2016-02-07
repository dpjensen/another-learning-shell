#pragma once
#include "common.h"
#define _GNU_SOURCE

#define NUM_BUILTINS 3

void initBuiltins();
int32_t parseLine(char **args);


/*
 * This struct 
 * is for our builtin functions
 */
 struct builtin{

    char *builtinStr;
    int(*builtinPtr)(char**);

};

