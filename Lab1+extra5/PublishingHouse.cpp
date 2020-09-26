#include "PublishingHouse.h"

PublishingHouse::PublishingHouse()
	: cur_date(Date(0, 0, 0)),max_character_id(0),max_series_id(0) {}

Date PublishingHouse::release_book(const std::string& name,
	const std::set<std::string>& authors, const std::uint32_t pages,
	const std::string& annotation, const int number,
	std::set<std::pair<size_t,uint16_t>> characters)
{
	++cur_date;
	//Перевірити чи всі герої правильні
	Book new_book(name, authors, cur_date, pages, annotation);
	id_books[cur_date] = new_book;
	books_number[cur_date] = number;

	std::map<size_t, std::map<size_t, bool>> was_inserted;
	for (auto series = id_series.begin(); series != id_series.end(); series++) {
		for (auto characters = id_characters.begin(); characters != id_characters.end(); characters++) {
			was_inserted[series->first][characters->first] = 0;
		}
	}

	for (auto& cur_character : characters) {
		id_characters.at(cur_character.first).update_role(new_book, cur_character.second);
		if (cur_character.second > 1) {
			if (characters_series[cur_character.first].size() == 0) {
				id_series[++max_series_id].insert(cur_date);
				for (auto characters = id_characters.begin(); characters != id_characters.end(); characters++) {
					was_inserted[max_series_id][characters->first] = 0;
				}
				for (auto& character : characters) {
					if (!was_inserted[max_series_id][character.first]) {
						insert_character_in_series(character.first, max_series_id);
						was_inserted[max_series_id][character.first] = 1;
					}
				}
			}
			else {
				for (auto& cur_series : characters_series[cur_character.first]) {
					id_series[cur_series.first].insert(cur_date);
					for (auto& character : characters) {
						if (!was_inserted[cur_series.first][character.first]) {
							insert_character_in_series(character.first, cur_series.first);
							was_inserted[cur_series.first][character.first] = 1;
						}
					}
				}
			}
			books_important_characters[cur_date].insert(cur_character.second);
		}
	}

	return cur_date;
}

size_t PublishingHouse::add_character(const std::string& name)
{
	id_characters[max_character_id++] = BookCharacter(name);
	characters_series[max_character_id - 1];
}

size_t PublishingHouse::add_character(const std::string& default_name, const std::set<std::string>& names)
{
	return size_t();
}

void PublishingHouse::sell_books(const Date& id, uint32_t number)
{
	uint32_t& cur_number = books_number.at(id);
	if (cur_number >= number) {
		cur_number -= number;
	}
	else {
		//throw
	}
}

Book PublishingHouse::get_book(const Date& id) const
{
	return id_books.at(id);
}

BookCharacter PublishingHouse::get_character(const size_t id) const
{
	return id_characters.at(id);
}

std::vector<std::vector<Book>> PublishingHouse::get_series() const
{
	std::vector<std::vector<Book>>answer;
	answer.reserve(id_series.size());
	std::vector<Book>series;
	for (const auto& cur_series : id_series) {
		series.reserve(cur_series.second.size());
		for (const Date& book : cur_series.second) {
			series.push_back(get_book(book));
		}
		answer.push_back(move(series));
	}
	return answer;
}

void PublishingHouse::update_book_name(const Date& id, const std::string& name)
{
	id_books.at(id).set_name(name);
}

void PublishingHouse::update_book_authors(const Date& id, const std::set<std::string>& authors)
{
	id_books.at(id).set_authors(authors);
}

void PublishingHouse::update_book_pages(const Date& id, const uint32_t pages)
{
	id_books.at(id).set_pages(pages);
}

void PublishingHouse::update_book_annotation(const Date& id, const std::string& annotation)
{
	id_books.at(id).set_annotation(annotation);
}

void PublishingHouse::update_character_default_name(size_t id, const std::string& name)
{
	id_characters.at(id).set_default_name(name);
}

void PublishingHouse::update_character_names(size_t id, const std::set<std::string>& names)
{
	id_characters.at(id).set_names(names);
}

void PublishingHouse::ban_book(const Date& date)
{
	Book book = id_books.at(date);
	for (size_t cur_character : books_important_characters[date]) {
		id_characters.at(cur_character).erase_book(book);
		for (auto cur_series = characters_series.at(cur_character).begin();
			cur_series != characters_series.at(cur_character).end();cur_series++) {
			cur_series->second--;
			id_series.at(cur_series->first).erase(date);
			if (cur_series->second == 0) {
				erase_character_from_series(cur_character, cur_series->first);
				characters_series.at(cur_character).erase(cur_series->first);
				cur_series--;
			}
		}
	}
	id_books.erase(date);
	books_number.erase(date);
	books_important_characters.erase(date);
}

void PublishingHouse::erase_character_from_series(size_t character, size_t series)
{

}

void PublishingHouse::insert_character_in_series(size_t character, size_t series)
{
	series_characters[series].insert(character);
	characters_series[character][series]++;
}
