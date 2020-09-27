#pragma once


#include "Book.h"

#include <string>
#include <set>
#include <map>
#include <cstdint>
#include <algorithm>
#include <vector>
#include <list>

enum class Role {
	main,
	secondary,
	episodic
};

std::ostream& operator << (std::ostream& os, const Role& role);

template <typename id>
class BookCharacter
{
public:
	BookCharacter();
	BookCharacter(const std::string& name);
	BookCharacter(const std::string& default_name,
		const std::set<std::string>& names);

	//BookCharacter& operator=(const BookCharacter& another);
	
	//BookCharacter& operator=(const BookCharacter& another);
	
	void promote(const id& book);
	void decrease(const id& book);

	void update_role(const id& book, const Role role);
	
	void erase_book(const id& book);
	
	std::string get_default_name() const;
	std::set<id> get_all_books() const;
	std::set<std::string> get_all_names() const;
	Role get_role(const id& book);

	void set_names(const std::set<std::string>& names);
	void add_name(const std::string& name);
	void erase_name(const std::string& name);
private:

	std::map<id, Role>books_and_roles;

	std::string default_name;

	std::set<std::string>names;
};

template <typename id>
bool operator==(const BookCharacter<id>& lhs, const BookCharacter<id>& rhs) {
	return lhs.get_default_name() == rhs.get_default_name();
}

template <typename id>
bool operator<(const BookCharacter<id>& lhs, const BookCharacter<id>& rhs) {
	return lhs.get_default_name() < rhs.get_default_name();
}

template<typename id>
inline BookCharacter<id>::BookCharacter() {}

template <typename id>
BookCharacter<id>::BookCharacter(const std::string& name)
	: default_name(name), names({name}) {}

template <typename book_id>
BookCharacter<book_id>::BookCharacter(const std::string& default_name,
	const std::set<std::string>& names)
	: default_name(default_name), names(names) {}

template <typename id>
void BookCharacter<id>::promote(const id& book)
{
	try
	{
		Role& role = books_and_roles.at(book);
		if (role == Role::episodic) {
			role = Role::secondary;
		}
		else if (role == Role::secondary) {
			role = Role::main;
		}
	}
	catch (const std::exception&) {}
}

template <typename id>
void BookCharacter<id>::decrease(const id& book)
{
	try
	{
		Role& role = books_and_roles.at(book);
		if (role == Role::main) {
			role = Role::secondary;
		}
		else if (role == Role::secondary) {
			role = Role::episodic;
		}
	}
	catch (const std::exception&) {}
}

template <typename id>
void BookCharacter<id>::update_role(const id& book, const Role role)
{
	books_and_roles[book] = role;
}

template <typename id>
std::set<id> BookCharacter<id>::get_all_books() const
{
	std::set<id>answer;
	for (auto it = books_and_roles.begin(); it != books_and_roles.end(); it++) {
		answer.insert(it->first);
	}
	return answer;
}

template <typename id>
std::set<std::string> BookCharacter<id>::get_all_names() const
{
	return this->names;
}

template<typename id>
inline Role BookCharacter<id>::get_role(const id& book)
{
	return books_and_roles.at(book);
}

template <typename id>
void BookCharacter<id>::set_names(const std::set<std::string>& names)
{
	this->names = names;
	this->names.insert(default_name);
}

template<typename id>
inline void BookCharacter<id>::add_name(const std::string& name)
{
	this->names.insert(name);
}

template<typename id>
inline void BookCharacter<id>::erase_name(const std::string& name)
{
	this->names.erase(name);
}

template <typename id>
void BookCharacter<id>::erase_book(const id& book)
{
	books_and_roles.erase(book);
}

template <typename id>
std::string BookCharacter<id>::get_default_name() const
{
	return this->default_name;
}
