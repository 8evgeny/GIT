#include <QLineEdit>
#include "ip_delegate.h"
#include "include/validators/ip_validator.h"

IpDelegate::IpDelegate(QObject* parent)
    : inherited(parent)
{
}
QWidget* IpDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem&, const QModelIndex&) const
{
    QLineEdit* editor = new QLineEdit(parent);
    editor->setValidator(new IpValidator(editor));
    return editor;
}
