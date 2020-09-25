#pragma once

#include <string>
#include <set>
#include <cstdint>
#include <vector>
#include <map>

#include "Date.h"

class Book
{
public:
	Book();
	Book(const std::string& name, const std::set<std::string>& authors, 
		const Date& date, const std::uint32_t pages, const std::string& annotation);
	Book(const std::string& name, const std::string& author,
		const Date& date, const std::uint32_t pages, const std::string& annotation);


	//void set_name(const std::string& name);
	void set_author(const std::string author);
	void set_authors(std::set<std::string>& authors);
	void set_date(const Date& date);
	void set_pages(const std::uint32_t pages);
	void set_annotation(const std::string& annotation);

	std::string get_name() const;
	std::set<std::string> get_authors() const;
	Date get_date() const;
	uint32_t get_pages() const;
	std::string get_annotation() const;

	void add_author(const std::string& new_author);

	void erase_author(const std::string& author);

	//If date - class, methods for date

	void add_pages(std::uint32_t new_pages_number);
	void erase_pages(std::uint32_t erased_pages_number);

	void insert_text_to_annotation(const std::string& text, size_t pos);
	void erase_text_from_annotation(const std::size_t begin_character,
		const std::size_t end_character);

private:
	std::string name;
	std::set<std::string> authors;
	Date date;				
	std::uint32_t pages;
	std::string annotation;			//Maybe max number of characters
};

//https://stackoverflow.com/questions/1102392/how-can-i-use-stdmaps-with-user-defined-types-as-key

namespace std
{
	template<> struct  less<Book>
	{
		bool operator() (const Book& lhs, const Book& rhs) const
		{
			if (lhs.get_date() == rhs.get_date()) {
				return lhs.get_name() < rhs.get_name();
			}
			else {
				return lhs.get_date() < rhs.get_date();
			}
		}
	};
}