#pragma once
#include <QStringListModel>
#include "include/main_pch.h"
#include "include/typedefs.h"
#include "string_list_pch.h"
#include "string_list_global.h"

IPS_UI(StringListDialog)

class STRINGLIST_DIALOG_SHARED_EXPORT StringListDialog final: public QDialog {
    IPS_DISABLE_COPY(StringListDialog)

public:
    using class_name = StringListDialog;
    using inherited  = QDialog;
    using model_t    = QStringListModel;
    using list_t     = QStringList;

private:
    void selectRow(QString const& current);

public:
    /**
         \code
              StringListDialog dlg(iFunction::tr("Functins"), QStringList({IPS_STR("1"), IPS_STR("2"), IPS_STR("3")}), IPS_STR("2"));
              if(dlg.exec() == StringListDialog::Accepted)
                  qDebug() << dlg.current();
          \endcode
      **/

    StringListDialog() = default;
    explicit StringListDialog(QString const& caption, list_t const& list, QString const& current = {}, QWidget* parent = nullptr);
    ~StringListDialog() override;

public:
    list_t stringList() const noexcept {
        return m_model.stringList();
    }
    QString current() noexcept;

private:    
    Ui::StringListDialog* ui = nullptr;
    model_t m_model;
};
