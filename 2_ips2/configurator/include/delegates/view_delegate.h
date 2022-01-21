#pragma once
#include <QStyledItemDelegate>

#define MAX_VALUE_CID 9999

class ViewDelegate : public QStyledItemDelegate {
public:
    using inherited = QStyledItemDelegate;

public:
    explicit ViewDelegate(uint lenght, QObject* parent = nullptr);

protected:
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;    

private:
    uint m_lenght;
};
