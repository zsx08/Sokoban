#include "levels.h"
#include<string>
#include<fstream>
using namespace std;

Level levels[MAX_LEVELS]={0};

//从文件加载地图
int ReadMapData() {
	ifstream file;
	string line;
	file.open("level.txt", ios::in);
	int idx = 0;
	while (idx < MAX_LEVELS && (file >> levels[idx].row >> levels[idx].col)) {
        int r = 0;
        file.ignore(1024, '\n');
        while (r < levels[idx].row && getline(file, line)) {
            strncpy(levels[idx].map[r], line.c_str(), MAX_COLS);
            levels[idx].map[r][MAX_COLS] = '\0';
            ++r;
        }
        ++idx;
	}
    return idx;
}
/*
char* MakeRandomMap() {
    
}*/