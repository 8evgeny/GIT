#pragma once
#include <QString>
#include "settings.h"
#include "include/typedefs.h"

/** \namespace settings */
namespace settings {

/** \namespace empty */
namespace {
        
inline QString last(QString const& name) noexcept {
    return IPS_STR("last/%1").arg(name);
}
inline QString last(QString&& name) noexcept {
    return IPS_STR("last/%1").arg(qMove(name));
}
inline bool isProjectSaved(QString const& path) noexcept {
    auto const& f1 = Settings::val(last(IPS_STR("project/f1"))).toString();
    auto const& f2 = Settings::val(last(IPS_STR("project/f2"))).toString();
    auto const& f3 = Settings::val(last(IPS_STR("project/f3"))).toString();
    return f1 == path || f2 == path || f3 == path;
}
inline void saveProject(QString const& path) noexcept {
    auto const& f1 = Settings::val(last(IPS_STR("project/f1"))).toString();
    auto const& f2 = Settings::val(last(IPS_STR("project/f2"))).toString();

    Settings::setVal(last(IPS_STR("project/f3")), f2);
    Settings::setVal(last(IPS_STR("project/f2")), f1);
    Settings::setVal(last(IPS_STR("project/f1")), path);
}
} // end empty namespace

/**
 * \code
        auto const& file = QFileDialog::getOpenFileName(this, iTitle::tr("Open an image")
                                        , settings::lastImageFile(), FileTypes::imageFiles());
        LOG_ERROR(!file.isEmpty(), "Invalid FileName %1", file);
        settings::setLastImageFile(file);
 * \endcode
**/
inline void setLastImageFile(QString&& path) noexcept {
    Settings::setVal(last(IPS_STR("image")), qMove(path));
}
inline void setLastImageFile(QString const& path) noexcept {
    Settings::setVal(last(IPS_STR("image")), path);
}
inline QString lastImageFile() noexcept {
    return Settings::val(last(IPS_STR("image"))).toString();
}

/**
 * \code
        auto const& file = QFileDialog::getSaveFileName(Q_NULLPTR, iProject::tr("Save a project...")
                                            , settings::lastProjectFile(), FileTypes::projectSaveFiles());
        LOG_ERROR(!file.isEmpty(), "Invalid FileName %1", file);
        settings::setLastProjectFile(file);
        m_model.save(file);
 * \endcode
**/
inline void setLastProjectFile(QString const& path) noexcept {
    if(!isProjectSaved(path))
        saveProject(path);
}
inline auto lastProjectFile() noexcept {
    return Settings::val(last(IPS_STR("project/f1"))).toString();
}
inline auto allProjectsFile() noexcept {
    return QStringList {
        { Settings::val(last(IPS_STR("project/f1"))).toString() }
      , { Settings::val(last(IPS_STR("project/f2"))).toString() }
      , { Settings::val(last(IPS_STR("project/f3"))).toString() }
    };
}
} // end namespace settings
