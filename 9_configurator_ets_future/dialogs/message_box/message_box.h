#pragma once
#include "message_box_pch.h"
#include "message_box_global.h"

IPS_UI(IpsMessageBox)

/**
 * \code
    IpsMessageBox::info(this, iMessage::tr("File '%11 has been saved sucsessfully.").arg(file));
    IpsMessageBox::error(this, iMessage::tr("An error occurrated while saving the '%1' file!").arg(file));
 *
 * \endcode
**/
class MESSAGEBOX_SHARED_EXPORT IpsMessageBox: public QDialog {
    IPS_DISABLE_COPY(IpsMessageBox)

 private:
    explicit IpsMessageBox(QWidget* parent, QPixmap const& icon, QString const& text);

 public:
    virtual ~IpsMessageBox() override;
    static void info(QWidget* parent, QString const& text) noexcept;
    static void error(QWidget* parent, QString const& text) noexcept;

 private:
    Ui::IpsMessageBox* ui = nullptr;
    QTimer* m_timer = nullptr;
};
