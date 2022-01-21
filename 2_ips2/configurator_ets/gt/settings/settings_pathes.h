#pragma once
#include <QString>
#include <QSettings>

/** \namespace gt::settings_pathes */   
namespace gt::settings_pathes {

static inline QString panel(QString const& name) noexcept {
    return GT_STR("panels/%1/").arg(name);
}
static inline QString panel(QString&& name) noexcept {
    return GT_STR("panels/%1/").arg(qMove(name));
}
static inline QString panel(QString const& name, QString const& value) noexcept {
    return GT_STR("panels/%1/%2").arg(name, value);
}
static inline QString panel(QString&& name, QString&& value) noexcept {
    return GT_STR("panels/%1/%2").arg(qMove(name), qMove(value));
}
static inline QString dialog(QString const& name) noexcept {
    return GT_STR("dialogs/%1/").arg(name);
}
static inline QString dialog(QString&& name) noexcept {
    return GT_STR("dialogs/%1/").arg(qMove(name));
}
/**
 * \code
 *      auto const& group = settings_pathes::dialog(name());
 * \endcode
**/
static inline QString dialog(QString const& name, QString const& value) noexcept {
    return GT_STR("dialogs/%1/%2").arg(name, value);
}
static inline QString dialog(QString&& name, QString&& value) noexcept {
    return GT_STR("dialogs/%1/%2").arg(qMove(name), qMove(value));
}
static inline QString window(QString const& name) noexcept {
    return GT_STR("windows/%1/").arg(name);
}
static inline QString window(QString&& name) noexcept {
    return GT_STR("windows/%1/").arg(qMove(name));
}
static inline QString window(QString const& name, QString const& value) noexcept {
    return GT_STR("windows/%1/%2").arg(name, value);
}
static inline QString window(QString&& name, QString&& value) noexcept {
    return GT_STR("windows/%1/%2").arg(qMove(name), qMove(value));
}

} // end namespace gt::settings_pathes
