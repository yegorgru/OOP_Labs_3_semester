#include "task.h"

Task::Task(){}

Task::Task(const std::string& title,const Date& date,const std::string& subject,const std::string& teacher,
                   const std::string& note):
    title(title),subject(subject),teacher(teacher),date(date),note(note) {}

std::string Task::get_title() const{
    return this->title;
}
void Task::set_title(const std::string& new_title){
    this->title = new_title;
}

std::string Task::get_subject() const{
    return this->subject;
}

void Task::set_subject(const std::string& subject){
    this->subject = subject;
}

std::string Task::get_teacher() const{
    return this->teacher;
}

void Task::set_teacher(const std::string& teacher){
    this->teacher = teacher;
}

Date Task::get_date() const{
    return this->date;
}
void Task::set_date(const Date& new_date){
    this->date = new_date;
}

std::string Task::get_note() const{
    return this->note;
}
void Task::set_note(const std::string& new_note){
    this->note = new_note;
}

bool Task::get_importance() const{
    return this->is_important;
}

void Task::set_importance(bool importance){
    this->is_important=importance;
}
