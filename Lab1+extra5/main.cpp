#include "TestFramework.h"
#include "Profile.h"
#include "Book.h"
#include "BookCharacter.h"
#include "PublishingHouse.h"
#include "Date.h"

void TestCharacter() {
	//Test Constructors
	{
		BookCharacter<std::string> character("Kotygoroshko");
		ASSERT_EQUAL(character.get_default_name(), "Kotygoroshko");
		ASSERT_EQUAL(character.get_all_names(),
			std::set<std::string>{ "Kotygoroshko" });
	}
	{
		std::set<std::string> names{ "Harry", "Potter", "a boy who survived" };
		BookCharacter<std::string> character("Harry Potter", names);
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

		character.update_role("Philosopher's Stone", Role::main);
		ASSERT_EQUAL(character.get_role("Philosopher's Stone"), Role::main);

		character.update_role("Philosopher's Stone", Role::secondary);
		ASSERT_EQUAL(character.get_role("Philosopher's Stone"), Role::secondary);

		character.decrease("Philosopher's Stone");
		ASSERT_EQUAL(character.get_role("Philosopher's Stone"), Role::episodic);

		character.decrease("Philosopher's Stone");
		ASSERT_EQUAL(character.get_role("Philosopher's Stone"), Role::episodic);

		character.promote("Philosopher's Stone");
		character.promote("Philosopher's Stone");
		ASSERT_EQUAL(character.get_role("Philosopher's Stone"), Role::main);

		character.promote("Philosopher's Stone");
		ASSERT_EQUAL(character.get_role("Philosopher's Stone"), Role::main);

		auto books = character.get_all_books();
		ASSERT_EQUAL(books, std::set<std::string>{ "Philosopher's Stone" });

		character.update_role("Chamber of Secrets", Role::main);
		character.update_role("Goblet of Fire", Role::episodic);
		books.insert("Goblet of Fire");
		books.insert("Chamber of Secrets");

		ASSERT_EQUAL(character.get_all_books(), books);

		character.erase_book("Chamber of Secrets");
		books.erase("Chamber of Secrets");

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

void TestHouse() {
	PublishingHouse PHouse;
	Book first_book("Book", { "" }, Date(1, 1, 1, 1, 1, 1), 10, "");
	auto id = PHouse.add_book(first_book);
	auto series = PHouse.get_series();
	AssertEqual(series, std::vector<std::vector<Book>> { {first_book} });
	Book second_book("Book2", { "" }, Date(1, 1, 1,1, 0, 1), 10, "");
	Book third_book("Book3", { "" }, Date(1,1,1,0, 1, 1), 10, "");
	PHouse.add_book(second_book);
	PHouse.add_book(third_book);
	AssertEqual(PHouse.get_series(),
		std::vector<std::vector<Book>> { {second_book,third_book,first_book} });


}


int main() {
	TestRunner tr;
	RUN_TEST(tr, TestCharacter);
	RUN_TEST(tr, TestBook);
	RUN_TEST(tr, TestHouse);
}
