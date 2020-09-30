#pragma once

#include <cstdint>
#include <iostream>
#include <map>

enum class MeasureTime {
	seconds,
	minutes,
	hours,
	days,
	months,
	years
};

enum class Day{
	Monday,
	Tuesday,
	Wednesday,
	Thursday,
	Friday,
	Saturday,
	Sunday
};

std::ostream& operator << (std::ostream& os, const Day& day);

class Date
{
public:
	Date(uint16_t seconds, uint16_t minutes, uint16_t hours, uint16_t day, uint16_t month, uint16_t year);

	Date(uint16_t seconds, uint16_t minutes, uint16_t hours, Day day, uint16_t number, bool begin, uint16_t month, uint16_t year);

	Date(uint16_t seconds, uint16_t minutes, uint16_t hours, Day day, uint16_t number, bool begin, uint16_t month, uint16_t year, int16_t time_zone);

	Date();

	Date(uint16_t seconds, uint16_t minutes, uint16_t hours, uint16_t day, uint16_t month, uint16_t year, int16_t time_zone);

	Date& operator=(const Date& date);

	std::uint16_t get_day() const;
	void set_day(uint16_t day);

	std::uint16_t get_month() const;
	void set_month(uint16_t month);

	int32_t get_year() const;
	void set_year(int32_t year);

	std::uint16_t get_hours() const;
	void set_hours(uint16_t hours);

	std::uint16_t get_minutes() const;
	void set_minutes(uint16_t minutes);

	std::uint16_t get_seconds() const;
	void set_seconds(uint16_t seconds);

	std::int16_t get_time_zone() const;
	void set_time_zone(int16_t zone);

	bool is_valid() const;
	bool is_leap() const;
	int32_t count_29_february(const Date& another_date) const;

	int32_t get_number_in_year() const;
	int32_t get_reverse_number_in_year() const;

	void promote(uint16_t number, MeasureTime measure);
	void decrease(uint16_t number, MeasureTime measure);

	void save_promote_year(uint16_t number);
	void save_promote_month(uint16_t number);
	void save_promote_day(uint16_t number);
	void save_promote_hour(uint16_t number);
	void save_promote_minute(uint16_t number);
	void save_promote_second(uint16_t number);

	void save_decrease_year(uint16_t number);
	void save_decrease_month(uint16_t number);
	void save_decrease_day(uint16_t number);
	void save_decrease_hour(uint16_t number);
	void save_decrease_minute(uint16_t number);
	void save_decrease_second(uint16_t number);

	int32_t difference(const Date& another, MeasureTime measure) const;
	Day get_day_of_week() const;
	uint16_t get_number_of_week(bool month_or_year);
private:
	void choose_day(Day day, uint16_t number, bool begin);

	const uint16_t days_in_monthes[13] = { 0,31,29,31,30,31,30,31,31,30,31,30,31 };

	uint16_t day;
	uint16_t month;
	int32_t year;
	uint16_t hours;
	uint16_t minutes;
	uint16_t seconds;

	int16_t time_zone;

	friend bool operator==(const Date& lhs, const Date& rhs);
	friend bool operator<(const Date& lhs, const Date& rhs);
};

Day statistics(const Date& bottom, const Date& top, uint16_t number);
Day statistics(int32_t year, uint16_t number);

bool operator==(const Date& lhs, const Date& rhs);
bool operator<(const Date& lhs, const Date& rhs);
std::ostream& operator << (std::ostream& os, const Date& date);
