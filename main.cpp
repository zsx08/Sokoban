//实现初始化与界面切换
#include "game.h"
#include "scene.h"
#include <stdio.h>
#include "data.h"

int main(void) {
    /*初始化*/
    levelamount = ReadMapData();//读取地图数据
    Scene currentscene = SceneTitle;

    /*主逻辑*/
    while (1) {
        switch (currentscene) {
        case SceneTitle:
            currentscene = RunTitle();
            break;
        case SceneLevel:
            currentscene = RunLevel();
            break;
        case SceneGame:
            currentscene = RunGame();
            break;
        case SceneEnd:
            currentscene = RunEnd();
            break;
        case Quit:
            printf("Goodbye!");
            return 0;
        }

    }
}
