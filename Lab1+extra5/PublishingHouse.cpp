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
		
	}
	else {
		characters[id] = character;
		return id;
	}
}

void PublishingHouse::ban_book(const book_id& id)
{
	if (id != book_id()) {
		for (auto cur_character : books_characters.at(id)) {
			characters[cur_character].erase_book(id);
		}
		books_characters.erase(id);
		books_important_characters.erase(id);
		books.erase(id);
	}
	else {
		//throw
	}
}

void PublishingHouse::ban_character(const character_id& id)
{
	if (id != character_id()) {
		for (auto book_id : characters[id].get_all_books()) {
			books_characters[book_id].erase(id);
			books_important_characters[book_id].erase(id);
		}
		characters.erase(id);
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
						for (auto important_character : books_important_characters[cur_book->first]) {
							for (size_t j = 0; j != i; j++) {
								if (series_characters[j].find(important_character) != end(series_characters[j])) {
									answer[i].insert(answer[i].end(), answer[j].begin(), answer[j].end());
									series_characters[j].insert(series_characters[i].begin(),
										series_characters[i].end());
								}
							}
							series_characters[i].insert(important_character);
							for (size_t j = i+1; j < answer.size(); j++) {
								if (series_characters[j].find(important_character) != end(series_characters[j])) {
									answer[i].insert(answer[i].end(), answer[j].begin(), answer[j].end());
									series_characters[j].insert(series_characters[i].begin(),
										series_characters[i].end());
								}
							}
						}
						break;
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
			if (lhs.size() == rhs.size()) {
				return lhs[0].get_name() < rhs[0].get_name();
			}
			else {
				return lhs.size() < rhs.size();
			}
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
		auto it = std::unique(begin(cur_series), end(cur_series));
		cur_series.erase(it, end(cur_series));
	}
	return answer;
}

void PublishingHouse::add_character_in_book(const character_id& id_character, const book_id& id_book, Role role)
{
	if (id_character != character_id() && id_book != book_id()) {
		if (characters.find(id_character) == end(characters)) {
			//throw
		}
		else {
			try
			{
				characters[id_character].update_role(id_book,role);
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

void PublishingHouse::erase_character_from_book(const character_id& id_character, const book_id& id_book)
{
	if (id_character != character_id() && id_book != book_id()) {
		if (characters.find(id_character) == end(characters)) {
			//throw
		}
		else {
			try
			{
				characters[id_character].erase_book(id_book);
				books_characters.at(id_book).erase(id_character);
				books_important_characters.at(id_book).erase(id_character);
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

BookCharacter<book_id> PublishingHouse::get_character(const character_id& id)
{
	return characters.at(id);
}

Book PublishingHouse::get_book(const book_id& id)
{
	return books.at(id);
}

void PublishingHouse::promote_role(const character_id& id_character, 
	const book_id& id_book)
{
	if (id_character != character_id() && id_book != book_id()) {
		if (characters.find(id_character) == end(characters)) {
			//throw
		}
		else {
			try
			{
				characters[id_character].promote(id_book);
				if (characters[id_character].get_role(id_book) != Role::episodic) {
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

void PublishingHouse::decrease_role(const character_id& id_character,
	const book_id& id_book)
{
	if (id_character != character_id() && id_book != book_id()) {
		if (characters.find(id_character) == end(characters)) {
			//throw
		}
		else {
			try
			{
				characters[id_character].decrease(id_book);
				if (characters[id_character].get_role(id_book) == Role::episodic) {
					books_important_characters.at(id_book).erase(id_character);
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

Role PublishingHouse::get_role(const character_id& id_character, const book_id& id_book)
{

	try
	{
		return characters.at(id_character).get_role(id_book);
	}
	catch (const std::exception&)
	{

	}
}

std::set<character_id> PublishingHouse::get_characters(const book_id& id_book)
{
	try
	{
		return books_characters.at(id_book);
	}
	catch (const std::exception&)
	{

	}
}

std::set<character_id> PublishingHouse::get_important_characters(const book_id& id_book)
{
	try
	{
		return books_important_characters.at(id_book);
	}
	catch (const std::exception&)
	{

	}
}

std::set<book_id> PublishingHouse::get_books(const character_id& id)
{
	try
	{
		return characters.at(id).get_all_books();
	}
	catch (const std::exception&)
	{

	}
}


