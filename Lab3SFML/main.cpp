// main.cpp
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <set>
#include <utility>

class Game {
public:
	Game() {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++)
			{
				if ((i == 0 || i == 7) && j > 1 && j < 6) {
					gridView[i][j] = 1;
				}
				else if ((j == 0 || j == 7) && i > 1 && i < 6) {
					gridView[i][j] = 2;
				}
				else {
					gridView[i][j] = 0;
				}
			}
		}
		cur_red_x = -1;
		cur_blue_x = -1;
		red_turn = true;
		cur_red_x = -1;
		cur_red_y = -1;
		cur_blue_x = -1;
		cur_blue_y = -1;
		winner = 0;
	}

	void restore() {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++)
			{
				if ((i == 0 || i == 7) && j > 1 && j < 6) {
					gridView[i][j] = 1;
				}
				else if ((j == 0 || j == 7) && i > 1 && i < 6) {
					gridView[i][j] = 2;
				}
				else {
					gridView[i][j] = 0;
				}
			}
		}
		cur_red_x = -1;
		cur_blue_x = -1;
		red_turn = true;
		cur_red_x = -1;
		cur_red_y = -1;
		cur_blue_x = -1;
		cur_blue_y = -1;
		winner = 0;
	}

	bool is_red_turn() {
		return red_turn;
	}

	int get_element(int x, int y) {
		return gridView[x][y];
	}

	void set_red_x_pos(int x) {
		this->cur_red_x = x;
	}
	void set_blue_x_pos(int x) {
		this->cur_blue_x = x;
	}
	void set_red_y_pos(int y) {
		this->cur_red_y = y;
	}
	void set_blue_y_pos(int y) {
		this->cur_blue_y = y;
	}

	int get_winner() {
		return winner;
	}

	int get_red_x_pos() {
		return this->cur_red_x;
	}
	int get_blue_x_pos() {
		return this->cur_blue_x;
	}
	int  get_red_y_pos() {
		return this->cur_red_y;
	}
	int  get_blue_y_pos() {
		return this->cur_blue_y;
	}

	void next_turn() {
		red_turn = !red_turn;
	}

	bool make_move(int direction) {
		if (!correct_move(direction)) {
			return false;
		}
		int enemy, ally, pos_x, pos_y;
		if (red_turn) {
			ally = 1;
			enemy = 2;
			pos_x = cur_red_x;
			pos_y = cur_red_y;
		}
		else {
			ally = 2;
			enemy = 1;
			pos_x = cur_blue_x;
			pos_y = cur_blue_y;
		}
		int new_pos_x = pos_x, new_pos_y = pos_y;
		if (direction == 0) {
			int counter = 0;
			int copy_pos_x = pos_x;
			while (copy_pos_x >= 0) {
				if (gridView[copy_pos_x][pos_y]) {
					counter++;
				}
				copy_pos_x--;
			}
			copy_pos_x = pos_x;
			while (counter > 0) {
				if (copy_pos_x == -1 || gridView[copy_pos_x][pos_y] == enemy) {
					counter = 0;
				}
				else if (gridView[copy_pos_x][pos_y] == 0) {
					new_pos_x = copy_pos_x;
					counter--;
				}
				copy_pos_x--;
			}
		}
		else if (direction == 1) {
			int counter = 0;
			int copy_pos_x = pos_x;
			while (copy_pos_x <= 7) {
				if (gridView[copy_pos_x][pos_y]) {
					counter++;
				}
				copy_pos_x++;
			}
			copy_pos_x = pos_x;
			while (counter > 0) {
				if (copy_pos_x == 8 || gridView[copy_pos_x][pos_y] == enemy) {
					counter = 0;
				}
				else if (gridView[copy_pos_x][pos_y] == 0) {
					new_pos_x = copy_pos_x;
					counter--;
				}
				copy_pos_x++;
			}
		}
		else if (direction == 2) {
			int counter = 0;
			int copy_pos_y = pos_y;
			while (copy_pos_y >= 0) {
				if (gridView[pos_x][copy_pos_y]) {
					counter++;
				}
				copy_pos_y--;
			}
			copy_pos_y = pos_y;
			while (counter > 0) {
				if (copy_pos_y == -1 || gridView[pos_x][copy_pos_y] == enemy) {
					counter = 0;
				}
				else if (gridView[pos_x][copy_pos_y] == 0) {
					new_pos_y = copy_pos_y;
					counter--;
				}
				copy_pos_y--;
			}
		}
		else if (direction == 3) {
			int counter = 0;
			int copy_pos_y = pos_y;
			while (copy_pos_y <= 7) {
				if (gridView[pos_x][copy_pos_y]) {
					counter++;
				}
				copy_pos_y++;
			}
			copy_pos_y = pos_y;
			while (counter > 0) {
				if (copy_pos_y == 8 || gridView[pos_x][copy_pos_y] == enemy) {
					counter = 0;
				}
				else if (gridView[pos_x][copy_pos_y] == 0) {
					new_pos_y = copy_pos_y;
					counter--;
				}
				copy_pos_y++;
			}
		}
		if (pos_x != new_pos_x) {
			gridView[pos_x][pos_y] = 0;
			gridView[new_pos_x][pos_y] = red_turn ? 1 : 2;
			check_closed(new_pos_x-1, pos_y);
			check_closed(new_pos_x+1, pos_y);
			check_closed(new_pos_x, pos_y-1);
			check_closed(new_pos_x, pos_y+1);
			is_end();
			return true;
		}
		if (pos_y != new_pos_y) {
			gridView[pos_x][pos_y] = 0;
			gridView[pos_x][new_pos_y] = red_turn ? 1 : 2;
			check_closed(pos_x - 1, new_pos_y);
			check_closed(pos_x + 1, new_pos_y);
			check_closed(pos_x, new_pos_y - 1);
			check_closed(pos_x, new_pos_y + 1);
			is_end();
			return true;
		}
		return false;
	}

	int is_end() {
		if (winner) {
			return winner;
		}
		std::set<std::pair<int, int>>elements;
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (gridView[i][j] == 1) {
					elements.insert({ i,j });
					check_elements(i, j, 1,elements);
					if (elements.size() == 8) {
						winner = 1;
						return 1;
					}
					else {
						elements.clear();
						break;
					}
				}
			}
		}
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (gridView[i][j] == 2) {
					elements.insert({ i,j });
					check_elements(i, j, 2, elements);
					if (elements.size() == 8) {
						winner = 2;
						return 2;
					}
					else {
						return 0;
					}
				}
			}
		}
	}

private:

	void check_elements(int x,int y,int el, std::set<std::pair<int, int>>& elements) {
		if (x - 1 >= 0 && gridView[x - 1][y] == el && !elements.count({ x - 1,y })) {
			elements.insert({ x - 1,y });
			check_elements(x - 1, y, el, elements);
		}
		if (x + 1 <= 7 && gridView[x + 1][y] == el && !elements.count({ x +1,y })) {
			elements.insert({ x + 1,y });
			check_elements(x + 1, y, el, elements);
		}
		if (y - 1 >= 0 && gridView[x][y - 1] == el && !elements.count({ x,y-1 })) {
			elements.insert({ x,y-1 });
			check_elements(x, y-1, el, elements);
		}
		if (y + 1 <= 7 && gridView[x][y + 1] == el && !elements.count({ x,y+1 })) {
			elements.insert({ x,y+1 });
			check_elements(x, y+1, el, elements);
		}
	}

	bool check_closed(int i,int j) {
		if (i >= 0 && i <= 7 && j >= 0 && j <= 7 && gridView[i][j]) {
			int enemy = gridView[i][j] == 1 ? 2 : 1;
			if (i - 1 >= 0 && gridView[i - 1][j] != enemy) {
				return false;
			}
			if (i + 1 <= 7 && gridView[i + 1][j] != enemy) {
				return false;
			}
			if (j - 1 >= 0 && gridView[i][j - 1] != enemy) {
				return false;
			}
			if (j + 1 <= 7 && gridView[i][j + 1] != enemy) {
				return false;
			}
			this->winner = enemy;
			return true;
		}
		return false;
	}

	bool correct_move(int direction) {
		if (red_turn && cur_red_x != -1) {
			if (direction == 0 && cur_red_x > 0) {
				return true;
			}
			if (direction == 1 && cur_red_x < 7) {
				return true;
			}
			if (direction == 2 && cur_red_y > 0) {
				return true;
			}
			if (direction == 3 && cur_red_y < 7) {
				return true;
			}
		}
		if (!red_turn && cur_blue_x != -1) {
			if (direction == 0 && cur_blue_x > 0) {
				return true;
			}
			if (direction == 1 && cur_blue_x < 7) {
				return true;
			}
			if (direction == 2 && cur_blue_y > 0) {
				return true;
			}
			if (direction == 3 && cur_blue_y < 7) {
				return true;
			}
		}
		return false;
	}

	bool red_turn;
	int cur_red_x;
	int cur_red_y;
	int cur_blue_x;
	int cur_blue_y;
	int gridView[8][8];
	int winner;
};

int main(){
	sf::Texture texture;
	sf::Texture background_texture;
	sf::Sprite sprite;
	sf::Sprite background_sprite;

	texture.loadFromFile("texture.png");
	background_texture.loadFromFile("background.jpg");
	sprite.setTexture(texture);
	background_sprite.setTexture(background_texture);

	int w = 80;
	Game game;
	sf::RenderWindow window(sf::VideoMode(640, 690), "Docking");

	sf::Font font;
	font.loadFromFile("sansation.ttf");

	sf::Text text("N - New Game", font, 20);
	text.setPosition(20, 10);

	sf::Text red_won("Red player win!", font, 90);
	red_won.setFillColor(sf::Color::Red);
	sf::Text blue_won("Blue player win!", font, 90);
	blue_won.setFillColor(sf::Color::Blue);

	while (window.isOpen())
	{

		sf::Vector2i pos = sf::Mouse::getPosition(window);
		int x = pos.x / w;
		int y = (pos.y-50) / w;

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			if (event.type == sf::Event::MouseButtonPressed) {
				if (event.key.code == sf::Mouse::Left) {
					if (game.is_red_turn()) {
						if (game.get_element(x, y) == 1) {
							game.set_red_x_pos(x);
							game.set_red_y_pos(y);
						}
					}
					else {
						if (game.get_element(x, y) == 2) {
							game.set_blue_x_pos(x);
							game.set_blue_y_pos(y);
						}
					}
				}
			}
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::N) {
					game.restore();
				}
				else if(!game.get_winner()){
					bool success = false;
					if (event.key.code == sf::Keyboard::Left) {
						success = game.make_move(0);
					}
					if (event.key.code == sf::Keyboard::Right) {
						success = game.make_move(1);
					}
					if (event.key.code == sf::Keyboard::Up) {
						success = game.make_move(2);
					}
					if (event.key.code == sf::Keyboard::Down) {
						success = game.make_move(3);
					}
					if (success) {
						game.next_turn();
						game.set_blue_x_pos(-1);
						game.set_red_x_pos(-1);
					}
				}
			}
		}
		window.clear(sf::Color::Black);
		background_sprite.setPosition(0, 50);
		window.draw(background_sprite);
		window.draw(text);
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++)
			{
				sprite.setPosition(i * w, j * w + 50);

				if (game.get_element(i, j) == 1) {
					sprite.setColor(sf::Color::Red);
				}
				else if (game.get_element(i, j) == 2) {
					sprite.setColor(sf::Color::Blue);
				}

				if (game.is_red_turn() && i == game.get_red_x_pos() && j == game.get_red_y_pos()) {
					sprite.setColor(sf::Color(255, 0, 0, 200));
				}
				else if (!game.is_red_turn() && i == game.get_blue_x_pos() && j == game.get_blue_y_pos()) {
					sprite.setColor(sf::Color(0, 0, 255, 200));
				}
				if (game.get_element(i, j)) {
					window.draw(sprite);
				}
			}
		}

		if (game.get_winner() == 1) {
			red_won.setPosition(4, 300);
			window.draw(red_won);
		}
		else if (game.get_winner() == 2) {
			blue_won.setPosition(4, 300);
			window.draw(blue_won);
		}

		window.display();
	}
	return 0;
}