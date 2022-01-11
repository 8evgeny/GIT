#ifndef QPROGARMDIALOG_H
#define QPROGARMDIALOG_H

#include <QDialog>


#include "work_object.h"

namespace Ui {
class qProgArmDialog;
}

class qProgArmDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit qProgArmDialog(QWidget *parent = 0);
    ~qProgArmDialog();
    
    void FillComboPorts() ;

    QThread WorkThread ;
    ProcessingThreadObject work_object ;

signals:
    void signalUpdateProgressBar(int i) ;
    void operate(int iOperation) ;

private slots:
    void on_pushButton_Browse_clicked();
    void on_pushButton_Connect_clicked();
    void on_pushButton_Rescan_clicked();
    void slotProcessingEnd() ;
    void slotUpdateProgressBarRange(int irange) ;
    void slotUpdateStatusBar(QString str) ;
    void on_pushButton_WriteDevice_clicked();
    void on_pushButton_Verify_clicked();
    void on_checkBox_Verify_clicked();
    void on_checkBox_Run_clicked();
    void on_pushButton_Run_clicked();

private:
    Ui::qProgArmDialog *ui;
};

#endif // QPROGARMDIALOG_H
