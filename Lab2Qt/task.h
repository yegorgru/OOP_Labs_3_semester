#ifndef TASK_H
#define TASK_H

#include <string>
#include "date.h"

class Task
{
public:
    Task();
    explicit Task(const std::string& title,const Date& date=Date(),const std::string& subject="",
                  const std::string& teacher="", const std::string& note="");

    std::string get_title() const;
    void set_title(const std::string& new_title);

    std::string get_subject() const;
    void set_subject(const std::string& subject);

    std::string get_teacher() const;
    void set_teacher(const std::string& teacher);

    Date get_date() const;
    void set_date(const Date& new_date);

    std::string get_note() const;
    void set_note(const std::string& new_note);

    bool get_importance() const;
    void set_importance(bool importance);
protected:
    std::string title;
    std::string subject;
    std::string teacher;
    Date date;
    std::string note;
    bool is_important=false;
};

namespace std
{
    template<> struct  less<Task>
    {
        bool operator() (const Task& lhs, const Task& rhs) const
        {
            return lhs.get_date()<rhs.get_date() ||
                    (lhs.get_date()== rhs.get_date() && lhs.get_title()<rhs.get_title());
        }
    };
}

#endif // TASK_H
