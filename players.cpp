#include "players.h"
#include "game.h"
#include "scene.h"
#include "data.h"
#include <stdio.h>
Scene SinglePlayer(GameState& game) {
    int command = 0;
    const char* message = "";
    command = readCommand();    //读指令

    if (command == 'q') {
        return SceneLevel;
    }
    else if (command == 'r') {
        if (!initGame(&game, levelid)) {
            renderGame(&game, "Failed to restart. Check level data in level.txt.");
            putchar('\n');
            return SceneTitle;
        }
        message = "Level restarted.";
        if (checkWin(&game)) {
            return SceneEnd;
        }
    }
    else if (command == 'w' || command == 'a' ||
        command == 's' || command == 'd') {
        updateGame(&game, command);
        if (checkWin(&game)) {
            renderGame(&game, message);
            return SceneEnd;
        }
    }
    else {
        message = "Invalid command.";
    }
    renderGame(&game, message);
    return Maintain;
}

Scene LocalHumanPlayer(GameState& game,int id) {
	int command = 0;
    const char* message = "";

	command = readCommand();
	switch (command) {
	case 'w':
	case 'a':
	case 's':
	case 'd':
        updateGame(&game, command, id);
        if (checkWin(&game)) {
            renderGame(&game, message);
            return SceneEnd;
        }
        break;
    case 'q':
        return Quit; //待定
    default:
        message = "Invalid command.";
    }
    renderGame(&game, "");
    return Maintain;
}