#pragma once
#include <QStyledItemDelegate>

class IpDelegate : public QStyledItemDelegate {
public:
    using inherited = QStyledItemDelegate;

public:
    explicit IpDelegate(QObject* parent = nullptr);

protected:
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
};
