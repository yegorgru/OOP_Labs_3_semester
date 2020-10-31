#include "settings.h"

Settings::Settings(){
    this->plot_days = 7;
    this->auto_clearing = false;
    this->auto_clearing_days = 100;
    this->work_week = WorkWeek::friday;
    this->subjects_number = 5;
    this->saved_in_last = 10;
}

Settings::Settings(int plot_days,bool auto_clearing,int auto_clearing_days,WorkWeek work_week,
                   int subjects_number, int saved_in_last)
{
    this->plot_days = plot_days;
    this->auto_clearing = auto_clearing;
    this->auto_clearing_days = auto_clearing_days;
    this->work_week = work_week;
    this->subjects_number = subjects_number;
    this->saved_in_last = saved_in_last;
}

int Settings::get_plot_days(){
    return this->plot_days;
}
void Settings::set_plot_days(int plot_days){
    this->plot_days = plot_days;
}

bool Settings::is_auto_clearing(){
    return this->auto_clearing;
}
void Settings::set_auto_clearing(bool auto_clearing){
    this->auto_clearing = auto_clearing;
}

int Settings::get_auto_clearing_days(){
    return this->auto_clearing_days;
}
void Settings::set_auto_clearing_days(int days){
    this->auto_clearing_days = days;
}

WorkWeek Settings::get_work_week(){
    return this->work_week;
}
void Settings::set_work_week(WorkWeek work_week){
    this->work_week = work_week;
}

int Settings::get_subjects_number(){
    return this->subjects_number;
}
void Settings::set_subjects_number(int subjects_number){
    this->subjects_number = subjects_number;
}

int Settings::get_saved_in_last(){
    return this->saved_in_last;
}
void Settings::set_saved_in_last(int number){
    this->saved_in_last = number;
}
