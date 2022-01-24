#pragma once
#include <QString>
#include "include/typedefs.h"
#include "main/classes/cid.h"

class DigitalStation {
public:
    using class_name = DigitalStation;

public:
    DigitalStation(QString const& name = IPS_STR("DigitalStation"),
            cid_t cid = {},
            uint const& id = {})
            : m_name(name)
            , m_cid(cid)
            , m_id(id)
    {
    }
    virtual ~DigitalStation() Q_DECL_EQ_DEFAULT;
    static class_name* defValue(QString const& name, cid_t cid, uint id) noexcept {        
        return new DigitalStation(name, cid, id);
    }
    void setNameDigitalStation(QString const& name) {
        m_name = name;
    }
    void setCid(cid_t cid) noexcept {
        m_cid = cid;
    }
    cid_t cid() const noexcept {
        return m_cid;
    }
    void setId(uint const& id) {
        m_id = id;
    }
    QString const& nameDigitalStation() const {
        return m_name;
    }    
    uint const& id() const {
        return m_id;
    }

private:
    QString m_name;
    cid_t m_cid;
    uint m_id;
};
