#include "GameController.h"

GameController::GameController(GameModel& game, GameRender& render):
	m_game(game),m_render(render) {}

void GameController::Run()
{
	sf::Event event;

	while (m_render.window().isOpen())
	{
		sf::Vector2i pos = sf::Mouse::getPosition(m_render.window());
		int x = pos.x / m_render.get_element_size();
		int y = (pos.y - 50) / m_render.get_element_size();
		while (m_render.window().pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				m_render.window().close();
			}
			if (event.type == sf::Event::MouseButtonPressed) {
				if (event.key.code == sf::Mouse::Left) {
					if (m_game.is_red_turn()) {
						if (m_game.get_element(x, y) == 1) {
							m_game.set_red_position(x, y);
						}
					}
					else {
						if (m_game.get_element(x, y) == 2) {
							m_game.set_blue_position(x, y);
						}
					}
				}
			}
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::N) {
					m_game.restore();
				}
				else if (!m_game.get_winner()) {
					bool success = false;
					if (event.key.code == sf::Keyboard::Left) {
						success = m_game.make_move(0);
					}
					if (event.key.code == sf::Keyboard::Right) {
						success = m_game.make_move(1);
					}
					if (event.key.code == sf::Keyboard::Up) {
						success = m_game.make_move(2);
					}
					if (event.key.code == sf::Keyboard::Down) {
						success = m_game.make_move(3);
					}
					if (success) {
						m_game.next_turn();
						m_game.set_red_position(-1, -1);
						m_game.set_blue_position(-1, -1);
					}
				}
			}
		}
		m_render.draw();
	}
}
