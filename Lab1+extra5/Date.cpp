#include "Date.h"

Date::Date()
	: day(0),month(0),year(0) {}

std::uint16_t Date::get_day() const
{
	return this->day;
}

std::uint16_t Date::get_month() const
{
	return this->month;
}

std::uint16_t Date::get_year() const
{
	return this->year;
}

bool operator==(const Date& lhs, const Date& rhs)
{
	return lhs.get_day() == rhs.get_day() && lhs.get_month() == rhs.get_month() &&
		lhs.get_year() == rhs.get_year();
}

bool operator<(const Date& lhs, const Date& rhs)
{
	return lhs.get_day() < rhs.get_day() ||
		lhs.get_month() < rhs.get_month() && lhs.get_day() == rhs.get_day() ||
		lhs.get_year() < rhs.get_year() &&
			lhs.get_month() == rhs.get_month() && lhs.get_day() == rhs.get_day();
}

std::ostream& operator<<(std::ostream& os, const Date& date)
{
	os << date.get_day() << '.' << date.get_month() << '.' << date.get_year();
	return os;
}

const Date& operator++(Date& date)
{
	date.day++;
	return date;
}
