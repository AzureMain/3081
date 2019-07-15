#ifndef DATE_H
#define DATE_H
#include <string>
class Date{
	private:
		int year, int month, int day;
	public:
		Date();
		Date (int,int,int);
		std::string show() const;
		bool equals (const Date&) const;
		bool before (const Date&) const;
		void day.inc(int);
}
#endif

Date::Date(){
	year=1900, month=1, day=1;
}
bool Date::equals (const Date &d) const{
	return year == d.year && month==d.month&&day=d.day;
}