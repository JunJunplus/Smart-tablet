#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>

#include "kernel_list.h"
#include "lcd.h"
#include "touch.h"
#include "bmp.h"

typedef struct Node
{
    int fd;
    char fileName[40];
    struct list_head list;
} Node_t;

extern void boot();
extern int endWith(char *originString, char *end);