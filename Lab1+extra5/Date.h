#pragma once

#include <cstdint>

class Date
{
public:
	Date(std::uint16_t day,std::uint16_t month, std::uint16_t year)
		:day(day),month(month),year(year) {}

	Date();

	std::uint16_t get_day() const;

	std::uint16_t get_month() const;

	std::uint16_t get_year() const;

private:
	std::uint16_t day;
	std::uint16_t month;
	std::uint16_t year;

	friend bool operator==(const Date& lhs, const Date& rhs);
	friend bool operator<(const Date& lhs, const Date& rhs);
	friend const Date& operator++(Date& date);
};


bool operator==(const Date& lhs, const Date& rhs);
bool operator<(const Date& lhs, const Date& rhs);
const Date& operator++(Date& date);
