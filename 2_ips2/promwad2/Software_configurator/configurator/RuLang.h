/*!
 \file RuLang.h

*/

#ifndef RULANG_H
#define RULANG_H

#include <QObject>
#include <QLocale>

/*!
 \brief Class of existing languages for translation.

 \class RuLang RuLang.h "RuLang.h"
*/
class RuLang: public QObject
{
    Q_OBJECT
public:
    RuLang() = default;

    /*!
     \brief Existing languages for translation.

     \enum E_Languages
    */
    enum E_Languages {
        ENG = QLocale::English,
        RUS = QLocale::Russian
    };

    Q_ENUM(E_Languages)
};

#endif // RULANG_H
