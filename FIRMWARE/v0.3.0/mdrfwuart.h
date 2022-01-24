#ifndef MDRFWUART_H
#define MDRFWUART_H

#include <QMainWindow>
#include "vasililoader.h"
#include <QThread>

namespace Ui {
class MdrFwUForm;
}

class MdrFwUart : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MdrFwUart(QString fwfnParam = QString(), QWidget *parent = nullptr);
    ~MdrFwUart() override;
    
signals:
    void operate(VasiliLoaderClient::WorkType work);
        
private slots:
    void on_openFilePB_clicked();
    void on_startStopPB_clicked();
    void on_dumpCB_clicked(bool checked);
    void on_dumpfnPB_clicked();
    void loaderFinished();
    void on_flasherfnPB_clicked();
    void on_action_Qt_triggered();
    void on_about_triggered();
    void logText(QString);
    void logStatus(QString);
    void updateComPortCB();
    
private:
    void closeEvent(QCloseEvent *event) override;
    void writeSettings();
    void readSettings();
    QString fwfnParam;
    VasiliLoaderClient *mdrLoader;
    Ui::MdrFwUForm *ui;
    QThread loaderThread;
};

#endif // MDRFWUART_H
