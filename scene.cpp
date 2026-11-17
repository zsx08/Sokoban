#include "scene.h"
#include <stdio.h>
#include "data.h"
#include "game.h"

/*ESC退出，Space进入模式*/
Scene RunTitle() {
	int command;
	int mode = 0;//表征是哪个模式
	while (1) {
		renderTitleScene(mode);
		command = readCommand();    //读指令
		switch (command) {	//暂时只有一个页面，不用加入页面切换
			case ' ': 
				return scenelist[mode];
			case 27:
				return Quit;
		}
	}
}

Scene RunGame() {
    GameState game;
    int command;
    const char* message = "";

    if (!initGame(&game,levelid)) {
        printf("Failed to initialize the built-in level.\n");
        return SceneTitle;
    }
    game.won = checkWin(&game);

    while (1) {
        renderGame(&game, message);
        command = readCommand();    //读指令
        message = "";

        if (command == 'q') {
            return SceneLevel;
        }
        else if (command == 'r') {
            if (!initGame(&game,levelid)) {
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
    }
}

/*ESC回退至Level，R重玩，Q下一关*/
Scene RunEnd() {
    int command;
    printf(levelid< levelamount -1?"You win! R to restart, ESC to quit ,N to next level": "You win the last level! R to restart, ESC to quit\n");
    while (1) {
        command = readCommand();
        switch (command) {
            case 27:
                return SceneLevel;
            case 'r':
                return SceneGame;
            case 'n':
                if (levelid < levelamount - 1) {
                    levelid++;
                    return SceneGame;
                }
                break;
        }
    }
}
/*ESC返回标题，Space进关卡，左右方向键选关*/
Scene RunLevel() {
    int command;
    while (1) {
        renderLevelSelectionScene(levelid);
        command = readCommand();
        switch (command) {
        case 27:
            return SceneTitle;
        case ' ':
            return SceneGame;
        case 331:
            if(levelid > 0)
                levelid--;
            break;
        case 333:
            if (levelid < levelamount - 1)
                levelid++;
            break;
        }
    }
}