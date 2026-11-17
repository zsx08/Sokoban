//控制界面逻辑（按键响应）与界面之间怎么切换的逻辑
#pragma once
typedef enum {
    SceneTitle,
    SceneLevel,
    SceneGame,
    SceneEnd,
    Quit
}Scene;

inline const Scene scenelist[] = { SceneLevel }; //支持的模式列表

Scene RunTitle();
Scene RunLevel();
Scene RunGame();
Scene RunEnd();


