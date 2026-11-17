#include "game.h"
#include <stdio.h>

int main(void) {
    GameState game;
    char command;
    const char *message = "";

    int levelnum=ReadMapData();//读取地图数据

    if (!initGame(&game)) {
        printf("Failed to initialize the built-in level.\n");
        return 1;
    }
    game.won = checkWin(&game);

    while (game.running) {
        renderGame(&game, message);
        command = readCommand();    //读指令
        message = "";

        if (command == 'q') {
            game.running = 0;
            message = "Goodbye!";
        } else if (command == 'r') {
            if (!initGame(&game)) {
                game.running = 0;
                renderGame(&game, "Failed to restart. Check DEFAULT_LEVEL in game.cpp.");
                putchar('\n');
                return 1;
            }
            game.won = checkWin(&game);
            message = game.won ? "You win! R to restart, Q to quit." :
                                "Level restarted.";
        } else if (game.won) {
            message = "Level complete. R to restart, Q to quit.";
        } else if (command == 'w' || command == 'a' ||
                   command == 's' || command == 'd') {
            updateGame(&game, command);  /* TODO：在 game.cpp 中补全。 */
            game.won = checkWin(&game);
        } else {
            message = "Invalid command.";
        }
    }

    renderGame(&game, message);
    putchar('\n');
    return 0;
}
