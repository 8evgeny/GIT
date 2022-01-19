#pragma once
#include <QMainWindow>
#include "window.h"

/**
 * \code
 *      *.h
 *          typedef Mainwindow main_window;
 *          static QString name() { return QStringLiteral("Notepad"); }
 *
 *      *.cpp
 *          main_window::init(this, name(), title, {{this, 400, 250}});
 *          main_window::settingsWrite();
 * \endcode
**/
/** begin namespace widget */
namespace widget {

class MainWindow: public Window<QMainWindow> {
 public:
    using class_name = MainWindow;
    using inherited  = Window<QMainWindow>;
    using settings_t = inherited::settings_t;
    using variant_t  = inherited::variant_t;
    using ilist_t    = inherited::ilist_t;

 protected:
    virtual void doSettingsRead() override {
        inherited::settings()->addVariant(variant_t::state(inherited::window()));
        auto const& group = settings_pathes::window(name());
        inherited::settings()->read(group);
    }
    virtual void doSettingsWrite() override {
        auto const& group = settings_pathes::window(name());
        inherited::settings()->write(group);
    }
    virtual void doSingleShotRead(ilist_t list) override {
        auto const& group = settings_pathes::window(name());
        settings_t settings(list);
        settings.read(group);
    }
    virtual void doSingleShotWrite(ilist_t list) override {
        auto const& group = settings_pathes::window(name());
        settings_t settings(list);
        settings.write(group);
    }

public:
    virtual ~MainWindow() = default;
    void exec() {
        inherited::window()->show();
        while (inherited::window()->isVisible())
           QApplication::processEvents();
    }
};
} // end namespace widget
