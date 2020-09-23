#include "Book.h"

Book::Book(const std::string& name, const std::set<std::string>& authors,
	const std::string& date, const std::uint32_t pages, const std::string& annotation)
	: name(name),authors(authors),date(date),pages(pages),annotation(annotation) {}

Book::Book(const std::string& name, const std::string& author, const std::string& date, const std::uint32_t pages, const std::string& annotation)
	: name(name), authors({ author }), date(date), pages(pages), annotation(annotation) {}

void Book::set_name(const std::string& name)
{
	this->name = name;
}

void Book::set_author(const std::string author)
{
	this->authors = { author };
}

void Book::set_authors(std::set<std::string>& authors)
{
	this->authors = authors;
}

void Book::set_authors(std::vector<std::string>& authors)
{
	this->authors = std::set<std::string>(authors.begin(), authors.end());
}

void Book::set_date(const std::string& date)
{
	this->date = date;
}

void Book::set_pages(const std::size_t pages)
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

std::string& Book::get_name()
{
	return this->name;
}

std::set<std::string> Book::get_authors() const
{
	return this->authors;
}

std::set<std::string>& Book::get_authors()
{
	return this->authors;
}

std::string Book::get_date() const
{
	return this->date;
}

std::string& Book::get_date()
{
	return this->date;
}

uint32_t Book::get_pages() const
{
	return this->pages;
}

uint32_t& Book::get_pages()
{
	return this->pages;
}

std::string Book::get_annotation() const
{
	return this->annotation;
}

std::string& Book::get_annotation()
{
	return this->annotation;
}

void Book::add_author(const std::string& new_author)
{
	this->authors.insert(new_author);
}

void Book::add_authors(std::set<std::string>& new_authors)
{
	this->authors.insert(new_authors.begin(), new_authors.end());
}

void Book::add_authors(std::vector<std::string>& new_authors)
{
	this->authors.insert(new_authors.begin(), new_authors.end());
}

void Book::erase_author(const std::string& author)
{
	this->authors.erase(author);
}

void Book::erase_authors(std::set<std::string>& authors)
{
	for (const std::string& cur : authors) {
		this->authors.erase(cur);
	}
}

void Book::erase_authors(std::vector<std::string>& authors)
{
	for (const std::string& cur : authors) {
		this->authors.erase(cur);
	}
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

void Book::add_prefix_to_annotation(const std::string& prefix)
{
	this->annotation.insert(this->annotation.begin(), prefix.begin(), prefix.end());
}

void Book::add_suffix_to_annotation(const std::string& suffix)
{
	this->annotation.append(suffix);
}

void Book::erase_prefix_from_annotation(const std::size_t characters_number)
{
	this->annotation.erase(0,characters_number);
}

void Book::erase_suffix_from_annotation(const std::size_t characters_number)
{
	this->annotation.resize(this->annotation.size() - characters_number);
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
