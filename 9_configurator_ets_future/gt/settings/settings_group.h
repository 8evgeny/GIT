#pragma once
#include <QString>
#include "settings.h"

/**
 * \code
    void settingsRead() {
        gt::SettingsGroup settings(settings_pathes::panel(name()));
        m_first_day_of_week = settings.value(QStringLiteral("firstDayOfWeek")).toUInt();
        m_splash_screen = settings.value(QStringLiteral("splashScreen")).toBool();
    }
    void settingsWrite() {
        gt::SettingsGroup settings(QStringLiteral("/options/"));
        settings.setValue(QStringLiteral("firstDayOfWeek"), m_first_day_of_week);
        settings.setValue(QStringLiteral("splashScreen"), m_splash_screen);
    }
 * \endcode
**/
/** \namespace gt */
namespace gt {

class SettingsGroup: public Settings {
public:
    using class_name = SettingsGroup;
    using inherited  = Settings;
    using settings_t = QSettings;

public:
    SettingsGroup(QString const& prefix) noexcept {
        inherited::beginGroup(prefix);
    }
    SettingsGroup(QString&& prefix) noexcept {
        inherited::beginGroup(qMove(prefix));
    }
    virtual ~SettingsGroup() noexcept {
        inherited::endGroup();
    }
 };
} // end namespace gt
