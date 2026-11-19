#pragma once
#define MAX_ROWS 20
#define MAX_COLS 40
#define MAX_BOXES 20
#define MAX_LEVELS 40
#include "data.h"

typedef struct {
	char map[MAX_ROWS][MAX_COLS + 1];
	int col;
	int row;
	int num;
}Level;
extern Level levels[MODENUM][MAX_LEVELS];

/*从txt读取数据，并在成功时返回关卡数，否则返回0*/
int ReadMapData();

char* MakeRandomMap();