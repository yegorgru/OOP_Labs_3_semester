#include "Library.h"

#include "Book.h"

#include <map>

std::pair<size_t,std::vector<size_t>> Library::add_book(const Book& new_book)
{
	std::pair<size_t, std::vector<size_t>> answer;
	auto check_availability = books_id.find(new_book);
	if (check_availability != end(books_id)) {
		//throw
	}
	//answer.first = insert_in_id_books(new_book);
	books.insert(new_book);
	//std::set<BookCharacter>new_characters = new_book.get_characters();
	//answer.second.reserve(new_characters.size());
	//for (const auto& i : new_characters) {
	//	answer.second.push_back(add_character(i));
	//}
	return answer;
}

size_t Library::insert_in_id_books(const Book& book)
{
	//Тест: перевірити коли size = 0;
	size_t id;
	if (id_books.size() == 0) {
		id = 0;
	}
	else if (id_books.size() / 2 > id_books.rbegin()->first) {
		id = id_books.rbegin()->first + 1;
	}
	else {
		auto lesser = id_books.begin();
		auto bigger = ++(id_books.begin());
		while (true) {
			if (bigger->first - lesser->first > 1) {
				id = bigger->first + 1;
			}
			else {
				lesser++;
				bigger++;
			}
		}
	}
	//id_books[id] = book;
	return id;
}

//size_t Library::add_character(const BookCharacter& character)
//{
//	//Тест: перевірити коли size = 0;
//	auto check_availability = characters_id.find(character);
//	if (check_availability != end(characters_id)) {
//		characters_number[check_availability->second]++;
//		return check_availability->second;
//	}
//	else {
//		size_t id;
//
//		if (id_characters.size() == 0) {
//			id = 0;
//		}
//		else if (id_characters.size() / 2 > id_characters.rbegin()->first) {
//			id = id_characters.rbegin()->first + 1;
//		}
//		else {
//			size_t id = id_characters.rbegin()->first + 1;
//			auto lesser = id_characters.begin();
//			auto bigger = ++(id_characters.begin());
//			while (true) {
//				if (bigger->first - lesser->first > 1) {
//					id = bigger->first + 1;
//					break;
//				}
//				else {
//					lesser++;
//					bigger++;
//				}
//			}
//		}
//		//this->id_characters[id] = character;
//		characters_number[id]++;
//		characters_id[character] = id;
//		return id;
//	}
//}
//
//void Library::insert_book_in_series(const Book& book)
//{
//	//for (const BookCharacter& character : book.get_characters()) {
//		//for (const Book& cur_book:character.get_all_books()) {
//			//for (auto cur_series = get_series_for_book(cur_book)) {
//
//			//}
//		//}
//	//}
//}
//
//std::vector<std::vector<std::set<Book>>::const_iterator> Library::get_series_for_book(const Book& book) const
//{
//	std::vector<std::vector<std::set<Book>>::const_iterator> answer;
//	for (auto cur = series.begin(); cur != series.end();cur++) {
//		if (cur->find(book) != cur->end()) {
//			answer.push_back(cur);
//		}
//	}
//	return answer;
//}




int main() {
	/*Book b("", "", "", 20, "");
	b.add_author("");
	std::vector<Book>v;
	std::map<Book, int>m;
	m[Book("", "", "", 4, "")];*/
}
