#include "teacheredit.h"
#include "ui_teacheredit.h"

TeacherEdit::TeacherEdit(bool new_teacher,const QString& name,
                         const QString& surname,const QString& phone,
                         const QString& mail, const QString& address,
                         const QString& website,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TeacherEdit)
{
    ui->setupUi(this);
    if(!new_teacher){
        ui->add_teacher_button->setText("Okay");
    }
    else{
        ui->delete_button->setVisible(false);
    }
    ui->name_line->setText(name);
    ui->surname_line->setText(surname);
    ui->phone_line->setText(phone);
    ui->mail_line->setText(mail);
    ui->address_line->setText(address);
    ui->website_line->setText(website);

    ui->cancel_button->setAutoDefault(false);
    ui->add_teacher_button->setAutoDefault(true);
    ui->delete_button->setAutoDefault(false);
}

TeacherEdit::~TeacherEdit()
{
    delete ui;
}

bool TeacherEdit::is_cancel(){
    return this->cancel;
}

Teacher TeacherEdit::get_teacher(){
    return this->teacher;
}

void TeacherEdit::on_add_teacher_button_clicked()
{
    this->teacher.set_name(ui->name_line->text().toStdString());
    this->teacher.set_surname(ui->surname_line->text().toStdString());
    this->teacher.set_phone(ui->phone_line->text().toStdString());
    this->teacher.set_mail(ui->mail_line->text().toStdString());
    this->teacher.set_address(ui->address_line->text().toStdString());
    this->teacher.set_website(ui->website_line->text().toStdString());
    this->cancel = false;
    this->close();
}

void TeacherEdit::on_cancel_button_clicked()
{
    this->cancel = true;
    this->close();
}

void TeacherEdit::on_delete_button_clicked()
{
    this->is_deleted = true;
    this->close();
}

bool TeacherEdit::deleted(){
    return this->is_deleted;
}
