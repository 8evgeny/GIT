#pragma once
#include <QStyledItemDelegate>

#define STRING_MAX_LENGTH_KEY 3

class KeyDelegate : public QStyledItemDelegate {
public:
    using inherited = QStyledItemDelegate;

public:
    explicit KeyDelegate(QObject* parent = nullptr);

protected:
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
};
