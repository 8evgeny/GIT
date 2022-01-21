#pragma once
#include <QFile>
#include "main/classes/oid.h"
#include "main/classes/cid.h"
#include "main/classes/gid.h"
#include "main/classes/gr.h"
#include "main/classes/base_model/items/base_item.h"
#include "main/classes/base_model/items/station_item/model_keys/keys.h"
#include "main/messages/model_tracks/tracks_model.h"
#include "main/messages/dialogs/messages/messages_dialog.h"

class RootItem;
class StationsItem;
class StationItem;
class ProjectInfoItem;
class GlobalParametersItem;
class GroupsItem;
class SipStationsItem;
class InterfacesItem;
class InputConnectionsItem;
class OutputConnectionsItem;

class BaseModel final: public QAbstractItemModel {
    Q_OBJECT
    IPS_DISABLE_COPY(BaseModel)

public:
    using inherited  = QAbstractItemModel;
    using class_name = BaseModel;
    using ip_list_t  = QList<QString>;

public:
    enum {
        ProjectInfoItemId = 0
        , GlobalParametersItemId = 1
        , StationsItemId = 2
        , GroupsItemId = 3
        , IntrfacesItemId = 4        
    };
    enum childsId {
         SipsItemId = 0
    };
    enum grandChildsId {
         InputConnectionsItemId = 0
        , OutputConnectionsItemId = 1
    };

private:
    static constexpr uint ProjectInfoItemIndex = 0;
    static constexpr uint GlobalParametersItemIndex = 1;
    static constexpr uint StationsItemIndex = 2;    
    static constexpr uint GroupsItemIndex = 3;
    static constexpr uint IntrfacesItemIndex = 4;    

private:
    BaseItem* toItem(const QModelIndex& index) const noexcept {
        return static_cast<BaseItem*>(index.internalPointer());
    }
    BaseItem* getRoot(const QModelIndex& index) const noexcept {
        return index.isValid()
            ? toItem(index)
            : m_root;
    }
    template<class T, class F>
    T getListCids(F f) noexcept;
    template<class T, class F>
    T getListGids(F f) noexcept;
    template<class T, class F>
    T getListOids(F f) noexcept;
    template<class T>
    T getListKeys(StationItem* station) noexcept;

public:
    explicit BaseModel(QObject* parent = nullptr);
    ~BaseModel() override;
    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = {}) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    bool removeRows(int position, int rows, const QModelIndex& index) final;
    int rowCount(const QModelIndex &parent = {}) const override;
    int columnCount(const QModelIndex &parent = {}) const override;

public:
    QVariantMap dataSerialize() const noexcept;
    QJsonDocument document() const noexcept;
    QByteArray compressData(QJsonDocument const& file) const noexcept;
    QByteArray toHash() noexcept;
    bool insertItem(int position, BaseItem* item, const QModelIndex& parent = {}) noexcept;
    void refreshName(const QModelIndex& index, const QVector<int>& roles = {Qt::EditRole, Qt::DisplayRole}) noexcept;
    RootItem* root() const noexcept;
    ProjectInfoItem* projectInfoItem() const noexcept;
    GlobalParametersItem* globalParametersItem() const noexcept;
    StationsItem* stationsItem() const noexcept;
    GroupsItem* groupItem() const noexcept;
    InterfacesItem* interfacesItem() const noexcept;
    SipStationsItem* sipStationsItem() const noexcept;
    InputConnectionsItem* inputConnectionsItem() const noexcept;
    OutputConnectionsItem* outputConnectionsItem() const noexcept;
    bool itemClick(const QModelIndex& index, QWidget* content, QWidget* settings);
    void selectProjectInfo(QWidget* content, QWidget* settings);
    bool save(QString const& file) const;
    bool load(QString const& file);

public:
    void setContent(QWidget* content) noexcept {
        m_content = content;
    }
    void setSettings(QWidget* settings) noexcept {
        m_settings = settings;
    }
    QWidget* settings() noexcept {
        return m_settings;
    }
    QWidget* content() noexcept {
        return m_content;
    }
    key_list_t keys(StationItem* station) noexcept;
    cid_list_t cids() noexcept;
    oid_list_t oids() noexcept;
    gid_list_t gids() noexcept;
    gr_list_t grs() noexcept;
    ip_list_t ips() noexcept;
    ip_list_t netmasks() noexcept;
    ip_list_t gateways() noexcept;

public:
    void setTree(QTreeView* tree) noexcept {
        m_tree = tree;
    }
    void setCurrent(BaseItem* current = nullptr) noexcept {
        m_current = current;
    }
    void setPretones(QStringList const& pretones) noexcept {
        m_pretones = pretones;
    }
    QTreeView* tree() const noexcept {
        return m_tree;
    }
    QFile* tracks() noexcept;
    QStringList pretones() noexcept {
        return m_pretones;
    }

private:
    QTreeView* m_tree = nullptr;
    QWidget* m_content = nullptr;
    QWidget* m_settings = nullptr;
    BaseItem* m_root = nullptr;
    BaseItem* m_current = nullptr;
    QStringList m_pretones = {};
};
