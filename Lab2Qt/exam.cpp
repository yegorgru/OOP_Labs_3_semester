#include "exam.h"

Exam::Exam(const std::string& title,const Date& date,const std::string& subject,const std::string& teacher,
                const ExamType type,const std::string& note) :
    Task(title,date,subject,teacher,note),type(type){}

Exam::Exam(const Task& task):Task(task){}

ExamType Exam::get_type() const{
    return this->type;
}
void Exam::set_type(const ExamType new_type){
    this->type = new_type;
}
