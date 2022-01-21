#pragma once
#include <QDialog>
#include <QPushButton>
#include <QDialogButtonBox>
#include "dialog.h"
#include <ps2/qt/convert/integer.h>
#include <ps2/qt/widgets/algorithm.h>

/** \namespace gt::dialog */
namespace gt::dialog {

class AEDialog: public QDialog, protected widget::Dialog {
    Q_DISABLE_COPY(AEDialog)

 public:
    using class_name = AEDialog;
    using inherited  = QDialog;
    using dialog_t   = widget::Dialog;
    using button_t   = QDialogButtonBox::StandardButton;

 protected:
    virtual bool doIsChanged() const = 0;
    virtual bool doIsValid() const = 0;
    virtual void doDataChanged(QDialogButtonBox* buttons) noexcept {
        ps2::windowTitle(this, doIsChanged());
        buttons->button(button_t::Ok)->setEnabled(doIsValid());
    }
    virtual void doDisable(QDialogButtonBox* buttons) noexcept {
        buttons->button(button_t::Ok)->setEnabled(false);
        ps2::windowTitle(this, false);
    }

 public:
    AEDialog(QWidget* parent = nullptr) noexcept
        : QDialog(parent) {
    }
    virtual ~AEDialog() = Q_DECL_EQ_DEFAULT;
};

} // end namespace gt::dialog
