#include "delete_tracks_dialog.h"
#include "ui_delete_tracks_dialog.h"

DeleteTracksDialog::DeleteTracksDialog(QString const& name, QWidget* parent)
    : QDialog(parent, Qt::FramelessWindowHint)
    , ui(new Ui::DeleteTracksDialog)
{
    ui->setupUi(this);
    ui->m_trackName->setReadOnly(true);
    ui->m_trackName->setText(name);
}
DeleteTracksDialog::~DeleteTracksDialog()
{
    delete ui;
}
