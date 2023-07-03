#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROW 16 // 迷宫行数
#define COL 27 // 迷宫列数

// DFS算法生成迷宫
void dfs(int (*maze)[COL], int x, int y) {
    int dir[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}}; // 右下左上四个方向
    int visit[4] = {0, 1, 2, 3}; // 随机打乱方向
    srand((unsigned)time(NULL)); // 以时间为种子生成随机数
    for (int i = 0; i < 4; i++) {
        int r = rand() % 4;
        int tmp = visit[i];
        visit[i] = visit[r];
        visit[r] = tmp;
    }
    for (int i = 0; i < 4; i++) {
        int nx = x + dir[visit[i]][0] * 2; // 横坐标
        int ny = y + dir[visit[i]][1] * 2; // 纵坐标
        if (nx >= 0 && nx < ROW && ny >= 0 && ny < COL && maze[nx][ny] == 1) {
            maze[x + dir[visit[i]][0]][y + dir[visit[i]][1]] = 0;
            maze[nx][ny] = 0;
            dfs(maze,nx, ny);
        }
    }
}

int map_init(int (*maze)[COL]) {
    // 初始化迷宫
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            if (i == 0 || i == ROW - 1 || j == 0 || j == COL - 1) {
                maze[i][j] = 1; // 边界设置为墙
            } else {
                maze[i][j] = 1 - (rand() % 2); // 随机生成路或墙
            }
        }
    }

    // 从起点开始生成迷宫
    maze[15][0] = 0;
    dfs(maze,15, 1);

    // 输出迷宫
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            printf("%d ", maze[i][j]);
        }
        printf("\n");
    }

    return 0;
}