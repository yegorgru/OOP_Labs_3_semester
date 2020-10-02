#pragma once

#include "Book.h"
#include "BookCharacter.h"
#include "Date.h"

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

	void ban_book(const book_id&);
	void ban_character(const character_id&);

	std::vector<std::vector<Book>>get_series();

	void add_character_in_book(const character_id& id_character, const book_id& id_book, Role role);
	void erase_character_from_book(const character_id& id_character, const book_id& id_book);

	BookCharacter<book_id> get_character(const character_id&);
	Book get_book(const  book_id&);

	void promote_role(const character_id&, const book_id&);
	void decrease_role(const character_id&, const  book_id&);
	Role get_role(const character_id&, const  book_id&);

	std::set<character_id> get_characters(const book_id&);
	std::set<character_id> get_important_characters(const book_id&);

	std::set<book_id> get_books(const  character_id&);
private:
	std::map<book_id, std::set<character_id>>books_characters;
	std::map<book_id, std::set<character_id>>books_important_characters;

	std::map<book_id, Book>books;
	std::map<character_id, BookCharacter<book_id>>characters;

	friend bool operator==(const PublishingHouse& lhs, const PublishingHouse& rhs);
	friend std::ostream& operator<<(std::ostream&, PublishingHouse PHouse);
};

bool operator==(const PublishingHouse& lhs, const PublishingHouse& rhs);

std::ostream& operator<<(std::ostream&, PublishingHouse PHouse);

template<typename T>
bool is_subset(std::set<T> lesser, std::set<T> bigger) {
	if (lesser.size() == 0 && bigger.size()!=0) {
		return false;
	}
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