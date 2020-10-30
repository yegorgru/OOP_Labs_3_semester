#ifndef EXAM_H
#define EXAM_H

#include <string>
#include "task.h"
#include "date.h"

enum class ExamType{
    written,
    oral
};

class Exam : public Task
{
public:
    explicit Exam(const std::string& title,const Date& date=Date(),const std::string& subject="", const std::string& teacher="",
                  const ExamType type=ExamType::written,const std::string& note = "");

    explicit Exam(const Task& Task);

    ExamType get_type() const;
    void set_type(const ExamType new_type);

private:
    ExamType type;
};

namespace std
{
    template<> struct  less<Exam>
    {
        bool operator() (const Exam& lhs, const Exam& rhs) const
        {
            return lhs.get_date()<rhs.get_date() ||
                    (lhs.get_date()== rhs.get_date() && lhs.get_title()<rhs.get_title());
        }
    };
}

#endif // EXAM_H
