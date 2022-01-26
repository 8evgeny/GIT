#include <gt/mvc.h>
#include "include/model.h"
#include "include/view.h"
#include "classes/algo.h"
#include "include/model.h"
#include "classes/defines.h"
#include "messages/dialogs/delete_tracks/delete_tracks_dialog.h"
#include "messages/model_tracks/tracks_model.h"
#include "messages/model_tracks/tracks_filter_model.h"
#include "messages_dialog.h"
#include "messages_dialog_instance.h"
#include "ui_messages_dialog.h"
#include "include/settings/last_settings.h"
#include "classes/app.h"
#include "include/log.h"
#include "classes/json.h"

MessagesDialog::MessagesDialog(QWidget* parent)
    : inherited(parent, Qt::FramelessWindowHint)
    , m_ui(new ui_t)
    , m_model(new TracksModel())
    , m_fmodel(new TracksFilterModel())
{
    m_ui->setupUi(this);    
    messagesDialogInstance(m_ui, m_model, m_fmodel, this);
    mh::simple::view::selectRow(m_ui->m_view, m_model->rowCount()-1);
    m_index = mh::selected(m_ui->m_view);
    LOG_ERROR(m_index.isValid(), "m_index must be is valid!");    
    initSignals();        
    auto&& file = settings::allProjectsFile();
    auto const& json = json::load(file.first()).object();
    if (!json.isEmpty()) {
        auto const& tracks = json.value(IPS_STR("Messages"));
        auto arr = tracks.toArray();
        if (!arr.isEmpty())
            m_model->deserialize(arr);
    }
    createCheckBoxes();
}
MessagesDialog::~MessagesDialog()
{    
    delete m_ui;
    delete m_model;
    delete m_fmodel;     
}
void MessagesDialog::createCheckBoxes() noexcept
{    
    m_ui->pb_remove->setDisabled(true);
    for (auto i=0; i< m_model->rowCount(); ++i) {
        auto indButton = m_model->index(i, 0);
        m_model->setData(indButton, false, TracksModel::TrackCheckedRole);
        m_check = new QCheckBox(m_ui->m_view);
        m_ui->m_view->setIndexWidget(indButton, new QWidget());
        QHBoxLayout* box = new QHBoxLayout(m_ui->m_view->indexWidget(indButton));
        box->addWidget(m_check);
        m_check->setChecked(false);
        QObject::connect(m_check, &QCheckBox::clicked, [this, indButton] (bool is) {
            mh::simple::view::selectRow(m_ui->m_view, indButton.row());
            if (is)
                m_model->setData(indButton, true, TracksModel::TrackCheckedRole);
            else
                m_model->setData(indButton, false, TracksModel::TrackCheckedRole);
            m_ui->pb_remove->setEnabled(isCheckedTracks());
        });
    }
}
bool MessagesDialog::isCheckedTracks() noexcept
{
    bool status = false;
    for (auto i=0; i < m_model->items().size(); ++i) {
         auto indx = m_model->index(i, 0);
         status = m_model->data(indx, TracksModel::TrackCheckedRole).toBool();
    }
    return status;
}
void MessagesDialog::onFilterChanged(QString const& mess) noexcept
{
    m_fmodel->setFilter(mess);
}
void MessagesDialog::onRemoveClicked() noexcept
{
   QString names;

   for (auto i=0; i < m_model->items().size(); ++i) {
        auto indx = m_model->index(i, 0);
        auto const& status = m_model->data(indx, TracksModel::TrackCheckedRole).toBool();
        if (status) {
            auto const& name = m_model->data(indx, TracksModel::TrackNameRole).toString();
            names = names + IPS_STR(" ") + name;
            m_pos.append(i);
        }
   }

   DeleteTracksDialog dlg{names};
   if (dlg.exec() != DeleteTracksDialog::Accepted)
       return;

   m_model->removeRows(m_pos.first(), m_pos.count());
   if (m_model->rowCount() > 0)
        mh::simple::view::selectRow(m_ui->m_view, m_model->rowCount()-1);

   ips::algo::clearList<int>(m_pos);
   createCheckBoxes();
}
void MessagesDialog::onLoadClicked() noexcept
{    
    auto const& file = QFileDialog::getOpenFileName(this, iProject::tr("Load"), {}, IPS_STR("*.wav *.mp3"));
    if(!file.isEmpty()) {
        Track track(file);
        m_model->insertItem(m_model->rowCount(), track);
    }
    createCheckBoxes();
}
void MessagesDialog::onCloseClicked() noexcept
{    
    inherited::accept();
    if (m_model->rowCount() != m_model->countDefaults()) {
        App::model()->save(IPS_STR("first_settings"));
        settings::setLastProjectFile(IPS_STR("first_settings"));
    }
}
void MessagesDialog::initSignals() noexcept
{   
    QObject::connect(m_model, &TracksModel::onUpdateView, this, [this] {
        m_ui->m_view->repaint();
    });    
}
