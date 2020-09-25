#include "BookCharacter.h"
//
//BookCharacter::BookCharacter(const std::string name)
//	: default_name(name) {}
//
//BookCharacter& BookCharacter::operator=(BookCharacter& another)
//{
//	this->default_name = another.default_name;
//	this->books_and_roles = another.books_and_roles;
//}
//
//void BookCharacter::promote(const Book& book)
//{
//	uint16_t& level = books_and_roles.at(book).role_level;
//	level = std::max(level + 1, 3);
//}
//
//void BookCharacter::decrease(const Book& book)
//{
//	uint16_t& level = books_and_roles.at(book).role_level;
//	level = std::min(level - 1, 1);
//}
//
//void BookCharacter::update_role(const Book& book, const Role& role)
//{
//	if (/*!check_availability(book) && */role.names.size() > 0) {
//		books_and_roles[book] = role;
//	}
//	else {
//		//throw
//	}
//}
//
//void BookCharacter::update_role(const Book& book, const std::string& name, const uint16_t level)
//{
//	//if (!check_availability(book)) {
//	books_and_roles[book] = { {name},level };
//	//}
//	//else {
//		//throw
//	//}
//}
//
//void BookCharacter::update_role(const Book& book, const uint16_t level)
//{
//	//if (!check_availability(book)) {
//	books_and_roles[book] = { {default_name},level };
//	//}
//	//else {
//		//throw
//	//}
//}
//
//std::set<Book> BookCharacter::get_all_books() const
//{
//	std::set<Book>answer;
//	for (auto it = books_and_roles.begin(); it != books_and_roles.end(); it++) {
//		answer.insert(it->first);
//	}
//	return answer;
//}
//
//std::set<std::string> BookCharacter::get_all_names() const
//{
//	std::set<std::string>answer;
//	for (auto it = books_and_roles.begin(); it != books_and_roles.end(); it++) {
//		answer.insert(it->second.names.begin(), it->second.names.end());
//	}
//	return answer;
//}
//
//bool BookCharacter::check_availability(const Book& book) const
//{
//	auto check = books_and_roles.find(book);
//	return check != end(books_and_roles);
//}
//
//void BookCharacter::erase_role(const Book& book)
//{
//	books_and_roles.erase(book);
//}
//
//void BookCharacter::set_default_name(const std::string& new_name)
//{
//	this->default_name = new_name;
//}

std::string BookCharacter::get_default_name() const
{
	return this->default_name;
}