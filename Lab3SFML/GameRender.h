#pragma once

#include <SFML/Graphics.hpp>

#include "GameModel.h"

class GameRender
{
public:
	GameRender(GameModel& game);
	sf::RenderWindow& window();
	void draw();

	int get_element_size();

private:
	GameModel& m_game;

	sf::RenderWindow m_window;

	sf::Texture texture;
	sf::Texture background_texture;

	sf::Sprite sprite;
	sf::Sprite background_sprite;

	sf::Font font;

	sf::Text up_text1;
	sf::Text up_text2;
	sf::Text red_won;
	sf::Text blue_won;
	int w;
};

