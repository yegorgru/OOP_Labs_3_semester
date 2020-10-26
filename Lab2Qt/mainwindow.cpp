#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /*QRect rec = QApplication::desktop()->availableGeometry();
    HEIGHT = rec.height();
    WIDTH = rec.width();*/
    HEIGHT=this->size().height();
    WIDTH=this->size().width();

    for(const auto& teacher:teachers){
        ui->teachers_list->addItem(QString::fromStdString(teacher.get_name()+" "+teacher.get_surname()));
    }
    for(const auto& subject:subjects){
        ui->subjects_list->addItem(QString::fromStdString(subject.get_name()));
    }
    for(const auto& task:homeworks){
        std::ostringstream os;
        os<<task.get_date()<<" | "<<task.get_title();
        ui->homeworks_list->addItem(QString::fromStdString(os.str()));
    }
    QPixmap pixmap(":/images/images/plus_image.jpg");
    QIcon ButtonIcon(pixmap);
    ui->add_teacher_button->setIcon(ButtonIcon);
    ui->add_teacher_button->setIconSize(QSize(HEIGHT/10, HEIGHT/10));
    ui->add_teacher_button->setFixedSize(QSize(HEIGHT/10, HEIGHT/10));

    ui->add_subject_button->setIcon(ButtonIcon);
    ui->add_subject_button->setIconSize(QSize(HEIGHT/10, HEIGHT/10));
    ui->add_subject_button->setFixedSize(QSize(HEIGHT/10, HEIGHT/10));

    ui->add_task_button->setIcon(ButtonIcon);
    ui->add_task_button->setIconSize(QSize(HEIGHT/10, HEIGHT/10));
    ui->add_task_button->setFixedSize(QSize(HEIGHT/10, HEIGHT/10));

    ui->add_homework_button_page->setIcon(ButtonIcon);
    ui->add_homework_button_page->setIconSize(QSize(HEIGHT/10, HEIGHT/10));
    ui->add_homework_button_page->setFixedSize(QSize(HEIGHT/10, HEIGHT/10));

    ui->add_exam_button_page->setIcon(ButtonIcon);
    ui->add_exam_button_page->setIconSize(QSize(HEIGHT/10, HEIGHT/10));
    ui->add_exam_button_page->setFixedSize(QSize(HEIGHT/10, HEIGHT/10));

    ui->add_exam_button->setVisible(false);
    ui->add_homework_button->setVisible(false);

    QDate date = QDate::currentDate();
    QTime time = QTime::currentTime();
    current_date = Date(time.second(),time.minute(),time.hour(),
                        date.day(),date.month(),date.year());
    std::ostringstream os;
    os<<current_date;
    ui->last_update_label->setText(QString::fromStdString("Last update: "+ os.str()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_add_teacher_button_clicked()
{
    TeacherEdit teacher_window;
    teacher_window.setModal(true);
    teacher_window.setFixedWidth(WIDTH/3);
    teacher_window.setFixedHeight(HEIGHT/3);
    teacher_window.exec();
    if(!teacher_window.is_cancel()){
        Teacher teacher = teacher_window.get_teacher();
        if(teacher.get_name()==""){
            teacher.set_name("default_name");
        }
        if(teacher.get_surname()==""){
            teacher.set_surname("default_surname");
        }
        for(auto i:teacher.get_name()){
            if(i==' '){
                QMessageBox::critical(this,"Attention!","Teacher's name cannot contain spaces");
                return;
            }
        }
        for(auto i:teacher.get_surname()){
            if(i==' '){
                QMessageBox::critical(this,"Attention!","Teacher's surname cannot contain spaces");
                return;
            }
        }
        if(this->teachers.find(teacher)==end(this->teachers)){
            ui->teachers_list->addItem(QString::fromStdString(teacher.get_name()+" "+teacher.get_surname()));
            this->teachers.insert(std::move(teacher));
        }
        else{
            QMessageBox::critical(this,"Attention!","Teacher with that name already exists");
        }
    }
}

void MainWindow::on_teachers_list_itemClicked(QListWidgetItem *item)
{
    std::string name_surname=item->text().toStdString();
    size_t middle = name_surname.find(' ');
    std::string name(name_surname.begin(),name_surname.begin()+middle);
    std::string surname(name_surname.begin()+middle+1,name_surname.end());
    auto it=this->teachers.find(Teacher(name,surname));
    Teacher old_teacher=*it;
    TeacherEdit teacher_window(false,QString::fromStdString(old_teacher.get_name()),
                               QString::fromStdString(old_teacher.get_surname()),
                               QString::fromStdString(old_teacher.get_phone()),
                               QString::fromStdString(old_teacher.get_mail()),
                               QString::fromStdString(old_teacher.get_address()),
                               QString::fromStdString(old_teacher.get_website()));
    teacher_window.setModal(true);
    teacher_window.setFixedWidth(WIDTH/3);
    teacher_window.setFixedHeight(HEIGHT/3);
    teacher_window.exec();
    if(teacher_window.deleted()){
        this->teachers.erase(old_teacher);
        delete item;
        return;
    }
    if(!teacher_window.is_cancel()){
        Teacher teacher = teacher_window.get_teacher();
        if(teacher.get_name()==""){
            teacher.set_name("default_name");
        }
        if(teacher.get_surname()==""){
            teacher.set_surname("default_surname");
        }
        for(auto i:teacher.get_name()){
            if(i==' '){
                QMessageBox::critical(this,"Attention!","Teacher's name cannot contain spaces");
                return;
            }
        }
        for(auto i:teacher.get_surname()){
            if(i==' '){
                QMessageBox::critical(this,"Attention!","Teacher's surname cannot contain spaces");
                return;
            }
        }
        auto it2 = this->teachers.find(teacher);
        if(it2==end(this->teachers) || it2==it){
            this->teachers.erase(old_teacher);
            item->setText(QString::fromStdString(teacher.get_name()+" "+teacher.get_surname()));
            this->teachers.insert(std::move(teacher));
        }
        else{
            QMessageBox::critical(this,"Attention!","Teacher with that name already exists");
        }
    }
}

void MainWindow::on_add_subject_button_clicked()
{
    SubjectEdit subject_window;
    subject_window.setModal(true);
    subject_window.setFixedWidth(WIDTH/3);
    subject_window.setFixedHeight(HEIGHT/3);
    subject_window.exec();
    if(!subject_window.is_cancel()){
        Subject subject = subject_window.get_subject();
        if(subject.get_name()==""){
            subject.set_name("default_name");
        }
        if(this->subjects.find(subject)==end(this->subjects)){
            ui->subjects_list->addItem(QString::fromStdString(subject.get_name()));
            this->subjects.insert(std::move(subject));
        }
        else{
            QMessageBox::critical(this,"Attention!","Subject with that name already exists");
        }
    }
}

void MainWindow::on_subjects_list_itemClicked(QListWidgetItem *item)
{
    auto it=this->subjects.find(Subject(item->text().toStdString()));
    Subject old_subject=*it;
    SubjectEdit subject_window(false,QString::fromStdString(old_subject.get_name()),
                               QString::fromStdString(old_subject.get_room()),
                               QString::fromStdString(old_subject.get_note()),this);
    subject_window.setModal(true);
    subject_window.setFixedWidth(WIDTH/3);
    subject_window.setFixedHeight(HEIGHT/3);
    subject_window.exec();
    if(subject_window.deleted()){
        this->subjects.erase(old_subject);
        delete item;
        return;
    }
    if(!subject_window.is_cancel()){
        Subject subject = subject_window.get_subject();
        if(subject.get_name()==""){
            subject.set_name("default_name");
        }
        auto it2 = this->subjects.find(subject);
        if(it2==end(this->subjects) || it2==it){
            this->subjects.erase(old_subject);
            item->setText(QString::fromStdString(subject.get_name()));
            this->subjects.insert(std::move(subject));
        }
        else{
            QMessageBox::critical(this,"Attention!","Subject with that name already exists");
        }
    }
}

void MainWindow::on_add_task_button_clicked()
{
    ui->add_exam_button->setVisible(true);
    ui->add_homework_button->setVisible(true);
    ui->add_task_button->setVisible(false);
}

void MainWindow::on_add_exam_button_clicked()
{

}

void MainWindow::on_add_homework_button_clicked()
{

}

void MainWindow::add_task(bool is_exam){
    std::set<std::string>all_teachers;
    std::set<std::string>all_subjects;
    for(const auto& i:teachers){
        all_teachers.insert(i.get_name()+" "+i.get_surname());
    }
    for(const auto& i:subjects){
        all_subjects.insert(i.get_name());
    }
    EditTask task_window(is_exam,true,all_teachers,all_subjects,this);
    task_window.setModal(true);
    task_window.setFixedWidth(WIDTH/3);
    task_window.setFixedHeight(HEIGHT/3);
    task_window.exec();
    if(!task_window.is_cancel()){
        if(is_exam){
            Exam exam(task_window.get_task());
            exam.set_type(task_window.get_type());
            if(exam.get_title()==""){
                exam.set_title("default_title");
            }
            if(this->exams.find(exam)==end(this->exams)){
                add_exam_content(exam);
                this->exams.insert(std::move(exam));
            }
            else{
                QMessageBox::critical(this,"Attention!","Exam with that name already exists");
            }
        }
        else{
            Task homework = task_window.get_task();

            if(homework.get_title()==""){
                homework.set_title("default_title");
            }
            if(this->homeworks.find(homework)==end(this->homeworks)){
                add_homework_content(homework);
                this->homeworks.insert(std::move(homework));
            }
            else{
                if(is_exam){
                    QMessageBox::critical(this,"Attention!","Exam with those name and date already exists");
                }
                else{
                    QMessageBox::critical(this,"Attention!","Homework with those name and date already exists");
                }
            }
        }
    }
}

void MainWindow::on_add_homework_button_page_clicked()
{
    add_task(false);
}

void MainWindow::on_add_exam_button_page_clicked()
{
    add_task(true);
}

void MainWindow::on_exams_list_itemClicked(QListWidgetItem *item)
{
    std::istringstream is(item->text().toStdString());
    Date date;
    is>>date;
    is.ignore();
    is.ignore();
    is.ignore();
    std::string str;
    is>>str;
    update_exam(str,date,item);
}

void MainWindow::on_homeworks_list_itemClicked(QListWidgetItem *item)
{
    std::istringstream is(item->text().toStdString());
    Date date;
    is>>date;
    is.ignore();
    is.ignore();
    is.ignore();
    std::string str;
    is>>str;
    update_homework(str,date,item);
}

void MainWindow::add_homework_content(const Task& new_homework){
    std::ostringstream new_os;
    new_os<<new_homework.get_date()<<" | "<<new_homework.get_title();
    ui->homeworks_list->addItem(QString::fromStdString(new_os.str()));
    if(new_homework.get_importance()){
        ui->important_list->addItem("Home "+QString::fromStdString(new_os.str()));
    }
}

void MainWindow::update_homework_content(const Task& old_homework,const Task& new_homework){
    std::ostringstream old_os;
    old_os<<old_homework.get_date()<<" | "<<old_homework.get_title();
    auto items = ui->homeworks_list->findItems(QString::fromStdString(old_os.str()),Qt::MatchExactly);
    std::ostringstream new_os;
    new_os<<new_homework.get_date()<<" | "<<new_homework.get_title();
    items.back()->setText(QString::fromStdString(new_os.str()));
    if(old_homework.get_importance() && new_homework.get_importance()){
        auto items = ui->important_list->findItems("Home "+QString::fromStdString(old_os.str()),
                                                   Qt::MatchExactly);
        items.back()->setText("Home " + QString::fromStdString(new_os.str()));
    }
    else if(!old_homework.get_importance() && new_homework.get_importance()){
        ui->important_list->addItem("Home "+QString::fromStdString(new_os.str()));
    }
    else if(old_homework.get_importance() && !new_homework.get_importance()){
        auto items = ui->important_list->findItems("Home "+QString::fromStdString(old_os.str()),
                                                   Qt::MatchExactly);
        delete items.back();
    }
}

void MainWindow::add_exam_content(const Exam& new_exam){
    std::ostringstream new_os;
    new_os<<new_exam.get_date()<<" | "<<new_exam.get_title();
    ui->exams_list->addItem(QString::fromStdString(new_os.str()));
    if(new_exam.get_importance()){
        ui->important_list->addItem("Exam "+QString::fromStdString(new_os.str()));
    }
}

void MainWindow::update_exam_content(const Exam& old_exam,const Exam& new_exam){
    std::ostringstream old_os;
    old_os<<old_exam.get_date()<<" | "<<old_exam.get_title();
    auto items = ui->exams_list->findItems(QString::fromStdString(old_os.str()),Qt::MatchExactly);
    std::ostringstream new_os;
    new_os<<new_exam.get_date()<<" | "<<new_exam.get_title();
    items.back()->setText(QString::fromStdString(new_os.str()));
    if(old_exam.get_importance() && new_exam.get_importance()){
        auto items = ui->important_list->findItems("Exam "+QString::fromStdString(old_os.str()),
                                                   Qt::MatchExactly);
        items.back()->setText("Exam "+QString::fromStdString(new_os.str()));
    }
    else if(!old_exam.get_importance() && new_exam.get_importance()){
        ui->important_list->addItem("Exam "+QString::fromStdString(new_os.str()));
    }
    else if(old_exam.get_importance() && !new_exam.get_importance()){
        auto items = ui->important_list->findItems("Exam "+QString::fromStdString(old_os.str()),
                                                   Qt::MatchExactly);
        delete items.back();
    }
}

void MainWindow::on_important_list_itemClicked(QListWidgetItem *item)
{
    std::istringstream is(item->text().toStdString());
    Date date;
    std::string type;
    is>>type;
    is>>date;
    is.ignore();
    is.ignore();
    is.ignore();
    std::string str;
    is>>str;
    if(type=="Home"){
        update_homework(str,date,item);
    }
    else{
        update_exam(str,date,item);
    }
}

void MainWindow::update_homework(const std::string& name, const Date& date,QListWidgetItem *item){
    std::set<std::string>all_teachers;
    std::set<std::string>all_subjects;
    for(const auto& i:teachers){
        all_teachers.insert(i.get_name()+" "+i.get_surname());
    }
    for(const auto& i:subjects){
        all_subjects.insert(i.get_name());
    }
    auto it=this->homeworks.find(Task(name,date));
    Task old_homework=*it;
    EditTask homework_window(false,false,all_teachers,all_subjects,this,old_homework.get_title(),
                            old_homework.get_teacher(),old_homework.get_subject(),
                            old_homework.get_date(), old_homework.get_note(),old_homework.get_importance());
    homework_window.setModal(true);
    homework_window.setFixedWidth(WIDTH/3);
    homework_window.setFixedHeight(HEIGHT/3);
    homework_window.exec();
    if(homework_window.deleted()){
        this->homeworks.erase(old_homework);
        delete item;
        return;
    }
    if(!homework_window.is_cancel()){
        Task homework = homework_window.get_task();
        if(homework.get_title()==""){
            homework.set_title("default_title");
        }
        auto it2 = this->homeworks.find(homework);
        if(it2==end(this->homeworks) || it2==it){
            this->homeworks.erase(old_homework);
            update_homework_content(old_homework,homework);
            this->homeworks.insert(std::move(homework));
        }
        else{
            QMessageBox::critical(this,"Attention!","Homework with those title and date already exists");
        }
    }
}

void MainWindow::update_exam(const std::string& name, const Date& date,QListWidgetItem *item){
    std::set<std::string>all_teachers;
    std::set<std::string>all_subjects;
    for(const auto& i:teachers){
        all_teachers.insert(i.get_name()+" "+i.get_surname());
    }
    for(const auto& i:subjects){
        all_subjects.insert(i.get_name());
    }
    auto it=this->exams.find(Exam(name,date));
    Exam old_exam=*it;
    EditTask exam_window(true,false,all_teachers,all_subjects,this,old_exam.get_title(),
                            old_exam.get_teacher(),old_exam.get_subject(),old_exam.get_date(),
                            old_exam.get_note(),old_exam.get_importance(),old_exam.get_type());
    exam_window.setModal(true);
    exam_window.setFixedWidth(WIDTH/3);
    exam_window.setFixedHeight(HEIGHT/3);
    exam_window.exec();
    if(exam_window.deleted()){
        this->exams.erase(old_exam);
        delete item;
        return;
    }
    if(!exam_window.is_cancel()){
        Exam exam(exam_window.get_task());
        exam.set_type(exam_window.get_type());
        if(exam.get_title()==""){
            exam.set_title("default_title");
        }
        auto it2 = this->exams.find(exam);
        if(it2==end(this->exams) || it2==it){
            this->exams.erase(old_exam);
            update_exam_content(old_exam,exam);
            this->exams.insert(std::move(exam));
        }
        else{
            QMessageBox::critical(this,"Attention!","Exam with those title and date already exists");
        }
    }
}
