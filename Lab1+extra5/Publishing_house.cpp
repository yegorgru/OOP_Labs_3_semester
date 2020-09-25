#include "Publishing_house.h"

Publishing_house::Publishing_house()
	: cur_date(Date(0, 0, 0)) {}

Date Publishing_house::release_book(const std::string& name,
	const std::set<std::string>& authors, const std::uint32_t pages,
	const std::string& annotation, const int number,
	std::set<std::pair<size_t,uint16_t>> characters)
{
	++cur_date;

	Book new_book(name, authors, cur_date, pages, annotation);
	id_books[cur_date] = new_book;
	books_number[cur_date] = number;

	for (auto cur_character : characters) {
		id_characters[cur_character.first].update_role(new_book, cur_character.second);
	}

	return cur_date;
}
