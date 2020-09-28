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

int main() {
	TestRunner tr;
	RUN_TEST(tr, TestCharacter<std::string>);
	RUN_TEST(tr, TestCharacter<int>);
	RUN_TEST(tr, TestBook);
	RUN_TEST(tr, TestHouseSeries);
	RUN_TEST(tr, TestHouseNotSeries);
}
