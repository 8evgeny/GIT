/**
 * \file      onan/conan-liblog/sources/log.h 
 * \brief     The Log class provides 
 * \author    S.Panin <dix75@mail.ru>
 * \copyright S.Panin, 2006 - 2021 
 * \version   v.1.0
 * \created   February  (the) 17(th), 2021, 16:02 MSK
 * \updated   February  (the) 17(th), 2021, 16:02 MSK
 * \TODO      
**/
#pragma once
#include <QUrl>
#include <QFile>
#include <QDebug>
#include <QString>
#include <QFileInfo>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonParseError>
#include <gt/literals.h>

/*
 * \code
        return LOG_ERROR(false, "%1 is not found or invalid. Object is %2", name, ser(data));
 * \endcode
*/

class Log final {
public:
    using class_name = Log;

 private:
    QByteArray format(QString const& msg) const noexcept {
        auto const& date = QDateTime::currentDateTime().toString(QLatin1String("dd.MM.yyyy:hh:mm:ss"));
        auto const& str = QStringLiteral("[%1] %2:%3 | %4")
            .arg(date)
                .arg(QString::fromStdString(m_file))
                .arg(m_line)
                .arg(msg);
        return str.toLatin1();
    }
    void write(QString const& msg) const noexcept {
        QFile file(m_log_file);
        if(!file.open(QFileDevice::Append))
            return;

        if(quint64(file.size()) > 100'000'000) {
            QFile::copy(m_log_file, QStringLiteral("%1~0").arg(m_log_file));
            QFile::remove(m_log_file);
        }

        file.write(format(msg));
        file.write("\n");
    }

 private:
    template<class T>
    static inline QString toStr(T const& val) noexcept {
        QString s;
        QDebug(&s) << static_cast<int>(val);
        return s;
    }
    static inline QString toStr(int val) noexcept {
        return QString::number(val);
    }
    static inline QString toStr(std::string const& val) noexcept {
        return QString::fromStdString(val);
    }
    static inline QString toStr(const char* const& val) noexcept {
        return QString::fromLocal8Bit(val);
    }
    static inline QString toStr(QUrl const& val) noexcept {
        return val.toString();
    }
    static inline QString toStr(QJsonValue const& val) noexcept {
        QString s;
        QDebug(&s) << val;
        return s;
    }
    static inline QString toStr(QJsonObject const& val) noexcept {
        QString s;
        QDebug(&s) << val;
        return s;
    }
    static inline QString toStr(QByteArray const& val) noexcept {
        return QString::fromUtf8(val);
    }
    static inline QString toStr(QFile::FileError err) noexcept {
        QString s;
        if(QFile::NoError == err) {
            s = QLatin1String("NoError");
        }
        else if(QFile::ReadError == err) {
            s = QLatin1String("ReadError");
        }
        else if(QFile::WriteError == err) {
            s = QLatin1String("WriteError");
        }
        else if(QFile::FatalError == err) {
            s = QLatin1String("FatalError");
        }
        else if(QFile::ResourceError == err) {
            s = QLatin1String("ResourceError");
        }
        else if(QFile::OpenError == err) {
            s = QLatin1String("OpenError");
        }
        else if(QFile::AbortError == err) {
            s = QLatin1String("AbortError");
        }
        else if(QFile::TimeOutError == err) {
            s = QLatin1String("TimeOutError");
        }
        else if(QFile::UnspecifiedError == err) {
            s = QLatin1String("UnspecifiedError");
        }
        else if(QFile::RemoveError == err) {
            s = QLatin1String("RemoveError");
        }
        else if(QFile::RenameError == err) {
            s = QLatin1String("RenameError");
        }
        else if(QFile::PositionError == err) {
            s = QLatin1String("PositionError");
        }
        else if(QFile::ResizeError == err) {
            s = QLatin1String("ResizeError");
        }
        else if(QFile::PermissionsError == err) {
            s = QLatin1String("PermissionsError");
        }
        else if(QFile::CopyError == err) {
            s = QLatin1String("CopyError");
        }
        else {
            s = QLatin1String("UnknownError FileError");
        }
        return QStringLiteral("%1[%2]").arg(s, QString::number(static_cast<int>(err)));
    }
    static inline QString toStr(QJsonParseError const& err) noexcept {
        QString s;
        if(QJsonParseError::NoError == err.error) {
            s = QLatin1String("NoError");
        }
        else if(QJsonParseError::UnterminatedObject == err.error) {
            s = QLatin1String("UnterminatedObject");
        }
        else if(QJsonParseError::MissingNameSeparator == err.error) {
            s = QLatin1String("MissingNameSeparator");
        }
        else if(QJsonParseError::UnterminatedArray == err.error) {
            s = QLatin1String("UnterminatedArray");
        }
        else if(QJsonParseError::MissingValueSeparator == err.error) {
            s = QLatin1String("MissingValueSeparator");
        }
        else if(QJsonParseError::IllegalValue == err.error) {
            s = QLatin1String("IllegalValue");
        }
        else if(QJsonParseError::TerminationByNumber == err.error) {
            s = QLatin1String("TerminationByNumber");
        }
        else if(QJsonParseError::IllegalNumber == err.error) {
            s = QLatin1String("IllegalNumber");
        }
        else if(QJsonParseError::IllegalEscapeSequence == err.error) {
            s = QLatin1String("IllegalEscapeSequence");
        }
        else if(QJsonParseError::IllegalUTF8String == err.error) {
            s = QLatin1String("IllegalUTF8String");
        }
        else if(QJsonParseError::UnterminatedString == err.error) {
            s = QLatin1String("UnterminatedString");
        }
        else if(QJsonParseError::MissingObject == err.error) {
            s = QLatin1String("MissingObject");
        }
        else if(QJsonParseError::DeepNesting == err.error) {
            s = QLatin1String("DeepNesting");
        }
        else if(QJsonParseError::DocumentTooLarge == err.error) {
            s = QLatin1String("DocumentTooLarge");
        }
        else if(QJsonParseError::GarbageAtEnd == err.error) {
            s = QLatin1String("GarbageAtEnd");
        }
        else {
            s = QLatin1String("UnknownError JsonParseError");
        }
        return QStringLiteral("%1[%2:%3]").arg(err.errorString(), QString::number(static_cast<int>(err.error)), s);
    }
    static inline QString toStr(QString const& str) noexcept {
        return str;
    }
    QString typedStr() const noexcept {
        QString s;
        if(m_type == 1)
            s = QLatin1String("DEBUG: ");
        else if(m_type == 2)
            s = QLatin1String("INFO: ");
        else if(m_type == 3)
            s = QLatin1String("WARNING: ");
        else if(m_type == 4)
            s = QLatin1String("ERROR: ");
        else if(m_type == 5)
            s = QLatin1String("FATAL: ");
        else if(m_type == 10)
            s = QLatin1String("REQUEST ");
        else if(m_type == 11)
            s = QLatin1String("RESPONSE: ");
        return s;
    }
    inline void print(QString const& s) const noexcept {
        write(s);
#ifdef SURV_DEBUG
        if(m_type <= 4) {
            auto str = QString(QStringLiteral("(%1)%2,%3")).arg(m_line).arg(s).arg(QString::fromStdString(m_file));
            qDebug() << m_fun.c_str() << str;
        }
        else if(m_type == 5)
            qt_assert_x(m_fun.c_str(), s.toLocal8Bit().constData(), m_file.c_str(),m_line);
        if(m_type == 10 || m_type == 11)
            qDebug() << s;
#endif
    }

public:
    Log(QString const& log_file, uint type, std::string&& file, std::string&& fun, int line) noexcept
    : m_log_file(log_file)
    , m_type(type)
    , m_file(std::move(file))
    , m_fun(fun)
    , m_line(line) {
    }
    bool operator()(bool cond, std::string&& s) noexcept {
        if(!cond) {
            print(QString::fromStdString(s));
        }
        return cond;
    }
    template<class T>
    bool operator()(bool cond, std::string&& text, T&& val) noexcept {
        if(cond) {
            return true;
        }

        auto&& args = QString::fromStdString(text).arg(toStr(val));
        auto&& s = QStringLiteral("%1%2").arg(typedStr(), std::move(args));
        print(std::move(s));
        return false;
    }
    template<class T1, class T2>
    bool operator()(bool cond, std::string&& text, T1&& val1, T2&& val2) noexcept {
        if(cond) {
            return true;
        }
        auto&& args = QString::fromStdString(std::move(text))
                .arg(toStr(std::forward<T1>(val1))
                   , toStr(std::forward<T2>(val2)));

        auto&& s = QStringLiteral("%1%2").arg(typedStr(), std::move(args));
        print(std::move(s));
        return false;
    }
    template<class T1, class T2, class T3>
    bool operator()(bool cond, std::string&& text, T1&& val1, T2&& val2, T3&& val3) noexcept {
        if(cond) {
            return true;
        }
        auto&& args = QString::fromStdString(std::move(text))
                .arg(toStr(std::forward<T1>(val1))
                   , toStr(std::forward<T2>(val2))
                   , toStr(std::forward<T3>(val3)));

        auto&& s = QStringLiteral("%1%2").arg(typedStr(), std::move(args));
        print(std::move(s));
        return false;
    }

 private:
    QString m_log_file;
    uint m_type = 0U;
    std::string m_file;
    std::string m_fun;
    int m_line = 0;
};

//#define LOG_REGUEST Log(program::logFile(), 10, __FILE__, __FUNCTION__, __LINE__)
//#define LOG_RESPONSE Log(program::logFile(), 11, __FILE__, __FUNCTION__, __LINE__)
//#define LOG_DEBUG Log(program::logFile(), 1, __FILE__, __FUNCTION__, __LINE__)
//#define LOG_INFO Log(program::logFile(), 2, __FILE__, __FUNCTION__, __LINE__)
//#define LOG_WARNING Log(program::logFile(), 3, __FILE__, __FUNCTION__, __LINE__)
//#define LOG_ERROR Log(program::logFile(), 4, __FILE__, __FUNCTION__, __LINE__)
//#define LOG_FATAL Log(program::logFile(), 5, __FILE__, __FUNCTION__, __LINE__)

