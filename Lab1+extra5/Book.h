/**
\file
\brief h file of class Book

definitions of Book methods and operators are here, std::less for Book
*/

#pragma once

#include <string>
#include <set>
#include <cstdint>
#include <vector>
#include <map>
#include <iostream>

#include "Date.h"

/**
\brief class for storing information about books

has information about:
- name
- authors
- release date
- number of pages
- annotation
*/

class Book
{
public:

	/**
	\brief basic constructor of Book.

	All fields are empty, pages = 0
	*/
	Book();

	/**
	\brief constructor for Book with known fields
	*/
	Book(const std::string& name, const std::set<std::string>& authors, 
		const Date& date, const std::uint32_t pages, const std::string& annotation);

	/**
	\brief sets authors of book
	
	\param authors set of authors names
	*/
	void set_authors(const std::set<std::string>& authors);

	/**
	\brief sets release date

	\param date Date of release date
	*/
	void set_date(const Date& date);

	/**
	\brief sets number of pages

	\param pages number of pages
	*/
	void set_pages(const std::uint32_t pages);

	/**
	\brief sets annotation

	\param annotation annotation of book
	*/
	void set_annotation(const std::string& annotation);

	/**
	\return name of book
	*/
	std::string get_name() const;

	/**
	\return authors of book
	*/
	std::set<std::string> get_authors() const;

	/**
	\return date of book
	*/
	Date get_date() const;

	/**
	\return number of pages
	*/
	uint32_t get_pages() const;

	/**
	\return annotation
	*/
	std::string get_annotation() const;

	/**
	\brief adds new author in book

	\param new_author new author name
	*/
	void add_author(const std::string& new_author);

	/**
	\brief removes author from book

	\param author removed author name
	*/
	void erase_author(const std::string& author);

private:
	std::string name;					///<name of book
	std::set<std::string> authors;		///<names of authors
	Date date;							///<release date
	uint32_t pages;						///<number of pages
	std::string annotation;				///<annotation
};

/**
Operator < for Books
*/
bool operator<(const Book& lhs, const Book& rhs);

/**
Operator == for Books
*/
bool operator==(const Book& lhs, const Book& rhs);

/**
Operator << for Books in streams
*/
std::ostream& operator << (std::ostream& os, const Book& book);


/**
\brief supplement namespace std

supplement namespace std with struct less in order to have opportunity to do std::set<Book> or std::map with key Book
*/
namespace std
{
	template<> struct  less<Book>
	{
		bool operator() (const Book& lhs, const Book& rhs) const
		{
			return lhs<rhs;
		}
	};
}