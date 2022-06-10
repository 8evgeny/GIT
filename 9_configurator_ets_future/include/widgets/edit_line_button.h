#pragma once
#include <QLineEdit>
#include <QCompleter>
#include <QToolButton>
#include <QHBoxLayout>
#include "dialogs/stringlist/string_list.h"
#include "include/typedefs.h"
#include "include/validators/activate_control_line_validator.h"

/*
 * \code
    auto edit = new gt::widget::EditLineButton(list, ui->m_cid);
 * \endcode
*/

/** \namespace gt::widget */
namespace gt::widget {

class EditLineButton: public QWidget {
    Q_OBJECT

 public:
    using class_name = EditLineButton;
    using inherited  = QWidget;

 public:
    explicit EditLineButton(QStringList const& list = {}, QWidget* parent = nullptr)
        : inherited(parent) {
        instanceWidgets(list);
        instanceConnections(list);
    }
    ~EditLineButton() {
        delete m_line;
        delete m_button;
        delete m_layout;
    }
    void instanceWidgets(QStringList const& list) {
        m_line = new QLineEdit(this);
        m_line->setValidator(new ActivateControlValidator);        
        m_button = new QToolButton(this);
        m_layout = new QHBoxLayout(this);
        m_layout->setMargin(0);
        m_layout->setSpacing(0);
        m_layout->addWidget(m_line);
        m_layout->addWidget(m_button);
        m_button->setText(QStringLiteral("..."));
        m_line->setCompleter(new QCompleter(list, this));
        m_line->completer()->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
        m_button->setFocusProxy(m_line);
        setFocusProxy(m_line);
    }
    void instanceConnections(QStringList const& list) {
        QObject::connect(m_line, &QLineEdit::editingFinished, this, [this] {
            if(m_close)
                emit editingFinished();
        });
        QObject::connect(m_line->completer(), QOverload<const QModelIndex &>::of(&QCompleter::activated), [=] {
            m_save = m_line->text();
            if(m_close) {
                emit editingFinished();
            }
        });
        QObject::connect(m_line, &QLineEdit::textChanged, this, &EditLineButton::textChanged);
        QObject::connect(m_button, &QToolButton::clicked, this, [this, list]() {
            m_close = false;
            StringListDialog dlg(iFunction::tr("CallerId and Groups"), list, m_line->text());
            QRect rect(500, 300, 286, 399);
            dlg.setGeometry(rect);
              if(dlg.exec() == StringListDialog::Accepted) {
                 setText(dlg.current());
                 m_save = dlg.current();
              }

             m_close = true;
        });
    }
    void setText(QString const& text) noexcept {
        if (!text.isEmpty())
            m_line->setText(text);
        m_save = text;
    }
    QString const& text() const noexcept {
        return m_save;
    }

 signals:
    void editingFinished();
    void textChanged();

 private:    
    QString m_save;
    QLineEdit* m_line;
    QToolButton* m_button;
    QHBoxLayout* m_layout;
    bool m_close = true;
};

} // end namespace gt::widget
