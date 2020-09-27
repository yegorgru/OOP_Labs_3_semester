#pragma once

#include <cstdint>
#include <iostream>

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
	Thursday,
	Wednesday,
	Tuesday,
	Friday,
	Saturday,
	Sunday
};

class Date
{
public:
	Date(uint16_t seconds,uint16_t minutes, uint16_t hours,uint16_t day,uint16_t month, uint16_t year)
		:seconds(seconds),minutes(minutes),hours(hours),day(day),month(month),year(year) {}

	Date();

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

	int32_t difference(const Date& another,MeasureTime measure) const;
	void promote(uint16_t number, MeasureTime measure);
	void decrease(uint16_t number, MeasureTime measure);

	bool is_valid() const;

	bool is_leap() const;
	int32_t count_leap_years(int32_t another_year) const;
	int32_t get_number_in_year() const;
	int32_t get_reverse_number_in_year() const;

	Day get_day_of_week();
private:
	const uint16_t days_in_monthes[13] = { 0,31,29,31,30,31,30,31,31,30,31,30,31 };

	void promote_month(uint16_t number);
	void promote_day(uint16_t number);
	void promote_hour(uint16_t number);
	void promote_minute(uint16_t number);
	void promote_second(uint16_t number);

	void decrease_month(uint16_t number);
	void decrease_day(uint16_t number);
	void decrease_hour(uint16_t number);
	void decrease_minute(uint16_t number);
	void decrease_second(uint16_t number);

	uint16_t day;
	uint16_t month;
	int32_t year;
	uint16_t hours;
	uint16_t minutes;
	uint16_t seconds;

	friend bool operator==(const Date& lhs, const Date& rhs);
	friend bool operator<(const Date& lhs, const Date& rhs);
};


bool operator==(const Date& lhs, const Date& rhs);
bool operator<(const Date& lhs, const Date& rhs);
std::ostream& operator << (std::ostream& os, const Date& date);
