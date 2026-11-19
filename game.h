#ifndef GAME_H
#define GAME_H

#include "levels.h"

typedef enum {
    TILE_FLOOR,
    TILE_WALL,
    TILE_GOAL
} Tile;

class Position {
    /* 坐标从 0 开始：行号向下增加，列号向右增加。 */
public:
    int row;
    int col;
};
bool operator== (const Position&, const Position&);

typedef struct {
    Tile terrain[MAX_ROWS][MAX_COLS];
    int rows;
    int cols;

    Position player[2];
    Position boxes[MAX_BOXES];
    int boxCount;
    int playernum;

    int steps[2];    /* 成功移动的次数，包含推箱子；移动失败不计步。 */
    int running;  /* 由 main.cpp 维护的运行标志。（1/0）*/
    int won;      /* 由 main.cpp 根据 checkWin() 的结果维护的通关标志。（1/0） */
} GameState;


/* 从源码中的内置关卡初始化，返回 1 表示成功，0 表示关卡数据不合法。id为关卡序号 */
int initGame(GameState *game, int id = 0, int playernum = 1);

/* Windows/macOS 终端单键输入，无需回车；返回小写指令，非法按键返回 '\0'。 */
int readCommand(void);

/* 清屏后重画地图和右侧图例；message 是界面提示，不属于游戏状态。 */
void renderGame(const GameState *game, const char *message);

/* 访问某个位置的地形前，先调用此函数检查坐标是否在地图范围内。 */
int inBounds(const GameState *game, Position p);

/* 返回箱子下标（可能为 0）；位置 p 没有箱子时返回 -1。 */
int findBox(const GameState *game, Position p);

/* 需要完成的功能：以下函数仅提供接口和实现提示。 */
/* 仅接收 w/a/s/d 指令；移动受阻时，游戏状态应保持不变。 */
void updateGame(GameState *game, char command, int playerid = 0);

/* 只读取状态，不修改状态；加载或重开关卡后也会立即调用。 */
int checkWin(const GameState *game);

/*渲染初始界面*/
void renderTitleScene(int selection);

/*渲染关卡选择界面*/
void renderLevelSelectionScene(int selection);
#endif
