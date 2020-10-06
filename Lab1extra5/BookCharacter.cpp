/**
\file
\brief cpp file of class BookCharacter

std::ostream& operator<<(std::ostream& os, const Role& role) implementation is here
*/


#include "BookCharacter.h"

std::ostream& operator<<(std::ostream& os, const Role& role)
{
    if (role == Role::main) {
        os << "main";
    }
    else if (role == Role::secondary) {
        os << "secondary";
    }
    else if (role == Role::episodic) {
        os << "episodic";
    }
    return os;
}
