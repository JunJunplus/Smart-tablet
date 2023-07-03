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
#include "photo.h"
#include "font.h"
//#include "map.h"

void show_map();
void game_map();

