#include "BookCharacter.h"
//#include "BookCharacter.h"
//
//template <typename id>
//BookCharacter<id>::BookCharacter(const std::string& name)
//	: default_name(name) {}
//
//template <typename id>
//BookCharacter<id>::BookCharacter(const std::string& default_name,
//	const std::set<std::string>& names)
//	: default_name(default_name),names(names) {}
//
///*BookCharacter& BookCharacter::operator=(const BookCharacter& another)
//{
//	this->default_name = another.default_name;
//	this->books_and_roles = another.books_and_roles;
//	return *this;
//}*/
//
//template <typename id>
//void BookCharacter<id>::promote(const id& book)
//{
//	uint16_t& level = books_and_roles.at(book);
//	level = std::max(level + 1, 3);
//}
//
//template <typename id>
//void BookCharacter<id>::decrease(const id& book)
//{
//	uint16_t& level = books_and_roles.at(book);
//	level = std::min(level - 1, 1);
//}
//
//template <typename id>
//void BookCharacter<id>::update_role(const id& book, const uint16_t level)
//{
//	books_and_roles[book] = level;
//}
//
//template <typename id>
//std::set<Book> BookCharacter<id>::get_all_books() const
//{
//	std::set<Book>answer;
//	for (auto it = books_and_roles.begin(); it != books_and_roles.end(); it++) {
//		answer.insert(it->first);
//	}
//	return answer;
//}
//
//template <typename id>
//std::set<std::string> BookCharacter<id>::get_all_names() const
//{
//	return this->names;
//}
//
//template <typename id>
//void BookCharacter<id>::set_names(const std::set<std::string>& names)
//{
//	this->names = names;
//	this->names.insert(default_name);
//}
//
//template <typename id>
//void BookCharacter<id>::erase_book(const id& book)
//{
//	books_and_roles.erase(book);
//}
//
//template <typename id>
//std::string BookCharacter<id>::get_default_name() const
//{
//	return this->default_name;
//}

std::ostream& operator<<(std::ostream& os, const Role& role)
{
    if (role == Role::main) {
        os << "main";
    }
    else if (role == Role::secondary) {
        os << "secondary";
    }
    else if (role == Role::episodic) {
        os << "episodic";
    }
    return os;
}
