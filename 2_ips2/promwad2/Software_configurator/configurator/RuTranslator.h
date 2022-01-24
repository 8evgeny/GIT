/*!
 \file RuTranslator.h

*/

#ifndef RUTRANSLATOR_H
#define RUTRANSLATOR_H

#include <QObject>
#include <QGuiApplication>
#include <QQuickView>
#include <QTranslator>
#include "RuLang.h"

/*!
 \brief This class is responsible for translating the language

 \class RuTranslator RuTranslator.h "RuTranslator.h"
*/
class RuTranslator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString emptyString READ getEmptyString NOTIFY languageChanged)

public:

    RuTranslator(QGuiApplication *app) { mApp = app; }

    /*!
     \brief Get empty string

     \fn getEmptyString
     \return QString
    */
    QString getEmptyString() { return ""; }

signals:

    /*!
     \brief Signal for changing the language

     \fn languageChanged
    */
    void languageChanged(qint32 langState);

public slots:
    /*!
     \brief Update languages. You can choose EN or RU languages.

     \fn updateLanguage
     \param lang Number language
    */
    void updateLanguage(int lang)
    {
        qint32 st = 0;
        switch (lang) {
        case RuLang::RUS: {
            mTranslator.load("main_ru", ":/translator");
            QGuiApplication::installTranslator(&mTranslator);
            st = 1;

        }
        break;
        default:
            QGuiApplication::removeTranslator(&mTranslator);
            break;
        }
        emit languageChanged(st);
    }

private:
    QGuiApplication *mApp; /*! main GUI */
    QTranslator mTranslator; /*! Translator */
};


#endif // RUTRANSLATOR_H
