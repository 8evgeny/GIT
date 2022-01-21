#include "main_window.h"
#include "classes/app.h"
#include "include/view.h"
#include "classes/json.h"
#include "ui_main_window.h"
#include "classes/defines.h"
#include "main/login/login.h"
#include "include/file_types.h"
#include "dialogs/about/about.h"
#include "dialogs/rs232/rs232.h"
#include "dialogs/network/network.h"
#include "dialogs/settings/settings.h"
#include "programmer/programmer_dialog.h"
#include "include/settings/last_settings.h"
#include "dialogs/message_box/message_box.h"
#include "dialogs/save_project/saveproject.h"
#include "dialogs/instruments/test/ipstest.h"
#include "classes/base_model/items/root_item.h"
#include "dialogs/instruments/firmware/firmware.h"
#include "main/messages/dialogs/messages/messages_dialog.h"
#include "classes/base_model/items/interfaces/interfaces_item.h"
#include "classes/base_model/items/stations_item/stations_item.h"
#include "classes/base_model/items/project_info_item/project_info_item.h"
#include "classes/base_model/items/interfaces/sip_stations_item/output_connections_item/output_connections_item.h"
#include "classes/base_model/items/interfaces/sip_stations_item/input_connections_item/input_connections_item.h"

#include <QIcon>
#include <string.h>

MainWindow::MainWindow(App*, QWidget* parent)
    : inherited(parent, Qt::FramelessWindowHint)
    , m_dest(new (unsigned char[1024]))
    , m_destLen(new (unsigned long[1024]))
    , m_destUn(new (unsigned char[1024]))
    , m_destLenUn(new (unsigned long[1024]))
    , ui(new Ui::MainWindow)
{        
    ui->setupUi(this);
    initSignals();
    createMenuFile();
    createMenuInstruments();    
    ui->tw_tree->setModel(App::model());
    App::model()->setTree(ui->tw_tree);
    App::model()->setContent(ui->tw_content);
    App::model()->setSettings(ui->tw_settings);
    App::model()->selectProjectInfo(ui->tw_content, ui->tw_settings);
    m_status = ui->m_statusbar;
    m_bar = new StatusBar(m_status);
    ui->c_compress->setIcon(QIcon(IPS_STR(":/svg/compress.png")));
    ui->sp1->restoreState(App::settings()->value(IPS_STR("main_window/splitter1")).toByteArray());
    ui->sp2->restoreState(App::settings()->value(IPS_STR("main_window/splitter2")).toByteArray());
    ui->tw_tree->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tw_tree->setSelectionBehavior(QAbstractItemView::SelectRows);
    mh::simple::view::selectRow(ui->tw_tree, 0);        
    QObject::connect(App::model(), &BaseModel::rowsAboutToBeRemoved, [this] (const QModelIndex& parent, int, int) {
        if (!(App::model()->root()->child(2)->childCount()-1))
            ui->tw_tree->setExpanded(parent, false);
    });
    QObject::connect(ui->sp1, &QSplitter::splitterMoved, [this] (int, int) {       
        App::settings()->setVal(IPS_STR("main_window/splitter1"), ui->sp1->saveState());
    });
    QObject::connect(ui->sp2, &QSplitter::splitterMoved, [this] (int, int) {       
        App::settings()->setVal(IPS_STR("main_window/splitter2"), ui->sp2->saveState());        
    });
    QObject::connect(ui->c_compress, &QPushButton::clicked, [this] (bool) {
          if (m_screenState) {showNormal(); m_screenState = false;}
          else {showFullScreen();m_screenState = true;}
    });
    QObject::connect(ui->f_folding, &QPushButton::clicked, [this] (bool) {
        if (m_screenState) {showNormal(); m_screenState = false;}
        else {showFullScreen();m_screenState = true;}
    });
    QObject::connect(ui->f_folding, &QPushButton::clicked, [this] (bool) {showMinimized();});
    updateUserFile();
}
void MainWindow::updateUserFile() noexcept
{
    App::settings()->setValue(IPS_STR("UserFile"), App::model()->document());
}
void MainWindow::changeEvent(QEvent* event)
{
    IPS_LANGUGE_EVENT(ui, this)
    inherited::changeEvent(event);
}
MainWindow::~MainWindow()
{
    delete[] m_dest;
    delete[] m_destLen;
    delete[] m_destUn;
    delete[] m_destLenUn;
    delete ui;
    delete m_status;
    delete m_bar;
}
void MainWindow::initRecentProjects(QMenu& menu, QMenu* sub) noexcept
{
    for (auto&& file: settings::allProjectsFile()) {
        QFileInfo info(file);
        if (info.exists()) {            
            sub->addAction(info.fileName(), [this, file] () {
                if (checkSaveProject())
                   this->loadProject(file);
            });
        }
    }
    if (!sub->isEmpty())
        menu.addMenu(sub);
}
void MainWindow::createMenuFile()
{
    QObject::connect(ui->b_file, &QPushButton::clicked, [this]() {
        QMenu menu(ui->b_file);
        menu.addAction(iMenu::tr("&New project"), [this]() {        
        if (checkSaveProject()) {            
            if (m_StationItem) {
                m_StationItem = false;
                QModelIndex index = App::model()->index(2, 0, {});
                App::model()->itemClick(index, ui->tw_content, ui->tw_settings);
            }
            for (auto const& item: App::model()->root()->items()) {
                item->clear();
                m_bar->clear();
                m_file.clear();
        }
        App::model()->sipStationsItem()->clear();
        App::model()->inputConnectionsItem()->clear();
        App::model()->outputConnectionsItem()->clear();
        App::model()->selectProjectInfo(ui->tw_content, ui->tw_settings);
        updateUserFile();
        mh::simple::view::selectRow(ui->tw_tree, 0);
        }
        });
        menu.addAction(iMenu::tr("&Load project"), this, SLOT(loadProject()));
        menu.addAction(iMenu::tr("&Save project"), this, SLOT(saveProject()))->setEnabled(!m_file.isEmpty());
        menu.addAction(iMenu::tr("Save project as"), this, SLOT(saveProjectAs()));
        //menu.addAction(iMenu::tr("&Network")
        //               , IPS_FUNCT(NetworkDialog().exec()));
        menu.addSeparator();
        QMenu sub(iMenu::tr("Recent Projects"), &menu);
        initRecentProjects(menu, &sub);
        menu.addSeparator();
        menu.addAction(iMenu::tr("&Exit"), this, [this] () {if(checkSaveProject(true)) App::exit();});
        menu.exec(mapToGlobal(ui->b_file->geometry().bottomLeft()));
    });
}
void MainWindow::createMenuInstruments()
{
    ips::algo::removeChildren(ui->b_instruments);
    QObject::connect(ui->b_instruments, &QPushButton::clicked, [this] () {
       QMenu menu(ui->b_instruments);
       menu.addAction(iMenu::tr("Check project"));
       menu.addAction(iMenu::tr("Check device"));
       menu.addAction(IPS_STR("RS232"), IPS_FUNCT(Rs232Dialog().exec()));
       menu.addAction(iMenu::tr("FirmWare"), IPS_FUNCT(FirmwareDialog().exec()));
       menu.addAction(iMenu::tr("Programmer"), IPS_THIS_FUNCT(ProgrammerDialog(App::model()->stationsItem(), this).exec()));
       menu.addAction(iMenu::tr("Test"), IPS_FUNCT(IpsTestDialog().exec()));
       menu.addAction(iMenu::tr("Service"), IPS_FUNCT(IpsTestDialog().exec()));
       menu.addAction(iMenu::tr("Messages"), [] () {
           auto dlg = App::messages();
           LOG_FATAL(dlg, "dlg can not be nullptr");
           dlg->exec();});
       menu.exec(this->mapToGlobal(ui->b_instruments->geometry().bottomLeft()));
    });
}
void MainWindow::loadProject(QString const& file)
{
    if (file.isEmpty()) {
        updateUserFile();
        return;}

    if (App::model()->load(file)) {        
        m_file = file;               
        settings::setLastProjectFile(file);
        m_bar->setProject(file);
        mh::simple::view::selectRow(ui->tw_tree, 0);
        IpsMessageBox::info(this, iMessage::tr("Project %1 has been sucsessfully loaded.").arg(file));
    }
    else
        IpsMessageBox::error(this, iMessage::tr("An error occurrated while loading the '%1' file!").arg(file));
}
void MainWindow::loadProject()
{
    if (checkSaveProject()) {
    auto const& file = QFileDialog::getOpenFileName(this, iProject::tr("Open a project...")
                                    , settings::lastProjectFile(), FileTypes::projectsFiles());        
        if (!file.isEmpty())
            loadProject(file);
    }
}
void MainWindow::saveProject()
{       
    LOG_ERROR(!m_file.isEmpty(), "Invalid FileName %1", m_file);
    settings::setLastProjectFile(m_file);

    if (App::model()->save(m_file)) {
        IpsMessageBox::info(this, iMessage::tr("File '%1 has been sucsessfully saved.").arg(m_file));
    }
    else
        IpsMessageBox::error(this, iMessage::tr("An error occurrated while saving the '%1' file!").arg(m_file));
}
bool MainWindow::saveProjectAs()
{   
    auto file = QFileDialog::getSaveFileName(this, iProject::tr("Save a project as...")
                                    , settings::lastProjectFile(), FileTypes::projectsFiles()); 

    if (file.isEmpty())
        return false;

    if (!file.endsWith(IPS_STR(".ips"))) {
        file = IPS_STR("%1.ips").arg(file);
        return false;
    }

    if (App::model()->save(file)) {                
        settings::setLastProjectFile(file);
        if (m_file.isEmpty() || !file.isEmpty()) {
           m_bar->setProject(file);
           m_file = file;
        }
        IpsMessageBox::info(this, iMessage::tr("File '%1 has been sucsessfully saved.").arg(file));
    }
    else
        IpsMessageBox::error(this, iMessage::tr("An error occurrated while saving the '%1' file!").arg(file));
    return true;
}
bool MainWindow::checkHashProject()
{    
    return (App::settings()->value(IPS_STR("UserFile")) == App::model()->document()) || (json::load(m_file) == App::model()->document());
}
bool MainWindow::checkSaveProject(bool is)
{    
    if (checkHashProject())
        return true;    

    SaveProjectDialog dlg;
    auto const& exec = dlg.exec();
    if (exec == SaveProjectDialog::Rejected && is)
       App::exit();

    if (exec == SaveProjectDialog::Rejected)
       return true;

    return (!checkHashProject() && (exec == SaveProjectDialog::Accepted)) && saveProjectAs();
}
void MainWindow::initSignals()
{
    QObject::connect(ui->tw_tree, SIGNAL(clicked(const QModelIndex&)), this, SLOT(onTreeViewClicked(const QModelIndex&)));
    QObject::connect(ui->b_about, &QPushButton::clicked, IPS_FUNCT(AboutDialog().exec()));
    //QObject::connect(ui->b_settings, &QPushButton::clicked, IPS_FUNCT(SettingsDialog().exec()));
    QObject::connect(ui->b_exit, &QPushButton::clicked, [this] {if(checkSaveProject(true)) App::exit();});
    QObject::connect(ui->b_lang, &QPushButton::clicked, [this]() {
        auto is = options::Language::current().isCurrentEnglish();
        QMenu menu(ui->b_lang);
        menu.addAction(iInfo::tr("Russian"), IPS_FUNCT(App::setLanguage(russianId())))->setEnabled(is);
        menu.addAction(iInfo::tr("English"), IPS_FUNCT(App::setLanguage(englishId())))->setEnabled(!is);
        menu.exec(mapToGlobal(ui->b_lang->geometry().bottomLeft()));
    });
}
void MainWindow::onTreeViewClicked(const QModelIndex& index)
{
    if (index.parent().isValid())
        m_StationItem = true;

    App::model()->itemClick(index, ui->tw_content, ui->tw_settings);
}
