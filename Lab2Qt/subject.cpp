#include "subject.h"

Subject::Subject(){}

Subject::Subject(const std::string& name,const std::string& room,
                 const std::string& note):
    name(name),room(room),note(note){};

std::string Subject::get_name() const{
    return this->name;
}
void Subject::set_name(const std::string& new_name){
    this->name = new_name;
}

std::string Subject::get_room()const{
     return this->room;
}
void Subject::set_room(const std::string& new_room){
    this->room = new_room;
}

std::string Subject::get_note()const{
     return this->note;
}
void Subject::set_note(const std::string& new_note){
    this->note = new_note;
}
