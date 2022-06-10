#pragma once
#include "include/main_pch.h"

/** \namespace settings_pathes */   
namespace settings_pathes {

static inline QString panel(QString const& name) Q_DECL_NOEXCEPT {
    return QStringLiteral("panels/%1/").arg(name);
}
static inline QString panel(QString&& name) Q_DECL_NOEXCEPT {
    return QStringLiteral("panels/%1/").arg(qMove(name));
}
static inline QString panel(QString const& name, QString const& value) Q_DECL_NOEXCEPT {
    return QStringLiteral("panels/%1/%2").arg(name, value);
}
static inline QString panel(QString&& name, QString&& value) Q_DECL_NOEXCEPT {
    return QStringLiteral("panels/%1/%2").arg(qMove(name), qMove(value));
}
static inline QString dialog(QString const& name) Q_DECL_NOEXCEPT {
    return QStringLiteral("dialogs/%1/").arg(name);
}
static inline QString dialog(QString&& name) Q_DECL_NOEXCEPT {
    return QStringLiteral("dialogs/%1/").arg(qMove(name));
}
/**
 * \code
 *      auto const& group = settings_pathes::dialog(name());
 * \endcode
**/
static inline QString dialog(QString const& name, QString const& value) Q_DECL_NOEXCEPT {
    return QStringLiteral("dialogs/%1/%2").arg(name, value);
}
static inline QString dialog(QString&& name, QString&& value) Q_DECL_NOEXCEPT {
    return QStringLiteral("dialogs/%1/%2").arg(qMove(name), qMove(value));
}
static inline QString window(QString const& name) Q_DECL_NOEXCEPT {
    return QStringLiteral("windows/%1/").arg(name);
}
static inline QString window(QString&& name) Q_DECL_NOEXCEPT {
    return QStringLiteral("windows/%1/").arg(qMove(name));
}
static inline QString window(QString const& name, QString const& value) Q_DECL_NOEXCEPT {
    return QStringLiteral("windows/%1/%2").arg(name, value);
}
static inline QString window(QString&& name, QString&& value) Q_DECL_NOEXCEPT {
    return QStringLiteral("windows/%1/%2").arg(qMove(name), qMove(value));
}

} // end namespace settings_pathes
