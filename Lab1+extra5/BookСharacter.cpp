#include "Book—haracter.h"

Book—haracter::Book—haracter(const std::string name)
	: default_name(name) {}

void Book—haracter::promote(const Book& book)
{
	uint16_t& level = books_and_roles.at(book).role_level;
	level = std::max(level+1, 3);
}

void Book—haracter::decrease(const Book& book)
{
	uint16_t& level = books_and_roles.at(book).role_level;
	level = std::min(level-1, 1);
}

void Book—haracter::update_role(const Book& book, const Role& role)
{
	if (/*!check_availability(book) && */role.names.size()>0) {
		books_and_roles[book] = role;
	}
	else {
		//throw
	}
}

void Book—haracter::update_role(const Book& book, const std::string& name, const uint16_t level)
{
	//if (!check_availability(book)) {
	books_and_roles[book] = { {name},level };
	//}
	//else {
		//throw
	//}
}

void Book—haracter::update_role(const Book& book, const uint16_t level)
{
	//if (!check_availability(book)) {
	books_and_roles[book] = { {default_name},level };
	//}
	//else {
		//throw
	//}
}

std::set<Book> Book—haracter::get_all_books() {
	std::set<Book>answer;
	for (auto it = books_and_roles.begin(); it != books_and_roles.end(); it++) {
		answer.insert(it->first);
	}
	return answer;
}

std::set<std::string> Book—haracter::get_all_names()
{
	std::set<std::string>answer;
	for (auto it = books_and_roles.begin(); it != books_and_roles.end(); it++) {
		answer.insert(it->second.names.begin(), it->second.names.end());
	}
	return answer;
}

bool Book—haracter::check_availability(const Book& book)
{
	auto check = books_and_roles.find(book);
	return check != end(books_and_roles);
}

void Book—haracter::set_default_name(const std::string& name)
{
	default_name = name;
}

void Book—haracter::erase_role(const Book& book)
{
	books_and_roles.erase(book);
}