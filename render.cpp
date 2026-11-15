#include "game.h"
#include "console_color.h"
#include <stdio.h>
#include <stdlib.h>

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
                if (game->player.row == r && game->player.col == c) {
                    symbol = isGoal ? '+' : '@';
                    color = isGoal ? COLOR_GREEN : COLOR_CYAN;
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
    printf("Steps: %d\n", game->steps);
    if (message != NULL && message[0] != '\0') printf("%s", message);
    else if (game->won) printf("You win! R to restart, Q to quit.");
    putchar('\n');

    if (game->running) {
        printf("W/A/S/D: move  R: restart  Q: quit (no Enter)");
    }
    /* 先显示完整画面，再等待下一次按键。 */
    fflush(stdout);
}
