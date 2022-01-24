#ifndef MDRFWUGUI_H
#define MDRFWUGUI_H

#include <QMainWindow>
#include <QProcess>

namespace Ui {
class MdrFwUForm;
}

class MdrFwUgui : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MdrFwUgui(QString fwfnParam = QString(), QWidget *parent = nullptr);
    ~MdrFwUgui() override;
        
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
    
    void printSO();
    void printSE();
    void cliEO(QProcess::ProcessError error);
    //void cliF(int exitCode, QProcess::ExitStatus exitStatus);
    void cliSC(QProcess::ProcessState newState);
    //void cliS();
private:
    void closeEvent(QCloseEvent *event) override;
    void writeSettings();
    void readSettings();
    QString fwfnParam;
    Ui::MdrFwUForm *ui;
    QProcess cli;
};

#endif // MDRFWUGUI_H
