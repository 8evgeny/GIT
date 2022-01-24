#pragma once
#include <QStyledItemDelegate>

class NameDelegate : public QStyledItemDelegate {
public:
    using inherited = QStyledItemDelegate;

public:
    explicit NameDelegate(uint lenght, QObject* parent = nullptr);

protected:
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;

private:
    uint m_lenght;
};
