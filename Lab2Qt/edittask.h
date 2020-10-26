#ifndef EDITTASK_H
#define EDITTASK_H

#include <QDialog>

#include <set>
#include <string>

#include "task.h"
#include "exam.h"
#include "date.h"

namespace Ui {
class EditTask;
}

class EditTask : public QDialog
{
    Q_OBJECT

public:
    explicit EditTask(bool exam,bool new_task,const std::set<std::string>& teachers, const std::set<std::string>&subjects,
                      QWidget *parent=nullptr,const std::string& title="",
                      const std::string& teacher="",const std::string& subject="",
                      const Date& date=Date(),const std::string& note="",bool is_imporatant = false,
                      const ExamType exam_type=ExamType::written);
    ~EditTask();

    bool is_cancel();

    ExamType get_type();

    Task get_task();

    bool deleted();

private slots:
    void on_cancel_button_clicked();

    void on_add_task_button_clicked();

    void on_delete_button_clicked();

private:
    Ui::EditTask *ui;

    bool cancel = true;
    bool is_deleted=false;

    Task task;
    ExamType type;
};

#endif // EDITTASK_H
