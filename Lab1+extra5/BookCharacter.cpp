#include "BookCharacter.h"

BookCharacter::BookCharacter(const std::string& name)
	: default_name(name) {}

BookCharacter::BookCharacter(const std::string& default_name,
	const std::set<std::string>& names)
	: default_name(default_name),names(names) {}

BookCharacter& BookCharacter::operator=(const BookCharacter& another)
{
	this->default_name = another.default_name;
	this->books_and_roles = another.books_and_roles;
	return *this;
}

void BookCharacter::promote(const Book& book)
{
	uint16_t& level = books_and_roles.at(book);
	level = std::max(level + 1, 3);
}

void BookCharacter::decrease(const Book& book)
{
	uint16_t& level = books_and_roles.at(book);
	level = std::min(level - 1, 1);
}

void BookCharacter::update_role(const Book& book, const uint16_t level)
{
	books_and_roles[book] = level;
}

std::set<Book> BookCharacter::get_all_books() const
{
	std::set<Book>answer;
	for (auto it = books_and_roles.begin(); it != books_and_roles.end(); it++) {
		answer.insert(it->first);
	}
	return answer;
}

std::set<std::string> BookCharacter::get_all_names() const
{
	return this->names;
}

void BookCharacter::set_default_name(const std::string& name)
{
	this->default_name = name;
}

void BookCharacter::set_names(const std::set<std::string>& names)
{
	this->names = names;
}

bool BookCharacter::check_availability(const Book& book) const
{
	return books_and_roles.find(book) != end(books_and_roles);
}

void BookCharacter::erase_book(const Book& book)
{
	books_and_roles.erase(book);
}

std::string BookCharacter::get_default_name() const
{
	return this->default_name;
}