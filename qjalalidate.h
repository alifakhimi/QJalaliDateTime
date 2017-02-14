#ifndef QJALALIDATE_H
#define QJALALIDATE_H

#include <QDate>
#include <QStringList>
#include <QList>
#include <QDebug>

struct ParsedDate
{
    int year, month, day;
};

class QJalaliDate
{
public:
    enum MonthNameType {
        DateFormat = 0,
        StandaloneFormat
    };

private:
    explicit QJalaliDate(qint64 jalaliDay) : jd(jalaliDay) {}

public:
    QJalaliDate() : jd(nullJd()) { }
    //QJalaliDate(const QDate &date) : jd(nullJd()) { }
    QJalaliDate(int year, int month, int day);

    static QJalaliDate currentDate();
    static QJalaliDate fromGregorianDay(qint64 jd);
    static QJalaliDate fromGregorian(const QDate &gdate);
    static QJalaliDate fromGregorian(int year, int month, int day);
    static QDate toGregorian(const QJalaliDate &jd);
    static QJalaliDate fromString(const QString &string, Qt::DateFormat format = Qt::TextDate);
    static QJalaliDate fromString(const QString &string, const QString &format);
    static bool isLeapYear(int year);
    static bool isValid(int year, int month, int day);

    int day() const;
    int	month() const;
    int	year() const;

    static ParsedDate getDateFromJalaliDay(qint64 jalaliDay);

    int dayOfWeek() const;
    int dayOfYear() const;
    int daysInMonth() const;
    int daysInYear() const;
    qint64 daysTo(const QJalaliDate &d) const;
    qint64 daysTo(const QDate &d) const;
    void getDate(int *year, int *month, int *day) const;
    bool setDate(int year, int month, int day);
    QString	toString(const QString &format) const;
    QString	toString(Qt::DateFormat format = Qt::TextDate) const;
    int	weekNumber(int *yearNumber = Q_NULLPTR) const;

    bool isNull() const { return !isValid(); }
    bool isValid() const { return jd >= minJd() && jd <= maxJd(); }
    QString getName() const;

    QJalaliDate addDays(qint64 ndays) const;
    QJalaliDate addMonths(int nmonths) const;
    QJalaliDate addYears(int nyears) const;

    bool operator==(const QJalaliDate &other) const { return jd == other.jd; }
    bool operator!=(const QJalaliDate &other) const { return jd != other.jd; }
    bool operator< (const QJalaliDate &other) const { return jd <  other.jd; }
    bool operator<=(const QJalaliDate &other) const { return jd <= other.jd; }
    bool operator> (const QJalaliDate &other) const { return jd >  other.jd; }
    bool operator>=(const QJalaliDate &other) const { return jd >= other.jd; }

private:
    // using extra parentheses around min to avoid expanding it if it is a macro
    static inline qint64 nullJd() { return (std::numeric_limits<qint64>::min)(); }
    static inline qint64 minJd() { return Q_INT64_C(-784350574879); }
    static inline qint64 maxJd() { return Q_INT64_C( 784354017364); }

    static inline qint64 jalaliDayFromDate(int year, int month, int day);

    qint64 jd; // Jalali Date
    QString m_name = "جلالی";
};

static const char gregorianDaysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
static const char jalaliDaysInMonth[] =    { 31, 31, 31, 31, 31, 31, 30, 30, 30, 30, 30, 29 };

static const QString jalaliMonthsName[] =
{
    QString::fromUtf8("فروردین"), QString::fromUtf8("اردیبهشت"), QString::fromUtf8("خرداد"),
    QString::fromUtf8("تیر"), QString::fromUtf8("مرداد"), QString::fromUtf8("شهریور"),
    QString::fromUtf8("مهر"), QString::fromUtf8("آبان"), QString::fromUtf8("آذر"),
    QString::fromUtf8("دی"), QString::fromUtf8("بهمن"), QString::fromUtf8("اسفند")
};

static const QString jalaliDaysName[] =
{
    QString::fromUtf8("شنبه"),
    QString::fromUtf8("یکشنبه"),
    QString::fromUtf8("دوشنبه"),
    QString::fromUtf8("سه شنبه"),
    QString::fromUtf8("چهارشنبه"),
    QString::fromUtf8("پنجشنبه"),
    QString::fromUtf8("آدینه")
};

#endif // QJALALIDATE_H
