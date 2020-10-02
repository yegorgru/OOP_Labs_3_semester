#include "Book.h"

Book::Book()
	: pages(0) {}

Book::Book(const std::string& name, const std::set<std::string>& authors,
	const Date& date, const std::uint32_t pages, const std::string& annotation)
	: name(name),authors(authors),date(date),pages(pages),annotation(annotation) {}


void Book::set_authors(const std::set<std::string>& authors)
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
}

void Book::erase_author(const std::string& author)
{
	this->authors.erase(author);
}

bool operator<(const Book& lhs, const Book& rhs)
{
	if (lhs.get_name() == rhs.get_name()) {
		if (lhs.get_authors() == rhs.get_authors()) {
			if (lhs.get_date() == rhs.get_date()) {
				if (lhs.get_pages() == rhs.get_pages()) {
					return lhs.get_annotation() < rhs.get_annotation();
				}
				else {
					return lhs.get_pages() < rhs.get_pages();
				}
			}
			else {
				return lhs.get_date() < rhs.get_date();
			}
		}
		else {
			return lhs.get_authors() < rhs.get_authors();
		}
	}
	else {
		return lhs.get_name() == rhs.get_name();
	}
}

bool operator==(const Book& lhs, const Book& rhs) {
	if (lhs.get_name() != rhs.get_name()) {
		return false;
	}
	if (lhs.get_authors() != rhs.get_authors()) {
		return false;
	}
	if (lhs.get_annotation() != rhs.get_annotation()) {
		return false;
	}
	if (lhs.get_date() != rhs.get_date()) {
		return false;
	}
	if (lhs.get_pages() != rhs.get_pages()) {
		return false;
	}
	return lhs.get_name() == rhs.get_name() &&
		lhs.get_authors() == rhs.get_authors() &&
		lhs.get_annotation() == rhs.get_annotation() &&
		lhs.get_date() == rhs.get_date() &&
		lhs.get_pages() == rhs.get_pages();
}

std::ostream& operator<<(std::ostream& os, const Book& book)
{
	os << "Name: "<<book.get_name()<<"\n";
	os << "Authors: ";
	for (const std::string& author : book.get_authors()) {
		os << author << " ";
	}
	os << "\nDate: " << book.get_date() << "\n";
	os << "Pages: " << book.get_pages()<<"\n";
	os << "Annotation: " << book.get_annotation();
	return os;
}
