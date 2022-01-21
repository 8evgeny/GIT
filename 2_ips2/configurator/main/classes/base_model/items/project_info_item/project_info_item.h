#pragma once
#include "classes/algo.h"
#include "classes/defines.h"
#include "model/model_comments.h"
#include "classes/base_model/items/base_item.h"

IPS_UI(ProjectInfo)

class ProjectInfoItem final: public QWidget, public BaseItem {
    Q_OBJECT
    IPS_DISABLE_COPY(ProjectInfoItem)
    friend class TestProjectInfoItem;

private:
    using inherited = BaseItem;
    using ui_t = Ui::ProjectInfo;

private:
    void instanceData();
    void instanceSiganls();
    void trancateDescription() noexcept;
    void selectedRowDelete() noexcept;

public:
    explicit ProjectInfoItem(BaseItem* item = nullptr)
         : inherited(item) {
    }
    virtual ~ProjectInfoItem() override;
    virtual ProjectInfoItem* show(QWidget* content, QWidget* set = {}) override;
    virtual void deshow() override;
    virtual bool error(BaseItem* = nullptr) const final {
        return true;
    }
    virtual QString name(BaseItem* Item = nullptr) const override;
    virtual QJsonValue serialize() const final;
    virtual bool deserialize(QJsonObject const&) final override;
    virtual bool clear() final;

public slots:
    void setCustomer(const QString& str) {
        m_customer = str;
    }
    void setLocation(const QString& str) {
        m_location = str;
    }
    void setProjectNumber(const QString& str) {
        m_number = str;
    }
    void setSystemID(const QString& str) {
        m_id = str;
    }
    void setDescription(const QString& str) {
        m_desc = str;
    }

public:
    QString const& customer() const {
        return m_customer;
    }
    QString const& location() const {
        return m_location;
    }
    QString const& projectNumber() const {
        return m_number;
    }
    QString const& systemID() const {
        return m_id;
    }
    QString const& description() const {
        return m_desc;
    }
    void changeEvent(QEvent *event) override final;

public:
    project_info::ModelComments const& table() const {
        return m_model;
    }

private:
    static constexpr uint m_maxDescription = 256;

private:
    QString m_customer;
    QString m_location;
    QString m_number;
    QString m_id;
    QString m_desc;
    int m_currentRow = 0;
    ui_t* ui = nullptr;
    project_info::ModelComments m_model;
};
