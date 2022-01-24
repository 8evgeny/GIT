#include "serverwindow.hpp"
#include "ui_serverwindow.h"

#include <islp/yellmsg.hpp>

serverWindow::serverWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::serverWindow)
  , m_processor(this)
{
  ui->setupUi(this);
  connect(&m_processor,SIGNAL(Refreshed()),this,SLOT(refreshList()));
  m_processor.start();
}

serverWindow::~serverWindow()
{
  delete ui;
}

void serverWindow::refreshList()
{
  ui->stationListView->clear();
  foreach (auto x, m_processor.stations()) {
      ui->stationListView->addItem(x.getName()+" S/N:"+x.getSerialStr()+" Type:"+x.getTypeStr()+" IP:"+x.getIPaddr());
    }
}


void serverWindow::on_yellButton_released()
{
    m_processor.yellStations();
}
