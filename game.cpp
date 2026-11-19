#include "game.h"
#include <string.h>

/* 修改此数组可编辑示例关卡，修改后需要重新编译。 */
static const char DEFAULT_LEVEL[][MAX_COLS + 1] = {
    "##########",
    "#        #",
    "#   $    #",
    "#   @  . #",
    "#        #",
    "##########"
};

int inBounds(const GameState *game, Position p) {
    return p.row >= 0 && p.row < game->rows &&
           p.col >= 0 && p.col < game->cols;
}

/*判断某一位置是否为墙体或其他玩家*/
int isWall(const GameState* game, Position p) {
    bool b = false;
    b = (b || (game->terrain[p.row][p.col] == TILE_WALL));
    for (int i = 0;i < game->playernum;i++) {
        b = (b || (game->player[i] == p));
    }
    return b;
}

int findBox(const GameState *game, Position p) {
    int i;
    for (i = 0; i < game->boxCount; ++i) {
        if (game->boxes[i].row == p.row && game->boxes[i].col == p.col) {
            return i;
        }
    }
    return -1;
}

int initGame(GameState *game,int id,int playernum) {
    int r, c;
    int playerCount = 0;
    int goalCount = 0;

    memset(game, 0, sizeof(*game));
    game->running = 1;
    game->playernum = playernum;
    //加载地图
    char map[MAX_ROWS][MAX_COLS + 1];
    memcpy(map,levels[id].map,sizeof(levels[id].map));

    /*      旧版，已废弃
    //计算行数列数
    game->rows = (int)(sizeof(map) / sizeof(map[0]));
    if (game->rows <= 0 || game->rows > MAX_ROWS) return 0;
    game->cols = (int)strlen(map[0]);
    if (game->cols == 0 || game->cols > MAX_COLS) return 0;
    */

    //计算行数列数
    game->rows = levels[id].row;
    if (game->rows <= 0 || game->rows > MAX_ROWS) return 0;
    game->cols = levels[id].col;
    if (game->cols == 0 || game->cols > MAX_COLS) return 0;

    //将关卡字符串加载为gamestate
    for (r = 0; r < game->rows; ++r) {
        /* 关卡的每一行应有相同的字符数量。 */
        if ((int)strlen(map[r]) != game->cols) return 0;

        for (c = 0; c < game->cols; ++c) {
            char ch = map[r][c];
            game->terrain[r][c] = TILE_FLOOR;
            if (ch == '#') game->terrain[r][c] = TILE_WALL;
            else if (ch == '.' || ch == '*' || ch == '+') {
                game->terrain[r][c] = TILE_GOAL;
                ++goalCount;
            }

            if (ch == '@' || ch == '+') {
                game->player[playerCount].row = r;
                game->player[playerCount].col = c;
                ++playerCount;
            } else if (ch == '$' || ch == '*') {
                if (game->boxCount >= MAX_BOXES) return 0;
                game->boxes[game->boxCount].row = r;
                game->boxes[game->boxCount].col = c;
                ++game->boxCount;
            } else if (ch != '#' && ch != '.' && ch != ' ' && ch != '@') {
                return 0;
            }
        }
    }

    return game->rows > 0 && playerCount == playernum &&
           game->boxCount > 0 && game->boxCount == goalCount;
}

void updateGame(GameState *game, char command,int playerid ) {
    /* TODO 1：将 W/A/S/D 指令转换为行、列方向上的位移。 */
    /* TODO 2：检查地图边界和墙壁。 */
    /* TODO 3：若前方有箱子，检查箱子后方的位置是否允许推动。 */
    /* TODO 4：若操作合法，更新箱子、玩家的位置和步数。 */
    Position p = game->player[playerid];
    switch (command) {
        case 'w': {
            p.row -= 1;
            break;
        }
        case 's': {
            p.row += 1;
            break;
        }
        case 'a': {
            p.col -= 1;
            break;
        }
        case 'd': {
            p.col += 1;
            break;
        }
    }
    if ((!inBounds(game, p)) || isWall(game, p)) {
        return;
    }
    if (int t = findBox(game, p);t > -1) {
        Position pbox = p;
        switch (command) {
            case 'w': {
                pbox.row -= 1;
                break;
            }
            case 's': {
                pbox.row += 1;
                break;
            }
            case 'a': {
                pbox.col -= 1;
                break;
            }
            case 'd': {
                pbox.col += 1;
                break;
            }
        }
        if ((!inBounds(game, pbox)) || isWall(game, pbox) || (findBox(game,pbox)!=-1)) {
            return;
        }
        else {
            game->boxes[t] = pbox;
        }
    }
    game->player[playerid] = p;
    game->steps[playerid]++;
}

int checkWin(const GameState *game) {
    /* TODO：仅当所有箱子都在目标点上时返回 1，否则返回 0。 */
    int goal = 1;
    for (int i = 0;i < game->boxCount && goal > 0;i++) {
        Position pbox = game->boxes[i];
        goal *= (game->terrain[pbox.row][pbox.col] == TILE_GOAL);
    }
    return goal;
}

bool operator== (const Position& p1, const Position& p2) {
    return (p1.col == p2.col && p1.row == p2.row);
}
