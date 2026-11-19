#include "game.h"
#include "console_color.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include "data.h"
#include "scene.h"

const std::string modelist[] = { "Level","MutiplePlayer"};//模式名

/*对于Windows系统和非Windows系统各用来清屏*/
static void clearScreen(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

/*打印图例*/
static void printLegend(int row) {
    const char symbols[] = "# .@$*+";
    const char *labels[] = {
        "Wall", "Floor (blank)", "Goal", "Player", "Box",
        "Box on goal", "Player on goal"
    };
    const ConsoleColor colors[] = {
        COLOR_DEFAULT, COLOR_DEFAULT, COLOR_MAGENTA, COLOR_CYAN,
        COLOR_YELLOW, COLOR_GREEN, COLOR_GREEN
    };
    if (row == 0) {
        printf("Legend");
    } else if (row >= 1 && row <= 7) {
        printColoredChar(symbols[row - 1], colors[row - 1]);
        printf("  %s", labels[row - 1]);
    }
}
/*按gamestate渲染，再输出messagae*/
void renderGame(const GameState *game, const char *message) {
    int r, c;
    int mapRows = game->rows > 8 ? game->rows : 8;

    fflush(stdout);
    clearScreen();
    resetTextColor();

    printf("=== Sokoban (framework) ===\n");
    for (r = 0; r < mapRows; ++r) {
        if (r < game->rows) {
            for (c = 0; c < game->cols; ++c) {
                Position p;
                int isGoal;
                char symbol = ' ';
                ConsoleColor color = COLOR_DEFAULT;
                p.row = r;
                p.col = c;
                isGoal = game->terrain[r][c] == TILE_GOAL;
                //判断渲染
                if (game->terrain[r][c] == TILE_WALL) symbol = '#';
                else if (isGoal) {
                    symbol = '.';
                    color = COLOR_MAGENTA;
                }
                if (findBox(game, p) >= 0) {
                    symbol = isGoal ? '*' : '$';
                    color = isGoal ? COLOR_GREEN : COLOR_YELLOW;
                }
                for (int i = 0;i < game->playernum;i++) {
                    if (game->player[i].row == r && game->player[i].col == c) {
                        symbol = isGoal ? '+' : '@';
                        color = isGoal ? COLOR_GREEN : COLOR_CYAN;
                    }
                }
                printColoredChar(symbol, color);
            }
        } else {
            printf("%*s", game->cols, "");
        }
        printf("    ");
        printLegend(r);
        putchar('\n');
    }
    for (int i = 0;i < game->playernum;i++) {
        printf("Steps: %d\n", game->steps[i]);
    }
    if (message != NULL && message[0] != '\0') printf("%s", message);
    else if (game->won) printf("You win! R to restart, Q to quit.");
    putchar('\n');

    if (game->running) {
        printf("W/A/S/D: move  R: restart  Q: quit (no Enter)");
    }
    /* 先显示完整画面，再等待下一次按键。 */
    fflush(stdout);
}


void renderTitleScene(int selection) {
    fflush(stdout);
    clearScreen();
    resetTextColor();

    printf("=== Sokoban Title ===\n");
    printf("====== Welcome! =====\n");
    printf("=== Mode Selection ==\n");
    for (int i = 0;i < sizeof(scenelist) / sizeof(Scene);i++) {
        std::cout<<modelist[i]<<"\t";
    }
    printf("\n");
    for (int i = 0;i < sizeof(scenelist) / sizeof(Scene);i++) {
        std::cout << (i==selection?"^\t" :"\t") << "\t";
    }
    printf("Press <- or -> to change mode\nPress Space to enter game\nPress Esc to quit\n");
    fflush(stdout);
}

void renderLevelSelectionScene(int selection) {
    fflush(stdout);
    clearScreen();
    resetTextColor();

    printf("=== Sokoban Selection ===\n");
    for (int j = 0;j < levelamount[gamemode]/5+1;j++) {
        for (int i = 5 * j;i < 5 * j +5 && i < levelamount[gamemode];i++) {
            printf("%d\t", i);
        }
        printf("\n");
        for (int i = 5 * j;i < 5 * j +5 && i < levelamount[gamemode];i++) {
            printf(i == selection ? "^\t" : "\t");
        }
        printf("\n");
    }
    
    printf("Press <- or -> to change level\nPress Space to enter game\nPress Esc to quit\n");
    fflush(stdout);
}