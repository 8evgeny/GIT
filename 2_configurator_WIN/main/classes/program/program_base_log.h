#pragma once
#include <QString>
#include <QByteArray>

class ProgramLog {
public:
    using class_name = ProgramLog;

protected:
    virtual void doWrite(QByteArray const& msg) const noexcept = 0;

private:
    QByteArray format(QString const& msg) const noexcept {
        auto&& str = QStringLiteral("[%1] %2:%3 | %4")
            .arg(QDateTime::currentDateTime().toString(QLatin1String("dd.MM.yyyy:hh:mm:ss")),
                 m_file, QString::number(m_line), msg);
        return str.toLatin1();
    }

public:
    explicit ProgramLog(QString const&& file, QString const&& fun, uint line)
        : m_file(std::move(file))
        , m_fun(std::move(fun))
        , m_line(line) {
    }
    explicit ProgramLog(QString&& file, QString&& fun, uint line)
        : m_file(std::move(file))
        , m_fun(std::move(fun))
        , m_line(line) {
    }
    virtual ~ProgramLog() = default;
    QString const& file() const noexcept {
        return m_file;
    }
    void setFile(QString const& file) noexcept {
        m_file = file;
    }
    void setFile(QString&& file) noexcept {
        m_file = std::move(file);
    }
    QString const& fun() const noexcept {
        return m_fun;
    }
    void setFun(QString const& fun) noexcept {
        m_fun = fun;
    }
    void setFun(QString&& fun) noexcept {
        m_fun = std::move(fun);
    }
    uint line() const noexcept {
        return m_line;
    }
    void setLine(uint line) noexcept {
        m_line = line;
    }
    void write(QString const& msg) noexcept {
        doWrite(format(msg));
    }
    void write(QString&& msg) noexcept {        
        doWrite(format(std::move(msg)));
    }

private:
    QString m_file;
    QString m_fun;
    uint    m_line;
};
