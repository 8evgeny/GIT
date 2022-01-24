#pragma once
#include "model_stations/model_stations.h"
#include "stations_item_settings/stations_item_settings.h"
#include "main/messages/dialogs/messages/messages_dialog.h"
#include "main/messages/model_tracks/tracks_model.h"

IPS_UI(StationsItem)

class StationsItem final : public QWidget, public BaseItem {
    Q_OBJECT
    IPS_DISABLE_COPY(StationsItem)

    friend class TestStationItems;
    friend class TestValidator;

public:
     using class_name = StationsItem;
     using ui_t       = Ui::StationsItem;
     using inherited  = BaseItem;

public:
    signals:
    void changedPretones(QStringList const& pretones);

public:
     void createMenu();
     void selectedRowDelete() noexcept;
     void fillTypes(QMenu* menu = nullptr, QMenu* sub = nullptr);

private:     
     void installSignals();         
     void changeEvent(QEvent* event) override;
     void instanceConnectionToSettings();
     bool deserializeTracks(QJsonObject const& obj) noexcept;

private:
     template<typename T>
     void clearList(QList<T>& list) noexcept;

private slots:
     void stationsSelectionChanged(const QItemSelection&, const QItemSelection&);

public:
     explicit StationsItem(BaseItem* item = nullptr);
     virtual ~StationsItem() override;

public:
     virtual StationsItem* show(QWidget* content, QWidget* settings = {}) override;
     virtual void deshow() override;
     virtual bool error(BaseItem* Item = nullptr) const override;
     virtual QString name(BaseItem* Item = nullptr) const override;
     virtual QJsonValue serialize() const override;
     virtual bool deserialize(QJsonObject const& obj) final override;
     virtual bool clear() final;

public:
     ModelStations* model() {
         return &m_model;
     }
     void setModelTones(TracksModel* modelTones) {
         m_modelTones = modelTones;
     }
     int currentRow() noexcept {
         return m_currentRow;
     }
     QStringList const& pretones() {
         return m_pretones;
     }

private:     
     ui_t* ui = nullptr;
     int m_currentRow = 0;
     QModelIndex m_index = {};
     ModelStations m_model;
     TracksModel* m_modelTones = nullptr;     
     StationsItemSettings m_settings;
     QStringList m_pretones = {};
     int m_countTracks = 4;
};
