#pragma once
#include <QMouseEvent>
#include <QApplication>
#include <QItemDelegate>
#include <QCheckBox>
#include <QDebug>
#include <QLineEdit>
#include <QStyleOptionViewItem>
#include <QStyledItemDelegate>
#include "messages/model_tracks/tracks_model.h"
#include "messages/dialogs/messages/messages_dialog.h"

class CheckBoxDelegate : public QStyledItemDelegate {

 public:
    using class_name = CheckBoxDelegate;
    using inherited  = QStyledItemDelegate;

 private:
    signals:
    void changedPosition(bool is);

 public:
    explicit CheckBoxDelegate(QObject* parent = nullptr)
        : QStyledItemDelegate(parent)
    {        
    }
    ~CheckBoxDelegate() = default;
   virtual QWidget* createEditor(QWidget* parent, QStyleOptionViewItem const&, QModelIndex const&) const override {
        QCheckBox *editor = new QCheckBox(parent);
        return editor;
    }
};
