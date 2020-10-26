#ifndef TEACHEREDIT_H
#define TEACHEREDIT_H

#include <QDialog>

#include "teacher.h"

namespace Ui {
class TeacherEdit;
}

class TeacherEdit : public QDialog
{
    Q_OBJECT

public:
    explicit TeacherEdit(bool new_teacher = true,const QString& name = "",
                         const QString& surname="",const QString& phone="",
                         const QString& mail="", const QString& address="",
                         const QString& website="",QWidget *parent = nullptr);
    ~TeacherEdit();

    Teacher get_teacher();

    bool is_cancel();

    bool deleted();
private slots:

    void on_add_teacher_button_clicked();

    void on_cancel_button_clicked();

    void on_delete_button_clicked();

private:
    Teacher teacher;
    bool cancel=true;
    bool is_deleted = false;
    Ui::TeacherEdit *ui;
};

#endif // TEACHEREDIT_H
