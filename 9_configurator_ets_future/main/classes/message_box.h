#pragma once
#include "program.h"
#include "include/main_pch.h"

/**
 * \code
 *     #include <main/message_box.h>
 *     MessageBox::info(this, iInfo::tr("Please, in order that changes become valid. \nReboot the application."));
 *     return MessageBox::info(true, this, iInfo::tr("Please, in order that changes become valid. \nReboot the application."));
 * \endcode
**/
class MessageBoxX {
public:
    typedef MessageBoxX class_name;

public:
    inline static QMessageBox::StandardButton info(QWidget* parent, QString const& text, QMessageBox::StandardButtons buttons = QMessageBox::Ok, QMessageBox::StandardButton defaultButton = QMessageBox::NoButton) {
        return QMessageBox::information(parent, program::fullName(), text, buttons, defaultButton);
    }
    inline static bool rinfo(bool result, QWidget* parent, QString const& text, QMessageBox::StandardButtons buttons = QMessageBox::Ok, QMessageBox::StandardButton defaultButton = QMessageBox::NoButton) {
        QMessageBox::information(parent, program::fullName(), text, buttons, defaultButton);
        return result;
    }
    inline static QMessageBox::StandardButton critical(QWidget* parent, QString const& text, QMessageBox::StandardButtons buttons = QMessageBox::Ok, QMessageBox::StandardButton defaultButton = QMessageBox::NoButton) {
        return QMessageBox::critical(parent, program::fullName(), text, buttons, defaultButton);
    }
    inline static bool critical(bool result, QWidget* parent, QString const& text, QMessageBox::StandardButtons buttons = QMessageBox::Ok, QMessageBox::StandardButton defaultButton = QMessageBox::NoButton) {
        QMessageBox::critical(parent, program::fullName(), text, buttons, defaultButton);
        return result;
    }
    inline static bool remove(QWidget* parent, QString const& message) {
        return info(parent, message,
                  QMessageBox::Yes | QMessageBox::No) != QMessageBox::No;
    }
};
