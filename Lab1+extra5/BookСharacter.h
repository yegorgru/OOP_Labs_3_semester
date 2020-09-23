#pragma once

#include "Book.h"

#include <string>
#include <set>
#include <map>
#include <cstdint>
#include <algorithm>
#include <vector>

struct Role {
	std::set<std::string>names;
	uint16_t role_level;

	Role(const std::set<std::string>& names, const uint16_t level) :
		names(names), role_level(level) {}
};

class BookСharacter
{
public:
	BookСharacter(const std::string name);

	void promote(const Book& book);
	void decrease(const Book& book);

	void update_role(const Book& book, const Role& role);
	void update_role(const Book& book, const std::string& name, const uint16_t level);
	void update_role(const Book& book, const uint16_t level);
	
	void erase_role(const Book& book);

	void set_default_name(const std::string& name);
	bool check_availability(const Book& book);

	std::set<Book> get_all_books();
	std::set<std::string> get_all_names();
private:

	std::map<Book, Role>books_and_roles;

	std::string default_name;
};

//Тест: видалити роль, ім'я з цієї ролі є в іншій ролі, воно не має пропасти з всіх імен