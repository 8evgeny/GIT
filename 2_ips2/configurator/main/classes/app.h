#pragma once
#include "include/log.h"
#include "include/typedefs.h"
#include "main/classes/program.h"
#include "main/classes/options/language.h"
#include "main/classes/base_model/base_model.h"
#include "main/main_window.h"
#include <QLibraryInfo>

#define APP_SETTINGS App::model()->globalParametersItem()

class App final : public QApplication {
public:
    using class_name = App;
    using inherited  = QApplication;

public:
    static void setLanguage(uint lang) {        
        options::Language::setCurrent(lang);
        instanceLanguage();
    }
    static void instanceLanguage() {
        auto&& lang = options::Language::currentBcp47Name();
        inherited::removeTranslator(trQt());
        inherited::removeTranslator(trApp());
        LOG_WARNING(trApp()->load(lang, program::paths::langs()), "Can't load translation file '%1' from a directory %2", lang, program::paths::langs());
        LOG_WARNING(trQt()->load(IPS_STR("qt_%1").arg(lang), program::paths::translations()), "Can't load Qt translation file '%1' from a directory %2", lang, program::paths::translations());
        inherited::installTranslator(trQt());
        inherited::installTranslator(trApp());
        QLocale::setDefault(options::Language::current().lang());
    }

public:    
    explicit App(int& arc, char** argv) noexcept
        :inherited(arc, argv)
    {
        init();
        instanceLanguage();
    }
    ~App() = default;
    void init() {
        QCoreApplication::setOrganizationName(program::organizationName());
        QCoreApplication::setOrganizationDomain(program::organizationDomain());
        QCoreApplication::setApplicationName(program::name());
        QCoreApplication::setApplicationVersion(program::version());
    }    
    static QTranslator* trApp() noexcept {
        static QTranslator tr;        
        return &tr;
    }
    static QTranslator* trQt() noexcept {
        static QTranslator tr;
        return &tr;
    }
    static Settings* settings() noexcept {
        static Settings settings;
        return &settings;
    }
    static BaseModel* model() noexcept {
        static BaseModel model;
        return &model;
    }
    static MessagesDialog* messages() noexcept {
        static MessagesDialog dlg;
        return &dlg;
    }
    static void exit() noexcept {
        inherited::exit();
    }
    static void quit() noexcept {
        inherited::instance()->quit();
    }
    static QString exeName() noexcept {
        auto const& data = inherited::arguments();
        return data.first();
    }
};
