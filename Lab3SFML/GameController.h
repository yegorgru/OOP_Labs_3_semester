#pragma once

#include <SFML/Graphics.hpp>

#include "GameRender.h"
#include "GameModel.h"

class GameController
{
	GameModel& m_game;
	GameRender& m_render;
public:
	GameController(GameModel& game, GameRender& render);
	void Run();
};

