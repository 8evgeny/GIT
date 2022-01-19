#include <QLineEdit>
#include "key_delegate.h"

KeyDelegate::KeyDelegate(QObject* parent)
    : inherited(parent)
{
}
QWidget* KeyDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem&, const QModelIndex&) const
{
    QLineEdit* editor = new QLineEdit(parent);
    editor->setMaxLength(STRING_MAX_LENGTH_KEY);
    editor->setValidator(new QIntValidator(0, 1000, editor));
    return editor;
}
