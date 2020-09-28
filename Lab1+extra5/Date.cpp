#include "Date.h"

Date::Date()
	: seconds(0),minutes(0),hours(0),day(0),month(0),year(0) {}

Date& Date::operator=(const Date& date)
{
	this->year = date.get_year();
	this->month = date.get_month();
	this->day = date.get_day();
	this->hours = date.get_hours();
	this->minutes = date.get_minutes();
	this->seconds = date.get_seconds();
	return *this;
}

std::uint16_t Date::get_day() const
{
	return this->day;
}

void Date::set_day(uint16_t day)
{
	this->day = day;
}

std::uint16_t Date::get_month() const
{
	return this->month;
}

void Date::set_month(uint16_t month)
{
	this->month = month;
}

int32_t Date::get_year() const
{
	return this->year;
}

void Date::set_year(int32_t year)
{
	this->year = year;
}

std::uint16_t Date::get_hours() const
{
	return this->hours;
}

void Date::set_hours(uint16_t hours)
{
	this->hours = hours;
}

std::uint16_t Date::get_minutes() const
{
	return this->minutes;
}

void Date::set_minutes(uint16_t minutes)
{
	this->minutes = minutes;
}

std::uint16_t Date::get_seconds() const
{
	return this->seconds;
}

void Date::set_seconds(uint16_t seconds)
{
	this->seconds = seconds;
}

int32_t Date::difference(const Date& another, MeasureTime measure) const
{
	if (is_valid() && another.is_valid()) {
		if (*this == another) {
			return 0;
		}
		else if (*this < another) {
			return -another.difference(*this,measure);
		}
		else {
			int32_t answer=0;
			if (measure == MeasureTime::years) {
				answer = this->year - another.get_year();
			}
			else if (measure == MeasureTime::months) {
				answer = difference(another, MeasureTime::years) * 12;
				if (this->month > another.get_month()) {
					answer += this->month - another.get_month();
				}
				else {
					answer -= another.get_month() - this->month;
				}
			}
			else if (measure == MeasureTime::days) {
				answer = difference(another, MeasureTime::years) * 365;
				answer += count_leap_years(another.get_year());
				answer += get_number_in_year();
				answer -= another.get_number_in_year();
			}
			else if (measure == MeasureTime::hours) {
				answer = difference(another, MeasureTime::days) * 24;
				answer += this->hours;
				answer -= another.get_hours();
			}
			else if (measure == MeasureTime::minutes) {
				answer = difference(another, MeasureTime::hours) * 60;
				answer += this->minutes;
				answer -= another.get_minutes();
			}
			else if (measure == MeasureTime::seconds) {
				answer = difference(another, MeasureTime::minutes) * 60;
				answer += this->seconds;
				answer -= another.get_seconds();
			}
			return answer;
		}
	}
	else {
		//throw
	}
}

void Date::promote(uint16_t number, MeasureTime measure)
{
	if (measure == MeasureTime::years) {
		this->year += number;
	}
	else if (measure == MeasureTime::months) {
		this->year += number/12;
		this->month += number % 12;
		promote_month(number);
	}
	if (measure == MeasureTime::days) {
		promote(number / 30, MeasureTime::months);
		number %= 30;
		promote_day(number);
	}
	if (measure == MeasureTime::hours) {
		promote(number / 24, MeasureTime::days);
		number %= 24;
		promote_hour(number);
	}
	if (measure == MeasureTime::minutes) {
		promote(number / 60, MeasureTime::hours);
		number %= 60;
		promote_minute(number);
	}
	if (measure == MeasureTime::seconds) {
		promote(number / 60, MeasureTime::minutes);
		number %= 60;
		promote_second(number);
	}
}

void Date::decrease(uint16_t number, MeasureTime measure)
{
	if (measure == MeasureTime::years) {
		this->year -= number;
	}
	else if (measure == MeasureTime::months) {
		this->year -= number / 12;
		number = number % 12;
		decrease_month(number);
		if (this->day > days_in_monthes[this->month]) {
			int32_t dif = this->day - days_in_monthes[this->month];
			this->day = days_in_monthes[this->month] - dif;
		}
	}
	if (measure == MeasureTime::days) {
		decrease(number / 30, MeasureTime::months);
		number %= 30;
		decrease_day(number);
	}
	if (measure == MeasureTime::hours) {
		decrease(number / 24, MeasureTime::days);
		number %= 24;
		decrease_hour(number);
	}
	if (measure == MeasureTime::minutes) {
		decrease(number / 60, MeasureTime::hours);
		number %= 60;
		decrease_minute(number);
	}
	if (measure == MeasureTime::seconds) {
		decrease(number / 60, MeasureTime::minutes);
		number %= 60;
		decrease_second(number);
	}
}

bool Date::is_valid() const
{
	if (seconds > 59 || minutes > 59 || hours > 23 || month > 12 ||
		month==0 || day ==0 || day>days_in_monthes[month]) {
		return false;
	}
	if (day == 29 && month == 2 && !is_leap()) {
		return false;
	}
	return true;
}

bool Date::is_leap() const
{
	if (year % 400 == 0 || year % 4 == 0 && year % 100 != 0) {
		return true;
	}
	else {
		return false;
	}
}

int32_t Date::count_leap_years(int32_t another_year) const
{
	if (another_year < this->year) {
		int32_t answer = (this->year - another_year)/4;
		if (another_year % 4 > this->year % 4) {
			answer++;
		}
		answer -= (this->year - another_year) / 100;
		if (another_year % 100 > this->year % 100) {
			answer--;
		}
		answer += (this->year - another_year) / 400;
		if (another_year % 400 > this->year % 400) {
			answer++;
		}
		return answer;
	}
}

int32_t Date::get_number_in_year() const
{
	if (is_valid()) {
		int32_t answer=0;
		for (size_t i = 1; i < this->month; i++) {
			answer += days_in_monthes[i];
		}
		answer += this->day;
		if (!is_leap() && this->month>2) {
			answer--;
		}
		return answer;
	}
	else {
		//throw
	}
}

int32_t Date::get_reverse_number_in_year() const
{
	int32_t answer = 365 - get_number_in_year();
	if (is_leap()) {
		answer++;
	}
	return answer;
}

Day Date::get_day_of_week()
{
	Date basis(1, 1, 1, 27, 9, 2020);
	uint16_t day = 0;
	int32_t dif;
	if (*this == basis) {
		return Day::Sunday;
	}
	else if (*this < basis) {
		dif = basis.difference(*this, MeasureTime::days);
		day = dif % 7;
		if (day == 0) {
			return Day::Sunday;
		}
		else if (day == 1) {
			return Day::Monday;
		}
		else if (day == 2) {
			return Day::Thursday;
		}
		else if (day == 3) {
			return Day::Wednesday;
		}
		else if (day == 4) {
			return Day::Tuesday;
		}
		else if (day == 5) {
			return Day::Friday;
		}
		else if (day == 6) {
			return Day::Saturday;
		}
	}
	else {
		dif = this->difference(basis, MeasureTime::days);
		day = dif % 7;
		if (day == 0) {
			return Day::Sunday;
		}
		else if (day == 1) {
			return Day::Saturday;
		}
		else if (day == 2) {
			return Day::Friday;
		}
		else if (day == 3) {
			return Day::Thursday;
		}
		else if (day == 4) {
			return Day::Wednesday;
		}
		else if (day == 5) {
			return Day::Tuesday;
		}
		else if (day == 6) {
			return Day::Monday;
		}
	}
}

void Date::promote_month(uint16_t number)
{
	if (this->month > 12) {
		this->month = month % 12;
		this->year++;
	}
	if (this->day > days_in_monthes[this->month]) {
		int32_t dif = this->day - days_in_monthes[this->month];
		this->day = days_in_monthes[this->month];
		promote(dif, MeasureTime::days);
	}
}

void Date::promote_day(uint16_t number)
{
	if (this->day + number > days_in_monthes[this->month]) {
		number -= days_in_monthes[this->month] - this->day;
		this->day = number;
		promote_month(1);
	}
	else {
		this->day += number;
	}
}

void Date::promote_hour(uint16_t number)
{
	if (number + this->hours > 23) {
		promote_day(1);
		this->hours = (number + this->hours) % 24;
	}
	else {
		this->hours += number;
	}
}

void Date::promote_minute(uint16_t number)
{
	if (number + this->minutes > 59) {
		promote_hour(1);
		this->minutes = (number + this->minutes) % 60;
	}
	else {
		this->minutes += number;
	}
}

void Date::promote_second(uint16_t number)
{
	if (number + this->seconds > 59) {
		promote_minute(1);
		this->seconds = (number + this->seconds) % 60;
	}
	else {
		this->seconds += number;
	}
}

void Date::decrease_month(uint16_t number)
{
	if (this->month > number) {
		this->month -= number;
	}
	else {
		this->month = 12 - number % 12 + this->month;
	}
}

void Date::decrease_day(uint16_t number)
{
	if (this->day > number) {
		this->day -= number;
	}
	else {
		if (this->month == 1) {
			this->year--;
			this->month = 12;
		}
		else {
			this->month--;
		}
		number -= this->day;
		this->day = days_in_monthes[this->month];
		this->day -= number;
	}
}

void Date::decrease_hour(uint16_t number)
{
	if (number < this->hours) {
		this->hours -= number;
	}
	else {
		decrease_day(1);
		number -= this->hours;
		this->hours = 24 - number;
	}
}

void Date::decrease_minute(uint16_t number)
{
	if (number < this->minutes) {
		this->minutes -= number;
	}
	else {
		decrease_hour(1);
		number -= this->minutes;
		this->minutes = 60 - number;
	}
}

void Date::decrease_second(uint16_t number)
{
	if (number < this->seconds) {
		this->seconds -= number;
	}
	else {
		decrease_hour(1);
		number -= this->seconds;
		this->seconds = 60 - number;
	}
}

bool operator==(const Date& lhs, const Date& rhs)
{
	return lhs.get_day() == rhs.get_day() && lhs.get_month() == rhs.get_month() &&
		lhs.get_year() == rhs.get_year() && lhs.get_seconds() == rhs.get_seconds()&&
		lhs.get_minutes() == rhs.get_minutes() && lhs.get_hours() == rhs.get_hours();
}

bool operator<(const Date& lhs, const Date& rhs)
{
	if (lhs.get_year() == rhs.get_year()) {
		if (lhs.get_month() == rhs.get_month()) {
			if (lhs.get_day() == rhs.get_day()) {
				if (lhs.get_hours() == rhs.get_hours()) {
					if (lhs.get_minutes() == rhs.get_minutes()) {
						return lhs.get_seconds() < rhs.get_seconds();
					}
					return lhs.get_minutes() < rhs.get_minutes();
				}
				else {
					return lhs.get_hours() < rhs.get_hours();
				}
			}
			else {
				return lhs.get_day() < rhs.get_day();
			}
		}
		else {
			return lhs.get_month() < rhs.get_month();
		}
	}
	else {
		return lhs.get_year() < rhs.get_year();
	}
}

std::ostream& operator<<(std::ostream& os, const Date& date)
{
	os<<date.get_seconds()<<':'<<date.get_minutes()<<':'<<date.get_hours()<<' '<< date.get_day() << '.' << date.get_month() << '.' << date.get_year();
	return os;
}
