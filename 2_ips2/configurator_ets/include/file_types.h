#pragma once
#include <QString>
#include "tr.h"

class FileTypes {
 public:
    using class_name = FileTypes;

 public:
    static inline QString separator() Q_DECL_NOEXCEPT {
        return QStringLiteral(";;");
    }
    static inline QString allFiles() Q_DECL_NOEXCEPT {
        return iFileTypes::tr("All Files(*.*)");
    }
    static inline QString projectFiles() Q_DECL_NOEXCEPT {
        return iFileTypes::tr("Project Files (*.ips)");
    }

 public:
    static inline QString projectsFiles() Q_DECL_NOEXCEPT {
        return QStringLiteral("%1;;%2").arg(projectFiles(), allFiles());
    }
};
