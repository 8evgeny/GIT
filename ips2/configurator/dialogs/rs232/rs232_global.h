#pragma once

#if defined(RS232_DIALOG_LIBRARY)
    #define RS232_DIALOG_SHARED_EXPORT Q_DECL_EXPORT
#else
    #define RS232_DIALOG_SHARED_EXPORT Q_DECL_IMPORT
#endif

