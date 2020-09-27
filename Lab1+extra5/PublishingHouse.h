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

//https://stackoverflow.com/questions/1102392/how-can-i-use-stdmaps-with-user-defined-types-as-key

using book_id = std::string;
using character_id = std::string;

/*namespace std
{
	template<> struct  less<book_id>
	{
		bool operator() (const book_id& lhs, const book_id& rhs) const
		{
			return *lhs < *rhs;
		}
	};
}

namespace std
{
	template<> struct  less<character_id>
	{
		bool operator() (const character_id& lhs, const character_id& rhs) const
		{
			return *lhs < *rhs;
		}
	};
}*/

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