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
#include "photo.h"
#include "color.h"

void game_wb();
void game_draw();

