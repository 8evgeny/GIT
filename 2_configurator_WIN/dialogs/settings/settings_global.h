#pragma once

#if defined(SETTINGS_DIALOG_LIBRARY)
    #define SETTINGS_DIALOG_SHARED_EXPORT Q_DECL_EXPORT
#else
    #define SETTINGS_DIALOG_SHARED_EXPORT Q_DECL_IMPORT
#endif

