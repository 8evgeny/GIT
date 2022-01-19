#include <QLineEdit>
#include "info_delegate.h"

NameDelegate::NameDelegate(uint lenght, QObject* parent)
    : inherited(parent)
    , m_lenght(lenght)
{
}
QWidget* NameDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem&, const QModelIndex&) const
{
    QLineEdit* editor = new QLineEdit(parent);
    editor->setMaxLength(static_cast<int>(m_lenght));
    return editor;
}
