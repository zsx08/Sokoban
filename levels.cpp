#include "levels.h"
#include<string>
#include<fstream>
#include "data.h"
using namespace std;

Level levels[MODENUM][MAX_LEVELS]={0};

//从文件加载地图
int ReadMapData() {
	ifstream file;
	string line;
	file.open("level.txt", ios::in);
    for (int i = 0;i < MODENUM;i++) {
        file >> levelamount[i];
    }
    int mode = 0;
	int idx = 0;
	while (idx < MAX_LEVELS && mode<MODENUM &&(file >> levels[mode][idx].row >> levels[mode][idx].col)) {
        int r = 0;
        file.ignore(1024, '\n');
        while (r < levels[mode][idx].row && getline(file, line)) {
            strncpy(levels[mode][idx].map[r], line.c_str(), MAX_COLS);
            levels[mode][idx].map[r][MAX_COLS] = '\0';
            ++r;
        }
        ++idx;
        if (idx == levelamount[mode]) {
            idx = 0;
            mode++;
        }
	}
    return idx;
}
/*
char* MakeRandomMap() {
    
}*/