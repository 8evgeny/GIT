#ifndef WORK_OBJECT_H
#define WORK_OBJECT_H


#include <qfiledialog.h>
#include <QTextStream>
#include <QMessageBox>
#include <QTextCursor>
#include <QThread>
#include <QMutex>
#include <QScrollBar>
#include <QPolygonF>
#include <QVector>
#include <QPoint>
#include <QList>


class qProgArmDialog ;





class ProcessingThreadObject : public QObject
{
     Q_OBJECT
     QThread workerThread;

public:
     explicit ProcessingThreadObject(QObject *parent = 0);
     ~ProcessingThreadObject() {
         workerThread.quit();
         workerThread.wait();
     }

     qProgArmDialog *owner ;

     int ibgn,iend, iOperation ;
     int index ;
     char filename[512] ;

    int LoadBootBinToChip() ;
    int ProgramHexFileToChip(char *filename) ;
    int VerifyHexFileToChip(char *filename) ;
    bool Verify(void) ;
    bool Program(void) ;
    void Run() ;
    bool Erase(void) ;

 public slots:
     void Work(int iOperation) ;

 signals:
     void signalUpdateStatusBar(QString) ;
     void signalUpdateProgressBar(int i) ;
     void signalUpdateProgressBarRange(int i) ;
     void signalWorkEnd() ;
};


#endif // WORK_OBJECT_H
