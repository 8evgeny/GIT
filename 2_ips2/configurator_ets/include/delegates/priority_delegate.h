#pragma once
#include <QStyledItemDelegate>

#define STRING_MAX_LENGTH_PRIORITY 2

class PriorityDelegate : public QStyledItemDelegate {
public:
    using inherited = QStyledItemDelegate;

public:
    explicit PriorityDelegate(QObject* parent = nullptr);

protected:
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
};
