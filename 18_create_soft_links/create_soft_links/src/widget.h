#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtWidgets>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void readConfig(const char* conf_file);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_listWidget_itemChanged(QListWidgetItem *item);
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

private:
    Ui::Widget *ui;
    QString previousText;
    std::map <std::string, std::string> config{};
};
#endif // WIDGET_H
