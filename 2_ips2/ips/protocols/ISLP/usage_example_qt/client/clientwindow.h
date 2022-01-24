#pragma once

#include <QMainWindow>
#include <intercomstation.hpp>
#include <tools/stationemulator.hpp>

QT_BEGIN_NAMESPACE
namespace Ui { class clientWindow; }
QT_END_NAMESPACE

class clientWindow : public QMainWindow
{
  Q_OBJECT

public:
  clientWindow(QWidget *parent = nullptr);
  ~clientWindow();

private slots:


  void on_serialEdit_editingFinished();

  void on_ipEdit_editingFinished();

  void on_maskEdit_editingFinished();

  void on_gatewayEdit_editingFinished();

  void on_nameEdit_editingFinished();

private:
  Ui::clientWindow *ui;
  intercomStation self;
  stationEmulator *client=nullptr;

  void refreshWindow();
};
