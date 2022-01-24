#include <stdio.h>
#include <string.h>
#include <stdlib.h>






#include "qprogarmdialog.h"
#include "ui_qprogarmdialog.h"

#define EXTERN
#include "glob_var.h"
#include "unit_lib.h"

FILE *fp ;

QString fileNamePath ;


static int iCommandSendFlag = 0 ;
int iPortOld ;


void DetectComPorts()
{
    //UnInitUnit() ;
    int i ;
#ifdef PLATFORM_WINDOWS
    HANDLE  hCom ;
#endif
#ifdef PLATFORM_LINUX

#define INVALID_HANDLE_VALUE	-1

    int hCom ;

#endif

    iNComPorts = 0 ;
    for( i=0; i<10; i++ )
    {

#ifdef PLATFORM_WINDOWS
        sprintf(cBuffer, "COM%d:",i) ;  // "//.//"
#endif
#ifdef PLATFORM_LINUX
        sprintf(cBuffer, "/dev/ttyS%d",i) ;  // "//.//"
#endif
        hCom = OpenSerialPort(cBuffer) ;
        if ( hCom != INVALID_HANDLE_VALUE )
        {
            //if( TestController( hCom) )
            {
                //printf(cBuffer) ;
                strcpy( Port_names[iNComPorts], cBuffer ) ;
                //ui->comboBox_Port->addItem( QString(&cBuffer[0]) ) ;
                if( strncmp(Tune.COMPORT_name,cBuffer,strlen(cBuffer)) == 0 ) iPortOld = iNComPorts ;
                iNComPorts++ ;
            }
            CloseSerialPort( hCom) ;

        }
    }

    for( i=10; i<50; i++ )
    {
#ifdef PLATFORM_WINDOWS
        sprintf(cBuffer, "//.//COM%d",i) ;  // "//.//"
#endif
#ifdef PLATFORM_LINUX
        sprintf(cBuffer, "/dev/ttyUSB%d",i-10) ;  // "//.//"
#endif

        hCom = OpenSerialPort(cBuffer) ;
        if ( hCom != INVALID_HANDLE_VALUE )
        {
            //if( TestController( hCom) )
            {
                //printf(cBuffer) ;
                strcpy( Port_names[iNComPorts], cBuffer ) ;
                //ui->comboBox_Port->addItem( QString(&cBuffer[5]) ) ;
                if( strncmp(Tune.COMPORT_name,&cBuffer[5],strlen(&cBuffer[5])) == 0 ) iPortOld = iNComPorts ;
                if( strncmp(Tune.COMPORT_name,cBuffer,strlen(cBuffer)) == 0 ) iPortOld = iNComPorts ;
                iNComPorts++ ;
            }
            CloseSerialPort( hCom) ;

        }
    }

#ifdef PLATFORM_LINUX
    for( i=0; i<10; i++ )
    {
        sprintf(cBuffer, "/dev/ttyACM%d",i) ;  // "//.//"

        hCom = OpenSerialPort(cBuffer) ;
        if ( hCom != INVALID_HANDLE_VALUE )
        {
            //if( TestController( hCom) )
            {
                //printf(cBuffer) ;
                strcpy( Port_names[iNComPorts], cBuffer ) ;
                //ui->comboBox_Port->addItem( QString(&cBuffer[5]) ) ;
                if( strncmp(Tune.COMPORT_name,&cBuffer[5],strlen(&cBuffer[5])) == 0 ) iPortOld = iNComPorts ;
                if( strncmp(Tune.COMPORT_name,cBuffer,strlen(cBuffer)) == 0 ) iPortOld = iNComPorts ;
                iNComPorts++ ;
            }
            CloseSerialPort( hCom) ;

        }
    }
#endif
    //OpenUnit() ;

}





qProgArmDialog::qProgArmDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::qProgArmDialog)
{
    ui->setupUi(this);

    int i ;

    for(i=0;i<5;i++) Tune.sFileHistory[i][0] = 0x0 ;


    work_object.moveToThread(&WorkThread);
    connect(this, SIGNAL(operate(int)), &work_object, SLOT(Work(int)));
    connect(&WorkThread, SIGNAL(finished()),SLOT(slotProcessingEnd()) ) ;
    connect(&work_object, SIGNAL(signalWorkEnd()),SLOT(slotProcessingEnd()) ) ;
    connect(&work_object, SIGNAL(signalUpdateProgressBar(int)), ui->progressBar,SLOT(setValue(int)));
    connect(&work_object, SIGNAL(signalUpdateProgressBarRange(int)),
                this,SLOT(slotUpdateProgressBarRange(int)));
    connect(&work_object, SIGNAL(signalUpdateStatusBar(QString)), this,SLOT(slotUpdateStatusBar(QString)));

    //processing_thread.setStackSize( 4096*100 ) ;
    WorkThread.start();


    fp = fopen("qprog1986.tune","rb") ;
    if(fp)
    {
        fread(&Tune, sizeof(Tune),1,fp) ;
        fclose(fp) ;
    }

    for(i=0;i<5;i++)
        if( Tune.sFileHistory[i][0] != 0x0 )
    {
        ui->comboBox_File->addItem(Tune.sFileHistory[i]) ;
    }
    ui->comboBox_File->setCurrentIndex( Tune.iCurrentFileIndex ) ;
    fileNamePath = QString(Tune.sCurrentDirectory) ;


    this->setWindowTitle( QString("Programmer for Milandr ARM processors")) ;
    ui->progressBar->setRange(0,10000);
    ui->progressBar->setValue(0);
    ui->progressBar->setHidden(true);



    FillComboPorts() ;

    this->ui->comboBox_Speed->addItem("115200") ;
    this->ui->comboBox_Speed->addItem("57600") ;
    this->ui->comboBox_Speed->addItem("38400") ;
    this->ui->comboBox_Speed->addItem("19200") ;
    this->ui->comboBox_Speed->addItem("9600") ;
    this->ui->comboBox_Speed->addItem("4800") ;
    this->ui->comboBox_Speed->addItem("2400") ;
    this->ui->comboBox_Speed->addItem("1800") ;
    this->ui->comboBox_Speed->addItem("1200") ;

    this->ui->comboBox_Speed->setCurrentIndex(Tune.tty_mode.iBaudsIndex);
    Tune.tty_mode.iBits = 8 ;
    Tune.tty_mode.iHwFlow = 0 ;
    Tune.tty_mode.iParityOn = 0 ;
    Tune.tty_mode.iStopBits = 2 ;
    Tune.tty_mode.iAddCRC16 = 0 ;
    Tune.tty_mode.iAddFastCRC16 = 0 ;

    ui->pushButton_Rescan->setEnabled(true);

    iUnitOn = 0 ;


    ui->pushButton_WriteDevice->setEnabled(false);
    ui->pushButton_Verify->setEnabled(false);
    ui->pushButton_Run->setEnabled(false);

    if((Tune.iUserMode&DEF_MODE_VERIFY)!=0)  ui->checkBox_Verify->setChecked(true);
    else  ui->checkBox_Verify->setChecked(false);
    if((Tune.iUserMode&DEF_MODE_RUN)!=0)  ui->checkBox_Run->setChecked(true);
    else  ui->checkBox_Run->setChecked(false);
}

qProgArmDialog::~qProgArmDialog()
{
    int i ;

    for(i=0;i<ui->comboBox_File->count();i++)
    {
        strcpy(Tune.sFileHistory[i],
               ui->comboBox_File->itemText(i).toLocal8Bit().data()) ;
    }

    strcpy(Tune.COMPORT_name, this->ui->comboBox_Ports->currentText().toLocal8Bit() ) ;
    Tune.iCurrentFileIndex = ui->comboBox_File->currentIndex() ;
    Tune.tty_mode.iBaudsIndex = ui->comboBox_Speed->currentIndex() ;

    fp = fopen("qprog1986.tune","wb") ;
    if(fp)
    {
        fwrite(&Tune, sizeof(Tune),1,fp) ;
        fclose(fp) ;
    }

    UnInitUnit();
    delete ui;
}

void qProgArmDialog::slotUpdateStatusBar(QString str)
{
    ui->label_Status->setText(QString(str)) ;
}

void qProgArmDialog::slotUpdateProgressBarRange(int irange)
{
    ui->progressBar->setRange(0,irange) ;
}

void qProgArmDialog::on_pushButton_Browse_clicked()
{
    QString Filters = QString("Binary *.BIN  (*.bin);;Hexadecimal *.HEX  (*.hex);;All files (*.*)") ;
    QString FilterRet ;

    QFileDialog myFileDialog(this, tr("Open File"),  QString()) ;

    int ires ;
    QDir dir = QDir(fileNamePath) ;

    if( !fileNamePath.isEmpty() )
        myFileDialog.setDirectory(dir);

    QString fileName = myFileDialog.getOpenFileName (this, tr("Select device program file"), QString(""), Filters, &FilterRet, 0  ) ;


    if( !fileName.isEmpty() )
    {
        fileNamePath = fileName.left(fileName.lastIndexOf('/') ) ;

        strcpy(Tune.sCurrentDirectory,fileNamePath.toLocal8Bit().data()) ;

        ui->comboBox_File->insertItem(0,fileName) ;
        ui->comboBox_File->setCurrentIndex(0);
        if(ui->comboBox_File->count()>5)
            ui->comboBox_File->removeItem(5);
    }

}



void qProgArmDialog::FillComboPorts()
{
    DetectComPorts() ;

    int i ;

    ui->comboBox_Ports->clear() ;

    for(i=0;i<iNComPorts;i++)
    {
        ui->comboBox_Ports->addItem( QString( Port_names[i] ) ) ;
    }
    if( iPortOld != -1 )
        ui->comboBox_Ports->setCurrentIndex(iPortOld) ;

}



void qProgArmDialog::on_pushButton_Connect_clicked()
{


    if( iUnitOn == 0 )
    {
        strcpy(Tune.COMPORT_name, this->ui->comboBox_Ports->currentText().toLocal8Bit() ) ;

        Tune.tty_mode.iBaudsIndex = ui->comboBox_Speed->currentIndex() ;
        Tune.tty_mode.iBauds = ui->comboBox_Speed->currentText().toInt() ;

        strcpy(cModeString, "baud=" ) ;
        strcat(cModeString, ui->comboBox_Speed->currentText().toLocal8Bit() ) ;
        strcat(cModeString, " parity=N" ) ;
        strcat(cModeString, " data=8" ) ;
        strcat(cModeString, " stop=1" ) ;

        iUnitOn = OpenUnit() ;

        ui->label_Connection->setText(QString().sprintf("Connected %10d",iChipConnectionSuccess));

        if( iUnitOn > 0 )
        {
            ui->pushButton_Connect->setText("Off");
            ui->pushButton_Connect->update();
            ui->pushButton_Rescan->setEnabled(false);
            ui->comboBox_Ports->setEnabled(false);
            ui->comboBox_Speed->setEnabled(false);

            ui->pushButton_WriteDevice->setEnabled(true);
            ui->pushButton_Run->setEnabled(true);
            ui->pushButton_Verify->setEnabled(true);

        }
    }
    else
    {
        UnInitUnit() ;
        iUnitOn = 0 ;
        iChipConnectionSuccess = 0 ;
        ui->label_Connection->setText(QString().sprintf("Connected %10d",iChipConnectionSuccess));

        ui->pushButton_Connect->setText("Connect");
        ui->pushButton_Connect->update();
        ui->pushButton_Rescan->setEnabled(true);
        ui->comboBox_Ports->setEnabled(true);
        ui->comboBox_Speed->setEnabled(true);

        ui->pushButton_WriteDevice->setEnabled(false);
        ui->pushButton_Run->setEnabled(false);
        ui->pushButton_Verify->setEnabled(false);
    }

}

void qProgArmDialog::on_pushButton_Rescan_clicked()
{
    strcpy(Tune.COMPORT_name, this->ui->comboBox_Ports->currentText().toLocal8Bit() ) ;
    FillComboPorts() ;
}



void qProgArmDialog::slotProcessingEnd()
{
    ui->progressBar->setRange(0,10000);
    ui->progressBar->setValue(0);
    ui->progressBar->setHidden(true);

    ui->pushButton_WriteDevice->setEnabled(true);
    ui->pushButton_Run->setEnabled(true);
    ui->pushButton_Verify->setEnabled(true);
}



void qProgArmDialog::on_pushButton_WriteDevice_clicked()
{
    strcpy(this->work_object.filename,
           ui->comboBox_File->currentText().toLocal8Bit().data()) ;

    ui->pushButton_WriteDevice->setEnabled(false);
    ui->pushButton_Verify->setEnabled(false);
    ui->pushButton_Run->setEnabled(false);

    ui->progressBar->setHidden(false);

    emit operate(  IDM_WRITE_FLASH ) ;
}

void qProgArmDialog::on_pushButton_Verify_clicked()
{
    strcpy(this->work_object.filename,
           ui->comboBox_File->currentText().toLocal8Bit().data()) ;

    ui->pushButton_WriteDevice->setEnabled(false);
    ui->pushButton_Verify->setEnabled(false);
    ui->pushButton_Run->setEnabled(false);

    ui->progressBar->setHidden(false);

    emit operate( IDM_VERIFY ) ;
}




void qProgArmDialog::on_checkBox_Verify_clicked()
{
    if(ui->checkBox_Verify->isChecked()) Tune.iUserMode |= DEF_MODE_VERIFY ;
    else  Tune.iUserMode &= ~DEF_MODE_VERIFY ;
}

void qProgArmDialog::on_checkBox_Run_clicked()
{
    if(ui->checkBox_Run->isChecked()) Tune.iUserMode |= DEF_MODE_RUN ;
    else  Tune.iUserMode &= ~DEF_MODE_RUN ;
}

void qProgArmDialog::on_pushButton_Run_clicked()
{
    ui->pushButton_WriteDevice->setEnabled(false);
    ui->pushButton_Verify->setEnabled(false);
    ui->pushButton_Run->setEnabled(false);

    emit operate( IDM_RUN ) ;
}
