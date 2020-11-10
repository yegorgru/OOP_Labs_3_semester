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
#include <QPrinter>
#include <QPrintDialog>
#include <QTextEdit>

#include <set>
#include <utility>
#include <string>
#include <sstream>
#include <map>
#include <exception>
#include <list>
#include <fstream>

#include "subject.h"
#include "teacher.h"
#include "exam.h"
#include "date.h"
#include "teacheredit.h"
#include "subjectedit.h"
#include "task.h"
#include "edittask.h"
#include "settings.h"

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

    void on_archive_list_itemClicked(QListWidgetItem *item);

    void on_clear_archive_button_clicked();

    void on_find_date_button_clicked();

    void on_calendar_list_itemClicked(QListWidgetItem *item);

    void on_apply_settings_button_clicked();

    void on_last_list_itemClicked(QListWidgetItem *item);

    void on_clear_printer_data_button_clicked();

    void on_print_button_clicked();

    void on_save_timetable_button_clicked();

private:
    void set_timetable_from_files();
    void read_data_from_files();
    void write_data_in_files();
    void write_exams_in_file();
    void write_homeworks_in_file();
    void write_homeworks_archive_in_file();
    void write_exams_archive_in_file();
    void write_subjects_in_file();
    void write_teachers_in_file();
    void write_timetable_in_file();
    void write_settings_in_file();

    void update_homework(const std::string& name, const Date& date);
    void update_exam(const std::string& name, const Date& date);

    void add_homework_content(const Task& new_homework);
    void update_homework_content(const Task& old_homework,const Task& new_homework);
    void delete_homework_content(const Task& homework);
    void delete_archive_content(QListWidgetItem *item);
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

    void update_calendar_list(const Date& date);

    void set_subjects_timetable();
    void add_subject_timetable(const Subject& subject);
    void delete_subject_timetable(const Subject& subject);
    void update_subjects_visibility_timetable();
    void set_subject_visible_timetable(int subject,bool visibility);

    void add_print_content_homework(const Task& homework);
    void add_print_content_exam(const Exam& exam);

    void apply_settings();
    void create_plots();

    void auto_clearing_archive();

    Ui::MainWindow *ui;

    int HEIGHT;
    int WIDTH;

    std::set<Teacher>teachers;
    std::set<Subject>subjects;
    std::set<Task>homeworks;
    std::set<Exam>exams;

    std::map<Date,int>dates_homeworks;
    std::map<Date,int>dates_exams;

    std::set<Task>homeworks_archive;
    std::set<Exam>exams_archive;

    Settings settings;

    std::list<Task>homeworks_print_data;
    std::list<Exam>exams_print_data;

    Date current_date;
    Date calendar_date;
    Day day_of_week;
};
#endif // MAINWINDOW_H
