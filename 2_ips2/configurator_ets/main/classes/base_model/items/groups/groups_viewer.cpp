#include "groups_viewer.h"
#include "ui_groups_item.h"

GroupsViewer::GroupsViewer(Ui::GroupsItem* ui)
    :ui(ui)
{
    qDebug() << "GroupsViewer____Constructor______ = ";    
    ui->m_table_view->setModel(App::model()->stationsItem()->model());    
    settings();
}
void GroupsViewer::settings() noexcept
{
    ui->m_table_view->verticalHeader()->hide();
    ui->m_table_view->setShowGrid(false);
    ui->m_table_view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->m_table_view->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->m_table_view->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
}
