#pragma once

#include <string>
#include <set>
#include <cstdint>
#include <vector>
//#include "BookCharacter.h"
#include <map>

class Book
{
public:
	//Book();
	Book(const std::string& name, const std::set<std::string>& authors, 
		const std::string& date, const std::uint32_t pages, const std::string& annotation);
	Book(const std::string& name, const std::string& author,
		const std::string& date, const std::uint32_t pages, const std::string& annotation);



	void set_name(const std::string& name);
	void set_author(const std::string author);
	void set_authors(std::set<std::string>& authors);
	void set_authors(std::vector<std::string>& authors);
	void set_date(const std::string& date);
	void set_pages(const std::uint32_t pages);
	void set_annotation(const std::string& annotation);
	//void set_characters(const std::set<BookCharacter>characters);
	//void set_characters(const std::vector<BookCharacter>characters);

	std::string get_name() const;
	std::string& get_name();
	std::set<std::string> get_authors() const;
	std::set<std::string>& get_authors();
	std::string get_date() const;
	std::string& get_date();
	uint32_t get_pages() const;
	uint32_t& get_pages();
	std::string get_annotation() const;
	std::string& get_annotation();
	//std::set<BookCharacter> get_characters() const;
	//std::set<BookCharacter>& get_characters();

	void add_author(const std::string& new_author);
	void add_authors(std::set<std::string>& new_authors);
	void add_authors(std::vector<std::string>& new_authors);

	void erase_author(const std::string& author);
	void erase_authors(std::set<std::string>& authors);
	void erase_authors(std::vector<std::string>& authors);

	//If date - class, methods for date

	void add_pages(std::uint32_t new_pages_number);
	void erase_pages(std::uint32_t erased_pages_number);

	void add_prefix_to_annotation(const std::string& prefix);
	void add_suffix_to_annotation(const std::string& suffix);
	void erase_prefix_from_annotation(const std::size_t characters_number);
	void erase_suffix_from_annotation(const std::size_t characters_number);
	void insert_text_to_annotation(const std::string& text, size_t pos);
	void erase_text_from_annotation(const std::size_t begin_character,
		const std::size_t end_character);

	//void add_character(const BookCharacter& character);
	void add_characters(std::set<std::string>& characters);
	void add_characters(std::vector<std::string>& characters);

private:
	std::string name;
	std::set<std::string> authors;
	std::string date;				//Maybe struct or class for date
	std::uint32_t pages;
	std::string annotation;			//Maybe max number of characters
	//std::set<BookCharacter>characters;
};

namespace std
{
	template<> struct  less<Book>
	{
		bool operator() (const Book& lhs, const Book& rhs) const
		{
			return lhs.get_date() < rhs.get_date();
		}
	};
}