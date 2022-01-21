#pragma once
#include <QtCore/qglobal.h>
#include "include/log.h"

class BaseItem {
    IPS_DISABLE_COPY(BaseItem)

public:
    using class_name = BaseItem;
    using items_t    = QList<BaseItem*>;
    using value_t    = typename items_t::value_type;
    using iterator_t = typename items_t::iterator;
    using const_iterator_t = typename items_t::const_iterator;

public:
    explicit BaseItem(BaseItem* item = nullptr)
         : m_items()
         , m_parent(item) {
    }
    virtual ~BaseItem() {
         deleteAllChilds();
    }

public:
    virtual BaseItem* show(QWidget* content, QWidget* settings = {}) = 0;
    virtual void deshow() = 0;
    virtual bool error(BaseItem* Item = nullptr) const = 0;
    virtual QString name(BaseItem* Item = nullptr) const = 0;
    virtual QJsonValue serialize() const = 0;
    virtual bool deserialize(QJsonObject const&) = 0;    
    virtual bool clear() = 0;

public:
    void appendChild(BaseItem* child) noexcept {
        m_items.append(child);
    }
    void removeChild(int position) {
        LOG_DEBUG(false, "Position is %1(%2)", position, m_items.size());
        LOG_FATAL(m_items.size(), "size cannot be 0");
        delete m_items[position];
        m_items[position] = nullptr;
        m_items.removeAt(position);
    }
    void deleteAllChilds() {
        qDeleteAll(m_items);
    }
    void insertChild(int row, BaseItem* child) noexcept {
        m_items.insert(row, child);
    }
    BaseItem* child(int row) const noexcept {
        return (0 <= row && row < childCount())
                ? m_items.value(row)
                : nullptr;
    }
    int childCount() const noexcept {
         return m_items.count();
    }
    int row() const {
        return m_parent
             ? m_parent->m_items.indexOf(const_cast<BaseItem*>(this))
             : 0;
    }
    void setBaseModelIndex(QModelIndex const& index) noexcept {
         m_index = index;
    }
    QModelIndex baseModelIndex() const noexcept {
         return m_index;
    }
    BaseItem* parent() const noexcept {
        return m_parent;
    }
    void setParent(BaseItem* item) noexcept {
        m_parent = item;
    }
    items_t const& items() const noexcept {
        return m_items;
    }
    items_t& items() noexcept {
        return m_items;
    }
    inline iterator_t begin() {
        return m_items.begin();
    }
    inline iterator_t end() {
        return m_items.end();
    }
    inline const_iterator_t cbegin() const {
        return m_items.begin();
    }
    inline const_iterator_t cend() const {
        return m_items.end();
    }

private:
    QModelIndex m_index;
    QList<BaseItem*> m_items;
    BaseItem* m_parent = nullptr;
};
