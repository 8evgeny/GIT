#pragma once
#include <QObject>

#define TR_CLASS(cls) class cls: public QObject { \
    Q_OBJECT \
};

TR_CLASS(iInfo)
TR_CLASS(iMenu)
TR_CLASS(iDesc)
TR_CLASS(iError)
TR_CLASS(iKey)
TR_CLASS(iProject)
TR_CLASS(iMessage)
TR_CLASS(iFileTypes)
TR_CLASS(iStation)
TR_CLASS(iFunction)
TR_CLASS(iGroups)
TR_CLASS(iDestination)
TR_CLASS(iInputConnections)
TR_CLASS(iOtputConnections)
TR_CLASS(iInterfaces)
