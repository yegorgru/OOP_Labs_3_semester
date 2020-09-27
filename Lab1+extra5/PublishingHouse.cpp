#include "PublishingHouse.h"

PublishingHouse::PublishingHouse()
{
}

book_id PublishingHouse::add_book(const Book& book)
{
	book_id id = book.get_name();
	if (id == book_id() || books.find(id) != end(books)) {
		//
	}
	else {
		books[id] = book;
		books_characters[id];
		books_important_characters[id];
		return id;
	}
}

character_id PublishingHouse::add_character(const BookCharacter<book_id>& character)
{
	character_id id = character.get_default_name();
	if (id==character_id() || characters.find(id) != end(characters)) {
		//
	}
	else {
		characters[id] = character;
		return id;
	}
}

void PublishingHouse::ban_book(book_id& id)
{
	if (id != book_id()) {
		for (auto cur_character : books_characters.at(id)) {
			characters[cur_character].erase_book(id);
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
		for (auto book_id : characters[id].get_all_books()) {
			books_characters[book_id].erase(id);
			books_important_characters[book_id].erase(id);
		}
		characters.erase(id);
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
		auto cur_book = books.begin();
		while (cur_book != books.end()) {
			bool is_unique = true;
			for (size_t i = 0; i != answer.size();i++) {
				if(is_subset(books_important_characters[cur_book->first], series_characters[i])) {
					is_unique = false;
					answer[i].push_back(cur_book->second);
					break;
				}
				for (auto character : books_important_characters[cur_book->first]) {
					if (series_characters[i].find(character) != end(series_characters[i])) {
						answer[i].push_back(cur_book->second);
						series_characters[i].insert(books_important_characters[cur_book->first].begin(),
							books_important_characters[cur_book->first].end());
					}
				}
			}
			if (is_unique) {
				answer.push_back({ cur_book->second });
				series_characters[answer.size() - 1] = books_important_characters[cur_book->first];
			}
			cur_book++;
		}
	}

	std::sort(begin(answer), end(answer), [](std::vector<Book>& lhs,
		std::vector <Book>& rhs) {
		return lhs.size() < rhs.size();
		});

	auto series = answer.begin();
	while (series != answer.end()) {
		bool was_erased = false;
		for (auto cur = series+1; cur != end(answer); cur++) {
			if (is_subvector(*series, *cur)) {
				series = answer.erase(series);
				was_erased = true;
				break;
			}
		}
		if (!was_erased) {
			++series;
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
		if (characters.find(id_character) == end(characters)) {
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

BookCharacter<book_id>& PublishingHouse::get_character(character_id id)
{
	return characters.at(id);
}

Book& PublishingHouse::get_book(book_id id)
{
	return books.at(id);
}


