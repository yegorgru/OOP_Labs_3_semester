#pragma once

#include <set>
#include <utility>
#include <array>

class GameModel
{
public:
	GameModel();

	void restore();

	bool is_red_turn();

	int get_element(int x, int y);

	void set_red_position(int x,int y);

	void set_blue_position(int x, int y);

	int get_winner();

	int get_red_x_pos();

	int get_blue_x_pos();

	int  get_red_y_pos();

	int  get_blue_y_pos();

	void next_turn();

	bool make_move(int direction);

	bool is_end();

private:

	void check_elements(int x, int y, int el, std::set<std::pair<int, int>>& elements);

	bool check_closed(int x, int y);

	bool is_correct_move(int direction);

	struct position {
		int x;
		int y;
	};

	bool red_turn;
	position cur_red_position;
	position cur_blue_position;
	std::array<std::array<int,8>,8> gridView;
	int winner;
};

