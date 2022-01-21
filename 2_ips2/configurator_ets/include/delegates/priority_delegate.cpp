#include <QLineEdit>
#include "priority_delegate.h"

PriorityDelegate::PriorityDelegate(QObject* parent)
    : inherited(parent)
{
}
QWidget* PriorityDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem&, const QModelIndex&) const
{
    QLineEdit* editor = new QLineEdit(parent);
    editor->setMaxLength(STRING_MAX_LENGTH_PRIORITY);
    editor->setValidator(new QIntValidator(0, 100, editor));
    return editor;
}
