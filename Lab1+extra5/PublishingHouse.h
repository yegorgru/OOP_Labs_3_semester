#pragma once

#include "Book.h"
#include "BookCharacter.h"
#include "Date.h"

/*#include <iterator>
#include <set>
#include <cstdint>
#include <utility>*/

#include <list>
#include <map>
#include <algorithm>

using book_id = std::string;
using character_id = std::string;

class PublishingHouse
{
public:
	PublishingHouse();
	
	book_id add_book(const Book& book);
	character_id add_character(const BookCharacter<book_id>& character);

	void ban_book(book_id&);
	void ban_character(character_id&);

	std::vector<std::vector<Book>>get_series();

	void add_character_in_book(character_id id_character, book_id id_book, Role role);

	BookCharacter<book_id>& get_character(character_id);
	Book& get_book(book_id);
private:
	std::map<book_id, std::set<character_id>>books_characters;
	std::map<book_id, std::set<character_id>>books_important_characters;

	std::map<book_id, Book>books;
	std::map<character_id, BookCharacter<book_id>>characters;
};

template<typename T>
bool is_subset(std::set<T> lesser, std::set<T> bigger) {
	for (const auto& i : lesser) {
		if (bigger.find(i) == end(bigger)) {
			return false;
		}
	}
	return true;
}

template<typename T>
bool is_subvector(std::vector<T> lesser, std::vector<T> bigger) {
	for (const auto& i : lesser) {
		if (find(bigger.begin(),bigger.end(),i) == end(bigger)) {
			return false;
		}
	}
	return true;
}