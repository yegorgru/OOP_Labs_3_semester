#pragma once

#include "Book.h"
#include "BookCharacter.h"

#include <iterator>
#include <set>

class Library
{
public:
	Library(){}

	std::pair<size_t, std::vector<size_t>> add_book(const Book& new_book);
private:
	size_t insert_in_id_books(const Book& book);
	size_t add_character(const BookCharacter& character);

	void insert_book_in_series(const Book& book);
	std::vector<std::vector<std::set<Book>>::const_iterator> get_series_for_book(const Book& book) const;

	std::set<Book>books;

	std::map<size_t, int>characters_number;

	std::map<size_t, Book>id_books;
	std::map<Book, size_t>books_id;

	std::map<size_t, BookCharacter>id_characters;
	std::map<BookCharacter, size_t>characters_id;

	std::map<size_t, std::set<BookCharacter>>books_characters;

	std::vector<std::set<Book>> series;
};

