#pragma once
#include <QString>

class LogsItem final {

public:
   using class_name = LogsItem;

 public:
    LogsItem()
        : m_logMessage()
    {
    }

 public:
    void setLogMessage(QString const& logMessage) noexcept {
        m_logMessage = logMessage;
    }
    QString const& logMessage() const noexcept {
        return m_logMessage;
    }  

 private:
    QString m_logMessage;    
};
