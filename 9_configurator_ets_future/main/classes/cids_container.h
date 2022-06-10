#pragma once
#include <QString>
#include <include/log.h>

class CidsContainer final
{
    using cid_t      = uint;
    using class_name = CidsContainer;

public:
    explicit CidsContainer(QString const& key, uint cid)
        : m_key(key)
        , m_cid(cid)
    {}
    ~CidsContainer() = default;

public:
    void setKey(QString const& key) noexcept {
        m_key = key;
    }
    void setCid(uint cid) noexcept {
        m_cid = cid;
    }
    QString const& key() noexcept {
        return m_key;
    }
    cid_t cid() noexcept {
        return m_cid;
    }

public:
   static inline int findPosCid(QList<class_name*> containers, QString const& key) noexcept
   {
       auto i = 0;
       for (auto container : containers) {
           LOG_FATAL(container, "container can not be nullptr");
           auto keyCont = container->key();
           if (key == keyCont)
               return i;
           ++i;
       }
       return i;
   }

private:
    QString m_key = {};
    cid_t m_cid;
};
