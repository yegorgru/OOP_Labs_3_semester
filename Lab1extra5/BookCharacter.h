/**
\file
\brief h file of class BookCharacter

definitions and implementations of BookCharacter methods and operators, enum class Role are here
*/

#pragma once


#include "Book.h"

#include <string>
#include <set>
#include <map>
#include <cstdint>
#include <algorithm>
#include <vector>
#include <list>
#include <iostream>

/**
\brief class for marking roles of characters in books
*/
enum class Role {
	main,		///< main role
	secondary,	///< secondary role
	episodic	///< episodic role
};

/**
Operator << for Role in streams
*/
std::ostream& operator << (std::ostream& os, const Role& role);

/**
\brief class for storing information about book characters

has information about:
- default name
- all names in all book
- roles in all books
*/

template <typename id>
class BookCharacter
{
public:

	/**
	\brief basic constructor of BookCharacter.

	All fields are empty
	*/
	BookCharacter();

	/**
	\brief constructor of BookCharacter with known default name
	*/
	BookCharacter(const std::string& name);

	/**
	\brief constructor of BookCharacter with known default name and all names
	*/
	BookCharacter(const std::string& default_name,
		const std::set<std::string>& names);
	
	/**
	\brief promotes character's role in some book

	role can't be bigger than Role::main
	\param book id of book to promote role in it
	*/
	void promote(const id& book);

	/**
	\brief decreases character's role in some book

	role can't be lesser than Role::episodic
	\param book id of book to promote role in it
	*/
	void decrease(const id& book);

	/**
	\brief adds new book(and role in it) or updated role in old book

	\param book id of book
	\param role role in that book
	*/
	void update_role(const id& book, const Role role);
	
	/**
	\brief erases book(and role in it)

	\param book id of erased book
	*/
	void erase_book(const id& book);
	
	/**
	\return default name of character
	*/
	std::string get_default_name() const;

	/**
	\return id of all books where is this character
	*/
	std::set<id> get_all_books() const;

	/**
	\return names of character in all books (+ default name)
	*/
	std::set<std::string> get_all_names() const;

	/**
	\param book book id with searched role
	\return role of character in all books (+ default name)
	*/
	Role get_role(const id& book) const;

	/**
	\brief sets all names of character

	\param names set of all names
	*/
	void set_names(const std::set<std::string>& names);

	/**
	\brief adds new name of character

	\param name new name of character
	*/
	void add_name(const std::string& name);

	/**
	\brief erases name of character

	\param name erased name of character
	*/
	void erase_name(const std::string& name);
private:

	std::map<id, Role>books_and_roles;	///<books id and role of character in those books
		
	std::string default_name;			///<default name of character

	std::set<std::string>names;			///<all names of character
};

/**
Operator << for BookCharacter in streams
*/
template <typename id>
std::ostream& operator<<(std::ostream& os, const BookCharacter<id>& character);

/**
Operator == for BookCharacters
*/
template <typename id>
bool operator==(const BookCharacter<id>& lhs, const BookCharacter<id>& rhs) {
	return lhs.get_default_name() == rhs.get_default_name();
}

/**
Operator < for BookCharacters
*/
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
inline Role BookCharacter<id>::get_role(const id& book) const
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

template <typename id>
std::ostream& operator<<(std::ostream& os, const BookCharacter<id>& character) {
	os << "Default name: " << character.get_default_name() << std::endl << "All names: ";
	for (const auto& i : character.get_all_names()) {
		os << i << " ";
	}
	os << std::endl << "All roles:" << std::endl;
	for (const auto& i : character.get_all_books()) {
		os << "Book:" << i << " Role:" << character.get_role(i) << std::endl;
	}
	return os;
}
