#include "GameModel.h"

GameModel::GameModel():gridView()
{
	for (size_t i = 0; i < 8; i++) {
		for (size_t j = 0; j < 8; j++)
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
	red_turn = true;
	cur_red_position = { -1,-1 };
	cur_blue_position = { -1,-1 };
	winner = 0;
}

void GameModel::restore()
{
	for (size_t i = 0; i < 8; i++) {
		for (size_t j = 0; j < 8; j++)
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
	red_turn = true;
	cur_red_position = { -1,-1 };
	cur_blue_position = { -1,-1 };
	winner = 0;
}

bool GameModel::is_red_turn()
{
	return red_turn;
}

int GameModel::get_element(int x, int y)
{
	return gridView[x][y];
}

void GameModel::set_red_position(int x, int y)
{
	cur_red_position = { x,y };
}

void GameModel::set_blue_position(int x, int y)
{
	cur_blue_position = { x,y };
}

int GameModel::get_winner()
{
	return winner;
}

int GameModel::get_red_x_pos()
{
	return cur_red_position.x;
}

int GameModel::get_blue_x_pos()
{
	return cur_blue_position.x;
}

int GameModel::get_red_y_pos()
{
	return cur_red_position.y;
}

int GameModel::get_blue_y_pos()
{
	return cur_blue_position.y;
}

void GameModel::next_turn()
{
	red_turn = !red_turn;
}

bool GameModel::make_move(int direction)
{
	if (!is_correct_move(direction)) {
		return false;
	}
	int enemy, ally, pos_x, pos_y;
	if (red_turn) {
		ally = 1;
		enemy = 2;
		pos_x = get_red_x_pos();
		pos_y = get_red_y_pos();
	}
	else {
		ally = 2;
		enemy = 1;
		pos_x = get_blue_x_pos();
		pos_y = get_blue_y_pos();
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
		check_closed(new_pos_x - 1, pos_y);
		check_closed(new_pos_x + 1, pos_y);
		check_closed(new_pos_x, pos_y - 1);
		check_closed(new_pos_x, pos_y + 1);
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

bool GameModel::is_end()
{
	if (winner) {
		return winner;
	}
	std::set<std::pair<int, int>>elements;
	for (size_t i = 0; i < 8; i++) {
		for (size_t j = 0; j < 8; j++) {
			if (gridView[i][j] == 1) {
				elements.insert({ i,j });
				check_elements(i, j, 1, elements);
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
	for (size_t i = 0; i < 8; i++) {
		for (size_t j = 0; j < 8; j++) {
			if (gridView[i][j] == 2) {
				elements.insert({ i,j });
				check_elements(i, j, 2, elements);
				if (elements.size() == 8) {
					winner = 2;
					return 1;
				}
				else {
					return 0;
				}
			}
		}
	}
	return 0;
}

void GameModel::check_elements(int x, int y, int el, std::set<std::pair<int, int>>& elements)
{
	if (x - 1 >= 0 && gridView[x - 1][y] == el && !elements.count({ x - 1,y })) {
		elements.insert({ x - 1,y });
		check_elements(x - 1, y, el, elements);
	}
	if (x + 1 <= 7 && gridView[x + 1][y] == el && !elements.count({ x + 1,y })) {
		elements.insert({ x + 1,y });
		check_elements(x + 1, y, el, elements);
	}
	if (y - 1 >= 0 && gridView[x][y - 1] == el && !elements.count({ x,y - 1 })) {
		elements.insert({ x,y - 1 });
		check_elements(x, y - 1, el, elements);
	}
	if (y + 1 <= 7 && gridView[x][y + 1] == el && !elements.count({ x,y + 1 })) {
		elements.insert({ x,y + 1 });
		check_elements(x, y + 1, el, elements);
	}
}

bool GameModel::check_closed(int x, int y)
{
	if (x >= 0 && x <= 7 && y >= 0 && y <= 7 && gridView[x][y]) {
		int enemy = gridView[x][y] == 1 ? 2 : 1;
		if (x - 1 >= 0 && gridView[x - 1][y] != enemy) {
			return false;
		}
		if (x + 1 <= 7 && gridView[x + 1][y] != enemy) {
			return false;
		}
		if (y - 1 >= 0 && gridView[x][y - 1] != enemy) {
			return false;
		}
		if (y + 1 <= 7 && gridView[x][y + 1] != enemy) {
			return false;
		}
		this->winner = enemy;
		return true;
	}
	return false;
}

bool GameModel::is_correct_move(int direction)
{
	if (red_turn && cur_red_position.x != -1) {
		if (direction == 0 && cur_red_position.x > 0) {
			return true;
		}
		if (direction == 1 && cur_red_position.x < 7) {
			return true;
		}
		if (direction == 2 && cur_red_position.y > 0) {
			return true;
		}
		if (direction == 3 && cur_red_position.y < 7) {
			return true;
		}
	}
	if (!red_turn && cur_blue_position.x != -1) {
		if (direction == 0 && cur_blue_position.x > 0) {
			return true;
		}
		if (direction == 1 && cur_blue_position.x < 7) {
			return true;
		}
		if (direction == 2 && cur_blue_position.y > 0) {
			return true;
		}
		if (direction == 3 && cur_blue_position.y < 7) {
			return true;
		}
	}
	return false;
}
