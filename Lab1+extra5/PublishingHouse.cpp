#include "PublishingHouse.h"

PublishingHouse::PublishingHouse()
{
}

book_id PublishingHouse::add_book(const Book& book)
{
	for (const auto& cur_book : books) {
		if (cur_book == book) {
			//throw
		}
	}
	books.push_back(book);
	books_characters[--books.end()];
	books_important_characters[--books.end()];
	return --books.end();
}

character_id PublishingHouse::add_character(const BookCharacter<book_id>& character)
{
	for (const BookCharacter<book_id>& cur_character : characters) {
		if (cur_character == character) {
			//throw
		}
	}
	characters.push_back(character);
	valid_characters.insert(--characters.end());
	return --characters.end();
}

void PublishingHouse::ban_book(book_id& id)
{
	if (id != book_id()) {
		for (auto cur_character : books_characters.at(id)) {
			cur_character->erase_book(id);
		}
		books_characters.erase(id);
		books_important_characters.erase(id);
		books.erase(id);
		id = book_id();
	}
	else {
		//throw
	}
}

void PublishingHouse::ban_character(character_id& id)
{
	if (id != character_id()) {
		for (auto book_id : id->get_all_books()) {
			books_characters[book_id].erase(id);
			books_important_characters[book_id].erase(id);
		}
		characters.erase(id);
		valid_characters.erase(id);
		id = character_id();
	}
	else {
		//throw
	}
}

std::vector<std::vector<Book>> PublishingHouse::get_series()
{
	std::map<size_t, std::set<character_id>>series_characters;
	std::vector<std::vector<Book>>answer;
	if (books.size() == 0) {
		return answer;
	}
	else {
		book_id cur_book = books.begin();
		/*answer.push_back({ *cur_book });
		series_characters[0] = books_important_characters[cur_book];
		cur_book++;*/
		while (cur_book != books.end()) {
			bool is_unique = true;
			for (size_t i = 0; i != answer.size();i++) {
				if(is_subset(books_important_characters[cur_book], series_characters[i])) {
					is_unique = false;
				}
				for (auto character : books_important_characters[cur_book]) {
					if (series_characters[i].find(character) != end(series_characters[i])) {
						answer[i].push_back(*cur_book);
						series_characters[i].insert(books_important_characters[cur_book].begin(), books_important_characters[cur_book].end());
					}
				}
			}
			if (is_unique) {
				answer.push_back({ *cur_book });
				series_characters[answer.size() - 1] = books_important_characters[cur_book];
			}
			cur_book++;
		}
	}
	for (auto& cur_series : answer) {
		std::sort(begin(cur_series), end(cur_series), [](Book& lhs,Book& rhs) {
				return lhs.get_date() < rhs.get_date();
			});
	}
	return answer;
}

void PublishingHouse::add_character_in_book(character_id id_character, book_id id_book, Role role)
{
	if (id_character != character_id() && id_book != book_id()) {
		if (valid_characters.find(id_character) == end(valid_characters)) {
			//throw
		}
		else {
			try
			{
				books_characters.at(id_book).insert(id_character);
				if (role != Role::episodic) {
					books_important_characters.at(id_book).insert(id_character);
				}
			}
			catch (const std::exception&)
			{
				//throw
			}
		}
	}
	else {
		//throw
	}
}


