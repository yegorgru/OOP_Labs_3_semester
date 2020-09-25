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
	struct Role {
		std::set<std::string>names;
		uint16_t role_level;

		Role(const std::set<std::string>& names, const uint16_t level) :
			names(names), role_level(level) {}
	};

public:
		BookCharacter(const std::string name);
	
		BookCharacter& operator=(BookCharacter& another);
	
		void promote(const Book& book);
		void decrease(const Book& book);
	
		void update_role(const Book& book, const Role& role);
		void update_role(const Book& book, const std::string& name, const uint16_t level);
		void update_role(const Book& book, const uint16_t level);
	
		void erase_role(const Book& book);
	
		//void set_default_name(const std::string& new_name);
		std::string get_default_name() const;
	
		bool check_availability(const Book& book) const;
	
	std::vector<Book> get_all_books() const;
		std::set<std::string> get_all_names() const;
private:

	std::map<Book, int>books_and_roles;

	std::string default_name;
};


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