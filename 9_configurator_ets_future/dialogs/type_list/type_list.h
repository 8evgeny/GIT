#pragma once
#include <QStringListModel>
#include "include/typedefs.h"
#include "type_list_pch.h"
#include "type_list_model.h"
#include "type_list_global.h"

IPS_UI(TypeListDialog)

template <class T>
class TYPELIST_DIALOG_SHARED_EXPORT TypeListDialog final: public QDialog {
    IPS_DISABLE_COPY(TypeListDialog)

public:
    using class_name = TypeListDialog;
    using inherited  = QDialog;
    using items_t    = T;
    using model_t    = TypeListModel<items_t>;
    using value_t    = typename items_t::value_type;

private:
    void selectRow(value_t const& current);

public:
    /**
         \code
              TypeListDialog dlg(iFunction::tr("Functins"), QStringList({IPS_STR("1"), IPS_STR("2"), IPS_STR("3")}), IPS_STR("2"));
              if(dlg.exec() == TypeListDialog::Accepted)
                  qDebug() << dlg.current();
          \endcode
      **/

    explicit TypeListDialog(QString const& caption, items_t const& items, value_t const& current, QWidget* parent = nullptr);
    ~TypeListDialog() override;

public:
    items_t items() const noexcept {
        return m_model.items();
    }
    value_t current() const noexcept;
    QVariant currentVariant() const noexcept;

private:    
    Ui::TypeListDialog* ui = nullptr;
    model_t m_model;
};
