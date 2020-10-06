/**
\file
\brief h file of class PublishingHouse

definitions of PublishingHouse methods and operators, is_subset and is_subvector functions are here
*/


#pragma once

#include "Book.h"
#include "BookCharacter.h"
#include "Date.h"

#include <map>
#include <algorithm>

using book_id = std::string;
using character_id = std::string;

/**
\brief class for storing information about books and characters in them

has information about:
- all Books of publishing house
- all Characters of publishing house
*/

class PublishingHouse
{
public:

	/**
	\brief basic constructor of Book.

	Constructs empty Publishing House
	*/
	PublishingHouse();
	
	/**
	\brief adds book

	\param book const reference to added Book
	\return id of added book (std::string)
	*/
	book_id add_book(const Book& book);

	/**
	\brief adds book character

	\param character const reference to added BookCharacter
	\return id of added BookCharacter (std::string)
	*/
	character_id add_character(const BookCharacter<book_id>& character);

	/**
	\brief erases book

	Erases book with that id from books and characters
	\param id id of erased book
	\throw throw std::invalid_argument("incorrect id") if id isn't valid
	*/
	void ban_book(const book_id& id);

	/**
	\brief erases character

	Erases character with that id
	\param id id of erased character
	\throw throw std::invalid_argument("incorrect id") if id isn't valid
	*/
	void ban_character(const character_id& id);

	/**
	\brief returns all series of books

	Two books belong to the same series, if they have common main or secondary character.
	Book in series are sorted by release dat.
	*/
	std::vector<std::vector<Book>>get_series();

	/**
	\brief adds book character in book

	\param id_character id of character
	\param id_book id of book
	\param role Role of character in book
	\throw throw std::invalid_argument("incorrect id") if id_character or id_book isn't valid
	*/
	void add_character_in_book(const character_id& id_character, const book_id& id_book, Role role);

	/**
	\brief erases book character from book

	Character will be deleted from book and will not be deleted from publishing house in general
	\param id_character id of character
	\param id_book id of book
	\throw throw std::invalid_argument("incorrect id") if id_character or id_book isn't valid
	*/
	void erase_character_from_book(const character_id& id_character, const book_id& id_book);

	/**
	\brief access to BookCharacter by its id

	\param id id of character
	\throw throw std::invalid_argument("incorrect id") if id isn't valid
	\return BookCharacter with that id
	*/
	BookCharacter<book_id> get_character(const character_id& id);

	/**
	\brief access to Book by its id

	\param id id of book
	\throw throw std::invalid_argument("incorrect id") if id isn't valid
	\return Book with that id
	*/
	Book get_book(const  book_id& id);

	/**
	\brief promotes role of character in some book

	role can't be bigger than Role::main
	\param id_character id of character
	\param id_book id of book
	\throw throw std::invalid_argument("incorrect id") if id_character or id_book isn't valid
	*/
	void promote_role(const character_id& id_character, const book_id& id_book);

	/**
	\brief decreases role of character in some book

	role can't be lesser than Role::episodic
	\param id_character id of character
	\param id_book id of book
	\throw throw std::invalid_argument("incorrect id") if id_character or id_book isn't valid
	*/
	void decrease_role(const character_id&, const  book_id&);

	/**
	\brief access to role of book character in book

	\param id_character id of book
	\param id_book id of book
	\throw throw std::invalid_argument("incorrect id") if id_character or id_book isn't valid
	\return Role of character in that book
	*/
	Role get_role(const character_id& id_character, const  book_id& id_book);

	/**
	\brief access to all characters that are in some book

	\throw throw std::invalid_argument("incorrect id") if id_book isn't valid
	*/
	std::set<character_id> get_characters(const book_id& id_book);

	/**
	\brief access to important characters that are in some book

	Important characters are characters with Role::main or Role::secondary roles
	\throw throw std::invalid_argument("incorrect id") if id_book isn't valid
	*/
	std::set<character_id> get_important_characters(const book_id& id_book);

	/**
	\brief access to all books of some character

	\param id_character id of book character
	\throw throw std::invalid_argument("incorrect id") if id_character isn't valid
	*/
	std::set<book_id> get_books(const  character_id& id_character);
private:
	std::map<book_id, std::set<character_id>>books_characters;				///<map of books and all their characters
	std::map<book_id, std::set<character_id>>books_important_characters;	///<map of books and their important characters

	std::map<book_id, Book>books;											///<all books of publishing house
	std::map<character_id, BookCharacter<book_id>>characters;				///<all book characters of publishing house

	friend bool operator==(const PublishingHouse& lhs, const PublishingHouse& rhs);
	friend std::ostream& operator<<(std::ostream&, PublishingHouse PHouse);
};

/**
Operator == for PublishingHouses
*/
bool operator==(const PublishingHouse& lhs, const PublishingHouse& rhs);

/**
Operator << for PublishingHouses in streams
*/
std::ostream& operator<<(std::ostream&, PublishingHouse PHouse);

/**
\brief function that checks if is some set subset of another set

\param lesser lesser set
\param bigger bigger set
\return true if all elements of lesser are in bigger and size of lesser > 0 (or both sets are empty) and false otherwise
*/
template<typename T>
bool is_subset(std::set<T> lesser, std::set<T> bigger) {
	if (lesser.size() == 0 && bigger.size()!=0) {
		return false;
	}
	for (const auto& i : lesser) {
		if (bigger.find(i) == end(bigger)) {
			return false;
		}
	}
	return true;
}

/**
\brief function that checks if is some vector subvector of another vector

\param lesser lesser vector
\param bigger bigger vector
\return true if all elements of lesser are in bigger and size of lesser > 0 (or both vectors are empty) and false otherwise
*/
template<typename T>
bool is_subvector(std::vector<T> lesser, std::vector<T> bigger) {
	if (lesser.size() == 0 && bigger.size() != 0) {
		return false;
	}
	for (const auto& i : lesser) {
		if (find(bigger.begin(),bigger.end(),i) == end(bigger)) {
			return false;
		}
	}
	return true;
}