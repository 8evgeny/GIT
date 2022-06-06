#pragma once
#include <QString>
#include "settings_pathes.h"

/**
 * \code
 *      App::settings()->setValue(QStringLiteral("buffer/OptionsDialog"), 1);
 *      App::settings()->value(QStringLiteral("buffer/OptionsDialog"), 1);
 *
 *      Settings::setVal(QStringLiteral("buffer/OptionsDialog"), 1);
 *      Settings::val(QStringLiteral("buffer/OptionsDialog"), 1);
 * \endcode
**/

/** \namespace gt */
namespace gt {

class Settings {
public:
    using class_name = Settings;
    using settings_t = QSettings;

private:
    settings_t  m_settings;

private:
    static inline QString panelsPath() noexcept {
        return QStringLiteral("panels/");
    }
    static inline QString dialogsPath() noexcept {
        return QStringLiteral("dialogs/");
    }
    static inline QString windowsPath() noexcept {
        return QStringLiteral("windows/");
    }
    static inline QString lastFile() noexcept {
        return QStringLiteral("last_file");
    }
    static inline QString createWindowGeometryPath(QString const& name) noexcept {
        return QStringLiteral("%1%2/geometry").arg(windowsPath(), name);
    }
    static inline QString createWindowStatePath(QString const& name) noexcept {
        return QStringLiteral("%1%2/state").arg(windowsPath(), name);
    }
    static inline QString createDialogGeometryPath(QString const& name) noexcept {
        return QStringLiteral("%1%2/geometry").arg(dialogsPath(), name);
    }

public:
    virtual ~Settings() Q_DECL_EQ_DEFAULT;
    void beginGroup(QString const& prefix) noexcept {
        m_settings.beginGroup(prefix);
    }
    void beginGroup(QString&& prefix) noexcept {
        m_settings.beginGroup(qMove(prefix));
    }
    void endGroup() noexcept {
        m_settings.endGroup();
    }
    QStringList allKeys() const noexcept {
        return m_settings.allKeys();
    }
    void removeKey(QString const& key) noexcept {
        m_settings.remove(key);
    }
    void setValue(QString const& key, QVariant const& value) noexcept {
        m_settings.setValue(key, value);
        m_settings.sync();
    }
    void setValue(QString&& key, QVariant&& value) noexcept {
        m_settings.setValue(qMove(key), qMove(value));
        m_settings.sync();
    }
    QVariant value(QString const& key, QVariant const& defaultValue = {}) const noexcept {
        return m_settings.value(key, defaultValue);
    }
    QVariant value(QString&& key, QVariant const& defaultValue = {}) const noexcept {
        return m_settings.value(qMove(key), defaultValue);
    }
    QVariant valueInt(QString const& key, int defaultValue = {}) const noexcept {
        return value(key, defaultValue).toInt();
    }
    QVariant valueInt(QString&& key, int defaultValue = int()) const noexcept {
        return value(qMove(key), defaultValue).toInt();
    }
    void setWindowFile(QString const& name, QString const& path) noexcept {
        setValue(settings_pathes::window(name, lastFile()), path);
    }
    QString windowFile(QString const& name) const noexcept {
        return value(settings_pathes::window(name, lastFile())).toString();
    }
    void setDialogFile(QString const& name, QString const& path) noexcept {
        setValue(settings_pathes::dialog(name, lastFile()), path);
    }
    QString dialogFile(QString const& name) const noexcept {
        return value(settings_pathes::dialog(name, lastFile())).toString();
    }
    void setPanelFile(QString const& name, QString const& path) noexcept {
        setValue(settings_pathes::panel(name, lastFile()), path);
    }
    QString panelFile(QString const& name) const noexcept {
        return value(settings_pathes::panel(name, lastFile())).toString();
    }
    static void removeVal(QString const& key) noexcept {
        class_name cls;
        cls.removeKey(key);
    }
    static void setVal(QString const& key, QVariant const& value) noexcept {
        class_name cls;
        cls.setValue(key, value);
    }
    static inline void setVal(QString&& key, QVariant&& value) noexcept {
        class_name cls;
        cls.setValue(qMove(key), qMove(value));
    }
    static inline QVariant val(QString const& key, QVariant const& defaultValue = {}) noexcept {
        class_name cls;
        return cls.value(key, defaultValue);
    }
    static inline QVariant val(QString&& key, QVariant const& defaultValue = {}) noexcept {
        class_name cls;
        return cls.value(qMove(key), defaultValue);
    }
    static inline QString valStr(QString const& key, QVariant const& defaultValue = {}) noexcept {
        class_name cls;
        return cls.value(key, defaultValue).toString();
    }
    static inline QString valStr(QString&& key, QVariant const& defaultValue = {}) noexcept {
        class_name cls;
        return cls.value(qMove(key), defaultValue).toString();
    }
};
} // end namespace gt
