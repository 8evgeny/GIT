#pragma once
#include <QString>
#include <QLocale>
#include <QDateTime>
#include "include/typedefs.h"
#include "include/convert/string.h"

class Comment final {
public:
    using class_name = Comment;

public:
    Comment(QDateTime const& date = QDateTime::currentDateTime(),
            QString const& author = {},
            QString const& comment = {})
            : m_date(date)
            , m_author(author)
            , m_comment(comment)
    {       
    }
    void setDateTime(QDateTime const& date) noexcept {
        m_date = date;
    }
    auto const& dateTime() const noexcept {
        return m_date;
    }
    void setAuthor(QString const& author) noexcept {
        m_author = author;
    }
    auto const& author() const noexcept {
        return m_author;
    }
    void setComment(QString const& comment) noexcept {
        m_comment = comment;
    }
    auto const& comment() const noexcept {
        return m_comment;
    }
    friend bool operator==(const class_name& lhs, const class_name& rhs) noexcept {
       return lhs.m_date == rhs.m_date &&
              lhs.m_author == rhs.m_author &&
              lhs.m_comment == rhs.m_comment;
    }
    friend bool operator!=(const class_name& lhs, const class_name& rhs) noexcept {
        return !(lhs == rhs);
    }

private:
    QDateTime m_date;
    QString m_author;
    QString m_comment;
};
