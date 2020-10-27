#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QRect>
#include <QRectF>
#include <QDesktopWidget>
#include <QtCore>
#include <QtGui>
#include <QListWidget>
#include <QListWidgetItem>
#include <QString>
#include <QIcon>
#include <QPixmap>
#include <QMessageBox>
#include <QDate>
#include <QTime>
#include <QVector>

#include <set>
#include <utility>
#include <string>
#include <sstream>
#include <map>
#include <exception>

#include "subject.h"
#include "teacher.h"
#include "exam.h"
#include "date.h"
#include "teacheredit.h"
#include "subjectedit.h"
#include "task.h"
#include "edittask.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



private slots:

    void on_add_teacher_button_clicked();

    void on_teachers_list_itemClicked(QListWidgetItem *item);

    void on_add_subject_button_clicked();

    void on_subjects_list_itemClicked(QListWidgetItem *item);

    void on_add_task_button_clicked();

    void on_add_exam_button_clicked();

    void on_add_homework_button_clicked();

    void on_add_homework_button_page_clicked();

    void on_add_exam_button_page_clicked();

    void on_exams_list_itemClicked(QListWidgetItem *item);

    void on_homeworks_list_itemClicked(QListWidgetItem *item);

    void on_important_list_itemClicked(QListWidgetItem *item);

    void on_today_list_itemClicked(QListWidgetItem *item);

    void on_tomorrow_list_itemClicked(QListWidgetItem *item);

private:
    void update_homework(const std::string& name, const Date& date,QListWidgetItem *item);
    void update_exam(const std::string& name, const Date& date,QListWidgetItem *item);

    void add_homework_content(const Task& new_homework);
    void update_homework_content(const Task& old_homework,const Task& new_homework);
    void delete_homework_content(const Task& homework);

    void update_current_date();

    void add_exam_content(const Exam& new_exam);
    void update_exam_content(const Exam& old_exam,const Exam& new_exam);
    void delete_exam_content(const Exam& exam);

    void update_exams_plot();
    void update_homeworks_plot();

    void add_task(bool exam);

    void add_homework_overview(const Task& task,bool today);
    void add_exam_overview(const Exam& exam,bool today);

    void delete_homework_overview(const Task& task,bool today);
    void delete_exam_overview(const Exam& exam,bool today);

    Ui::MainWindow *ui;

    int HEIGHT;
    int WIDTH;

    std::set<Teacher>teachers;
    std::set<Subject>subjects;
    std::set<Task>homeworks;
    std::set<Exam>exams;

    std::map<Date,int>dates_homeworks;
    std::map<Date,int>dates_exams;

    Date current_date;
    Day day_of_week;
};
#endif // MAINWINDOW_H
