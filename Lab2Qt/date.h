/**
\file
\brief h file of class Date
definitions of Date methods and operators, enum classes MeasureTime and Day are here
*/

#pragma once

#include <cstdint>
#include <iostream>
#include <map>

/**
\brief class for time measures
*/
enum class MeasureTime {
    seconds,
    minutes,
    hours,
    days,
    months,
    years
};


/**
\brief class for days of week
*/
enum class Day{
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday,
    Sunday
};

/**
Operator << for Days in streams
*/
std::ostream& operator << (std::ostream& os, const Day& day);


/**
\brief class for storing information about date and time
has information about:
- seconds
- minutes
- hours
- day
- month
- year
*/
class Date
{
public:
    /**
    \brief constructor for Date with known fields
    */
    Date(uint16_t seconds, uint16_t minutes, uint16_t hours, uint16_t day, uint16_t month, uint16_t year);

    /**
    \brief alternative constructor of Date.
    if
    \param number number of day with some day in week in month
    \param begin starts count from begin of month if true and from end otherwise
    */
    Date(uint16_t seconds, uint16_t minutes, uint16_t hours, Day day, uint16_t number, bool begin, uint16_t month, uint16_t year);

    /**
    \brief constructor for Date with unknown fields. All fields will be 0
    */
    Date();

    /**
    \brief copy operator
    This date will be equal to date
    \param date copied date
    \return reference to this date
    */
    Date& operator=(const Date& date);

    /**
    \return day of Date
    */
    std::uint16_t get_day() const;

    /**
    sets day of Date
    */
    void set_day(uint16_t day);

    /**
    \return month of Date
    */
    std::uint16_t get_month() const;

    /**
    sets month of Date
    */
    void set_month(uint16_t month);

    /**
    \return year of Date
    */
    int32_t get_year() const;

    /**
    sets year of Date
    */
    void set_year(int32_t year);

    /**
    \return hours of Date
    */
    std::uint16_t get_hours() const;

    /**
    sets hours of Date
    */
    void set_hours(uint16_t hours);

    /**
    \return minutes of Date
    */
    std::uint16_t get_minutes() const;

    /**
    set minutes of Date
    */
    void set_minutes(uint16_t minutes);

    /**
    \return seconds of Date
    */
    std::uint16_t get_seconds() const;

    /**
    sets seconds of Date
    */
    void set_seconds(uint16_t seconds);

    /**
    \brief checks id Date is valid
    Checks if is there out of range and correct 29 february for leap and not leap years
    */
    bool is_valid() const;

    /**
    \brief Checks if year of date is leap
    */
    bool is_leap() const;

    /**
    \brief counts all 29 february between this date anf another_date
    if this date or another_date is 29 february, it will not be counted
    \param another_date end of counting
    */
    int32_t count_29_february(const Date& another_date) const;

    /**
    \throw throw std::invalid_argument("invalid date") if date isn't valid
    \return number of day of this date in the year
    */
    int32_t get_number_in_year() const;

    /**
    \throw throw std::invalid_argument("invalid date") if date isn't valid
    \return number of day of this date in the year if starts counting from end of year
    */
    int32_t get_reverse_number_in_year() const;

    /**
    \brief increases this date by number of measure
    \param number number of measures to increase
    \param measure MeasureTime of increasing
    */
    void promote(uint16_t number, MeasureTime measure);

    /**
    \brief decreases this date by number of measure
    \param number number of measures to decrease
    \param measure MeasureTime of decreasing
    */
    void decrease(uint16_t number, MeasureTime measure);

    /**
    \brief safely increases this date by number of years
    this date will be valid after increasing
    \param number number of years
    */
    void save_promote_year(uint16_t number);
    /**
    \brief safely increases this date by number of months
    this date will be valid after increasing
    \param number number of months
    */
    void save_promote_month(uint16_t number);

    /**
    \brief safely increases this date by number of days
    this date will be valid after increasing
    \param number number of days
    */
    void save_promote_day(uint16_t number);

    /**
    \brief safely increases this date by number of hours
    this date will be valid after increasing
    \param number number of hours
    */
    void save_promote_hour(uint16_t number);

    /**
    \brief safely increases this date by number of minutes
    this date will be valid after increasing
    \param number number of minutes
    */
    void save_promote_minute(uint16_t number);

    /**
    \brief safely increases this date by number of seconds
    this date will be valid after increasing
    \param number number of seconds
    */
    void save_promote_second(uint16_t number);

    /**
    \brief safely decreases this date by number of years
    this date will be valid after decreasing
    \param number number of years
    */
    void save_decrease_year(uint16_t number);

    /**
    \brief safely decreases this date by number of months
    this date will be valid after decreasing
    \param number number of months
    */
    void save_decrease_month(uint16_t number);

    /**
    \brief safely decreases this date by number of days
    this date will be valid after decreasing
    \param number number of days
    */
    void save_decrease_day(uint16_t number);

    /**
    \brief safely decreases this date by number of hours
    this date will be valid after decreasing
    \param number number of hours
    */
    void save_decrease_hour(uint16_t number);

    /**
    \brief safely decreases this date by number of minutes
    this date will be valid after decreasing
    \param number number of minutes
    */
    void save_decrease_minute(uint16_t number);

    /**
    \brief safely decreases this date by number of seconds
    this date will be valid after decreasing
    \param number number of seconds
    */
    void save_decrease_second(uint16_t number);

    /**
    \brief difference between basic date and another in some MeasureTime
    \param another another date in difference
    \param measure type of MeasureTime in difference
    \throw throw std::invalid_argument("invalid date") if this date or another isn't valid
    \return number number of years
    */
    int32_t difference(const Date& another, MeasureTime measure) const;

    /**
    \return Day in week for this date
    */
    Day get_day_of_week() const;

    /**
    \brief return number of week with this date in months or year
    \param month_or_year week in month if true and in year otherwise
    \return 0 when there wasn't Monday between begib of month or year and this date
    */
    uint16_t get_number_of_week(bool month_or_year);
private:

    /**
    \brief chooses number of day for alternative constructor
    */
    void choose_day(Day day, uint16_t number, bool begin);

    /**
    max numbers f days in month of year. days_in_monthes[0] just for easement
    */
    const uint16_t days_in_monthes[13] = { 0,31,29,31,30,31,30,31,31,30,31,30,31 };

    uint16_t day;		///<day in month
    uint16_t month;		///<number of month
    int32_t year;		///<year of date
    uint16_t hours;		///<hours
    uint16_t minutes;	///<minutes
    uint16_t seconds;	///<seconds

    friend bool operator==(const Date& lhs, const Date& rhs);
    friend bool operator<(const Date& lhs, const Date& rhs);
};

/**
\brief function to get some statistics
Counts what day of week is the most common among days with some number in month. Only dates bigger than bottom and lesser than top are taken into account
\param bottom min Date
\param top max Date
\param number number in month to get statistics
\return the most common day
*/
Day statistics(const Date& bottom, const Date& top, uint16_t number);

/**
\brief another version of statistics function
Counts what day of week is the most common in year among days with some number in month.
\param year year to get statistics
\param number number in month to get statistics
\return the most common day
*/
Day statistics(int32_t year, uint16_t number);

/**
Operator == for Dates
*/
bool operator==(const Date& lhs, const Date& rhs);

/**
Operator != for Dates
*/
bool operator!=(const Date& lhs, const Date& rhs);

/**
Operator < for Dates
*/
bool operator<(const Date& lhs, const Date& rhs);

/**
Operator << for Dates in streams
*/
std::ostream& operator << (std::ostream& os, const Date& date);

std::istream& operator>>(std::istream& is, Date& date);
