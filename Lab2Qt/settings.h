#ifndef SETTINGS_H
#define SETTINGS_H

enum class WorkWeek{
    friday,
    saturday
};

class Settings
{
public:
    Settings();

    Settings(int plot_days,bool auto_clearing,int auto_clearing_days,WorkWeek work_week,
             int subjects_number, int saved_in_last);

    int get_plot_days();
    void set_plot_days(int plot_days);

    bool is_auto_clearing();
    void set_auto_clearing(bool auto_clearing);

    int get_auto_clearing_days();
    void set_auto_clearing_days(int days);

    WorkWeek get_work_week();
    void set_work_week(WorkWeek work_week);

    int get_subjects_number();
    void set_subjects_number(int subjects_number);

    int get_saved_in_last();
    void set_saved_in_last(int number);
private:
    int plot_days;
    bool auto_clearing;
    int auto_clearing_days;
    WorkWeek work_week;
    int subjects_number;
    int saved_in_last;
};

#endif // SETTINGS_H
