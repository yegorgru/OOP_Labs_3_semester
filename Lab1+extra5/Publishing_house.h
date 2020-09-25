#pragma once

#include "Book.h"
#include "BookCharacter.h"
#include "Date.h"

#include <iterator>
#include <set>
#include <cstdint>
#include <utility>

class Publishing_house
{
public:
	Publishing_house();

	Date release_book(const std::string& name, const std::set<std::string>& authors,
		const std::uint32_t pages, const std::string& annotation,
		const int number, std::set<std::pair<size_t, uint16_t>>characters);
	

private:

	std::map<size_t, BookCharacter>id_characters;
	std::map<size_t, size_t>characters_series;

	std::map<Date, Book>id_books;
	std::map<Date, uint32_t>books_number;

	std::map<size_t, std::set<Book>>id_series;

	Date cur_date;
};