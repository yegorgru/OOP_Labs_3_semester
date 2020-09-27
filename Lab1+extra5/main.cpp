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

}

void TestHouse() {
	PublishingHouse PHouse;
	Book first_book("Book", "", Date(1, 1, 1,1, 1,1), 10, "");
	auto id = PHouse.add_book(first_book);
	auto series = PHouse.get_series();
	AssertEqual(series, std::vector<std::vector<Book>> { {first_book} });
	Book second_book("Book2", "", Date(1, 1, 1,1, 0, 1), 10, "");
	Book third_book("Book3", "", Date(1,1,1,0, 1, 1), 10, "");
	PHouse.add_book(second_book);
	PHouse.add_book(third_book);
	AssertEqual(PHouse.get_series(),
		std::vector<std::vector<Book>> { {second_book,third_book,first_book} });
}


int main() {
	TestRunner tr;
	RUN_TEST(tr, TestCharacter);
	RUN_TEST(tr, TestHouse);
}
