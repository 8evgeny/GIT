#pragma once
#include <QDialog>
#include "window.h"
#include <main/settings/settings_pathes.h>

/**
 * \code
 *      *.h
 *          using dialog = widget::Dialogg;
 *          static QString name() { return QStringLiteral("Notepad"); }
 *
 *      *.cpp
 *          dialog::init(this, name(), title, {{this, 400}});
 *          dialog::settingsWrite();
 * \endcode
**/
/** begin namespace widget */
namespace widget {

class Dialog: public Window<QDialog> {
 public:
    using class_name = Dialog;
    using inherited  = Window<QDialog>;
    using settings_t = inherited::settings_t;
    using variant_t  = inherited::variant_t;
    using ilist_t    = inherited::ilist_t;

 protected:
    virtual void doSettingsRead() Q_DECL_OVERRIDE {
        auto const& group = settings_pathes::dialog(name());
        inherited::settings()->read(group);
    }
    virtual void doSettingsWrite() Q_DECL_OVERRIDE {
        auto const& group = settings_pathes::dialog(name());
        inherited::settings()->write(group);
    }
    virtual void doSingleShotRead(ilist_t list) Q_DECL_OVERRIDE {
        auto const& group = settings_pathes::dialog(name());
        settings_t settings(list);
        settings.read(group);
    }
    virtual void doSingleShotWrite(ilist_t list) Q_DECL_OVERRIDE {
        auto const& group = settings_pathes::dialog(name());
        settings_t settings(list);
        settings.write(group);
    }

 public:
    Dialog() Q_DECL_EQ_DEFAULT;
};

} // end namespace widget
