#include "TestFramework.h"
#include "Profile.h"
#include "Book.h"
#include "BookCharacter.h"
#include "PublishingHouse.h"
#include "Date.h"

#include <string>
#include <vector>
#include <random>

std::random_device rd;
std::mt19937 mersenne(rd());

std::string random_element(std::string) {
	std::string answer;
	for (int i = 0; i < 10; i++) {
		answer.push_back(65 + mersenne() % 26);
	}
	return answer;
}

int random_element(int) {
	return mersenne() % 100;
}

template<typename T>
void TestCharacter() {
	//Test Constructors
	{
		BookCharacter<T> character("Kotygoroshko");
		ASSERT_EQUAL(character.get_default_name(), "Kotygoroshko");
		ASSERT_EQUAL(character.get_all_names(),
			std::set<std::string>{ "Kotygoroshko" });
	}
	{
		std::set<std::string> names{ "Harry", "Potter", "a boy who survived" };
		BookCharacter<T> character("Harry Potter", names);
		ASSERT_EQUAL(character.get_default_name(), "Harry Potter");
		ASSERT_EQUAL(character.get_all_names(),names);


		//Test methods
		character.erase_name("Harry");
		names.erase("Harry");
		ASSERT_EQUAL(character.get_all_names(), names);

		character.add_name("Harry");
		names.insert("Harry");
		ASSERT_EQUAL(character.get_all_names(), names);

		character.set_names({ "Potter" });
		names = { "Potter","Harry Potter" };
		ASSERT_EQUAL(character.get_all_names(), names);

		auto random_book = random_element(T());
		character.update_role(random_book, Role::main);
		ASSERT_EQUAL(character.get_role(random_book), Role::main);

		character.update_role(random_book, Role::secondary);
		ASSERT_EQUAL(character.get_role(random_book), Role::secondary);

		character.decrease(random_book);
		ASSERT_EQUAL(character.get_role(random_book), Role::episodic);

		character.decrease(random_book);
		ASSERT_EQUAL(character.get_role(random_book), Role::episodic);

		character.promote(random_book);
		character.promote(random_book);
		ASSERT_EQUAL(character.get_role(random_book), Role::main);

		character.promote(random_book);
		ASSERT_EQUAL(character.get_role(random_book), Role::main);

		auto books = character.get_all_books();
		ASSERT_EQUAL(books, std::set<T>{ random_book });

		auto random_book2 = random_element(T());
		auto random_book3 = random_element(T());
		character.update_role(random_book2, Role::main);
		character.update_role(random_book3, Role::episodic);
		books.insert(random_book3);
		books.insert(random_book2);

		ASSERT_EQUAL(character.get_all_books(), books);

		character.erase_book(random_book2);
		books.erase(random_book2);

		ASSERT_EQUAL(character.get_all_books(), books);
	}
}

void TestBook() {
	Book book("Chamber of Secrets", { "J. K. Rowling" },
		Date(1, 1, 1, 1, 1, 2005), 400,
		"second book about Harry Potter, small wizard");
	ASSERT_EQUAL("Chamber of Secrets", book.get_name());
	ASSERT_EQUAL(std::set<std::string>{ "J. K. Rowling" }, book.get_authors());
	ASSERT_EQUAL(Date(1, 1, 1, 1, 1, 2005), book.get_date());
	ASSERT_EQUAL(400, book.get_pages());
	ASSERT_EQUAL("second book about Harry Potter, small wizard",
		book.get_annotation());

	book.erase_author("J. K. Rowling");
	ASSERT_EQUAL(std::set<std::string>{}, book.get_authors());

	book.add_author("T. Shevchenko");
	ASSERT_EQUAL(std::set<std::string>{"T. Shevchenko"}, book.get_authors());

	std::set<std::string> authors{ "J. K. Rowling" ,"Baron Munchausen" };
	book.set_authors(authors);
	ASSERT_EQUAL(authors, book.get_authors());

	Date new_date(2, 2, 2, 2, 2, 2020);
	book.set_date(new_date);
	ASSERT_EQUAL(new_date, book.get_date());

	book.set_pages(200);
	ASSERT_EQUAL(200, book.get_pages());

	book.set_annotation("small annotation");
	ASSERT_EQUAL("small annotation", book.get_annotation());
}

void TestHouseSeries() {
	{
		PublishingHouse PHouse;

		//Empty series
		auto series = PHouse.get_series();
		ASSERT_EQUAL(series, std::vector<std::vector<Book>>{});

		//1 series with 1 book without characters
		Book first_book("Book", { "" }, Date(1, 1, 1, 1, 1, 1), 10, "");
		auto first_book_id = PHouse.add_book(first_book);
		series = PHouse.get_series();
		ASSERT_EQUAL(series, std::vector<std::vector<Book>> { {first_book} });

		//1 series with 3 books without characters
		Book second_book("Book2", { "" }, Date(1, 1, 1, 1, 0, 1), 10, "");
		Book third_book("Book3", { "" }, Date(1, 1, 1, 0, 1, 1), 10, "");
		auto second_book_id = PHouse.add_book(second_book);
		auto third_book_id = PHouse.add_book(third_book);
		std::vector<std::vector<Book>> true_series{ {second_book, third_book, first_book} };
		series = PHouse.get_series();
		ASSERT_EQUAL(series, true_series);

		//1 unimportant character
		auto character_id = PHouse.add_character(BookCharacter<std::string>("First character"));
		PHouse.add_character_in_book(character_id, first_book_id, Role::episodic);
		series = PHouse.get_series();
		ASSERT_EQUAL(series, true_series);

		//1 important character, 2 series
		PHouse.promote_role(character_id, first_book_id);
		series = PHouse.get_series();
		true_series = {
			{first_book},
			{second_book,third_book}
		};
		ASSERT_EQUAL(series, true_series);

		//new book in series with character
		PHouse.add_character_in_book(character_id, second_book_id, Role::main);
		series = PHouse.get_series();
		true_series = {
			{third_book},
			{second_book,first_book}
		};
		ASSERT_EQUAL(series, true_series);
	}
	{
		//3 books, 2 characters
		PublishingHouse PHouse;
		Book first_book("Book", { "" }, Date(1, 1, 1, 1, 1, 1), 10, "");
		auto first_book_id = PHouse.add_book(first_book);
		Book second_book("Book2", { "" }, Date(1, 1, 1, 1, 0, 1), 10, "");
		Book third_book("Book3", { "" }, Date(1, 1, 1, 0, 1, 1), 10, "");
		auto second_book_id = PHouse.add_book(second_book);
		auto third_book_id = PHouse.add_book(third_book);
		auto first_character_id = PHouse.add_character(BookCharacter<std::string>("First character"));
		auto second_character_id = PHouse.add_character(BookCharacter<std::string>("Second character"));
		PHouse.add_character_in_book(first_character_id, first_book_id, Role::episodic);
		PHouse.add_character_in_book(second_character_id, second_book_id, Role::secondary);
		PHouse.add_character_in_book(first_character_id, second_book_id, Role::main);
		PHouse.add_character_in_book(second_character_id, third_book_id, Role::secondary);
		auto series = PHouse.get_series();
		std::vector<std::vector<Book>>true_series = {
			{first_book},
			{second_book,third_book}
		};
		ASSERT_EQUAL(series, true_series);


		PHouse.promote_role(first_character_id, first_book_id);
		series = PHouse.get_series();
		true_series = {
			{second_book, third_book, first_book}
		};
		ASSERT_EQUAL(series, true_series);

		PHouse.decrease_role(second_character_id, third_book_id);
		series = PHouse.get_series();
		true_series = {
			{third_book},
			{second_book,first_book}
		};
		ASSERT_EQUAL(series, true_series);
	}
	{
		//3 books, 3 characters
		PublishingHouse PHouse;
		Book first_book("A", { "" }, Date(1, 1, 1, 1, 1, 1), 10, "");
		auto first_book_id = PHouse.add_book(first_book);
		Book second_book("B", { "" }, Date(1, 1, 1, 1, 0, 1), 10, "");
		Book third_book("C", { "" }, Date(1, 1, 1, 0, 1, 1), 10, "");
		auto second_book_id = PHouse.add_book(second_book);
		auto third_book_id = PHouse.add_book(third_book);
		auto first_character_id = PHouse.add_character(BookCharacter<std::string>("First character"));
		auto second_character_id = PHouse.add_character(BookCharacter<std::string>("Second character"));
		auto third_character_id = PHouse.add_character(BookCharacter<std::string>("Third character"));
		PHouse.add_character_in_book(first_character_id, first_book_id, Role::secondary);
		PHouse.add_character_in_book(first_character_id, second_book_id, Role::secondary);
		PHouse.add_character_in_book(first_character_id, third_book_id, Role::secondary);
		PHouse.add_character_in_book(second_character_id, second_book_id, Role::secondary);
		PHouse.add_character_in_book(second_character_id, third_book_id, Role::secondary);
		PHouse.add_character_in_book(third_character_id, third_book_id, Role::secondary);
		auto series = PHouse.get_series();
		std::vector<std::vector<Book>>true_series = {
			{second_book, third_book, first_book}
		};
		ASSERT_EQUAL(series, true_series);
	}
	{
		//9 books, 3 characters
		PublishingHouse PHouse;
		Book A("A", { "" }, Date(9, 1, 1, 1, 1, 1), 10, "");
		auto first_book_id = PHouse.add_book(A);
		Book B("B", { "" }, Date(8, 1, 1, 1, 1, 1), 10, "");
		auto second_book_id = PHouse.add_book(B);
		Book C("C", { "" }, Date(7, 1, 1, 1, 1, 1), 10, "");
		auto third_book_id = PHouse.add_book(C);
		Book D("D", { "" }, Date(6, 1, 1, 1, 1, 1), 10, "");
		auto fourth_book_id = PHouse.add_book(D);
		Book E("E", { "" }, Date(5, 1, 1, 1, 1, 1), 10, "");
		auto fifth_book_id = PHouse.add_book(E);
		Book F("F", { "" }, Date(4, 1, 1, 1, 1, 1), 10, "");
		auto sixth_book_id = PHouse.add_book(F);
		Book G("G", { "" }, Date(3, 1, 1, 1, 1, 1), 10, "");
		auto seventh_book_id = PHouse.add_book(G);
		Book H("H", { "" }, Date(2, 1, 1, 1, 1, 1), 10, "");
		auto eighth_book_id = PHouse.add_book(H);
		Book I("I", { "" }, Date(1, 1, 1, 1, 1, 1), 10, "");
		auto ninth_book_id = PHouse.add_book(I);
		auto first_character_id = PHouse.add_character(BookCharacter<std::string>("First character"));
		auto second_character_id = PHouse.add_character(BookCharacter<std::string>("Second character"));
		auto third_character_id = PHouse.add_character(BookCharacter<std::string>("Third character"));
		
		PHouse.add_character_in_book(first_character_id, ninth_book_id, Role::secondary);
		PHouse.add_character_in_book(second_character_id, eighth_book_id, Role::secondary);
		PHouse.add_character_in_book(third_character_id, seventh_book_id, Role::secondary);
		PHouse.add_character_in_book(first_character_id, sixth_book_id, Role::secondary);
		PHouse.add_character_in_book(second_character_id, fifth_book_id, Role::secondary);
		PHouse.add_character_in_book(third_character_id, fourth_book_id, Role::secondary);
		PHouse.add_character_in_book(first_character_id, third_book_id, Role::secondary);
		PHouse.add_character_in_book(second_character_id, second_book_id, Role::secondary);
		PHouse.add_character_in_book(third_character_id, first_book_id, Role::secondary);

		auto series = PHouse.get_series();
		std::vector<std::vector<Book>>true_series = {
			{G, D, A},
			{H, E, B},
			{I, F, C}
		};
		ASSERT_EQUAL(series, true_series);

		//merge 1 and 2 series
		PHouse.add_character_in_book(third_character_id, fifth_book_id, Role::main);
		series = PHouse.get_series();
		true_series = {
			{I, F, C},
			{H,G, E,D, B,A}
		};
		ASSERT_EQUAL(series, true_series);

		//merge series
		PHouse.add_character_in_book(second_character_id, ninth_book_id, Role::main);
		series = PHouse.get_series();
		true_series = {
			{I,H,G,F, E,D,C, B,A}
		};
		ASSERT_EQUAL(series, true_series);

		//split series
		PHouse.erase_character_from_book(second_character_id, fifth_book_id);
		series = PHouse.get_series();
		true_series = {
			{G,E,D,A},
			{I,H,F,C,B}
		};
		ASSERT_EQUAL(series, true_series);

		//split 2 series
		PHouse.erase_character_from_book(first_character_id, ninth_book_id);
		series = PHouse.get_series();
		true_series = {
			{F,C},
			{I,H,B},
			{G,E,D,A}
		};
		ASSERT_EQUAL(series, true_series);
	}
}

void TestHouseNotSeries() {

	//get characters, get important characters, promote role, decrease role, get role
	{
		PublishingHouse PHouse;
		Book first_book("A", { "" }, Date(1, 1, 1, 1, 1, 1), 10, "");
		auto first_book_id = PHouse.add_book(first_book);
		auto first_character_id = PHouse.add_character(BookCharacter<std::string>("First character"));
		auto second_character_id = PHouse.add_character(BookCharacter<std::string>("Second character"));
		std::set<std::string> true_characters{};
		std::set<std::string> characters = PHouse.get_characters(first_book_id);
		ASSERT_EQUAL(characters, true_characters);
		characters = PHouse.get_important_characters(first_book_id);
		ASSERT_EQUAL(characters, true_characters);

		PHouse.add_character_in_book(first_character_id, first_book_id, Role::main);
		PHouse.add_character_in_book(second_character_id, first_book_id, Role::episodic);
		true_characters.insert(first_character_id);
		characters = PHouse.get_important_characters(first_book_id);
		ASSERT_EQUAL(characters, true_characters);

		ASSERT_EQUAL(PHouse.get_role(first_character_id, first_book_id), Role::main);
		PHouse.promote_role(first_character_id, first_book_id);
		ASSERT_EQUAL(PHouse.get_role(first_character_id, first_book_id), Role::main);
		ASSERT_EQUAL(PHouse.get_role(second_character_id, first_book_id), Role::episodic);
		PHouse.decrease_role(second_character_id, first_book_id);
		ASSERT_EQUAL(PHouse.get_role(second_character_id, first_book_id), Role::episodic);

		true_characters.insert(second_character_id);
		characters = PHouse.get_characters(first_book_id);
		ASSERT_EQUAL(characters, true_characters);

		PHouse.promote_role(second_character_id, first_book_id);
		characters = PHouse.get_important_characters(first_book_id);
		ASSERT_EQUAL(characters, true_characters);

		PHouse.decrease_role(first_character_id, first_book_id);
		characters = PHouse.get_important_characters(first_book_id);
		ASSERT_EQUAL(characters, true_characters);

		PHouse.decrease_role(first_character_id, first_book_id);
		characters = PHouse.get_important_characters(first_book_id);
		true_characters.erase(first_character_id);
		ASSERT_EQUAL(characters, true_characters);

		ASSERT_EQUAL(PHouse.get_role(first_character_id, first_book_id), Role::episodic);
		ASSERT_EQUAL(PHouse.get_role(second_character_id, first_book_id), Role::secondary);
	}
	//get books, add role, erase role, get book, get character,add character, add book
	{
		PublishingHouse PHouse;
		Book first_book("A", { "Author1" }, Date(1, 49, 1, 1, 1, 1), 345, "");
		auto first_book_id = PHouse.add_book(first_book);
		ASSERT_EQUAL(first_book, PHouse.get_book(first_book_id));

		Book second_book("B", { "Author1","Author2" }, Date(1, 1, 1, 1, 1, 1), 100, "dgfhf");
		auto second_book_id = PHouse.add_book(second_book);
		ASSERT_EQUAL(second_book, PHouse.get_book(second_book_id));

		Book third_book("C", { "" }, Date(1, 1, 10, 1, 45, 1), 15, "xfbdgnfgn");
		auto third_book_id = PHouse.add_book(third_book);
		ASSERT_EQUAL(third_book, PHouse.get_book(third_book_id));

		BookCharacter<std::string> character1("First character", { "A","B" });
		auto first_character_id = PHouse.add_character(character1);
		BookCharacter<std::string> character2("Second character");
		auto second_character_id = PHouse.add_character(character2);

		ASSERT_EQUAL(PHouse.get_character(first_character_id), character1);
		ASSERT_EQUAL(PHouse.get_character(second_character_id), character2);

		std::set<std::string>true_books = {};
		ASSERT_EQUAL(true_books, PHouse.get_books(first_character_id));

		PHouse.add_character_in_book(first_character_id, first_book_id, Role::secondary);
		true_books.insert(first_book_id);
		ASSERT_EQUAL(true_books, PHouse.get_books(first_character_id));

		PHouse.add_character_in_book(first_character_id, second_book_id, Role::secondary);
		true_books.insert(second_book_id);
		ASSERT_EQUAL(true_books, PHouse.get_books(first_character_id));
		ASSERT_EQUAL(std::set<std::string>{first_character_id}, PHouse.get_characters(first_book_id));

		PHouse.erase_character_from_book(first_character_id, first_book_id);
		true_books.erase(first_book_id);
		ASSERT_EQUAL(true_books, PHouse.get_books(first_character_id));
		ASSERT_EQUAL(std::set<std::string>{}, PHouse.get_characters(first_book_id));
	}
	//ban character, ban role
	{
		PublishingHouse PHouse;
		Book first_book("A", { "" }, Date(1, 1, 1, 1, 1, 1), 10, "");
		auto first_book_id = PHouse.add_book(first_book);
		Book second_book("B", { "" }, Date(1, 1, 1, 1, 1, 1), 10, "");
		auto second_book_id = PHouse.add_book(second_book);
		BookCharacter<std::string> character1("First character");
		auto first_character_id = PHouse.add_character(character1);
		BookCharacter<std::string> character2("Second character");
		auto second_character_id = PHouse.add_character(character2);
		BookCharacter<std::string> character3("Third character");
		auto third_character_id = PHouse.add_character(character3);
		PHouse.add_character_in_book(first_character_id, first_book_id, Role::main);
		PHouse.add_character_in_book(first_character_id, second_book_id, Role::main);
		PHouse.add_character_in_book(second_character_id, first_book_id, Role::main);
		PHouse.add_character_in_book(second_character_id, second_book_id, Role::main);
		PHouse.add_character_in_book(third_character_id, first_book_id, Role::main);
		PHouse.add_character_in_book(third_character_id, second_book_id, Role::main);
		std::set<std::string>true_books = { first_book_id, second_book_id };
		ASSERT_EQUAL(PHouse.get_books(first_character_id), true_books);
		std::set<std::string>true_characters = { first_character_id,
			second_character_id,third_character_id };
		ASSERT_EQUAL(PHouse.get_characters(first_book_id), true_characters);

		PHouse.ban_book(second_book_id);
		true_books.erase(second_book_id);
		ASSERT_EQUAL(PHouse.get_books(first_character_id), true_books);
		PHouse.ban_character(second_character_id);
		true_characters.erase(second_character_id);
		ASSERT_EQUAL(PHouse.get_characters(first_book_id), true_characters);
		PHouse.ban_character(first_character_id);
		true_characters.erase(first_character_id);
		ASSERT_EQUAL(PHouse.get_characters(first_book_id), true_characters);
		PHouse.ban_book(first_book_id);
		true_books.erase(first_book_id);
		ASSERT_EQUAL(PHouse.get_books(third_character_id), true_books);
	}
}

void TestDateBasic() {
	Date date(1, 2, 3, 4, 5, 6);
	ASSERT_EQUAL(date.get_year(), 6);
	date.set_year(7);
	ASSERT_EQUAL(date.get_year(), 7);
	ASSERT_EQUAL(date.get_month(), 5);
	date.set_month(6);
	ASSERT_EQUAL(date.get_month(), 6);
	ASSERT_EQUAL(date.get_day(), 4);
	date.set_day(5);
	ASSERT_EQUAL(date.get_day(), 5);
	ASSERT_EQUAL(date.get_hours(), 3);
	date.set_hours(4);
	ASSERT_EQUAL(date.get_hours(), 4);
	ASSERT_EQUAL(date.get_minutes(), 2);
	date.set_minutes(3);
	ASSERT_EQUAL(date.get_minutes(), 3);
	ASSERT_EQUAL(date.get_seconds(), 1);
	date.set_seconds(2);
	ASSERT_EQUAL(date.get_seconds(), 2);

	Date another = date;
	ASSERT_EQUAL(another.get_year(), 7);
	ASSERT_EQUAL(another.get_month(), 6);
	ASSERT_EQUAL(another.get_day(), 5);
	ASSERT_EQUAL(another.get_hours(), 4);
	ASSERT_EQUAL(another.get_minutes(), 3);
	ASSERT_EQUAL(another.get_seconds(), 2);
}

void TestDateLeap() {
	ASSERT(Date(1, 1, 1, 1, 1, 2020).is_leap());
	ASSERT(Date(1, 1, 1, 1, 1, 2024).is_leap());
	ASSERT(Date(1, 1, 1, 1, 1, 2000).is_leap());
	ASSERT(Date(1, 1, 1, 1, 1, 2096).is_leap());
	ASSERT(Date(1, 1, 1, 1, 1, 4).is_leap());
	ASSERT(!(Date(1, 1, 1, 1, 1, 3).is_leap()));
	ASSERT(!(Date(1, 1, 1, 1, 1, 2100).is_leap()));
	ASSERT(!(Date(1, 1, 1, 1, 1, 2200).is_leap()));
	ASSERT(!(Date(1, 1, 1, 1, 1, 2300).is_leap()));
	ASSERT(!(Date(1, 1, 1, 1, 1, 2021).is_leap()));

	{
		Date basis(1, 1, 1, 1, 1, 2020);
		ASSERT_EQUAL(0, basis.count_29_february(Date(1, 1, 1, 1, 1, 2020)));
		ASSERT_EQUAL(0, basis.count_29_february(Date(1, 1, 1, 29, 2, 2020)));
		ASSERT_EQUAL(1, basis.count_29_february(Date(1, 1, 1, 1, 3, 2020)));
		ASSERT_EQUAL(1, basis.count_29_february(Date(1, 1, 1, 1, 1, 2021)));
		ASSERT_EQUAL(1, basis.count_29_february(Date(1, 1, 1, 1, 1, 2022)));
		ASSERT_EQUAL(1, basis.count_29_february(Date(1, 1, 1, 1, 1, 2023)));
		ASSERT_EQUAL(1, basis.count_29_february(Date(1, 1, 1, 1, 1, 2024)));
		ASSERT_EQUAL(1, basis.count_29_february(Date(1, 1, 1, 29, 2, 2024)));
		ASSERT_EQUAL(2, basis.count_29_february(Date(1, 1, 1, 1, 3, 2024)));
		ASSERT_EQUAL(2, basis.count_29_february(Date(1, 1, 1, 1, 1, 2025)));
		ASSERT_EQUAL(7, basis.count_29_february(Date(1, 1, 1, 1, 1, 2045)));
	}
	{
		Date basis(1, 1, 1, 1, 1, 2000);
		ASSERT_EQUAL(0, basis.count_29_february(Date(1, 1, 1, 28, 2, 2000)));
		ASSERT_EQUAL(1, basis.count_29_february(Date(1, 1, 1, 1, 3, 2000)));
		ASSERT_EQUAL(2, basis.count_29_february(Date(1, 1, 1, 1, 1, 2005)));
	}
	{
		Date basis(1, 1, 1, 1, 1, 2100);
		ASSERT_EQUAL(0, basis.count_29_february(Date(1, 1, 1, 28, 2, 2100)));
		ASSERT_EQUAL(0, basis.count_29_february(Date(1, 1, 1, 1, 3, 2100)));
		ASSERT_EQUAL(1, basis.count_29_february(Date(1, 1, 1, 1, 1, 2105)));
	}
	{
		Date basis(1, 1, 1, 1, 1, 2400);
		ASSERT_EQUAL(0, basis.count_29_february(Date(1, 1, 1, 28, 2, 2400)));
		ASSERT_EQUAL(1, basis.count_29_february(Date(1, 1, 1, 1, 3, 2400)));
		ASSERT_EQUAL(2, basis.count_29_february(Date(1, 1, 1, 1, 1, 2405)));
	}
	{
		Date basis(1, 1, 1, 1, 1, 2000);
		ASSERT_EQUAL(25, basis.count_29_february(Date(1, 1, 1, 1, 1, 2100)));
		ASSERT_EQUAL(25, basis.count_29_february(Date(1, 1, 1, 1, 3, 2100)));
		ASSERT_EQUAL(49, basis.count_29_february(Date(1, 1, 1, 1, 1, 2200)));
		ASSERT_EQUAL(49, basis.count_29_february(Date(1, 1, 1, 1, 3, 2200)));
		ASSERT_EQUAL(73, basis.count_29_february(Date(1, 1, 1, 1, 1, 2300)));
		ASSERT_EQUAL(73, basis.count_29_february(Date(1, 1, 1, 1, 3, 2300)));
		ASSERT_EQUAL(97, basis.count_29_february(Date(1, 1, 1, 1, 1, 2400)));
		ASSERT_EQUAL(98, basis.count_29_february(Date(1, 1, 1, 1, 3, 2400)));
		ASSERT_EQUAL(122, basis.count_29_february(Date(1, 1, 1, 1, 1, 2500)));
		ASSERT_EQUAL(122, basis.count_29_february(Date(1, 1, 1, 1, 3, 2500)));
	}
	{
		Date basis(1, 1, 1, 31, 12, 2019);
		ASSERT_EQUAL(0, basis.count_29_february(Date(1, 1, 1, 1, 1, 2020)));
		ASSERT_EQUAL(0, basis.count_29_february(Date(1, 1, 1, 29, 2, 2020)));
		ASSERT_EQUAL(1, basis.count_29_february(Date(1, 1, 1, 1, 3, 2020)));
		ASSERT_EQUAL(1, basis.count_29_february(Date(1, 1, 1, 1, 1, 2021)));
		ASSERT_EQUAL(1, basis.count_29_february(Date(1, 1, 1, 1, 1, 2022)));
		ASSERT_EQUAL(1, basis.count_29_february(Date(1, 1, 1, 1, 1, 2023)));
		ASSERT_EQUAL(1, basis.count_29_february(Date(1, 1, 1, 1, 1, 2024)));
		ASSERT_EQUAL(1, basis.count_29_february(Date(1, 1, 1, 29, 2, 2024)));
		ASSERT_EQUAL(2, basis.count_29_february(Date(1, 1, 1, 1, 3, 2024)));
		ASSERT_EQUAL(2, basis.count_29_february(Date(1, 1, 1, 1, 1, 2025)));
		ASSERT_EQUAL(7, basis.count_29_february(Date(1, 1, 1, 1, 1, 2045)));
	}
	{
		Date basis(1, 1, 1, 31, 12, 1999);
		ASSERT_EQUAL(0, basis.count_29_february(Date(1, 1, 1, 28, 2, 2000)));
		ASSERT_EQUAL(1, basis.count_29_february(Date(1, 1, 1, 1, 3, 2000)));
		ASSERT_EQUAL(2, basis.count_29_february(Date(1, 1, 1, 1, 1, 2005)));

		ASSERT_EQUAL(25, basis.count_29_february(Date(1, 1, 1, 1, 1, 2100)));
		ASSERT_EQUAL(25, basis.count_29_february(Date(1, 1, 1, 1, 3, 2100)));
		ASSERT_EQUAL(49, basis.count_29_february(Date(1, 1, 1, 1, 1, 2200)));
		ASSERT_EQUAL(49, basis.count_29_february(Date(1, 1, 1, 1, 3, 2200)));
		ASSERT_EQUAL(73, basis.count_29_february(Date(1, 1, 1, 1, 1, 2300)));
		ASSERT_EQUAL(73, basis.count_29_february(Date(1, 1, 1, 1, 3, 2300)));
		ASSERT_EQUAL(97, basis.count_29_february(Date(1, 1, 1, 1, 1, 2400)));
		ASSERT_EQUAL(98, basis.count_29_february(Date(1, 1, 1, 1, 3, 2400)));
		ASSERT_EQUAL(122, basis.count_29_february(Date(1, 1, 1, 1, 1, 2500)));
		ASSERT_EQUAL(122, basis.count_29_february(Date(1, 1, 1, 1, 3, 2500)));
	}
	{
		Date basis(1, 1, 1, 31, 12, 2099);
		ASSERT_EQUAL(0, basis.count_29_february(Date(1, 1, 1, 28, 2, 2100)));
		ASSERT_EQUAL(0, basis.count_29_february(Date(1, 1, 1, 1, 3, 2100)));
		ASSERT_EQUAL(1, basis.count_29_february(Date(1, 1, 1, 1, 1, 2105)));
	}

	{
		Date basis(1, 1, 1, 1, 1, 2019);
		ASSERT_EQUAL(0, basis.count_29_february(Date(1, 1, 1, 1, 1, 2020)));
		ASSERT_EQUAL(0, basis.count_29_february(Date(1, 1, 1, 29, 2, 2020)));
		ASSERT_EQUAL(1, basis.count_29_february(Date(1, 1, 1, 1, 3, 2020)));
		ASSERT_EQUAL(1, basis.count_29_february(Date(1, 1, 1, 1, 1, 2021)));
		ASSERT_EQUAL(1, basis.count_29_february(Date(1, 1, 1, 1, 1, 2022)));
		ASSERT_EQUAL(1, basis.count_29_february(Date(1, 1, 1, 1, 1, 2023)));
		ASSERT_EQUAL(1, basis.count_29_february(Date(1, 1, 1, 1, 1, 2024)));
		ASSERT_EQUAL(1, basis.count_29_february(Date(1, 1, 1, 29, 2, 2024)));
		ASSERT_EQUAL(2, basis.count_29_february(Date(1, 1, 1, 1, 3, 2024)));
		ASSERT_EQUAL(2, basis.count_29_february(Date(1, 1, 1, 1, 1, 2025)));
		ASSERT_EQUAL(7, basis.count_29_february(Date(1, 1, 1, 1, 1, 2045)));
	}
	{
		Date basis(1, 1, 1, 1, 1, 1999);
		ASSERT_EQUAL(0, basis.count_29_february(Date(1, 1, 1, 28, 2, 2000)));
		ASSERT_EQUAL(1, basis.count_29_february(Date(1, 1, 1, 1, 3, 2000)));
		ASSERT_EQUAL(2, basis.count_29_february(Date(1, 1, 1, 1, 1, 2005)));

		ASSERT_EQUAL(25, basis.count_29_february(Date(1, 1, 1, 1, 1, 2100)));
		ASSERT_EQUAL(25, basis.count_29_february(Date(1, 1, 1, 1, 3, 2100)));
		ASSERT_EQUAL(49, basis.count_29_february(Date(1, 1, 1, 1, 1, 2200)));
		ASSERT_EQUAL(49, basis.count_29_february(Date(1, 1, 1, 1, 3, 2200)));
		ASSERT_EQUAL(73, basis.count_29_february(Date(1, 1, 1, 1, 1, 2300)));
		ASSERT_EQUAL(73, basis.count_29_february(Date(1, 1, 1, 1, 3, 2300)));
		ASSERT_EQUAL(97, basis.count_29_february(Date(1, 1, 1, 1, 1, 2400)));
		ASSERT_EQUAL(98, basis.count_29_february(Date(1, 1, 1, 1, 3, 2400)));
		ASSERT_EQUAL(122, basis.count_29_february(Date(1, 1, 1, 1, 1, 2500)));
		ASSERT_EQUAL(122, basis.count_29_february(Date(1, 1, 1, 1, 3, 2500)));
	}
	{
		Date basis(1, 1, 1, 1, 1, 2099);
		ASSERT_EQUAL(0, basis.count_29_february(Date(1, 1, 1, 28, 2, 2100)));
		ASSERT_EQUAL(0, basis.count_29_february(Date(1, 1, 1, 1, 3, 2100)));
		ASSERT_EQUAL(1, basis.count_29_february(Date(1, 1, 1, 1, 1, 2105)));
	}
	{
		Date basis(1, 1, 1, 1, 3, 2020);
		ASSERT_EQUAL(-1, basis.count_29_february(Date(1, 1, 1, 28, 2, 2020)));
		ASSERT_EQUAL(0, basis.count_29_february(Date(1, 1, 1, 2, 3, 2020)));
		ASSERT_EQUAL(0, basis.count_29_february(Date(1, 1, 1, 29, 2, 2024)));
		ASSERT_EQUAL(1, basis.count_29_february(Date(1, 1, 1, 1, 3, 2024)));
	}
}

void TestDateIsValid() {
	ASSERT(Date(1, 1, 1, 28, 9, 2020).is_valid());

	ASSERT(!(Date(1, 1, 1, 0, 1, 2020).is_valid()));
	ASSERT(Date(1, 1, 1, 1, 1, 2020).is_valid());
	ASSERT(Date(1, 1, 1, 31, 1, 2020).is_valid());
	ASSERT(!(Date(1, 1, 1, 32, 1, 2020).is_valid()));

	ASSERT(!(Date(1, 1, 1, 0, 2, 2020).is_valid()));
	ASSERT(Date(1, 1, 1, 1, 2, 2020).is_valid());
	ASSERT(Date(1, 1, 1, 29, 2, 2020).is_valid());
	ASSERT(!(Date(1, 1, 1, 30, 2, 2020).is_valid()));

	ASSERT(!(Date(1, 1, 1, 0, 2, 2021).is_valid()));
	ASSERT(Date(1, 1, 1, 1, 2, 2021).is_valid());
	ASSERT(Date(1, 1, 1, 28, 2, 2021).is_valid());
	ASSERT(!(Date(1, 1, 1, 29, 2, 2021).is_valid()));

	ASSERT(!(Date(1, 1, 1, 0, 3, 2020).is_valid()));
	ASSERT(Date(1, 1, 1, 1, 3, 2020).is_valid());
	ASSERT(Date(1, 1, 1, 31, 3, 2020).is_valid());
	ASSERT(!(Date(1, 1, 1, 32, 3, 2020).is_valid()));

	ASSERT(!(Date(1, 1, 1, 0, 4, 2020).is_valid()));
	ASSERT(Date(1, 1, 1, 1, 4, 2020).is_valid());
	ASSERT(Date(1, 1, 1, 30, 4, 2020).is_valid());
	ASSERT(!(Date(1, 1, 1, 31, 4, 2020).is_valid()));

	ASSERT(!(Date(1, 1, 1, 0, 5, 2020).is_valid()));
	ASSERT(Date(1, 1, 1, 1, 5, 2020).is_valid());
	ASSERT(Date(1, 1, 1, 31, 5, 2020).is_valid());
	ASSERT(!(Date(1, 1, 1, 32, 5, 2020).is_valid()));

	ASSERT(!(Date(1, 1, 1, 0, 6, 2020).is_valid()));
	ASSERT(Date(1, 1, 1, 1, 6, 2020).is_valid());
	ASSERT(Date(1, 1, 1, 30, 6, 2020).is_valid());
	ASSERT(!(Date(1, 1, 1, 31, 6, 2020).is_valid()));

	ASSERT(!(Date(1, 1, 1, 0, 7, 2020).is_valid()));
	ASSERT(Date(1, 1, 1, 1, 7, 2020).is_valid());
	ASSERT(Date(1, 1, 1, 31, 7, 2020).is_valid());
	ASSERT(!(Date(1, 1, 1, 32, 7, 2020).is_valid()));

	ASSERT(!(Date(1, 1, 1, 0, 8, 2020).is_valid()));
	ASSERT(Date(1, 1, 1, 1, 8, 2020).is_valid());
	ASSERT(Date(1, 1, 1, 31, 8, 2020).is_valid());
	ASSERT(!(Date(1, 1, 1, 32, 8, 2020).is_valid()));

	ASSERT(!(Date(1, 1, 1, 0, 9, 2020).is_valid()));
	ASSERT(Date(1, 1, 1, 1, 9, 2020).is_valid());
	ASSERT(Date(1, 1, 1, 30, 9, 2020).is_valid());
	ASSERT(!(Date(1, 1, 1, 31, 9, 2020).is_valid()));

	ASSERT(!(Date(1, 1, 1, 0, 10, 2020).is_valid()));
	ASSERT(Date(1, 1, 1, 1, 10, 2020).is_valid());
	ASSERT(Date(1, 1, 1, 31, 10, 2020).is_valid());
	ASSERT(!(Date(1, 1, 1, 32, 10, 2020).is_valid()));

	ASSERT(!(Date(1, 1, 1, 0, 11, 2020).is_valid()));
	ASSERT(Date(1, 1, 1, 1, 11, 2020).is_valid());
	ASSERT(Date(1, 1, 1, 30, 11, 2020).is_valid());
	ASSERT(!(Date(1, 1, 1, 31, 11, 2020).is_valid()));

	ASSERT(!(Date(1, 1, 1, 0, 12, 2020).is_valid()));
	ASSERT(Date(1, 1, 1, 1, 12, 2020).is_valid());
	ASSERT(Date(1, 1, 1, 31, 12, 2020).is_valid());
	ASSERT(!(Date(1, 1, 1, 32, 12, 2020).is_valid()));

	ASSERT(!(Date(-1, 1, 1, 1, 12, 2020).is_valid()));
	ASSERT(!(Date(1, -1, 1, 1, 12, 2020).is_valid()));
	ASSERT(!(Date(1, 1, -1, 1, 12, 2020).is_valid()));
	
	ASSERT(!(Date(60, 1, 1, 1, 12, 2020).is_valid()));
	ASSERT(!(Date(1, 60, 1, 1, 12, 2020).is_valid()));
	ASSERT(!(Date(1, 1, 24, 1, 12, 2020).is_valid()));

	ASSERT(Date(0, 1, 1, 1, 12, 2020).is_valid());
	ASSERT(Date(1, 0, 1, 31, 12, 2020).is_valid());
	ASSERT(Date(1, 1, 0, 1, 12, 2020).is_valid());

	ASSERT(Date(59, 1, 1, 31, 12, 2020).is_valid());
	ASSERT(Date(1, 59, 1, 31, 12, 2020).is_valid());
	ASSERT(Date(1, 1, 23, 31, 12, 2020).is_valid());
}

void TestDateNumberInYear() {
	ASSERT_EQUAL(Date(1, 1, 1, 1, 1, 2019).get_number_in_year(), 1);
	ASSERT_EQUAL(Date(1, 1, 1, 1, 1, 2019).get_reverse_number_in_year(), 365);

	ASSERT_EQUAL(Date(1, 1, 1, 28, 2, 2019).get_number_in_year(), 59);
	ASSERT_EQUAL(Date(1, 1, 1, 28, 2, 2019).get_reverse_number_in_year(), 366-59);

	ASSERT_EQUAL(Date(1, 1, 1, 1, 3, 2019).get_number_in_year(), 60);
	ASSERT_EQUAL(Date(1, 1, 1, 1, 3, 2019).get_reverse_number_in_year(), 366 - 60);

	ASSERT_EQUAL(Date(1, 1, 1, 28, 2, 2020).get_number_in_year(), 59);
	ASSERT_EQUAL(Date(1, 1, 1, 28, 2, 2020).get_reverse_number_in_year(), 367 - 59);

	ASSERT_EQUAL(Date(1, 1, 1, 29, 2, 2020).get_number_in_year(), 60);
	ASSERT_EQUAL(Date(1, 1, 1, 29, 2, 2020).get_reverse_number_in_year(), 307);

	ASSERT_EQUAL(Date(1, 1, 1, 1, 3, 2020).get_number_in_year(), 61);
	ASSERT_EQUAL(Date(1, 1, 1, 1, 3, 2020).get_reverse_number_in_year(), 367 - 61);

	ASSERT_EQUAL(Date(1, 1, 1, 1, 12, 2019).get_number_in_year(), 335);
	ASSERT_EQUAL(Date(1, 1, 1, 1, 12, 2019).get_reverse_number_in_year(), 31);

	ASSERT_EQUAL(Date(1, 1, 1, 1, 12, 2020).get_number_in_year(), 336);
	ASSERT_EQUAL(Date(1, 1, 1, 1, 12, 2020).get_reverse_number_in_year(), 31);

	ASSERT_EQUAL(Date(1, 1, 1, 31, 12, 2019).get_number_in_year(), 365);
	ASSERT_EQUAL(Date(1, 1, 1, 31, 12, 2019).get_reverse_number_in_year(), 1);

	ASSERT_EQUAL(Date(1, 1, 1, 31, 12, 2020).get_number_in_year(), 366);
	ASSERT_EQUAL(Date(1, 1, 1, 31, 12, 2020).get_reverse_number_in_year(), 1);
}

void TestDateSavePromoteDecrease() {
	{
		Date date(1, 1, 1, 1, 1, 2020);
		date.save_promote_year(20);
		ASSERT_EQUAL(date, Date(1, 1, 1, 1, 1, 2040));
		date.save_decrease_year(20);
		ASSERT_EQUAL(date, Date(1, 1, 1, 1, 1, 2020));
	}
	{
		Date date(1, 1, 1, 29, 2, 2020);
		date.save_promote_year(1);
		ASSERT_EQUAL(date, Date(1, 1, 1, 1, 3, 2021));
	}
	{
		Date date(1, 1, 1, 29, 2, 2020);
		date.save_decrease_year(1);
		ASSERT_EQUAL(date, Date(1, 1, 1, 1, 3, 2019));
	}

	{
		Date date(1, 1, 1, 1, 1, 2020);
		date.save_promote_month(5);
		ASSERT_EQUAL(date, Date(1, 1, 1, 1, 6, 2020));
		date.save_decrease_month(5);
		ASSERT_EQUAL(date, Date(1, 1, 1, 1, 1, 2020));
		date.save_promote_month(12);
		ASSERT_EQUAL(date, Date(1, 1, 1, 1, 1, 2021));
		date.save_decrease_month(12);
		ASSERT_EQUAL(date, Date(1, 1, 1, 1, 1, 2020));
		date.save_decrease_month(1);
		ASSERT_EQUAL(date, Date(1, 1, 1, 1, 12, 2019));
		date.save_promote_month(1);
		ASSERT_EQUAL(date, Date(1, 1, 1, 1, 1, 2020));
	}
	{
		Date date(1, 1, 1, 29, 1, 2019);
		date.save_promote_month(1);
		ASSERT_EQUAL(date, Date(1, 1, 1, 1, 3, 2019));
	}
	{
		Date date(1, 1, 1, 29, 3, 2019);
		date.save_decrease_month(1);
		ASSERT_EQUAL(date, Date(1, 1, 1, 28, 2, 2019));
	}
	{
		Date date(1, 1, 1, 29, 1, 2020);
		date.save_promote_month(1);
		ASSERT_EQUAL(date, Date(1, 1, 1, 29, 2, 2020));
	}
	{
		Date date(1, 1, 1, 29, 3, 2020);
		date.save_decrease_month(1);
		ASSERT_EQUAL(date, Date(1, 1, 1, 29, 2, 2020));
	}
	{
		Date date(1, 1, 1, 29, 1, 2019);
		date.save_promote_month(1);
		ASSERT_EQUAL(date, Date(1, 1, 1, 1, 3, 2019));
	}
	{
		Date date(1, 1, 1, 31, 10, 2019);
		date.save_decrease_month(1);
		ASSERT_EQUAL(date, Date(1, 1, 1, 30, 9, 2019));
	}
	{
		Date date(1, 1, 1, 31, 10, 2019);
		date.save_promote_month(1);
		ASSERT_EQUAL(date, Date(1, 1, 1, 1, 11, 2019));
	}
}

int main() {
	TestRunner tr;
	RUN_TEST(tr, TestCharacter<std::string>);
	RUN_TEST(tr, TestCharacter<int>);
	RUN_TEST(tr, TestBook);
	RUN_TEST(tr, TestHouseSeries);
	RUN_TEST(tr, TestHouseNotSeries);
	RUN_TEST(tr, TestDateBasic);
	RUN_TEST(tr, TestDateLeap);
	RUN_TEST(tr, TestDateIsValid);
	RUN_TEST(tr, TestDateNumberInYear);
	RUN_TEST(tr, TestDateSavePromoteDecrease);
}
