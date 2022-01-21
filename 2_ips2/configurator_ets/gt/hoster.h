/**
 * \file      surv/projects/cnf/include/hoster.h 
 * \brief     The Hoster class provides 
 * \author    S.Panin <dix75@mail.ru>
 * \copyright S.Panin, 2006 - 2020 
 * \version   v.1.1
 * \created   August    (the) 08(th), 2020, 01:54 MSK
 * \updated   November  (the) 19(th), 2020, 12:56 MSK
 * \TODO      
**/
#pragma once
#include <functional>

/** \namespace gt */
namespace gt {

template<class T>
class Hoster final {
 public:
    using class_name = Hoster;
    using value_t    = T;

 public:
    Hoster(value_t const& val) noexcept
        : m_value(val)
        , m_host(val) {
    }
    void set(value_t const& val) noexcept {
        m_value = val;
        m_host = val;
    }
    value_t const& value() const noexcept {
        return m_value;
    }
    value_t const& host() const noexcept {
        return m_host;
    }
    value_t& host() noexcept {
        return m_host;
    }
    /**
     * \code
         m_server.setMemberData<StatusType>(&Server::setStatus, status);
     * \endcode
     **/
    template<class U>
    void setMemberData(std::function<void(T&, U)> fun, U val) noexcept {
        fun(m_value, val);
        fun(m_host, val);
    }

    bool isEqual() noexcept {
        return m_value == m_host;
    }
    class_name& operator=(value_t const& val) noexcept {
        m_value = val;
        m_host  = val;
        return *this;
    }
    friend bool operator==(class_name const& lhs, class_name const& rhs) noexcept {
        return lhs.m_value == rhs.h_value
            && lhs.m_host == rhs.m_host;
    }
    friend bool operator!=(class_name const& lhs, class_name const& rhs) noexcept {
        return !(lhs == rhs);
    }

 private:
    value_t m_value;
    value_t m_host;
};

}  // end namespace gt
