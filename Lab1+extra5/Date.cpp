#include "Date.h"

Date::Date(uint16_t seconds, uint16_t minutes, uint16_t hours, uint16_t day,
	uint16_t month, uint16_t year)
	:seconds(seconds), minutes(minutes), hours(hours),
	day(day), month(month), year(year) {}

Date::Date(uint16_t seconds, uint16_t minutes, uint16_t hours, Day day,
	uint16_t number, bool begin, uint16_t month, uint16_t year)
	:seconds(seconds),minutes(minutes),hours(hours),month(month),
	year(year)
{
	choose_day(day, number, begin);
}

Date::Date()
	: seconds(0),minutes(0),hours(0),day(0),month(),year() {}

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
		else if (another<*this) {
			return -another.difference(*this,measure);
		}
		else {
			int32_t answer=0;
			if (measure == MeasureTime::years) {
				answer = another.get_year()-this->year;
				if (another<Date(this->seconds, this->minutes, this->hours, this->day, this->month,
					another.get_year())) {
					answer--;
				}
			}
			else if (measure == MeasureTime::months) {
				answer = difference(another, MeasureTime::years) * 12;
				if (this->month > another.get_month()) {
					answer += 12-this->month;
					answer+=another.get_month();
				}
				else {
					answer -= -another.get_month() + this->month;
				}
				if (another < Date(this->seconds, this->minutes, this->hours, this->day, another.get_month(),
					another.get_year())) {
					answer--;
				}
			}
			else if (measure == MeasureTime::days) {
				if (this->day == 29 && this->month == 2 && is_leap()) {
					Date tmp(this->seconds, this->minutes, this->hours, 1, 3, this->year);
					if (another < tmp) {
						return 0;
					}
					else {
						return tmp.difference(another, MeasureTime::days) + 1;
					}
				}

				answer = difference(another, MeasureTime::years) * 365;
				answer += count_29_february(another);

				if (another< Date(this->seconds, this->minutes, this->hours, this->day,
					this->month, another.get_year())) {
					answer += get_reverse_number_in_year() - 1;
					answer += another.get_number_in_year();
					if (another.is_leap() && Date(0, 0, 0, 29, 2, another.get_year()) < another) {
						answer--;
					}
					if (is_leap() && *this < Date(0, 0, 0, 29, 2, this->year)) {
						answer--;
					}
				}
				else {
					answer -= get_number_in_year();
					answer += another.get_number_in_year();

					if (another.is_leap() && Date(59, 59, 23, 29, 2, another.get_year()) < another) {
						answer--;
					}
					
					if (is_leap() && Date(0, 0, 0, 29, 2, get_year()) < *this) {
						answer++;
					}
				}
				
				if (another < Date(this->seconds, this->minutes, this->hours,
					another.get_day(), another.get_month(), another.get_year())) {
					answer--;
				}
			}
			else if (measure == MeasureTime::hours) {
				answer = difference(another, MeasureTime::days) * 24;
				if (another < Date(this->seconds, this->minutes, this->hours,
					another.get_day(), another.get_month(), another.get_year())) {
					answer += 24 - this->hours + another.get_hours();
				}
				else {
					answer -= this->hours;
					answer += another.get_hours();
				}

				if (another < Date(this->seconds, this->minutes, another.get_hours(),
					another.get_day(), another.get_month(), another.get_year())) {
					answer--;
				}
			}
			else if (measure == MeasureTime::minutes) {
				answer = difference(another, MeasureTime::hours) * 60;
				if (another < Date(this->seconds, this->minutes, another.get_hours(),
					another.get_day(), another.get_month(), another.get_year())) {
					answer += 60 - this->minutes + another.get_minutes();
				}
				else {
					answer -= this->minutes;
					answer += another.get_minutes();
				}

				if (another < Date(this->seconds, another.get_minutes(), another.get_hours(),
					another.get_day(), another.get_month(), another.get_year())) {
					answer--;
				}
			}
			else if (measure == MeasureTime::seconds) {
				answer = difference(another, MeasureTime::minutes) * 60;
				if (another < Date(this->seconds, another.get_minutes(), another.get_hours(),
					another.get_day(), another.get_month(), another.get_year())) {
					answer += 60 - this->seconds + another.get_seconds();
				}
				else {
					answer -= this->seconds;
					answer += another.get_seconds();
				}
			}
			return answer;
		}
	}
	else {
		throw std::invalid_argument("invalid date");
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

int32_t Date::count_29_february(const Date& another_date) const
{
	if (*this == another_date) {
		return 0;
	}
	else if (*this < another_date) {
		int32_t answer = (another_date.get_year() - this->year)/4;
		if (another_date.get_year() % 4 < this->year % 4) {
			answer++;
		}
		answer -= (another_date.get_year() - this->year) / 100;
		if (another_date.get_year() % 100 < this->year % 100) {
			answer--;
		}
		answer += (another_date.get_year() - this->year) / 400;
		if (another_date.get_year() % 400 < this->year % 400) {
			answer++;
		}
		if (*this < Date(0, 0, 0, 29, 2, this->year) &&
			is_leap()) {
			answer++;
		}
		if (another_date.is_leap() &&
			another_date < Date(0, 0, 0, 1, 3, another_date.get_year())) {
			answer--;
		}
		if (*this < Date(0, 0, 0, 1, 3, this->year) &&
			Date(59, 59, 23, 28, 2, this->year) < *this &&
			another_date < Date(0, 0, 0, 1, 3, another_date.get_year()) &&
			Date(59, 59, 23, 28, 2, another_date.get_year()) < another_date) {
			return 0;
		}
		return answer;
	}
	else {
		return -another_date.count_29_february(*this);
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
		throw std::invalid_argument("invalid date");
	}
}

int32_t Date::get_reverse_number_in_year() const
{
	try
	{
		int32_t answer = 366 - get_number_in_year();
		if (is_leap()) {
			answer++;
		}
		return answer;
	}
	catch (const std::exception&)
	{
		throw std::invalid_argument("invalid date");
	}
}

Day Date::get_day_of_week() const
{
	Date basis(0, 0, 0, 27, 9, 2020);
	uint16_t day = 0;
	int32_t dif;
	if (*this == basis) {
		return Day::Sunday;
	}
	else if (basis< *this) {
		dif = basis.difference(*this, MeasureTime::days);
		day = dif % 7;
		
		if (day == 0) {
			return Day::Sunday;
		}
		else if (day == 1) {
			return Day::Monday;
		}
		else if (day == 2) {
			return Day::Tuesday;
		}
		else if (day == 3) {
			return Day::Wednesday;
		}
		else if (day == 4) {
			return Day::Thursday;
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
		if (basis < Date(this->seconds, this->minutes, this->hours, basis.get_day(), basis.get_month(), basis.get_year())) {
			dif++;
		}
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
	return Day::Monday;
}

uint16_t Date::get_number_of_week(bool month_or_year)
{
	if (month_or_year) {
		uint16_t buf = this->day;
		this->day = 0;
		uint16_t counter = 0;
		while (this->day != buf) {
			this->day++;
			if (get_day_of_week() == Day::Monday) {
				counter++;
			}
		}
		return counter;
	}
	else {
		Date basis(0, 0, 0, 1, 1, this->year);
		while (basis.get_day_of_week()!=Day::Monday) {
			basis.save_promote_day(1);
		}
		if (*this < basis) {
			return 0;
		}
		else {
			int32_t dif = basis.difference(*this, MeasureTime::days);
			return 1 + dif / 7;
		}
	}
}

void Date::choose_day(Day day,uint16_t number,bool begin)
{
	if (begin) {
		if (number == 0) {
			this->day = 1;
			return;
		}
		this->day = 0;
		uint16_t counter = 0;
		while (this->day != days_in_monthes[this->month] && counter != number) {
			this->day++;
			if (get_day_of_week() == day) {
				counter++;
			}
		}
		if (this->day == 29 && this->month == 2 && !is_leap()) {
			this->day = 28;
		}
	}
	else {
		if (number == 0) {
			this->day = days_in_monthes[this->month];
			return;
		}
		this->day = days_in_monthes[this->month] + 1;
		if (this->day == 30 && this->month == 2 && !is_leap()) {
			this->day = 29;
		}
		uint16_t counter = 0;
		while (this->day != 1 && counter != number) {
			this->day--;
			if (get_day_of_week() == day) {
				counter++;
			}
		}
	}
}

void Date::promote(uint16_t number, MeasureTime measure)
{
	if (measure == MeasureTime::years) {
		save_promote_year(number);
	}
	else if (measure == MeasureTime::months) {
		save_promote_month(number);
	}
	if (measure == MeasureTime::days) {
		save_promote_day(number);
	}
	if (measure == MeasureTime::hours) {
		save_promote_hour(number);
	}
	if (measure == MeasureTime::minutes) {
		save_promote_minute(number);
	}
	if (measure == MeasureTime::seconds) {
		save_promote_second(number);
	}
}

void Date::decrease(uint16_t number, MeasureTime measure)
{
	if (measure == MeasureTime::years) {
		save_decrease_year(number);
	}
	else if (measure == MeasureTime::months) {
		save_decrease_month(number);
	}
	if (measure == MeasureTime::days) {
		save_decrease_day(number);
	}
	if (measure == MeasureTime::hours) {
		save_decrease_hour(number);
	}
	if (measure == MeasureTime::minutes) {
		save_decrease_minute(number);
	}
	if (measure == MeasureTime::seconds) {
		save_decrease_second(number);
	}
}

void Date::save_promote_year(uint16_t number)
{
	this->year += number;
	if (this->month == 2 && this->day == 29 && !is_leap()) {
		this->month = 3;
		this->day = 1;
	}
}

void Date::save_decrease_year(uint16_t number)
{
	this->year -= number;
	if (this->month == 2 && this->day == 29 && !is_leap()) {
		this->month = 3;
		this->day = 1;
	}
}

void Date::save_promote_month(uint16_t number)
{
	save_promote_year(number / 12);
	number %= 12;
	if (this->month + number > 12) {
		this->year++;
		this->month = this->month + number-12;
	}
	else {
		this->month += number;
	}
	if (this->day > 28 && this->month == 2 && !is_leap()) {
		uint16_t dif = this->day - 28;
		this->day = dif;
		this->month = 3;
	}
	else if (this->day > days_in_monthes[this->month]) {
		uint16_t dif = this->day-days_in_monthes[this->month];
		this->day = dif;
		if (this->month == 13) {
			this->year++;
			this->month = 1;
		}
	}
}

void Date::save_decrease_month(uint16_t number)
{
	save_decrease_year(number / 12);
	number %= 12;
	if (this->month <= number) {
		this->year--;
		this->month = 12 - number + this->month;
	}
	else {
		this->month -= number;
	}
	if (this->day > 28 && this->month == 2 && !is_leap()) {
		uint16_t dif = this->day-28 ;
		this->day = 29-dif;
	}
	else if (this->day > days_in_monthes[this->month]) {
		uint16_t dif = this->day-days_in_monthes[this->month];
		this->day = days_in_monthes[this->month]+1-dif;
		if (this->month == 0) {
			this->year--;
			this->month = 12;
		}
	}
}

void Date::save_promote_day(uint16_t number)
{
	if (this->day + number > days_in_monthes[this->month]) {
		number -= days_in_monthes[this->month]-this->day+1;
		if (this->month == 2 && !is_leap()) {
			number++;
		}
		save_promote_month(1);
		this->day = 1;
		save_promote_day(number);
	}
	else {
		this->day += number;
	}
	if (this->month == 2 && this->day == 29 && !is_leap()) {
		this->month = 3;
		this->day = 1;
	}
}

void Date::save_decrease_day(uint16_t number)
{
	if (this->day <= number) {
		number -= this->day;
		save_decrease_month(1);
		this->day = days_in_monthes[this->month];
		
		if (this->day == 29 && this->month == 2 && !is_leap()) {
			this->day--;
		}
		save_decrease_day(number);
	}
	else {
		this->day -= number;
	}
	if (this->month == 2 && this->day == 29 && !is_leap()) {
		this->day = 28;
	}
}

void Date::save_promote_hour(uint16_t number)
{
	save_promote_day(number / 24);
	number %= 24;
	if (number + this->hours > 23) {
		save_promote_day(1);
		this->hours = (number + this->hours) % 24;
	}
	else {
		this->hours += number;
	}
}

void Date::save_decrease_hour(uint16_t number)
{
	save_decrease_day(number / 24);
	number %= 24;
	if (number <= this->hours) {
		this->hours -= number;
	}
	else {
		save_decrease_day(1);
		number -= this->hours;
		this->hours = 24 - number;
	}
}

void Date::save_promote_minute(uint16_t number)
{
	save_promote_hour(number / 60);
	number %= 60;
	if (number + this->minutes > 59) {
		save_promote_hour(1);
		this->minutes = (number + this->minutes) % 60;
	}
	else {
		this->minutes += number;
	}
}

void Date::save_decrease_minute(uint16_t number)
{
	save_decrease_hour(number / 60);
	number %= 60;
	if (number <= this->minutes) {
		this->minutes -= number;
	}
	else {
		save_decrease_hour(1);
		number -= this->minutes;
		this->minutes = 60 - number;
	}
}

void Date::save_promote_second(uint16_t number)
{
	save_promote_minute(number / 60);
	number %= 60;
	if (number + this->seconds > 59) {
		save_promote_minute(1);
		this->seconds = (number + this->seconds) % 60;
	}
	else {
		this->seconds += number;
	}
}

void Date::save_decrease_second(uint16_t number)
{
	save_decrease_minute(number / 60);
	number %= 60;
	if (number <= this->seconds) {
		this->seconds -= number;
	}
	else {
		save_decrease_minute(1);
		number -= this->seconds;
		this->seconds = 60 - number;
	}
}

std::ostream& operator<<(std::ostream& os, const Day& day)
{
	if (day == Day::Friday) {
		os << "Friday";
	}
	else if (day == Day::Monday) {
		os << "Monday";
	}
	else if (day == Day::Saturday) {
		os << "Saturday";
	}
	else if (day == Day::Sunday) {
		os << "Sunday";
	}
	else if (day == Day::Thursday) {
		os << "Thursday";
	}
	else if (day == Day::Tuesday) {
		os << "Tuesday";
	}
	else if (day == Day::Wednesday) {
		os << "Wednesday";
	}
	return os;
}

Day statistics(const Date& bottom, const Date& top, uint16_t number)
{
	if (number == 0 || number > 31) {
		throw std::invalid_argument("invalid number");
	}
	if (top < bottom) {
		throw std::invalid_argument("top < bottom");
	}
	else if (top == bottom && top<bottom && top.difference(bottom,MeasureTime::days)==0
		&& top.get_day() == bottom.get_day()) {
		if (number == bottom.get_day()) {
			return bottom.get_day_of_week();
		}
		else {
			return Day::Monday;
		}
	}
	else {
		Date basis(0, 0, 0, number, bottom.get_month(), bottom.get_year());
		if (basis < bottom || !basis.is_valid()) {
			basis.save_promote_month(1);
		}
		std::map<Day, int>numbers;
		while (basis < top) {
			if (basis.is_valid()) {
				numbers[basis.get_day_of_week()]++;
			}
			basis.save_promote_month(1);
			basis.set_day(number);
		}
		if (numbers.size() == 0) {
			return Day::Monday;
		}
		else {
			Day max = numbers.begin()->first;
			int max_number = numbers.begin()->second;
			for (auto it = ++numbers.begin(); it != numbers.end(); it++) {
				if (it->second > max_number) {
					max = it->first;
					max_number = it->second;
				}
			}
			return max;
		}
	}
}

Day statistics(int32_t year, uint16_t number)
{
	return statistics(Date(59, 59, 23, 31, 12, year - 1), Date(0, 0, 0, 1, 1, year + 1), number);
}

bool operator==(const Date& lhs, const Date& rhs)
{
	return lhs.get_day() == rhs.get_day() && lhs.get_month() == rhs.get_month() &&
		lhs.get_year() == rhs.get_year() && lhs.get_seconds() == rhs.get_seconds() &&
		lhs.get_minutes() == rhs.get_minutes() &&
		lhs.get_hours() == rhs.get_hours();
}

bool operator!=(const Date& lhs, const Date& rhs)
{
	return !(lhs == rhs);
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
