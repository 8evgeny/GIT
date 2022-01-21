#pragma once
#include "include/typedefs.h"
#include "activate_control_lines_pch.h"
#include "activate_control_lines_global.h"
#include "include/widgets/edit_line_button.h"
#include "include/validators/activate_control_line_validator.h"

IPS_UI(ActivateControlLinesDialog)

class ACTIVATE_CONTROL_LINES_DIALOG_SHARED_EXPORT ActivateControlLinesDialog: public QDialog {
    Q_OBJECT
    IPS_DISABLE_COPY(ActivateControlLinesDialog)

 public:
    using class_name = ActivateControlLinesDialog;
    using inherited = QDialog;

public:
    explicit ActivateControlLinesDialog(QStringList const& list, QWidget* parent = nullptr);
    ~ActivateControlLinesDialog() override;

public:
    QString cid() const noexcept;
    QString value() const noexcept;

private:    
    Ui::ActivateControlLinesDialog* ui = nullptr;
    gt::widget::EditLineButton* m_edit = nullptr;
};
