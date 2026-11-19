//控制界面逻辑（按键响应）与界面之间怎么切换的逻辑
#pragma once
typedef enum {
    SceneTitle,
    SceneLevel,
    SceneGame,
    SceneEnd,
    SceneMutiple,
    Quit,
    Maintain
}Scene;

inline const Scene scenelist[] = { SceneGame,SceneMutiple }; //支持的游戏模式列表

Scene RunTitle();
Scene RunLevel();
Scene RunGame();
Scene RunEnd();
Scene RunMutiple();


