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
    void parseInFolder();
    void addPdfItemsToList();

private slots:
    void on_pushButton_Save_clicked();
    void on_pushButton_Load_clicked();
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
    void on_listWidget_2_itemDoubleClicked(QListWidgetItem *item);
    void createSimLink(std::string nameDoc);

private:
    Ui::Widget *ui;
    QString previousText;
    std::map <std::string, std::string> config{};
    std::vector<std::string> vectorPDF;
};
#endif // WIDGET_H
