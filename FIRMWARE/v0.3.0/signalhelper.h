#ifndef SIGNALHELPER_H
#define SIGNALHELPER_H

#include <QObject>

class SignalHelper : public QObject
{
    Q_OBJECT
    
  public:
    SignalHelper();
    bool gui;
    
  public slots:
    void statusChanged(QString st);
    void textChanged(QString txt);
    void workFinished();
    void stopButtonSetEnable(bool);
    void dumpFilenameChanged(QString dumpfn);
};

#endif // SIGNALHELPER_H
