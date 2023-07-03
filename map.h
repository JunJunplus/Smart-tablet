#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROW 16 // 迷宫行数
#define COL 27 // 迷宫列数>

void dfs(int (*maze)[COL], int x, int y);
int map_init(int (*maze)[COL]);