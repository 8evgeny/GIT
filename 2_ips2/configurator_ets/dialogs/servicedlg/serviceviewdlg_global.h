#pragma once

#if defined(SERVICEVIEW_DIALOG_LIBRARY)
    #define SERVICEVIEW_DIALOG_SHARED_EXPORT Q_DECL_EXPORT
#else
    #define SERVICEVIEW_DIALOG_SHARED_EXPORT Q_DECL_IMPORT
#endif

