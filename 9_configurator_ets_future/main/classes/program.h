#pragma once
#include "include/main_pch.h"

#define APP_PATH(str) QCoreApplication::applicationDirPath() + QStringLiteral(str);
#define HOME_PATH(str) program::homeFolder() + QStringLiteral(str);
/**
 * \code
 *      program::fullName();
 * \endcode
**/
//!< begin namespace program
namespace program {
    inline QString setupName() noexcept {
        return QStringLiteral("configurator.exe");
    }
    inline QString name() noexcept {
        return QStringLiteral("Configurator IPS");
    }
    inline QString regName() noexcept {
        return GT_STR("IPSConfigurator");
    }
    inline QString productName() noexcept {
        return GT_STR("GIT-Comm IPS");
    }
    inline QString build() noexcept {
        return QString::number(VERSION_PATCH);
    }
    inline QString version() noexcept {
        return QStringLiteral("%1.%2")
                .arg(VERSION_MAJOR)
                .arg(VERSION_MINOR);
    }
    inline QString fullProductName() noexcept {
        return GT_STR("%1 %2 %3")
               .arg(productName())
               .arg(version())
               .arg(build());
    }
    inline QString fullName() noexcept {
        return QStringLiteral("%1 %2")
               .arg(name(), version());
    }
    inline QString date() noexcept {
        return GT_STR(__DATE__);
    }
    inline QString fullVersion() noexcept {
        return QStringLiteral("%1 %2, build %3 (%4)")
                .arg(name(), version(), build(), date());
    }
    inline QString organizationName() noexcept {
        return QStringLiteral("GIT");
    }
    inline QString organizationDomain() noexcept {
        return QStringLiteral("git-holding.ru");
    }
    inline QString homeFolder() noexcept {
        return QDir::home().absolutePath() + GT_STR("/.githolding/ips/configurator");
    }
    inline QString help() noexcept {
        return APP_PATH("/Help/Help.chm")
    }
    inline QString author() noexcept {
        return QLatin1String("Sergey Panin, ALexander Kuzichkin");
    }
    inline QString copyright() noexcept {
        return QStringLiteral("Copyright (c) 2018-2021, GIT. All rights reserved.");
    }
    inline QString homepage() noexcept {
        return QStringLiteral("https://git-holding.ru/");
    }
    inline QString logFile() noexcept {
#if defined(Q_OS_LINUX)
        return GT_STR("/var/log/ips_configurator.log");
#endif
        return HOME_PATH("/log.log")
    }
    inline QString supportMail() noexcept {
        return QStringLiteral("support@git-holding.ru");
    }
    /** \namespace registry */
    namespace registry {
        inline QString software() noexcept {
            return QStringLiteral("HKEY_CURRENT_USER\\SOFTWARE");
        }
        inline QString ips() noexcept {
            return QLatin1String("HKEY_CURRENT_USER\\SOFTWARE\\IPS");
        }
    } // end namespace registry
    namespace ext {
        inline QString translation() noexcept {
            return QStringLiteral("*.qm");
        }
    }
    namespace paths {
        inline auto sounds() noexcept -> QString {
#if defined(Q_OS_LINUX)
            return GT_STR("/usr/share/ips_onfigurator/sounds");
#endif
            return APP_PATH("/sounds");
        }
        inline auto langs() noexcept -> QString {
#if defined(Q_OS_LINUX)
            return GT_STR("/usr/share/ips_onfigurator/langs");
#endif
            return APP_PATH("/langs");
        }
        inline auto translations() noexcept -> QString {
#if defined(Q_OS_LINUX)
            return GT_STR("/usr/share/ips_onfigurator/translations");
#endif
            return APP_PATH("/translations")
        }
    }
} // end namespace program
