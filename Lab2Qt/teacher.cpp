#include "teacher.h"

Teacher::Teacher(){}

Teacher::Teacher(const std::string& name, const std::string& surname,
                 const std::string& phone, const std::string& mail,
                 const std::string& address, const std::string& website):
    name(name), surname(surname),phone(phone),mail(mail),address(address),website(website) {}

void Teacher::set_name(const std::string& new_name){
    this->name = new_name;
}

std::string Teacher::get_name() const{
    return this->name;
}

void Teacher::set_surname(const std::string& new_surname){
    this->surname = new_surname;
}

std::string Teacher::get_surname() const{
    return this->surname;
}

void Teacher::set_phone(const std::string& new_phone){
    this->phone = new_phone;
}

std::string Teacher::get_phone()const{
    return this->phone;
}

void Teacher::set_mail(const std::string& new_mail){
    this->mail = new_mail;
}

std::string Teacher::get_mail()const{
    return this->mail;
}

void Teacher::set_address(const std::string& new_address){
    this->address = new_address;
}

std::string Teacher::get_address()const{
    return this->address;
}

void Teacher::set_website(const std::string& new_website){
    this->website = new_website;
}

std::string Teacher::get_website()const{
    return this->website;
}

bool operator==(const Teacher& lhs,const Teacher& rhs){
    return lhs.get_name() == rhs.get_name() && lhs.get_surname() == rhs.get_surname();
}
