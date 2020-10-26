#include "subjectedit.h"
#include "ui_subjectedit.h"

SubjectEdit::SubjectEdit(bool new_subject,const QString& name,
                                     const QString& room,const QString& note,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SubjectEdit)
{
    ui->setupUi(this);
    if(!new_subject){
        ui->add_subject_button->setText("Okay");
    }
    else{
        ui->delete_button->setVisible(false);
    }
    ui->cancel_button->setAutoDefault(false);
    ui->add_subject_button->setAutoDefault(true);
    ui->delete_button->setAutoDefault(false);

    ui->name_line->setText(name);
    ui->room_line->setText(room);
    ui->note_line->setText(note);
}

SubjectEdit::~SubjectEdit()
{
    delete ui;
}

bool SubjectEdit::is_cancel(){
    return this->cancel;
}

Subject SubjectEdit::get_subject(){
    return this->subject;
}

void SubjectEdit::on_cancel_button_clicked()
{
    this->cancel = true;
    this->close();
}

void SubjectEdit::on_add_subject_button_clicked()
{
    this->subject.set_name(ui->name_line->text().toStdString());
    this->subject.set_room(ui->room_line->text().toStdString());
    this->subject.set_note(ui->note_line->text().toStdString());
    this->cancel = false;
    this->close();
}

void SubjectEdit::on_delete_button_clicked()
{
    this->is_deleted = true;
    this->close();
}

bool SubjectEdit::deleted(){
    return this->is_deleted;
}
