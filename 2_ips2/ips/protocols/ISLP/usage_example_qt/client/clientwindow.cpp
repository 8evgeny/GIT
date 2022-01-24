#include "clientwindow.h"
#include "ui_clientwindow.h"

#include <cstring>

#include <QRegExpValidator>

#include "tools/generators.hpp"
#include <ipsnames.hpp>

clientWindow::clientWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::clientWindow)
{
  ui->setupUi(this);

  QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
  QRegExp ipRegex ("^" + ipRange
                       + "\\." + ipRange
                       + "\\." + ipRange
                       + "\\." + ipRange + "$");
  auto *ipValidator = new QRegExpValidator(ipRegex, this);
  ui->ipEdit->setValidator(ipValidator);
  ui->maskEdit->setValidator(ipValidator);
  ui->gatewayEdit->setValidator(ipValidator);
  ui->serialEdit->setMaxLength(12);
  ui->nameEdit->setMaxLength(12);


  generateStation(&self);
  refreshWindow();

  client=new stationEmulator(self,this);
  (*client)();
}

clientWindow::~clientWindow()
{
  delete ui;
}

/**
 * @brief clientWindow::refreshWindow
 *
 */
void clientWindow::refreshWindow()
{
  ui->serialEdit->setText(self.getSerialStr());
  ui->nameEdit->setText(self.getName());
  ui->ipEdit->setText(self.getIPaddr());
  ui->maskEdit->setText(self.getNetmask());
  ui->gatewayEdit->setText(self.getGateway());
  ui->macDisplay->setText(self.getMACStr());
  ui->typeDisplay->setText(IPS::Stations::QTypeName(self.board_type));

}


void clientWindow::on_serialEdit_editingFinished()
{
   QString serial_text(ui->serialEdit->text());
   serial_text.resize(sizeof(self.serial_number),'0');
   memcpy(&self.serial_number,serial_text.toLocal8Bit().data(),sizeof(self.serial_number));
   refreshWindow();
}

void clientWindow::on_ipEdit_editingFinished()
{
    int ip_num=inet_addr(ui->ipEdit->text().toStdString().c_str());
    memcpy(&self.ipaddr,&ip_num,sizeof(self.ipaddr));
    refreshWindow();
}

void clientWindow::on_maskEdit_editingFinished()
{
  int ip_num=inet_addr(ui->maskEdit->text().toStdString().c_str());
  memcpy(&self.netmask,&ip_num,sizeof(self.netmask));
  refreshWindow();
}

void clientWindow::on_gatewayEdit_editingFinished()
{
  int ip_num=inet_addr(ui->gatewayEdit->text().toStdString().c_str());
  memcpy(&self.gateway,&ip_num,sizeof(self.gateway));
  refreshWindow();
}



void clientWindow::on_nameEdit_editingFinished()
{
    self.setName(ui->nameEdit->text().toStdString().c_str());
    refreshWindow();
}
