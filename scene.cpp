#include "scene.h"
#include <stdio.h>
#include "data.h"
#include "game.h"
#include "players.h"

/*ESC退出，Space进入模式*/
Scene RunTitle() {
	int command = 0;
	while (1) {
		renderTitleScene(gamemode);
		command = readCommand();    //读指令
		switch (command) {	//暂时只有一个页面，不用加入页面切换
			case ' ': 
				return SceneLevel;
			case 27:
				return Quit;
            case 331:
                if (gamemode > 0)
                    gamemode--;
                break;
            case 333:
                if (gamemode < sizeof(scenelist) / sizeof(Scene))
                    gamemode++;
                break;
        }
	}
}

Scene RunGame() {
    GameState game;
    Scene gamestate;
    if (!initGame(&game,levelid)) {
        printf("Failed to initialize the built-in level.\n");
        return SceneTitle;
    }
    game.won = checkWin(&game);
    renderGame(&game, "");
    while (1) {
        gamestate = SinglePlayer(game);
        if (gamestate != Maintain) {
            return gamestate;
        }
    }
}

/*ESC回退至Level，R重玩，Q下一关*/
Scene RunEnd() {
    int command = 0;
    printf(levelid< levelamount[gamemode] - 1 ? "You win! R to restart, ESC to quit ,N to next level" : "You win the last level! R to restart, ESC to quit\n");
    while (1) {
        command = readCommand();
        switch (command) {
            case 27:
                return SceneLevel;
            case 'r':
                return scenelist[gamemode];
            case 'n':
                if (levelid < levelamount[gamemode] - 1) {
                    levelid++;
                    return scenelist[gamemode];
                }
                break;
        }
    }
}
/*ESC返回标题，Space进关卡，左右方向键选关*/
Scene RunLevel() {
    int command = 0;
    while (1) {
        renderLevelSelectionScene(levelid);
        command = readCommand();
        switch (command) {
        case 27:
            return SceneTitle;
        case ' ':
            return scenelist[gamemode];
        case 331:
            if(levelid > 0)
                levelid--;
            break;
        case 333:
            if (levelid < levelamount[gamemode] - 1)
                levelid++;
            break;
        }
    }
}
//此处只运行循环，逻辑实现分离到mutipleplayer.h中的接口里
Scene RunMutiple() {
    int playerid = 0;
    Scene gamestate;
    GameState g;
    if (!initGame(&g, levelid,2)) {
        printf("Failed to initialize the built-in level.\n");
        return SceneTitle;
    }
    g.won = checkWin(&g);
    renderGame(&g, "");
    while (1) {
        gamestate = LocalHumanPlayer(g,playerid);
        if (gamestate != Maintain) {
            return gamestate;
        }
        playerid = (playerid + 1) % 2;
    }
}