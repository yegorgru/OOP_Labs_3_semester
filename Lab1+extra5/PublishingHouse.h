#pragma once

#include "Book.h"
#include "BookCharacter.h"
#include "Date.h"

#include <iterator>
#include <set>
#include <cstdint>
#include <utility>

class PublishingHouse
{
public:
	PublishingHouse();

	Date release_book(const std::string& name, const std::set<std::string>& authors,
		const std::uint32_t pages, const std::string& annotation,
		const int number, std::set<std::pair<size_t, uint16_t>>characters);
	
	size_t add_character(const std::string& name);
	size_t add_character(const std::string& default_name,const std::set<std::string>& names);

	void sell_books(const Date& id, uint32_t number = 1);

	Book get_book(const Date& id) const;
	BookCharacter get_character(const size_t id) const;

	std::vector<std::vector<Book>>get_series() const;

	void update_book_name(const Date& id, const std::string& name);
	void update_book_authors(const Date& id, const std::set<std::string>& authors);
	void update_book_pages(const Date& id, const uint32_t pages);
	void update_book_annotation(const Date& id, const std::string& annotation);

	void update_character_default_name(size_t id,const std::string& name);
	void update_character_names(size_t id,const std::set<std::string>& names);

	void ban_book(const Date& date);

	void erase_character_from_series(size_t character, size_t series);
private:
	void insert_character_in_series(size_t character, size_t series);


	std::map<size_t, BookCharacter>id_characters;
	std::map<size_t, std::map<size_t,uint32_t>>characters_series;

	std::map<Date, Book>id_books;
	std::map<Date, uint32_t>books_number;
	std::map<Date, std::set<size_t>>books_important_characters;

	std::map<size_t, std::set<Date>>id_series;
	std::map<size_t, std::set<size_t>>series_characters;

	Date cur_date;

	size_t max_character_id;
	size_t max_series_id;
};