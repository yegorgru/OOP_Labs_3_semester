#ifndef SUBJECTEDIT_H
#define SUBJECTEDIT_H

#include <QDialog>
#include <QString>

#include "subject.h"

namespace Ui {
class SubjectEdit;
}

class SubjectEdit : public QDialog
{
    Q_OBJECT

public:
    explicit SubjectEdit(bool new_subject=true,const QString& name="",
                         const QString& room="",const QString& note="",QWidget *parent = nullptr);
    ~SubjectEdit();

    bool is_cancel();
    Subject get_subject();

    bool deleted();
private slots:
    void on_cancel_button_clicked();

    void on_add_subject_button_clicked();

    void on_delete_button_clicked();

private:
    Ui::SubjectEdit *ui;
    Subject subject;
    bool cancel=true;
    bool is_deleted = false;
};

#endif // SUBJECTEDIT_H
