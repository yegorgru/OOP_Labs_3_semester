#pragma once


#include "Book.h"

#include <string>
#include <set>
#include <map>
#include <cstdint>
#include <algorithm>
#include <vector>

class BookCharacter
{
public:
		BookCharacter(const std::string name);
	
		BookCharacter& operator=(BookCharacter& another);
	
		void promote(const Book& book);
		void decrease(const Book& book);

		void update_role(const Book& book, const uint16_t level);
	
		void erase_book(const Book& book);
	
		std::string get_default_name() const;
	
		bool check_availability(const Book& book) const;
	
		std::set<Book> get_all_books() const;
		std::set<std::string> get_all_names() const;
private:

	std::map<Book, uint16_t>books_and_roles;

	std::string default_name;

	std::set<std::string>names;
};

//https://stackoverflow.com/questions/1102392/how-can-i-use-stdmaps-with-user-defined-types-as-key

namespace std
{
	template<> struct  less<BookCharacter>
	{
		bool operator() (const BookCharacter& lhs, const BookCharacter& rhs) const
		{
			return lhs.get_default_name() < rhs.get_default_name();
		}
	};
}