#include "qjalalidate.h"

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

QJalaliDate QJalaliDate::fromGregorianDay(qint64 jd)
{
}

QDate QJalaliDate::toGregorian(const QJalaliDate &jd)
{
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
}

bool QJalaliDate::isValid(int year, int month, int day)
{
    return true;
}

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
    if (isValid(year, month, day))
        jd = jalaliDayFromDate(year, month, day);
    else
        jd = nullJd();

    return isValid();
}

QString QJalaliDate::toString(const QString &format) const
{
    const ParsedDate d = getDateFromJalaliDay(jd);

    return QString(format).arg(d.year, 4, 10, QChar('0')).arg(d.month, 2, 10, QChar('0')).arg(d.day, 2, 10, QChar('0'));

//    QStringList jalali;
//    jalali << QString::number(jy) << QString::number(jm) << QString::number(jd); // << m_jalaliMonthsName[jm] ;
//    return jalali.join('-');
}

QString QJalaliDate::toString(Qt::DateFormat format) const
{
//    if (!isValid())
//        return QString();

//    switch (format) {
//    case Qt::SystemLocaleDate:
//    case Qt::SystemLocaleShortDate:
//        return QLocale::system().toString(*this, QLocale::ShortFormat);
//    case Qt::SystemLocaleLongDate:
//        return QLocale::system().toString(*this, QLocale::LongFormat);
//    case Qt::LocaleDate:
//    case Qt::DefaultLocaleShortDate:
//        return QLocale().toString(*this, QLocale::ShortFormat);
//    case Qt::DefaultLocaleLongDate:
//        return QLocale().toString(*this, QLocale::LongFormat);
//    case Qt::RFC2822Date:
//        return QLocale::c().toString(*this, QStringLiteral("dd MMM yyyy"));
//    default:
//#ifndef QT_NO_TEXTDATE
//    case Qt::TextDate:
//        return toStringTextDate(*this);
//#endif
//    case Qt::ISODate:
//    case Qt::ISODateWithMs:
//        return toStringIsoDate(jd);
//    }
}

int QJalaliDate::weekNumber(int *yearNumber) const
{
}

QString QJalaliDate::getName() const
{
    return m_name;
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
