#pragma once
#include <QDialog>
#include "main/classes/algo.h"
#include "include/typedefs.h"
#include <QCheckBox>
#include <QModelIndex>

IPS_UI(MessagesDialog)

class TracksModel;
class TracksFilterModel;
class MessDlgTest;

class MessagesDialog final: public QDialog {
      Q_OBJECT
      Q_DISABLE_COPY(MessagesDialog)

 friend class MessDlgTest;

 private:
    using class_name = MessagesDialog;
    using inherited  = QDialog;
    using ui_t       = Ui::MessagesDialog;

 private:
    void initSignals() noexcept;
    void createCheckBoxes() noexcept;
    bool isCheckedTracks() noexcept;

 public:
    explicit MessagesDialog(QWidget* parent = nullptr);
    ~MessagesDialog();

 protected slots:
    void onFilterChanged(QString const& mess) noexcept;
    void onRemoveClicked() noexcept;
    void onLoadClicked() noexcept;
    void onCloseClicked() noexcept;

 public:
    TracksModel* model() noexcept {
        return m_model;
    }

 private:
    QList<int> m_pos = {};
    QModelIndex m_index = {};
    QCheckBox* m_check = nullptr;
    ui_t* m_ui = nullptr;
    TracksModel* m_model = nullptr;
    TracksFilterModel* m_fmodel = nullptr;
};
