#include <QLineEdit>
#include "view_delegate.h"

ViewDelegate::ViewDelegate(uint lenght, QObject* parent)
    : inherited(parent)
    , m_lenght(lenght)
{
}
QWidget* ViewDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem&, const QModelIndex&) const
{
    QLineEdit* editor = new QLineEdit(parent);
    editor->setMaxLength(static_cast<int>(m_lenght));
    editor->setValidator(new QIntValidator(0, MAX_VALUE_CID, editor));
    return editor;
}
