#pragma once

#include <QMainWindow>

#include <mcastRecieve.hpp>
#include <mcastSend.hpp>
#include <process/msgprocessor.hpp>

#define ST_MODE_STANDART  uint8_t(0xA0)
#define ST_MODE_EXTENDED  uint8_t(0xFA)

QT_BEGIN_NAMESPACE
namespace Ui { class serverWindow; }
QT_END_NAMESPACE

class serverWindow : public QMainWindow
{
  Q_OBJECT

public:
  serverWindow(QWidget *parent = nullptr);
  ~serverWindow();

private slots:
  void refreshList();
  void on_yellButton_released();

private:
  Ui::serverWindow *ui;

  msgProcessor m_processor;

};
