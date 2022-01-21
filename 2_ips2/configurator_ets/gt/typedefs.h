#pragma once
#include <numeric>
#include <climits>

/**
 * \code
 *      GT_UI(OptionsDialog)
 * \endcode
**/
#define GT_UI(ui) namespace Ui { class ui; }
#define GT_STR(name) QStringLiteral(name)
#define GT_URL(name) QNetworkRequest(QUrl(QStringLiteral(name)))
#define GT_FUNCT(act) [](){act;}
#define GT_APP_PATH(str) QCoreApplication::applicationDirPath() + GT_STR(str);
#define GT_UTF8(_1)  QString::fromUtf8(_1)
#define GT_UTF81(_1, _2)  QString::fromUtf8(_1).arg(_2)
#define GT_UTF82(_1, _2, _3)  QString::fromUtf8(_1).arg(_2).arg(_3)

#define GT_ADD_ACTION(menu, _a1)                                       \
    ([&menu](auto func1) {                                             \
        auto act = menu.addAction(func1.ico(), func1.text(), func1);   \
        act->setObjectName(func1.objectName());                        \
    })(_a1);                                                           \

#define GT_ADD_ACTION_DISABLED(menu, _a1)                              \
    ([&menu](auto func1, bool is = true) {                             \
        auto act = menu.addAction(func1.ico(), func1.text(), func1);   \
        act->setEnabled(false);                                        \
        act->setObjectName(func1.objectName());                        \
    })(_a1);                                                           \

