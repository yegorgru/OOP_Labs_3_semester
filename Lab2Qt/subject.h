#ifndef SUBJECT_H
#define SUBJECT_H

#include <string>
#include <set>

#include "teacher.h"

class Subject
{
public:
    explicit Subject(const std::string& name,const std::string& room="",
                     const std::string& note = "");
    Subject();

    std::string get_name() const;
    void set_name(const std::string& new_name);

    std::string get_room() const;
    void set_room(const std::string& new_room);

    std::string get_note() const;
    void set_note(const std::string& new_note);

private:
    std::string name;
    std::string room;

    std::string note;
};

namespace std
{
    template<> struct  less<Subject>
    {
        bool operator() (const Subject& lhs, const Subject& rhs) const
        {
            return lhs.get_name()<rhs.get_name();
        }
    };
}

#endif // SUBJECT_H
