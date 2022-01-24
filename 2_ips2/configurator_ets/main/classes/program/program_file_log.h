#pragma once
#include "main/classes/literals.h"
#include "main/classes/program/program_base_log.h"

class ProgramFileLog Q_DECL_FINAL: public ProgramLog {
public:
    using class_name = ProgramFileLog;
    using inherited  = ProgramLog;

private:
    QString m_log_file;
    quint64 m_max_file_size = 100_Mb;

protected:
    void update() const Q_DECL_NOEXCEPT {
        if(m_max_file_size == 0) {
            return;
        }
        QFileInfo info(m_log_file);
        if(quint64(info.size()) > m_max_file_size) {QFile::remove(m_log_file);}
    }
    void doWrite(QByteArray const& msg) const Q_DECL_NOEXCEPT Q_DECL_OVERRIDE {        
        QFile file(m_log_file);
        if(!file.open(QFileDevice::Append)) {
            return;
        }
        file.write(msg);
        file.write("\n");
    }

public:
    explicit ProgramFileLog(QString const&& log_file, QString const& fun, QString const&& file, uint line)
        : inherited(qMove(file), qMove(fun), line)
        , m_log_file(qMove(log_file)) {
        update();
    }
    explicit ProgramFileLog(QString const&& log_file, const char* file, const char* fun, uint line)
        : ProgramFileLog(qMove(log_file), QString::fromLatin1(file), QString::fromLatin1(fun), line) {
        update();
    }
    void setMaxFileSize(quint64 size_in_bytes = 100_Mb) Q_DECL_NOEXCEPT {
        m_max_file_size = size_in_bytes;
    }
    QString const& logFile() const Q_DECL_NOEXCEPT {
        return m_log_file;
    }
    void setLogFile(QString const& log_file) Q_DECL_NOEXCEPT {
        m_log_file = log_file;
    }
    void setLogFile(QString&& log_file) Q_DECL_NOEXCEPT {
        m_log_file = qMove(log_file);
    }
};
