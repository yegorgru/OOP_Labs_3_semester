#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    HEIGHT=this->size().height();
    WIDTH=this->size().width();

    settings.set_plot_days(7);
    settings.set_work_week(WorkWeek::friday);
    settings.set_auto_clearing(false);
    settings.set_subjects_number(5);
    settings.set_auto_clearing_days(100);

    ui->days_plot_spin_box->setValue(settings.get_plot_days());

    ui->auto_clearing_check->setChecked(settings.is_auto_clearing());

    ui->auto_clearing_spin_box->setValue(settings.get_auto_clearing_days());

    ui->work_week_combo_box->addItem("5-day");
    ui->work_week_combo_box->addItem("6-day");
    ui->work_week_combo_box->setCurrentIndex(settings.get_work_week() == WorkWeek::friday ? 0 : 1);

    ui->lessons_per_day_combo_box->addItem("0");
    ui->lessons_per_day_combo_box->addItem("1");
    ui->lessons_per_day_combo_box->addItem("2");
    ui->lessons_per_day_combo_box->addItem("3");
    ui->lessons_per_day_combo_box->addItem("4");
    ui->lessons_per_day_combo_box->addItem("5");
    ui->lessons_per_day_combo_box->addItem("6");
    ui->lessons_per_day_combo_box->addItem("7");
    ui->lessons_per_day_combo_box->addItem("8");
    ui->lessons_per_day_combo_box->setCurrentIndex(settings.get_subjects_number());

    apply_settings();

    update_current_date();
    calendar_date = Date(0,0,0,current_date.get_day(),current_date.get_month(),
                         current_date.get_year());
    update_calendar_list(calendar_date);

    ui->date_edit_calendar->setDate(QDate::currentDate());

    for(const auto& teacher:teachers){
        ui->teachers_list->addItem(QString::fromStdString(teacher.get_name()+" "+teacher.get_surname()));
    }
    for(const auto& subject:subjects){
        ui->subjects_list->addItem(QString::fromStdString(subject.get_name()));
    }
    Date check_today = Date(0,0,0,current_date.get_day(),current_date.get_month(),
                      current_date.get_year());
    Date check_tomorrow = check_today;
    check_tomorrow.promote(1,MeasureTime::days);
    for(const auto& task:homeworks){
        std::ostringstream os;
        os<<task.get_date()<<" | "<<task.get_title();
        ui->homeworks_list->addItem(QString::fromStdString(os.str()));
        dates_homeworks[task.get_date()]++;
        if(task.get_date() == check_today){
            add_homework_overview(task,true);
        }
        else if(task.get_date() == check_tomorrow){
            add_homework_overview(task,false);
        }
    }

    for(const auto& exam:exams){
        std::ostringstream os;
        os<<exam.get_date()<<" | "<<exam.get_title();
        ui->exams_list->addItem(QString::fromStdString(os.str()));
        dates_exams[exam.get_date()]++;
        if(exam.get_date() == check_today){
            add_exam_overview(exam,true);
        }
        else if(exam.get_date() == check_tomorrow){
            add_exam_overview(exam,false);
        }
    }

    for(const auto& exam:exams_archive){
        std::ostringstream os;
        os<<"Exam | "<<exam.get_date()<<" | "<<exam.get_title();
        ui->archive_list->addItem(QString::fromStdString(os.str()));
    }

    for(const auto& homework:homeworks_archive){
        std::ostringstream os;
        os<<"Home | "<<homework.get_date()<<" | "<<homework.get_title();
        ui->archive_list->addItem(QString::fromStdString(os.str()));
    }

    set_subjects_timetable();

    QPixmap pixmap(":/images/images/plus_image.jpg");
    QIcon ButtonIcon(pixmap);
    ui->add_teacher_button->setIcon(ButtonIcon);
    ui->add_teacher_button->setIconSize(QSize(HEIGHT/10, HEIGHT/10));
    ui->add_teacher_button->setFixedSize(QSize(HEIGHT/10, HEIGHT/10));

    ui->add_subject_button->setIcon(ButtonIcon);
    ui->add_subject_button->setIconSize(QSize(HEIGHT/10, HEIGHT/10));
    ui->add_subject_button->setFixedSize(QSize(HEIGHT/10, HEIGHT/10));

    ui->add_homework_button_page->setIcon(ButtonIcon);
    ui->add_homework_button_page->setIconSize(QSize(HEIGHT/10, HEIGHT/10));
    ui->add_homework_button_page->setFixedSize(QSize(HEIGHT/10, HEIGHT/10));

    ui->add_exam_button_page->setIcon(ButtonIcon);
    ui->add_exam_button_page->setIconSize(QSize(HEIGHT/10, HEIGHT/10));
    ui->add_exam_button_page->setFixedSize(QSize(HEIGHT/10, HEIGHT/10));


    ui->add_exam_button->setVisible(false);
    ui->add_homework_button->setVisible(false);

    day_of_week = current_date.get_day_of_week();

    create_plots();

    ui->tabWidget->tabBar()->setStyleSheet("font-weight: bold; font-size: "+QString::number(HEIGHT/40)+"px;");
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
        if(subject.get_name()=="None"){
            subject.set_name("None:)");
        }
        if(this->subjects.find(subject)==end(this->subjects)){
            ui->subjects_list->addItem(QString::fromStdString(subject.get_name()));
            add_subject_timetable(subject);
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
        delete_subject_timetable(old_subject);
        delete item;
        return;
    }
    if(!subject_window.is_cancel()){
        Subject subject = subject_window.get_subject();
        if(subject.get_name()==""){
            subject.set_name("default_name");
        }
        if(subject.get_name()=="None"){
            subject.set_name("None:)");
        }
        auto it2 = this->subjects.find(subject);
        if(it2==end(this->subjects) || it2==it){
            this->subjects.erase(old_subject);
            delete_subject_timetable(old_subject);
            add_subject_timetable(subject);
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
    add_task(true);
    ui->add_exam_button->setVisible(false);
    ui->add_homework_button->setVisible(false);
    ui->add_task_button->setVisible(true);
}

void MainWindow::on_add_homework_button_clicked()
{
    add_task(false);
    ui->add_exam_button->setVisible(false);
    ui->add_homework_button->setVisible(false);
    ui->add_task_button->setVisible(true);
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
                dates_exams[exam.get_date()]++;
                this->exams.insert(exam);
                add_exam_content(exam);
            }
            else{
                QMessageBox::critical(this,"Attention!","Exam with those name and date already exists");
            }
        }
        else{
            Task homework = task_window.get_task();

            if(homework.get_title()==""){
                homework.set_title("default_title");
            }
            if(this->homeworks.find(homework)==end(this->homeworks)){
                dates_homeworks[homework.get_date()]++;
                this->homeworks.insert(homework);
                add_homework_content(homework);
            }
            else{
                QMessageBox::critical(this,"Attention!","Homework with those name and date already exists");
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
    update_exam(str,date);
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
    update_homework(str,date);
}

void MainWindow::add_homework_content(const Task& new_homework){
    std::ostringstream new_os;
    new_os<<new_homework.get_date()<<" | "<<new_homework.get_title();
    ui->homeworks_list->addItem(QString::fromStdString(new_os.str()));
    if(new_homework.get_importance()){
        ui->important_list->addItem("Home | "+QString::fromStdString(new_os.str()));
    }
    Date check_today = Date(0,0,0,current_date.get_day(),current_date.get_month(),
                      current_date.get_year());
    Date check_tomorrow = check_today;
    check_tomorrow.promote(1,MeasureTime::days);
    if(new_homework.get_date() == check_today){
        add_homework_overview(new_homework,true);
    }
    else if(new_homework.get_date() == check_tomorrow){
        add_homework_overview(new_homework,false);
    }
    if(new_homework.get_date() == calendar_date){
        ui->calendar_list->addItem("Home | "+QString::fromStdString(new_homework.get_title()));
    }
    update_homeworks_plot();
}

void MainWindow::update_homework_content(const Task& old_homework,const Task& new_homework){
    std::ostringstream old_os;
    old_os<<old_homework.get_date()<<" | "<<old_homework.get_title();
    auto items = ui->homeworks_list->findItems(QString::fromStdString(old_os.str()),Qt::MatchExactly);
    std::ostringstream new_os;
    new_os<<new_homework.get_date()<<" | "<<new_homework.get_title();
    items.back()->setText(QString::fromStdString(new_os.str()));
    /*if(old_homework.get_importance() && new_homework.get_importance()){
        auto items = ui->important_list->findItems("Home | "+QString::fromStdString(old_os.str()),
                                                   Qt::MatchExactly);
        items.back()->setText("Home | " + QString::fromStdString(new_os.str()));
    }
    else if(!old_homework.get_importance() && new_homework.get_importance()){
        ui->important_list->addItem("Home | "+QString::fromStdString(new_os.str()));
    }*/
    if(old_homework.get_importance()){
        auto items = ui->important_list->findItems("Home | "+QString::fromStdString(old_os.str()),
                                                   Qt::MatchExactly);
        delete items.back();
    }
    if(new_homework.get_importance()){
        ui->important_list->addItem("Home | "+QString::fromStdString(new_os.str()));
    }
    Date check_today = Date(0,0,0,current_date.get_day(),current_date.get_month(),
                      current_date.get_year());
    Date check_tomorrow = check_today;
    check_tomorrow.promote(1,MeasureTime::days);
    if(old_homework.get_date() == check_today){
        delete_homework_overview(old_homework,true);
    }
    else if(old_homework.get_date() == check_tomorrow){
        delete_homework_overview(old_homework,false);
    }
    if(new_homework.get_date() == check_today){
        add_homework_overview(new_homework,true);
    }
    else if(new_homework.get_date() == check_tomorrow){
        add_homework_overview(new_homework,false);
    }
    if(old_homework.get_date() == calendar_date){
        auto items = ui->calendar_list->findItems("Home | "+
                    QString::fromStdString(old_homework.get_title()), Qt::MatchExactly);
        delete items.back();
    }
    if(new_homework.get_date() == calendar_date){
        ui->calendar_list->addItem("Home | "+QString::fromStdString(new_homework.get_title()));
    }
    update_homeworks_plot();
}

void MainWindow::delete_homework_content(const Task& homework){
    std::ostringstream os;
    os<<homework.get_date()<<" | "<<homework.get_title();
    auto items = ui->homeworks_list->findItems(QString::fromStdString(os.str()),Qt::MatchExactly);
    delete items.back();
    if(homework.get_importance()){
        auto items = ui->important_list->findItems("Home | "+QString::fromStdString(os.str()),
                                                   Qt::MatchExactly);
        delete items.back();
    }
    Date check_today = Date(0,0,0,current_date.get_day(),current_date.get_month(),
                      current_date.get_year());
    Date check_tomorrow = check_today;
    check_tomorrow.promote(1,MeasureTime::days);
    if(homework.get_date() == check_today){
        delete_homework_overview(homework,true);
    }
    else if(homework.get_date() == check_tomorrow){
        delete_homework_overview(homework,false);
    }
    update_homeworks_plot();

    if(homework.get_date() == calendar_date){
        auto items = ui->calendar_list->findItems("Home | "+
                    QString::fromStdString(homework.get_title()), Qt::MatchExactly);
        delete items.back();
    }

    std::ostringstream archive_os;
    archive_os<<"Home | "<<homework.get_date()<<" | "<<homework.get_title();
    ui->archive_list->addItem(QString::fromStdString(archive_os.str()));
}

void MainWindow::add_exam_content(const Exam& new_exam){
    std::ostringstream new_os;
    new_os<<new_exam.get_date()<<" | "<<new_exam.get_title();
    ui->exams_list->addItem(QString::fromStdString(new_os.str()));
    if(new_exam.get_importance()){
        ui->important_list->addItem("Exam | "+QString::fromStdString(new_os.str()));
    }

    Date check_today = Date(0,0,0,current_date.get_day(),current_date.get_month(),
                      current_date.get_year());
    Date check_tomorrow = check_today;
    check_tomorrow.promote(1,MeasureTime::days);
    if(new_exam.get_date() == check_today){
        add_exam_overview(new_exam,true);
    }
    else if(new_exam.get_date() == check_tomorrow){
        add_exam_overview(new_exam,false);
    }
    if(new_exam.get_date() == calendar_date){
        ui->calendar_list->addItem("Exam | " + QString::fromStdString(new_exam.get_title()));
    }
    update_exams_plot();
}

void MainWindow::update_exam_content(const Exam& old_exam,const Exam& new_exam){
    std::ostringstream old_os;
    old_os<<old_exam.get_date()<<" | "<<old_exam.get_title();
    auto items = ui->exams_list->findItems(QString::fromStdString(old_os.str()),Qt::MatchExactly);
    std::ostringstream new_os;
    new_os<<new_exam.get_date()<<" | "<<new_exam.get_title();
    items.back()->setText(QString::fromStdString(new_os.str()));
    /*if(old_exam.get_importance() && new_exam.get_importance()){
        auto items = ui->important_list->findItems("Exam | "+QString::fromStdString(old_os.str()),
                                                   Qt::MatchExactly);
        items.back()->setText("Exam | "+QString::fromStdString(new_os.str()));
    }
    else if(!old_exam.get_importance() && new_exam.get_importance()){
        ui->important_list->addItem("Exam | "+QString::fromStdString(new_os.str()));
    }*/
    if(old_exam.get_importance()){
        auto items = ui->important_list->findItems("Exam | "+QString::fromStdString(old_os.str()),
                                                   Qt::MatchExactly);
        delete items.back();
    }
    if(new_exam.get_importance()){
        ui->important_list->addItem("Exam | "+QString::fromStdString(new_os.str()));
    }

    Date check_today = Date(0,0,0,current_date.get_day(),current_date.get_month(),
                      current_date.get_year());
    Date check_tomorrow = check_today;
    check_tomorrow.promote(1,MeasureTime::days);
    if(old_exam.get_date() == check_today){
        delete_exam_overview(old_exam,true);
    }
    else if(old_exam.get_date() == check_tomorrow){
        delete_exam_overview(old_exam,false);
    }
    if(new_exam.get_date() == check_today){
        add_exam_overview(new_exam,true);
    }
    else if(new_exam.get_date() == check_tomorrow){
        add_exam_overview(new_exam,false);
    }
    if(old_exam.get_date() == calendar_date){
        auto items = ui->calendar_list->findItems("Exam | "+
                    QString::fromStdString(old_exam.get_title()), Qt::MatchExactly);
        delete items.back();
    }
    if(new_exam.get_date() == calendar_date){
        ui->calendar_list->addItem("Exam | "+QString::fromStdString(new_exam.get_title()));
    }
    update_exams_plot();
}

void MainWindow::delete_exam_content(const Exam& exam){
    std::ostringstream os;
    os<<exam.get_date()<<" | "<<exam.get_title();
    auto items = ui->exams_list->findItems(QString::fromStdString(os.str()),Qt::MatchExactly);
    delete items.back();
    if(exam.get_importance()){
        auto items = ui->important_list->findItems("Exam | "+QString::fromStdString(os.str()),
                                                   Qt::MatchExactly);
        delete items.back();
    }

    Date check_today = Date(0,0,0,current_date.get_day(),current_date.get_month(),
                      current_date.get_year());
    Date check_tomorrow = check_today;
    check_tomorrow.promote(1,MeasureTime::days);
    if(exam.get_date() == check_today){
        delete_exam_overview(exam,true);
    }
    else if(exam.get_date() == check_tomorrow){
        delete_exam_overview(exam,false);
    }

    update_exams_plot();

    if(exam.get_date() == calendar_date){
        auto items = ui->calendar_list->findItems("Exam | "+
                    QString::fromStdString(exam.get_title()), Qt::MatchExactly);
        delete items.back();
    }

    std::ostringstream archive_os;
    archive_os<<"Exam | "<<exam.get_date()<<" | "<<exam.get_title();
    ui->archive_list->addItem(QString::fromStdString(archive_os.str()));
}

void MainWindow::on_important_list_itemClicked(QListWidgetItem *item)
{
    std::istringstream is(item->text().toStdString());
    Date date;
    std::string type;
    is>>type;
    is.ignore();
    is.ignore();
    is.ignore();
    is>>date;
    is.ignore();
    is.ignore();
    is.ignore();
    std::string str;
    is>>str;
    if(type=="Home"){
        update_homework(str,date);
    }
    else{
        update_exam(str,date);
    }
}

void MainWindow::update_homework(const std::string& name, const Date& date){
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
        dates_homeworks[old_homework.get_date()]--;
        this->homeworks.erase(old_homework);
        this->homeworks_archive.insert(old_homework);
        delete_homework_content(old_homework);
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
            this->homeworks.insert(homework);
            dates_homeworks[old_homework.get_date()]--;
            dates_homeworks[homework.get_date()]++;
            update_homework_content(old_homework,homework);
        }
        else{
            QMessageBox::critical(this,"Attention!","Homework with those title and date already exists");
        }
    }
}

void MainWindow::update_exam(const std::string& name, const Date& date){
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
        dates_exams[old_exam.get_date()]--;
        this->exams.erase(old_exam);
        this->exams_archive.insert(old_exam);
        delete_exam_content(old_exam);
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
            this->exams.insert(exam);
            dates_exams[old_exam.get_date()]--;
            dates_exams[exam.get_date()]++;
            update_exam_content(old_exam,exam);
        }
        else{
            QMessageBox::critical(this,"Attention!","Exam with those title and date already exists");
        }
    }
}

void MainWindow::update_exams_plot(){
    ui->exams_plot->clearGraphs();

    // seconds of current time, we'll use it as starting point in time for data:
    QDateTime cur = QDateTime::currentDateTime();
    cur.setTime(QTime(0,0,0));
    double now = cur.toTime_t();
    update_current_date();


    ui->exams_plot->addGraph();
    QColor color(255,0, 0, 127);
    ui->exams_plot->graph()->setLineStyle(QCPGraph::lsLine);
    ui->exams_plot->graph()->setPen(QPen(color.lighter(200)));
    ui->exams_plot->graph()->setBrush(QBrush(color));
    // generate random walk data:
    QVector<QCPGraphData> timeData(settings.get_plot_days());
    Date copy_date = current_date;
    copy_date.set_seconds(0);
    copy_date.set_minutes(0);
    copy_date.set_hours(0);
    for (int i=0; i<this->settings.get_plot_days(); ++i)
    {
      timeData[i].key = now + 24*3600*i;
      try{
          timeData[i].value = dates_exams.at(copy_date);
      }
      catch(const std::exception&){
          timeData[i].value = 0;
      }
      copy_date.promote(1,MeasureTime::days);
    }
    ui->exams_plot->graph()->data()->set(timeData);

    ui->exams_plot->xAxis->setRange(now, now+24*3600*(this->settings.get_plot_days()-1));

    ui->exams_plot->replot();
}

void MainWindow::update_homeworks_plot(){
    ui->homeworks_plot->clearGraphs();

    // seconds of current time, we'll use it as starting point in time for data:
    QDateTime cur = QDateTime::currentDateTime();
    cur.setTime(QTime(0,0,0));
    double now = cur.toTime_t();
    update_current_date();
    ui->homeworks_plot->addGraph();
    QColor color(20,112, 150, 150);
    ui->homeworks_plot->graph()->setLineStyle(QCPGraph::lsLine);
    ui->homeworks_plot->graph()->setPen(QPen(color.lighter(200)));
    ui->homeworks_plot->graph()->setBrush(QBrush(color));
    // generate random walk data:
    QVector<QCPGraphData> timeData(settings.get_plot_days());
    Date copy_date = current_date;
    copy_date.set_seconds(0);
    copy_date.set_minutes(0);
    copy_date.set_hours(0);
    for (int i=0; i<this->settings.get_plot_days(); ++i)
    {
      timeData[i].key = now + 24*3600*i;
      try{
          timeData[i].value = dates_homeworks.at(copy_date);
      }
      catch(const std::exception&){
          timeData[i].value = 0;
      }
      copy_date.promote(1,MeasureTime::days);
    }
    ui->homeworks_plot->graph()->data()->set(timeData);

    ui->homeworks_plot->xAxis->setRange(now, now+24*3600*(this->settings.get_plot_days()-1));

    ui->homeworks_plot->replot();
}

void MainWindow::update_current_date(){
    QDate date = QDate::currentDate();
    QTime time = QTime::currentTime();
    current_date = Date(time.second(),time.minute(),time.hour(),
                        date.day(),date.month(),date.year());
}

void MainWindow::add_homework_overview(const Task& task,bool today){
    if(today){
        if(task.get_importance()){
            ui->today_list->addItem("!!! Home | " + QString::fromStdString(task.get_title()));
        }
        else{
            ui->today_list->addItem("    Home | " + QString::fromStdString(task.get_title()));
        }
    }
    else{
        if(task.get_importance()){
            ui->tomorrow_list->addItem("!!! Home | " + QString::fromStdString(task.get_title()));
        }
        else{
            ui->tomorrow_list->addItem("    Home | " + QString::fromStdString(task.get_title()));
        }
    }
}

void MainWindow::add_exam_overview(const Exam& exam,bool today){
    if(today){
        if(exam.get_importance()){
            ui->today_list->addItem("!!! Exam | " + QString::fromStdString(exam.get_title()));
        }
        else{
            ui->today_list->addItem("    Exam | " + QString::fromStdString(exam.get_title()));
        }
    }
    else{
        if(exam.get_importance()){
            ui->tomorrow_list->addItem("!!! Exam | " + QString::fromStdString(exam.get_title()));
        }
        else{
            ui->tomorrow_list->addItem("    Exam | " + QString::fromStdString(exam.get_title()));
        }
    }
}

void MainWindow::delete_homework_overview(const Task& task,bool today){
    if(today){
        std::string name;
        if(task.get_importance()){
            name="!!! Home | ";
        }
        else{
            name="    Home | ";
        }
        name+=task.get_title();
        auto items = ui->today_list->findItems(QString::fromStdString(name),Qt::MatchExactly);
        delete items.back();
    }
    else{
        std::string name;
        if(task.get_importance()){
            name="!!! Home | ";
        }
        else{
            name="    Home | ";
        }
        name+=task.get_title();
        auto items = ui->tomorrow_list->findItems(QString::fromStdString(name),Qt::MatchExactly);
        delete items.back();
    }
}

void MainWindow::delete_exam_overview(const Exam& exam,bool today){
    if(today){
        std::string name;
        if(exam.get_importance()){
            name="!!! Exam | ";
        }
        else{
            name="    Exam | ";
        }
        name+=exam.get_title();
        auto items = ui->today_list->findItems(QString::fromStdString(name),Qt::MatchExactly);
        delete items.back();
    }
    else{
        std::string name;
        if(exam.get_importance()){
            name="!!! Exam | ";
        }
        else{
            name="    Exam | ";
        }
        name+=exam.get_title();
        auto items = ui->tomorrow_list->findItems(QString::fromStdString(name),Qt::MatchExactly);
        delete items.back();
    }
}

void MainWindow::on_today_list_itemClicked(QListWidgetItem *item)
{
    std::istringstream is(item->text().toStdString());
    is.ignore();
    is.ignore();
    is.ignore();
    is.ignore();
    Date date = current_date;
    date.set_seconds(0);
    date.set_minutes(0);
    date.set_hours(0);
    std::string type;
    is>>type;
    is.ignore();
    is.ignore();
    is.ignore();
    std::string name;
    is>>name;
    if(type=="Home"){
         update_homework(name,date);
    }
    else{
        update_exam(name,date);
    }
}

void MainWindow::on_tomorrow_list_itemClicked(QListWidgetItem *item)
{
    std::istringstream is(item->text().toStdString());
    is.ignore();
    is.ignore();
    is.ignore();
    is.ignore();
    Date date = current_date;
    date.promote(1,MeasureTime::days);
    date.set_seconds(0);
    date.set_minutes(0);
    date.set_hours(0);
    std::string type;
    is>>type;
    is.ignore();
    is.ignore();
    is.ignore();
    std::string name;
    is>>name;
    if(type=="Home"){
         update_homework(name,date);
    }
    else{
        update_exam(name,date);
    }
}

void MainWindow::on_archive_list_itemClicked(QListWidgetItem *item)
{
    QMessageBox msgBox;
    msgBox.setText("Restore task?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    auto reply = msgBox.exec();
    if(reply == QMessageBox::Yes){
        std::istringstream is(item->text().toStdString());
        Date date;
        std::string type;
        is>>type;
        is.ignore();
        is.ignore();
        is.ignore();
        is>>date;
        is.ignore();
        is.ignore();
        is.ignore();
        std::string str;
        is>>str;
        if(type=="Home"){
            auto it = homeworks_archive.find(Task(str,date));
            Task restored = *it;
            if(this->homeworks.find(restored)==end(this->homeworks)){
                dates_homeworks[restored.get_date()]++;
                homeworks_archive.erase(it);
                homeworks.insert(restored);
                add_homework_content(restored);
                delete_archive_content(item);
            }
            else{
                QMessageBox::critical(this,"Attention!","Homework with that name and date already exists");
            }
        }
        else if(type=="Exam"){
            auto it = exams_archive.find(Exam(str,date));
            Exam restored = *it;
            if(this->exams.find(restored)==end(this->exams)){
                dates_exams[restored.get_date()]++;
                exams_archive.erase(it);
                exams.insert(restored);
                add_exam_content(restored);
                delete_archive_content(item);
            }
            else{
                QMessageBox::critical(this,"Attention!","Exam with that name and date already exists");
            }
        }
    }
}

void MainWindow::delete_archive_content(QListWidgetItem *item){
    delete item;
}

void MainWindow::on_clear_archive_button_clicked()
{
    ui->archive_list->clear();
    this->homeworks_archive.clear();
    this->exams_archive.clear();
}

void MainWindow::update_calendar_list(const Date& date){
    ui->calendar_list->clear();
    auto begin_exams = exams.upper_bound(Exam("",date));
    auto end_exams = exams.upper_bound(Exam(std::string(1000,'z'),date));
    for(auto it = begin_exams;it!=end_exams;it++){
        std::ostringstream os;
        os<<"Exam | "<<it->get_title();
        ui->calendar_list->addItem(QString::fromStdString(os.str()));
    }
    auto begin_homeworks = homeworks.upper_bound(Task("",date));
    auto end_homeworks = homeworks.upper_bound(Task(std::string(1000,'z'),date));
    for(auto it = begin_homeworks;it!=end_homeworks;it++){
        std::ostringstream os;
        os<<"Home | "<<it->get_title();
        ui->calendar_list->addItem(QString::fromStdString(os.str()));
    }
}

void MainWindow::on_find_date_button_clicked()
{
    QDate date = ui->date_edit_calendar->date();
    calendar_date = Date(0,0,0,date.day(),date.month(),date.year());
    update_calendar_list(calendar_date);
}

void MainWindow::on_calendar_list_itemClicked(QListWidgetItem *item)
{
    std::istringstream is(item->text().toStdString());
    Date date;
    std::string type;
    is>>type;
    is.ignore();
    is.ignore();
    is.ignore();
    std::string str;
    is>>str;
    if(type=="Home"){
        update_homework(str,calendar_date);
    }
    else{
        update_exam(str,calendar_date);
    }
}

void MainWindow::set_subjects_timetable(){
    add_subject_timetable(Subject("None"));
    for(const auto& subject:subjects){
        QString name = QString::fromStdString(subject.get_name());
        ui->monday_1->addItem(name);
        ui->monday_2->addItem(name);
        ui->monday_3->addItem(name);
        ui->monday_4->addItem(name);
        ui->monday_5->addItem(name);
        ui->monday_6->addItem(name);
        ui->monday_7->addItem(name);
        ui->monday_8->addItem(name);

        ui->tuesday_1->addItem(name);
        ui->tuesday_2->addItem(name);
        ui->tuesday_3->addItem(name);
        ui->tuesday_4->addItem(name);
        ui->tuesday_5->addItem(name);
        ui->tuesday_6->addItem(name);
        ui->tuesday_7->addItem(name);
        ui->tuesday_8->addItem(name);

        ui->wednesday_1->addItem(name);
        ui->wednesday_2->addItem(name);
        ui->wednesday_3->addItem(name);
        ui->wednesday_4->addItem(name);
        ui->wednesday_5->addItem(name);
        ui->wednesday_6->addItem(name);
        ui->wednesday_7->addItem(name);
        ui->wednesday_8->addItem(name);

        ui->thursday_1->addItem(name);
        ui->thursday_2->addItem(name);
        ui->thursday_3->addItem(name);
        ui->thursday_4->addItem(name);
        ui->thursday_5->addItem(name);
        ui->thursday_6->addItem(name);
        ui->thursday_7->addItem(name);
        ui->thursday_8->addItem(name);

        ui->friday_1->addItem(name);
        ui->friday_2->addItem(name);
        ui->friday_3->addItem(name);
        ui->friday_4->addItem(name);
        ui->friday_5->addItem(name);
        ui->friday_6->addItem(name);
        ui->friday_7->addItem(name);
        ui->friday_8->addItem(name);

        ui->saturday_1->addItem(name);
        ui->saturday_2->addItem(name);
        ui->saturday_3->addItem(name);
        ui->saturday_4->addItem(name);
        ui->saturday_5->addItem(name);
        ui->saturday_6->addItem(name);
        ui->saturday_7->addItem(name);
        ui->saturday_8->addItem(name);
    }
}

void MainWindow::add_subject_timetable(const Subject& subject){
    QString name = QString::fromStdString(subject.get_name());
    ui->monday_1->addItem(name);
    ui->monday_2->addItem(name);
    ui->monday_3->addItem(name);
    ui->monday_4->addItem(name);
    ui->monday_5->addItem(name);
    ui->monday_6->addItem(name);
    ui->monday_7->addItem(name);
    ui->monday_8->addItem(name);

    ui->tuesday_1->addItem(name);
    ui->tuesday_2->addItem(name);
    ui->tuesday_3->addItem(name);
    ui->tuesday_4->addItem(name);
    ui->tuesday_5->addItem(name);
    ui->tuesday_6->addItem(name);
    ui->tuesday_7->addItem(name);
    ui->tuesday_8->addItem(name);

    ui->wednesday_1->addItem(name);
    ui->wednesday_2->addItem(name);
    ui->wednesday_3->addItem(name);
    ui->wednesday_4->addItem(name);
    ui->wednesday_5->addItem(name);
    ui->wednesday_6->addItem(name);
    ui->wednesday_7->addItem(name);
    ui->wednesday_8->addItem(name);

    ui->thursday_1->addItem(name);
    ui->thursday_2->addItem(name);
    ui->thursday_3->addItem(name);
    ui->thursday_4->addItem(name);
    ui->thursday_5->addItem(name);
    ui->thursday_6->addItem(name);
    ui->thursday_7->addItem(name);
    ui->thursday_8->addItem(name);

    ui->friday_1->addItem(name);
    ui->friday_2->addItem(name);
    ui->friday_3->addItem(name);
    ui->friday_4->addItem(name);
    ui->friday_5->addItem(name);
    ui->friday_6->addItem(name);
    ui->friday_7->addItem(name);
    ui->friday_8->addItem(name);

    ui->saturday_1->addItem(name);
    ui->saturday_2->addItem(name);
    ui->saturday_3->addItem(name);
    ui->saturday_4->addItem(name);
    ui->saturday_5->addItem(name);
    ui->saturday_6->addItem(name);
    ui->saturday_7->addItem(name);
    ui->saturday_8->addItem(name);
}

void MainWindow::delete_subject_timetable(const Subject& subject){
    QString name = QString::fromStdString(subject.get_name());
    auto item = ui->monday_1->findText(name);
    if(ui->monday_1->currentIndex() == item){
        ui->monday_1->setCurrentIndex(0);
    }
    ui->monday_1->removeItem(item);
    item = ui->monday_2->findText(name);
    ui->monday_2->removeItem(item);
    item = ui->monday_3->findText(name);
    ui->monday_3->removeItem(item);
    item = ui->monday_4->findText(name);
    ui->monday_4->removeItem(item);
    item = ui->monday_5->findText(name);
    ui->monday_5->removeItem(item);
    item = ui->monday_6->findText(name);
    ui->monday_6->removeItem(item);
    item = ui->monday_7->findText(name);
    ui->monday_7->removeItem(item);
    item = ui->monday_8->findText(name);
    ui->monday_8->removeItem(item);

    item = ui->tuesday_1->findText(name);
    ui->tuesday_1->removeItem(item);
    item = ui->tuesday_2->findText(name);
    ui->tuesday_2->removeItem(item);
    item = ui->tuesday_3->findText(name);
    ui->tuesday_3->removeItem(item);
    item = ui->tuesday_4->findText(name);
    ui->tuesday_4->removeItem(item);
    item = ui->tuesday_5->findText(name);
    ui->tuesday_5->removeItem(item);
    item = ui->tuesday_6->findText(name);
    ui->tuesday_6->removeItem(item);
    item = ui->tuesday_7->findText(name);
    ui->tuesday_7->removeItem(item);
    item = ui->tuesday_8->findText(name);
    ui->tuesday_8->removeItem(item);

    item = ui->wednesday_1->findText(name);
    ui->wednesday_1->removeItem(item);
    item = ui->wednesday_2->findText(name);
    ui->wednesday_2->removeItem(item);
    item = ui->wednesday_3->findText(name);
    ui->wednesday_3->removeItem(item);
    item = ui->wednesday_4->findText(name);
    ui->wednesday_4->removeItem(item);
    item = ui->wednesday_5->findText(name);
    ui->wednesday_5->removeItem(item);
    item = ui->wednesday_6->findText(name);
    ui->wednesday_6->removeItem(item);
    item = ui->wednesday_7->findText(name);
    ui->wednesday_7->removeItem(item);
    item = ui->wednesday_8->findText(name);
    ui->wednesday_8->removeItem(item);

    item = ui->thursday_1->findText(name);
    ui->thursday_1->removeItem(item);
    item = ui->thursday_2->findText(name);
    ui->thursday_2->removeItem(item);
    item = ui->thursday_3->findText(name);
    ui->thursday_3->removeItem(item);
    item = ui->thursday_4->findText(name);
    ui->thursday_4->removeItem(item);
    item = ui->thursday_5->findText(name);
    ui->thursday_5->removeItem(item);
    item = ui->thursday_6->findText(name);
    ui->thursday_6->removeItem(item);
    item = ui->thursday_7->findText(name);
    ui->thursday_7->removeItem(item);
    item = ui->thursday_8->findText(name);
    ui->thursday_8->removeItem(item);

    item = ui->friday_1->findText(name);
    ui->friday_1->removeItem(item);
    item = ui->friday_2->findText(name);
    ui->friday_2->removeItem(item);
    item = ui->friday_3->findText(name);
    ui->friday_3->removeItem(item);
    item = ui->friday_4->findText(name);
    ui->friday_4->removeItem(item);
    item = ui->friday_5->findText(name);
    ui->friday_5->removeItem(item);
    item = ui->friday_6->findText(name);
    ui->friday_6->removeItem(item);
    item = ui->friday_7->findText(name);
    ui->friday_7->removeItem(item);
    item = ui->friday_8->findText(name);
    ui->friday_8->removeItem(item);

    item = ui->saturday_1->findText(name);
    ui->saturday_1->removeItem(item);
    item = ui->saturday_2->findText(name);
    ui->saturday_2->removeItem(item);
    item = ui->saturday_3->findText(name);
    ui->saturday_3->removeItem(item);
    item = ui->saturday_4->findText(name);
    ui->saturday_4->removeItem(item);
    item = ui->saturday_5->findText(name);
    ui->saturday_5->removeItem(item);
    item = ui->saturday_6->findText(name);
    ui->saturday_6->removeItem(item);
    item = ui->saturday_7->findText(name);
    ui->saturday_7->removeItem(item);
    item = ui->saturday_8->findText(name);
    ui->saturday_8->removeItem(item);
}

void MainWindow::apply_settings(){
    update_homeworks_plot();
    update_exams_plot();
    if(settings.get_work_week() == WorkWeek::friday){
        ui->saturday_block->setVisible(false);
    }
    else{
        ui->saturday_block->setVisible(true);
    }
    if(!settings.is_auto_clearing()){
        ui->auto_clearing_spin_box->setVisible(false);
        ui->auto_clearing_label->setVisible(false);
    }
    else{
        auto_clearing_archive();
        ui->auto_clearing_spin_box->setVisible(true);
        ui->auto_clearing_label->setVisible(true);
    }
    update_subjects_visibility_timetable();
}

void MainWindow::create_plots(){
    // set locale to english, so we get english month names:
    ui->exams_plot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));

    // configure bottom axis to show date instead of number:
    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setDateTimeFormat("d. MMMM\nyyyy");
    ui->exams_plot->xAxis->setTicker(dateTicker);
    // configure left axis text labels:
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTick(0, "Free time:)");
    textTicker->addTick(10, "Oops...");
    ui->exams_plot->yAxis->setTicker(textTicker);
    // set a more compact font size for bottom and left axis tick labels:
    ui->exams_plot->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
    ui->exams_plot->yAxis->setTickLabelFont(QFont(QFont().family(), 8));
    // set axis labels:
    ui->exams_plot->xAxis->setLabel("Date");
    // make top and right axes visible but without ticks and labels:
    ui->exams_plot->xAxis2->setVisible(true);
    ui->exams_plot->yAxis2->setVisible(true);
    ui->exams_plot->xAxis2->setTicks(false);
    ui->exams_plot->yAxis2->setTicks(false);
    ui->exams_plot->xAxis2->setTickLabels(false);
    ui->exams_plot->yAxis2->setTickLabels(false);
    // set axis ranges to show all data:

    ui->exams_plot->yAxis->setRange(0, 15);

    update_exams_plot();


    // set locale to english, so we get english month names:
    ui->homeworks_plot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));

    // configure bottom axis to show date instead of number:
    //QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setDateTimeFormat("d. MMMM\nyyyy");
    ui->homeworks_plot->xAxis->setTicker(dateTicker);
    // configure left axis text labels:
    //QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTick(0, "Free time:)");
    textTicker->addTick(10, "Oops...");
    ui->homeworks_plot->yAxis->setTicker(textTicker);
    // set a more compact font size for bottom and left axis tick labels:
    ui->homeworks_plot->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
    ui->homeworks_plot->yAxis->setTickLabelFont(QFont(QFont().family(), 8));
    // set axis labels:
    ui->homeworks_plot->xAxis->setLabel("Date");
    // make top and right axes visible but without ticks and labels:
    ui->homeworks_plot->xAxis2->setVisible(true);
    ui->homeworks_plot->yAxis2->setVisible(true);
    ui->homeworks_plot->xAxis2->setTicks(false);
    ui->homeworks_plot->yAxis2->setTicks(false);
    ui->homeworks_plot->xAxis2->setTickLabels(false);
    ui->homeworks_plot->yAxis2->setTickLabels(false);
    // set axis ranges to show all data:
    ui->homeworks_plot->yAxis->setRange(0, 15);

    update_homeworks_plot();
}

void MainWindow::auto_clearing_archive(){
    if(settings.is_auto_clearing()){
        Date bottom = current_date;
        bottom.decrease(settings.get_auto_clearing_days(),MeasureTime::days);
        auto exams_top_it = exams_archive.upper_bound(Exam("",bottom));
        for(auto it = exams_archive.begin();it!=exams_top_it;it++){
            std::ostringstream os;
            os<<"Exam | "<<it->get_date()<<" | "<<it->get_title();
            auto items = ui->archive_list->findItems(QString::fromStdString(os.str()),Qt::MatchExactly);
            delete items.back();
        }
        exams_archive.erase(exams_archive.begin(),exams_top_it);
        auto homeworks_top_it = homeworks_archive.upper_bound(Task("",bottom));
        for(auto it = homeworks_archive.begin();it!=homeworks_top_it;it++){
            std::ostringstream os;
            os<<"Home | "<<it->get_date()<<" | "<<it->get_title();
            auto items = ui->archive_list->findItems(QString::fromStdString(os.str()),Qt::MatchExactly);
            delete items.back();
        }
        homeworks_archive.erase(homeworks_archive.begin(),homeworks_top_it);
    }
}

void MainWindow::update_subjects_visibility_timetable(){
    for(int i=1;i<=settings.get_subjects_number();i++){
        set_subject_visible_timetable(i,true);
    }
    for(int i=settings.get_subjects_number()+1;i<=8;i++){
        set_subject_visible_timetable(i,false);
    }
}

void MainWindow::set_subject_visible_timetable(int subject,bool visibility){
    if(subject ==1){
        ui->monday_1->setVisible(visibility);
        ui->tuesday_1->setVisible(visibility);
        ui->wednesday_1->setVisible(visibility);
        ui->thursday_1->setVisible(visibility);
        ui->friday_1->setVisible(visibility);
        ui->saturday_1->setVisible(visibility);
    }
    if(subject ==2){
        ui->monday_2->setVisible(visibility);
        ui->tuesday_2->setVisible(visibility);
        ui->wednesday_2->setVisible(visibility);
        ui->thursday_2->setVisible(visibility);
        ui->friday_2->setVisible(visibility);
        ui->saturday_2->setVisible(visibility);
    }
    if(subject ==3){
        ui->monday_3->setVisible(visibility);
        ui->tuesday_3->setVisible(visibility);
        ui->wednesday_3->setVisible(visibility);
        ui->thursday_3->setVisible(visibility);
        ui->friday_3->setVisible(visibility);
        ui->saturday_3->setVisible(visibility);
    }
    if(subject ==4){
        ui->monday_4->setVisible(visibility);
        ui->tuesday_4->setVisible(visibility);
        ui->wednesday_4->setVisible(visibility);
        ui->thursday_4->setVisible(visibility);
        ui->friday_4->setVisible(visibility);
        ui->saturday_4->setVisible(visibility);
    }
    if(subject ==5){
        ui->monday_5->setVisible(visibility);
        ui->tuesday_5->setVisible(visibility);
        ui->wednesday_5->setVisible(visibility);
        ui->thursday_5->setVisible(visibility);
        ui->friday_5->setVisible(visibility);
        ui->saturday_5->setVisible(visibility);
    }
    if(subject ==6){
        ui->monday_6->setVisible(visibility);
        ui->tuesday_6->setVisible(visibility);
        ui->wednesday_6->setVisible(visibility);
        ui->thursday_6->setVisible(visibility);
        ui->friday_6->setVisible(visibility);
        ui->saturday_6->setVisible(visibility);
    }
    if(subject ==7){
        ui->monday_7->setVisible(visibility);
        ui->tuesday_7->setVisible(visibility);
        ui->wednesday_7->setVisible(visibility);
        ui->thursday_7->setVisible(visibility);
        ui->friday_7->setVisible(visibility);
        ui->saturday_7->setVisible(visibility);
    }
    if(subject ==8){
        ui->monday_8->setVisible(visibility);
        ui->tuesday_8->setVisible(visibility);
        ui->wednesday_8->setVisible(visibility);
        ui->thursday_8->setVisible(visibility);
        ui->friday_8->setVisible(visibility);
        ui->saturday_8->setVisible(visibility);
    }
}

void MainWindow::on_apply_settings_button_clicked()
{
    this->settings.set_plot_days(ui->days_plot_spin_box->value());
    this->settings.set_work_week(ui->work_week_combo_box->currentText()=="5-day"?
                                     WorkWeek::friday : WorkWeek::saturday);
    this->settings.set_auto_clearing(ui->auto_clearing_check->isChecked());
    this->settings.set_auto_clearing_days(ui->auto_clearing_spin_box->value());
    this->settings.set_subjects_number(ui->lessons_per_day_combo_box->currentText().toInt());
    apply_settings();
}
