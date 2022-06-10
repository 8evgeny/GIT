#pragma once
#include <numeric>
#include <climits>

/**
 * \code
 *      IPS_UI(OptionsDialog)
 * \endcode
**/
#define IPS_UI(ui) namespace Ui { class ui; }
#define IPS_STR(name) QStringLiteral(name)
#define GT_STR(name) QStringLiteral(name)
#define IPS_FUNCT(act) [](){act;}
#define IPS_THIS_FUNCT(act) [this](){act;}

#define IPS_DISABLE_COPY(Class) \
    Class(const Class &) = delete;\
    Class &operator=(const Class &) = delete;

#define IPS_DISABLE_MOVE(Class) \
    Class(Class &&) = delete; \
    Class &operator=(Class &&) = delete;

#define IPS_DISABLE_COPY_MOVE(Class) \
    IPS_DISABLE_COPY(Class) \
    IPS_DISABLE_MOVE(Class)
