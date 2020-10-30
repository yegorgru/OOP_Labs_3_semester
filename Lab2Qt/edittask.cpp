#include "edittask.h"
#include "ui_edittask.h"

EditTask::EditTask(bool exam,bool new_task,const std::set<std::string>& teachers, const std::set<std::string>&subjects,
                   QWidget *parent,const std::string& title,
                   const std::string& teacher,const std::string& subject,
                   const Date& date,const std::string& note,
                   bool is_imporatant,const ExamType exam_type) :
    QDialog(parent),
    ui(new Ui::EditTask)
{
    ui->setupUi(this);
    if(is_imporatant){
        ui->important_checkBox->setChecked(true);
    }
    bool was_teacher=false;
    for(const std::string& t:teachers){
        if(t==teacher){
            was_teacher = true;
            continue;
        }
        ui->teacher_box->addItem(QString::fromStdString(t));
    }
    if(was_teacher){
        ui->teacher_box->insertItem(0,QString::fromStdString(teacher));
        ui->teacher_box->setCurrentIndex(0);
    }

    ui->title_line->setText(QString::fromStdString(title));
    if(new_task){
        ui->dateEdit->setDate(QDate::currentDate());
    }
    else{
        ui->dateEdit->setDate(QDate(date.get_year(),date.get_month(),date.get_day()));
    }
    ui->note_line->setText(QString::fromStdString(note));


    bool was_subject = false;
    for(const std::string& s:subjects){
        if(s==subject){
            was_subject = true;
            continue;
        }
        ui->subject_box->addItem(QString::fromStdString(s));
    }
    if(was_subject){
        ui->subject_box->insertItem(0,QString::fromStdString(subject));
        ui->teacher_box->setCurrentIndex(0);
    }

    if(exam){
        ui->exam_type_box->addItem("Written");
        ui->exam_type_box->addItem("Oral");
        ui->exam_type_box->setCurrentIndex(exam_type==ExamType::written ? 0:1);
    }

    ui->add_task_button->setAutoDefault(true);
    ui->cancel_button->setAutoDefault(false);
    ui->delete_button->setAutoDefault(false);

    if(!new_task){
        ui->add_task_button->setText("Okay");
    }
    else{
        ui->delete_button->setVisible(false);
    }

    if(!exam){
        ui->exam_type_box->setVisible(false);
        ui->type_label->setVisible(false);
    }
}

EditTask::~EditTask()
{
    delete ui;
}

bool EditTask::is_cancel(){
    return this->cancel;
}

ExamType EditTask::get_type(){
    return this->type;
}

Task EditTask::get_task(){
    return this->task;
}

void EditTask::on_cancel_button_clicked()
{
    this->cancel=true;
    this->close();
}

void EditTask::on_add_task_button_clicked()
{
    this->task.set_title(ui->title_line->text().toStdString());
    this->task.set_teacher(ui->teacher_box->currentText().toStdString());
    this->task.set_note(ui->subject_box->currentText().toStdString());
    this->task.set_note(ui->note_line->text().toStdString());
    Date date;
    date.set_day(ui->dateEdit->date().day());
    date.set_month(ui->dateEdit->date().month());
    date.set_year(ui->dateEdit->date().year());
    this->task.set_date(date);
    this->type=ui->exam_type_box->currentText()=="Written"?ExamType::written : ExamType::oral;
    cancel = false;
    bool is_important = ui->important_checkBox->isChecked();
    this->task.set_importance(is_important);
    this->close();
}

void EditTask::on_delete_button_clicked()
{
    this->is_deleted = true;
    this->close();
}

bool EditTask::deleted(){
    return this->is_deleted;
}
