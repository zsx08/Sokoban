//玩家逻辑系统接口
#pragma once
#include "game.h"
#include "scene.h"
//单玩家接口
Scene SinglePlayer(GameState& game);
void LocalHumanPlayer(GameState &game,int id = 0);//零代表首位本地玩家
