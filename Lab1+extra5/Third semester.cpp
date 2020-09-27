#include "TestFramework.h"
#include "Profile.h"
#include "Book.h"
#include "BookCharacter.h"
#include "PublishingHouse.h"
#include "Date.h"

void TestHouse() {
	PublishingHouse PHouse;
	Book first_book("Book", "", Date(1, 1, 1), 10, "");
	auto id = PHouse.add_book(first_book);
	auto series = PHouse.get_series();
	AssertEqual(series, std::vector<std::vector<Book>> { {first_book} });
}


int main() {
	TestRunner tr;
	RUN_TEST(tr, TestHouse);
}
