#include "qjalalidate.h"

QJalaliDate::QJalaliDate(const QDate &date) : jd(nullJd())
{
    setDateFromGregorian(date.year(), date.month(), date.day());
}

QJalaliDate::QJalaliDate(int year, int month, int day) : jd(nullJd())
{
    setDate(year, month, day);
}

QJalaliDate QJalaliDate::currentDate()
{
    QDate gd = QDate::currentDate();
    QJalaliDate d;
    qint64 j = jalaliDayFromDate(gd.year(), gd.month(), gd.day());
    d.jd = j;
    return d;
}

QDate QJalaliDate::toGregorian(const QJalaliDate &jd)
{
    return toGregorian(jd.year(), jd.month(), jd.day());
}

QDate QJalaliDate::toGregorian(int year, int month, int day)
{
    int miladiYear, marchDayDiff, dayCount;

    //set default value
    int miladiDate[]      =   { 1, 1, 1 };
    char farvardin1st[]   =   { 1, 1, 1 };
    char miladiMonth[]    =   { 30, 31, 30, 31, 31, 30, 31, 30, 31, 31, 28, 31 };

    miladiYear = year + 621;

    //Detemining the Farvardin the First
    if (QDate::isLeapYear(miladiYear)){
        //this is a Miladi leap year so Shamsi is leap too so the 1st of Farvardin is March 20 (3/20)
        farvardin1st[1] = 3; // change default values
        farvardin1st[2] = 20;
        marchDayDiff = 12;
    } else {
        //this is not a Miladi leap year so Shamsi is not leap too so the 1st of Farvardin is March 21 (3/21)
        farvardin1st[1] = 3;
        farvardin1st[2] = 21;
        marchDayDiff = 11;
    }

    if (QDate::isLeapYear(miladiYear + 1))
        miladiMonth[10] = miladiMonth[10] + 1; //Adding one day to Feb

    //Calculate the day count for input shamsi date from 1st Farvadin
    if (month >= 1 && month <= 6)
        dayCount = ((month - 1) * 31) + day;
    else
        dayCount = (6 * 31) + ((month - 7) * 30) + day;

    //Finding the correspond miladi month and day
    int remainDay;
    if (dayCount <= marchDayDiff) {
        //So we are in 20 (for leap year or 21 for none leap year) to 31 march
        miladiDate[2] = dayCount + (31 - marchDayDiff);
        miladiDate[1] = 3;
        miladiDate[0] = miladiYear;

    }
    else
        remainDay = dayCount - marchDayDiff;

    int i;
    i = 0; // starting from April

    while (remainDay > miladiMonth[i]) {
        remainDay = remainDay - miladiMonth[i];
        i = i + 1;
    }

    miladiDate[2] = remainDay;

    if (i > 8) {
        // We are in the next Miladi Year
        miladiDate[1] = i - 8;
        miladiDate[0] = miladiYear + 1;
    } else {
        miladiDate[1] = i + 4;
        miladiDate[0] = miladiYear;
    }

    int y = miladiDate[0];
    int m = miladiDate[1];
    int d = miladiDate[2];

    return QDate(y, m, d);
}

QJalaliDate QJalaliDate::fromGregorian(const QDate &gdate)
{
    return fromGregorian(gdate.year(), gdate.month(), gdate.day());
}

QJalaliDate QJalaliDate::fromGregorian(int year, int month, int day)
{
    return QJalaliDate(year, month, day);
}

QJalaliDate QJalaliDate::fromString(const QString &string, Qt::DateFormat format)
{
}

QJalaliDate QJalaliDate::fromString(const QString &string, const QString &format)
{
}

bool QJalaliDate::isLeapYear(int year)
{
    int mod;
    mod = year % 33;
    if (year <= 1472 && year >= 1343) {
        if (mod == 1 || mod == 5 || mod == 9 || mod == 13 || mod == 17 || mod == 22 || mod == 26 || mod == 23)
            return true;
        else
            return false;
    }

    if (year <= 1342 && year >= 1244) {
        if (mod == 1 || mod == 5 || mod ==9 || mod == 13 || mod == 17 || mod == 22 || mod == 26 || mod == 23)
            return true;
        else
            return false;
    }

    return false;
}

bool QJalaliDate::isValid(int year, int month, int day)
{
    return true;
}

/*!
 * \brief QJalaliDate::day
 * \return
 */
int QJalaliDate::day() const
{
    if (isNull())
        return 0;

    return getDateFromJalaliDay(jd).day;
}

/*!
    Returns the number corresponding to the month of this date, using
    the following convention:
    \list
    \li 1 = "January"
    \li 2 = "February"
    \li 3 = "March"
    \li 4 = "April"
    \li 5 = "May"
    \li 6 = "June"
    \li 7 = "July"
    \li 8 = "August"
    \li 9 = "September"
    \li 10 = "October"
    \li 11 = "November"
    \li 12 = "December"
    \endlist
    Returns 0 if the date is invalid.
    \sa year(), day()
*/
int QJalaliDate::month() const
{
    if (isNull())
        return 0;

    return getDateFromJalaliDay(jd).month;
}

int QJalaliDate::year() const
{
    if (isNull())
        return 0;

    return getDateFromJalaliDay(jd).year;
}

ParsedDate QJalaliDate::getDateFromJalaliDay(qint64 jalaliDay)
{
    int jNp = div(jalaliDay, 12053).quot; // 12053 = 365 * 33 + 32 / 4

    jalaliDay = jalaliDay % 12053;

    int year = 979 + 33 * jNp + 4 * div(jalaliDay, 1461).quot; // 1461 = 365 * 4 + 4 / 4

    jalaliDay %= 1461;

    if(jalaliDay >= 366) {
        year += div(jalaliDay - 1, 365).quot;
        jalaliDay = (jalaliDay - 1) % 365;
    }

    int ii;
    for(int i = 0; i < 11 && jalaliDay >= jalaliDaysInMonth[i]; ++i){
        jalaliDay -= jalaliDaysInMonth[i];
        ii = i;
    }

    int  month = ii + 2;
    int  day = jalaliDay + 1;

    const ParsedDate result = { year, month, day };
    return result;
}

int QJalaliDate::dayOfWeek() const
{
}

int QJalaliDate::dayOfYear() const
{
}

int QJalaliDate::daysInMonth() const
{
}

int QJalaliDate::daysInYear() const
{
}

qint64 QJalaliDate::daysTo(const QJalaliDate &d) const
{
}

qint64 QJalaliDate::daysTo(const QDate &d) const
{
}

void QJalaliDate::getDate(int *year, int *month, int *day) const
{
}

bool QJalaliDate::setDate(int year, int month, int day)
{
    const QDate d = toGregorian(year, month, day);
    return setDateFromGregorian(d.year(), d.month(), d.day());
}

QDate QJalaliDate::toGregorian() const
{
    return toGregorian(this->year(), this->month(), this->day());
}

bool QJalaliDate::setDateFromGregorian(int year, int month, int day)
{
    if (isValid(year, month, day))
        jd = jalaliDayFromDate(year, month, day);
    else
        jd = nullJd();

    return isValid();
}

qint64 QJalaliDate::jalaliDayFromDate(int year, int month, int day)
{
    year = year - 1600;
    month = month - 1;
    day = day - 1;

    int gDayNo = 365 * year + div(year + 3, 4).quot - div(year + 99, 100).quot + div(year + 399, 400).quot;

    for(int i=0; i < month; ++i)
        gDayNo += gregorianDaysInMonth[i];

    if(month > 1 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)))
        //leap and after Feb
        gDayNo++;

    gDayNo += day;

    qint64 jDayNo = gDayNo - 79;

    return jDayNo;
}

QString QJalaliDate::toString(const QString &format) const
{
    const ParsedDate d = getDateFromJalaliDay(jd);

    QString day;

    int i = 0;
    while((i = format.indexOf("ddd")) != -1){
        
    }

    return QString(format).arg(d.year, 4, 10, QChar('0')).arg(d.month, 2, 10, QChar('0')).arg(d.day, 2, 10, QChar('0'));

//    QStringList jalali;
//    jalali << QString::number(jy) << QString::number(jm) << QString::number(jd); // << m_jalaliMonthsName[jm] ;
//    return jalali.join('-');
}

int QJalaliDate::weekNumber(int *yearNumber) const
{
}

QString QJalaliDate::getName() const
{
    return m_name;
}
