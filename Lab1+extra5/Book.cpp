#include "Book.h"

Book::Book()
	: pages(0) {}

Book::Book(const std::string& name, const std::set<std::string>& authors,
	const Date& date, const std::uint32_t pages, const std::string& annotation)
	: name(name),authors(authors),date(date),pages(pages),annotation(annotation) {}

Book::Book(const std::string& name, const std::string& author,
	const Date& date, const std::uint32_t pages, const std::string& annotation)
	: name(name), authors({ author }), date(date), pages(pages), annotation(annotation) {}

//void Book::set_name(const std::string& name)
//{
//	this->name = name;
//}

void Book::set_author(const std::string author)
{
	this->authors = { author };
}

void Book::set_authors(std::set<std::string>& authors)
{
	this->authors = authors;
}

void Book::set_date(const Date& date)
{
	this->date = date;
}

void Book::set_pages(const std::uint32_t pages)
{
	this->pages = pages;
}

void Book::set_annotation(const std::string& annotation)
{
	this->annotation = annotation;
}

std::string Book::get_name() const
{
	return this->name;
}

std::set<std::string> Book::get_authors() const
{
	return this->authors;
}

Date Book::get_date() const
{
	return this->date;
}

uint32_t Book::get_pages() const
{
	return this->pages;
}

std::string Book::get_annotation() const
{
	return this->annotation;
}

void Book::add_author(const std::string& new_author)
{
	this->authors.insert(new_author);
	std::set<Book>s;
	std::map<Book, int>m;
	//m[Book()] = 2;
}

void Book::erase_author(const std::string& author)
{
	this->authors.erase(author);
}

void Book::add_pages(std::uint32_t new_pages_number)
{
	this->pages += new_pages_number;
}

void Book::erase_pages(std::uint32_t erased_pages_number)
{
	uint32_t new_pages_number = this->pages - erased_pages_number;
	if (new_pages_number > 0) {
		this->pages = new_pages_number;
	}
	else {
		//throw
	}
}

void Book::insert_text_to_annotation(const std::string& text,size_t pos)
{
	try
	{
		this->annotation.insert(pos, text);
	}
	catch (const std::exception&)
	{
		//throw
	}
}

void Book::erase_text_from_annotation(const std::size_t begin_character, const std::size_t end_character)
{
	try
	{
		this->annotation.erase(this->annotation.begin()+begin_character,
			this->annotation.end()+end_character);
	}
	catch (const std::exception&)
	{
		//throw
	}
}

bool operator<(const Book& lhs, const Book& rhs)
{
	return lhs.get_date() < rhs.get_date();
}

