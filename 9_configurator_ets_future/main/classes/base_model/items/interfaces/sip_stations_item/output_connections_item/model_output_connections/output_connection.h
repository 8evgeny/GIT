#pragma once
#include <QString>

class OutputConnection final {
public:
    using class_name = OutputConnection;

public:
    OutputConnection(uint oid = 0, QString const& name = {}, uint number = {})
          : m_oid(oid)
          , m_name(name)
          , m_number(number)
    {
    }
    virtual ~OutputConnection() = default;
    void setOid(uint oid) noexcept {
        m_oid = oid;
    }
    void setName(QString const& name) noexcept {
        m_name = name;
    }
    constexpr void setNumber(uint number) noexcept {
        m_number = number;
    }
    constexpr uint oid() const noexcept {
        return m_oid;
    }
    auto const& name() const noexcept {
        return m_name;
    }
    constexpr uint number() const noexcept {
        return m_number;
    }
    auto const& comment() const noexcept {
        return m_comment;
    }
    void setHash(QString const& hash) noexcept {
        m_hash = hash;
    }
    QString const& toHash() noexcept {
        return m_hash;
    }
    void setComment(QString const& comment) noexcept {
        m_comment = comment;
    }

private:
    uint m_oid = 0;
    QString m_name = {};
    uint m_number = {};
    QString m_hash = {};
    QString m_comment = {};
};
