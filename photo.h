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
#include "boot.h"

int (*lcdbuf)[800 * 480];

extern struct list_head *head;
extern bool slide_open;

extern void photo_show();
extern void small_photo_show();
extern void init_photo();
extern void double_photo(struct list_head *phead);
extern void slide_photo(struct list_head *phead);
extern void *slide_photo_pth(void *phead);
