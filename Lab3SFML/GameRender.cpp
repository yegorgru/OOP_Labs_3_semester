#include "GameRender.h"

GameRender::GameRender(GameModel& game):
	m_game(game), m_window(sf::VideoMode(640, 690), "Docking",sf::Style::Titlebar|sf::Style::Close)
{
	texture.loadFromFile("texture.png");
	background_texture.loadFromFile("background.jpg");
	sprite.setTexture(texture);
	background_sprite.setTexture(background_texture);
	w = 80;
	font.loadFromFile("sansation.ttf");
	up_text1 = sf::Text("N - New Game,     mouse click - choose block,     Arrow Keys - move", font, 20);
	up_text1.setPosition(5, 2);
	up_text2 = sf::Text("Close the enemy or connect all your blocks!", font, 20);
	up_text2.setPosition(5, 25);

	red_won = sf::Text("Red player win!", font, 90);
	red_won.setFillColor(sf::Color::Red);
	blue_won = sf::Text("Blue player win!", font, 90);
	blue_won.setFillColor(sf::Color::Blue);
}

sf::RenderWindow& GameRender::window()
{
	return m_window;
}

void GameRender::draw()
{
	m_window.clear(sf::Color::Black);
	background_sprite.setPosition(0, 50);
	m_window.draw(background_sprite);
	m_window.draw(up_text1);
	m_window.draw(up_text2);
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++)
		{
			sprite.setPosition(i * w, j * w + 50);

			if (m_game.get_element(i, j) == 1) {
				sprite.setColor(sf::Color::Red);
			}
			else if (m_game.get_element(i, j) == 2) {
				sprite.setColor(sf::Color::Blue);
			}

			if (m_game.is_red_turn() && i == m_game.get_red_x_pos() && j == m_game.get_red_y_pos()) {
				sprite.setColor(sf::Color(255, 0, 0, 200));
			}
			else if (!m_game.is_red_turn() && i == m_game.get_blue_x_pos() && j == m_game.get_blue_y_pos()) {
				sprite.setColor(sf::Color(0, 0, 255, 200));
			}
			if (m_game.get_element(i, j)) {
				m_window.draw(sprite);
			}
		}
	}

	if (m_game.get_winner() == 1) {
		red_won.setPosition(4, 300);
		m_window.draw(red_won);
	}
	else if (m_game.get_winner() == 2) {
		blue_won.setPosition(4, 300);
		m_window.draw(blue_won);
	}

	m_window.display();
}

int GameRender::get_element_size()
{
	return w;
}
