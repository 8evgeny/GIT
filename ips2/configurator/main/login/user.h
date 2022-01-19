#pragma once
#include <QString>
#include "status_type.h"

class User final {
    explicit User() = default;
    virtual ~User() = default;
    int id() const noexcept {
        return m_id;
    }
    void setId(int id) noexcept {
        m_id = id;
    }
    int gid() const noexcept {
        return m_gid;
    }
    void setGid(int gid) noexcept {
        m_gid = gid;
    }
    int pid() const noexcept {
        return m_pid;
    }
    void setPid(int pid) noexcept {
        m_pid = pid;
    }
    int num() const noexcept {
        return m_num;
    }
    void setNum(int num) noexcept {
        m_num = num;
    }
    QString const& login() const noexcept {
        return m_login;
    }
    void setLogin(QString const& login) noexcept {
        m_login = login;
    }
    QString const& firstName() const noexcept {
        return m_first_name;
    }
    void setFirstName(QString const& first_name) noexcept {
        m_first_name = first_name;
    }
    QString const& lastName() const noexcept {
        return m_last_name;
    }
    void setLastName(QString const& last_name) noexcept {
        m_last_name = last_name;
    }
    QString fullName() const noexcept {
        return IPS_STR("%1 %2").arg(m_first_name, m_last_name);
    }
    QString const& desc() const noexcept {
        return m_desc;
    }
    void setDesc(QString const& desc) noexcept {
        m_desc = desc;
    }
    QString const& email() const noexcept {
        return m_email;
    }
    void setEmail(QString const& email) noexcept {
        m_email = email;
    }
    QString const& expired() const noexcept {
        return m_expired;
    }
    void setExpired(QString const& expired) noexcept {
        m_expired = expired;
    }
    bool isLocked() const noexcept {
        return m_status != StatusType::Enabled;
    }
    void setIsLocked(bool is) noexcept {
        m_status = is
                ? StatusType::Disabled
                : StatusType::Enabled;
    }
    StatusType status() const noexcept {
        return m_status;
    }
    void setStatus(StatusType status) noexcept {
        m_status = status;
    }
    friend bool operator==(class_name const& lhs, class_name const& rhs) noexcept {
        return lhs.m_id == rhs.m_id
                && lhs.m_gid == rhs.m_gid
                && lhs.m_pid == rhs.m_pid
                && lhs.m_num == rhs.m_num
                && lhs.m_login == rhs.m_login
                && lhs.m_first_name == rhs.m_first_name
                && lhs.m_last_name == rhs.m_last_name
                && lhs.m_desc == rhs.m_desc
                && lhs.m_email == rhs.m_email
                && lhs.m_expired == rhs.m_expired
                && lhs.m_status == rhs.m_status
                ;
    }
    friend bool operator!=(class_name const& lhs, class_name const& rhs) noexcept {
        return !(lhs == rhs);
    }

private:
   int        m_id;
   int        m_gid;
   int        m_pid;
   int        m_num;
   QString    m_login;
   QString    m_first_name;
   QString    m_last_name;
   QString    m_desc;
   QString    m_email;
   QString    m_expired;
   StatusType m_status;
};

Q_DECLARE_METATYPE(StatusType)
