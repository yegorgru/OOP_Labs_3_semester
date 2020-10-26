#ifndef TEACHER_H
#define TEACHER_H

#include <string>
#include <set>

#include "subject.h"

class Teacher
{
public:
    Teacher();
    explicit Teacher(const std::string& name, const std::string& surname,
                     const std::string& phone = "", const std::string& mail = "",
                     const std::string& address = "", const std::string& website = "");

    std::string get_name() const;
    void set_name(const std::string& new_name);

    std::string get_surname() const;
    void set_surname(const std::string& new_surname);

    std::string get_phone() const;
    void set_phone(const std::string& new_phone);

    std::string get_mail() const;
    void set_mail(const std::string& new_mail);

    std::string get_address()const;
    void set_address(const std::string& new_address);

    std::string get_website() const;
    void set_website(const std::string& new_website);
private:
    std::string name;
    std::string surname;
    std::string phone;
    std::string mail;
    std::string address;
    std::string website;
};

bool operator==(const Teacher& lhs,const Teacher& rhs);

namespace std
{
    template<> struct  less<Teacher>
    {
        bool operator() (const Teacher& lhs, const Teacher& rhs) const
        {
            return lhs.get_name()<rhs.get_name() || (lhs.get_name() == rhs.get_name()
                    && lhs.get_surname() < rhs.get_surname());
        }
    };
}

#endif // TEACHER_H
