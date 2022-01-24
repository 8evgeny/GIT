#include "limitrows.h"
#include "ui_limitrows.h"

LimitRowsDialog::LimitRowsDialog(QWidget* parent) :
    QDialog(parent, Qt::FramelessWindowHint),
    ui(new Ui::LimitRows)
{
    ui->setupUi(this);
    QCursor cursor;
    cursor.setShape(Qt::PointingHandCursor);
    ui->b_ok->setCursor(cursor);
    QObject::connect(ui->b_ok, &QPushButton::clicked, [this] (bool ok) {
        if (!ok)
            this->close();
    });
}
LimitRowsDialog::~LimitRowsDialog()
{
    delete ui;
}
