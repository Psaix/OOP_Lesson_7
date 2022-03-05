#include <iostream>
#include <memory>


//____________________________________________________________________________________________________________
//____________________________________________TASK1___________________________________________________________
//____________________________________________________________________________________________________________


class Date {
	int m_day;
	int m_month;
	int m_year;

	friend std::ostream& operator<<(std::ostream& os, const Date& date);

public:
	int setDay(int day) {
		m_day = day;
		return m_day;
	}
	int setMonth(int month) {
		m_month = month;
		return m_month;
	}
	int setYear(int year) {
		m_year = year;
		return m_year;
	}	

	int getDay() const{
		return m_day;
	}
	int getMonth() const{
		return m_month;
	}
	int getYear() const{
		return m_year;
	}
};

std::ostream& operator<<(std::ostream& os, const Date& date)
{
	os << "Date: " << date.m_day << "." << date.m_month << "." << date.m_year << "\n";
	return os;
}



//____________________________________________________________________________________________________________
//____________________________________________TASK2___________________________________________________________
//____________________________________________________________________________________________________________



Date compareDates(std::unique_ptr<Date>& date1, std::unique_ptr<Date>& date2)
{
	if (date1->getYear() > date2->getYear())
		return *date1;
	if (date1->getYear() < date2->getYear())
		return *date2;
	if (date1->getYear() == date2->getYear())
	{
		if(date1->getMonth() > date2->getMonth())
			return *date1;
		if (date1->getMonth() < date2->getMonth())
			return *date2;
		if (date1->getMonth() == date2->getMonth())
		{
			if(date1->getDay() > date2->getDay())
				return *date1;
			if (date1->getDay() < date2->getDay())
				return *date2;
			if (date1->getDay() == date2->getDay())
			{
				std::cout << "Dates are equal" << "\n";
				return *date1;
			}
		}
	}
}

void swapDates(std::unique_ptr<Date>& date1, std::unique_ptr<Date>& date2)
{
	std::unique_ptr<Date> tPtr = move(date1);
	date1 = move(date2);
	date2 = move(tPtr);
}


int main()
{

//____________________________________________TASK1___________________________________________________________

	std::unique_ptr<Date> today(new Date);
	today->setDay(5);
	today->setMonth(3);
	today->setYear(2022);

	std::cout << *today;

	std::unique_ptr<Date> date = move(today);


	if (today == nullptr)
		std::cout << "Pointer today = nullptr" << std::endl;
	else
		std::cout << "Pointer today = " << *today << std::endl;
	
	if (date == nullptr)
		std::cout << "Pointer date = nullptr" << std::endl;
	else
		std::cout << "Pointer date = " << *date << std::endl;


//____________________________________________TASK2___________________________________________________________

	std::unique_ptr<Date> date1(new Date);
	std::unique_ptr<Date> date2(new Date);

	date1->setDay(12);
	date1->setMonth(11);
	date1->setYear(1985);

	date2->setDay(14);
	date2->setMonth(07);
	date2->setYear(1997);

	std::cout << compareDates(date1, date2);

	swapDates(date1, date2);

	std::cout << *date1;
	std::cout << *date2;


	return 0;
}