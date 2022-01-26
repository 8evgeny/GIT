#include <QTest>
#include <QDialogButtonBox>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "main_window.h"
#include "classes/app.h"
#include "include/view.h"
#include "classes/json.h"
#include "ui_main_window.h"
#include "classes/defines.h"
#include "main/login/login.h"
#include "include/file_types.h"
#include "dialogs/about/about.h"
#include "dialogs/setupconnectionsdlg/setupconnections.h"
#include "dialogs/fmwaredlg/fmwaredlg.h"
#include "dialogs/configdlg/configuredlg.h"
#include "dialogs/debugdlg/debugviewdlg.h"
#include "dialogs/servicedlg/serviceviewdlg.h"
#include "dialogs/network_configuring/network_configuring.h"
#include "dialogs/network/network.h"
#include "dialogs/settings/settings.h"
#include "programmer/programmer_dialog.h"
#include "include/settings/last_settings.h"
#include "dialogs/message_box/message_box.h"
#include "dialogs/save_project/saveproject.h"
#include "dialogs/instruments/test/ipstest.h"
#include "dialogs/cryptodlg/test_cryptodlg.h"
#include "classes/base_model/items/root_item.h"
#include "dialogs/instruments/firmware/firmware.h"
#include "main/messages/dialogs/messages/messages_dialog.h"
#include "classes/base_model/items/interfaces/interfaces_item.h"
#include "classes/base_model/items/stations_item/stations_item.h"
#include "classes/base_model/items/project_info_item/project_info_item.h"
#include "classes/base_model/items/interfaces/sip_stations_item/output_connections_item/output_connections_item.h"
#include "classes/base_model/items/interfaces/sip_stations_item/input_connections_item/input_connections_item.h"
#include "classes/base_model/items/station_item/station_item_settings/citgit_helper.h"

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
	createMenuBarInstruments();
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
//            sub->addAction(info.fileName(), [this, file] () {
            sub->addAction(file, [this, file] () {
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
//        App::model()->sipStationsItem()->clear();
//        App::model()->inputConnectionsItem()->clear();
//        App::model()->outputConnectionsItem()->clear();
        App::model()->selectProjectInfo(ui->tw_content, ui->tw_settings);
        updateUserFile();
        mh::simple::view::selectRow(ui->tw_tree, 0);
        }
        });
        menu.addAction(iMenu::tr("&Load project"), this, SLOT(loadProject()));
        menu.addAction(iMenu::tr("&Save project"), this, SLOT(saveProject()))->setEnabled(!m_file.isEmpty());
        menu.addAction(iMenu::tr("Save project as"), this, SLOT(saveProjectAs()));
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
//    ips::algo::removeChildren(ui->b_instruments);
    QObject::connect(ui->b_instruments, &QPushButton::clicked, [this] () {
       pMenuInstruments->exec(this->mapToGlobal(ui->b_instruments->geometry().bottomLeft()));
    });
}

void MainWindow::RS232SetupDlg()
{
	SetupConnectionsDialog* _dlg = new SetupConnectionsDialog(this, RS232Mode);
	_dlg->setConnectionsParamStr(ConnectionsParamStr);
	if (_dlg->exec() == QDialog::Accepted){
		ConnectionsParamStr = _dlg->getConnectionsParamStr();
		pActionRs232Dlg->setText(QObject::tr("RS232(%1)").arg(ConnectionsParamStr));
		App::settings()->setValue(QString::fromLocal8Bit("communication/channel"), QString::fromLocal8Bit("comport"));
		App::settings()->setValue(QString::fromLocal8Bit("communication/paramStr"), ConnectionsParamStr);
	}
}

void MainWindow::NetworkSetupDlg()
{
	SetupConnectionsDialog* _dlg = new SetupConnectionsDialog(this, NetworkMode);
	_dlg->setConnectionsParamStr(ConnectionsParamStr);
	if (_dlg->exec() == QDialog::Accepted){
		ConnectionsParamStr = _dlg->getConnectionsParamStr();
		pActionNetworkDlg->setText(QObject::tr("Network(%1)").arg(ConnectionsParamStr));
		App::settings()->setValue(QString::fromLocal8Bit("communication/channel"), QString::fromLocal8Bit("ethernet"));
		App::settings()->setValue(QString::fromLocal8Bit("communication/paramStr"), ConnectionsParamStr);
	}
}

void MainWindow::ConfigureDlg()
{
	if (pActionRs232Dlg->isChecked()){
		RS232ConfigDlg();
	} else {
		NetworkConfigDlg();
	}
}

void MainWindow::RS232ConfigDlg()
{
	QString newConfStr;
	QString currentConfStr;
	currentConfStr = App::model()->getConfigureInfo();
	ConfigureDialog* _dlg = new ConfigureDialog(this);
	_dlg->setConfigureStr(currentConfStr);
	_dlg->setTitle(QObject::tr("RS232"));
	_dlg->setMode(RS232Mode);
	_dlg->setRS232Name(ConnectionsParamStr);
	 if (_dlg->exec() == QDialog::Accepted){
		 if (_dlg->isAcceptLoaddedConfigure()){
			 newConfStr = _dlg->getConfigureStr();
			 addStationConfig(currentConfStr, newConfStr);
		 }
	 }
	delete _dlg;
}
void MainWindow::NetworkConfigDlg()
{
	QString newConfStr;
	QString currentConfStr;
	currentConfStr = App::model()->getConfigureInfo();
	ConfigureDialog* _dlg = new ConfigureDialog(this);
	_dlg->setConfigureStr(currentConfStr);
	_dlg->setTitle(QObject::tr("Network"));
	_dlg->setMode(NetworkMode);
	_dlg->setSelfIP(ConnectionsParamStr);
	if (_dlg->exec() == QDialog::Accepted){
		 if (_dlg->isAcceptLoaddedConfigure()){
			 newConfStr = _dlg->getConfigureStr();
			 addStationConfig(currentConfStr, newConfStr);
		 }
	}
	delete _dlg;
}

void MainWindow::FmWareDlg()
{
	if (pActionRs232Dlg->isChecked()){
		RS232FmWareDlg();
	} else {
		NetworkFmwareDlg();
	}
}

void MainWindow::RS232FmWareDlg()
{
	QString newConfStr;
	QString currentConfStr;
	currentConfStr = App::model()->getConfigureInfo();
	FmWareDialog* _dlg = new FmWareDialog(this);
	_dlg->setConfigureStr(currentConfStr);
	_dlg->setTitle(QObject::tr("RS232"));
	_dlg->setMode(RS232Mode);
	_dlg->setRS232Name(ConnectionsParamStr);
	 _dlg->exec();
	delete _dlg;
}

void MainWindow::NetworkFmwareDlg()
{
	QString newConfStr;
	QString currentConfStr;
	currentConfStr = App::model()->getConfigureInfo();
	FmWareDialog* _dlg = new FmWareDialog(this);
	_dlg->setConfigureStr(currentConfStr);
	_dlg->setTitle(QObject::tr("Network"));
	_dlg->setMode(NetworkMode);
	_dlg->setSelfIP(ConnectionsParamStr);
	_dlg->exec();
	delete _dlg;
}

void MainWindow::DebugDlg()
{
	if (pActionRs232Dlg->isChecked()){
		RS232DebugDlg();
	} else {
		NetworkDebugDlg();
	}
}

void MainWindow::RS232DebugDlg()
{
	QString currentConfStr;
	currentConfStr = App::model()->getConfigureInfo();
	DebugViewDialog* _dlg = new DebugViewDialog(this);
	_dlg->setConfigureStr(currentConfStr);
	_dlg->setTitle(QObject::tr("RS232"));
	_dlg->setMode(RS232Mode);
	_dlg->setRS232Name(ConnectionsParamStr);
	 _dlg->exec();
	delete _dlg;
}

void MainWindow::NetworkDebugDlg()
{
	QString currentConfStr;
	currentConfStr = App::model()->getConfigureInfo();
	DebugViewDialog* _dlg = new DebugViewDialog(this);
	_dlg->setConfigureStr(currentConfStr);
	_dlg->setTitle(QObject::tr("Network"));
	_dlg->setMode(NetworkMode);
	_dlg->setSelfIP(ConnectionsParamStr);
	_dlg->exec();
	delete _dlg;
}

void MainWindow::ServiceDlg()
{
	if (pActionRs232Dlg->isChecked()){
		RS232ServiceDlg();
	} else {
		NetworkServiceDlg();
	}
}

void MainWindow::RS232ServiceDlg()
{
	QString currentConfStr;
	currentConfStr = App::model()->getConfigureInfo();
	ServiceViewDialog* _dlg = new ServiceViewDialog(this);
	_dlg->setConfigureStr(currentConfStr);
	_dlg->setTitle(QObject::tr("RS232"));
	_dlg->setMode(RS232Mode);
	_dlg->setRS232Name(ConnectionsParamStr);
	 _dlg->exec();
	delete _dlg;
}

void MainWindow::NetworkServiceDlg()
{
	QString currentConfStr;
	currentConfStr = App::model()->getConfigureInfo();
	ServiceViewDialog* _dlg = new ServiceViewDialog(this);
	_dlg->setConfigureStr(currentConfStr);
	_dlg->setTitle(QObject::tr("Network"));
	_dlg->setMode(NetworkMode);
	_dlg->setSelfIP(ConnectionsParamStr);
	_dlg->exec();
	delete _dlg;
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
		updateUserFile();
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
	Q_UNUSED(is);
    if (checkHashProject())
        return true;    

    SaveProjectDialog dlg;
//    auto const& exec = dlg.exec();
	dlg.exec();
	QDialogButtonBox::ButtonRole btnRole = dlg.getExecRole();
	if (btnRole == QDialogButtonBox::RejectRole)
		return false;
	if (btnRole == QDialogButtonBox::NoRole)
		return true;
	if (btnRole == QDialogButtonBox::YesRole){
		return saveProjectAs();
	}
/*
    if (exec == SaveProjectDialog::Rejected && is)
       App::exit();

    if (exec == SaveProjectDialog::Rejected)
       return true;

    return (!checkHashProject() && (exec == SaveProjectDialog::Accepted)) && saveProjectAs();*/
	return true;
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

void MainWindow::addStationConfig(QString strCurrentStationsConfig, QString strNewStationConfig){
	Q_UNUSED(strCurrentStationsConfig);
//	QMessageBox::StandardButton ret;
	int rcvCidStation, newRcvCidStation;
	QString rcvCidStationStr;
	QJsonDocument doc = QJsonDocument::fromJson(strNewStationConfig.toLocal8Bit());
	QJsonObject srcMapObj = doc.object();
	QJsonObject srcGlobalParametersObj = srcMapObj[QString::fromLocal8Bit("Global parameters")].toObject();
	int _verNewConf = srcGlobalParametersObj[QString::fromLocal8Bit("version")].toInt();
	if (_verNewConf != 1){
		IpsMessageBox::error(this, iMessage::tr("Version configure impossible\n read version configure: %1").arg(_verNewConf));
		return;
	}
//Массив описаний кнопок станции в считанном объекте
	QJsonArray srcArrActivebuttons;
//Массив описаний групповых вызовов станции в считанном объекте
	QJsonArray dstArrGroupCalls;
//Целевой объект - станция
	QJsonObject dstStation;
//Объект "Stations" для интерфейса/ Включает в себя массив из одной считанной станции
	QJsonObject dstStations;
//Массив описания станций. В данном случае одной - считанной
	QJsonArray dstListStations;
//Объект - значение описания кнопки станции
	QJsonObject tmpObj;
//Массив описаний кнопок для интерфейса
	QJsonArray dstArrStationButton;
//Объект описание массива кнопок для интерфейса
	QJsonObject dstItems;
//вычитываем и формируем параметры станции для интерфейса
	dstStation[QString::fromLocal8Bit("cid")] = srcGlobalParametersObj[QString::fromLocal8Bit("cid")];
	rcvCidStation = srcGlobalParametersObj[QString::fromLocal8Bit("cid")].toInt();
	dstStation[QString::fromLocal8Bit("gateway")] = srcGlobalParametersObj[QString::fromLocal8Bit("gateway")];
	dstStation[QString::fromLocal8Bit("ip")] = srcGlobalParametersObj[QString::fromLocal8Bit("ip")];
	dstStation[QString::fromLocal8Bit("mask")] = srcGlobalParametersObj[QString::fromLocal8Bit("mask")];
	dstStation[QString::fromLocal8Bit("name")] = srcGlobalParametersObj[QString::fromLocal8Bit("name")];
	dstStation[QString::fromLocal8Bit("type")] = srcGlobalParametersObj[QString::fromLocal8Bit("type")];
	dstStation[QString::fromLocal8Bit("SN")] = srcGlobalParametersObj[QString::fromLocal8Bit("SN")];
	srcArrActivebuttons = srcMapObj[QString::fromLocal8Bit("activebuttons")].toArray();
	foreach (const QJsonValue & value, srcArrActivebuttons) {
		tmpObj = value.toObject();
		tmpObj.remove(QString::fromLocal8Bit("destination_ip"));
		tmpObj[QString::fromLocal8Bit("activate_lines")] = QString::fromLocal8Bit("");
		tmpObj[QString::fromLocal8Bit("pretone_checked")] = QString::fromLocal8Bit("0");
		tmpObj[QString::fromLocal8Bit("pretone_duration")] = 2;
        tmpObj[QString::fromLocal8Bit("pretone_id")] = 0;
        tmpObj[QString::fromLocal8Bit("pretone_times")] = 0;
		dstArrStationButton.append(tmpObj);
	}
	dstItems[QString::fromLocal8Bit("activestation")] = dstArrStationButton;
	dstStation[QString::fromLocal8Bit("items")] = dstItems;
	dstStation[QString::fromLocal8Bit("hash")] = srcGlobalParametersObj[QString::fromLocal8Bit("hash")];

	dstListStations.append(dstStation);
	dstStations[QString::fromLocal8Bit("Stations")] = dstListStations;
	if (findStation(strCurrentStationsConfig, rcvCidStation)){
		rcvCidStationStr.setNum(rcvCidStation);
		rcvCidStationStr.prepend(QString::fromLocal8Bit("CID"));
//		ret = QMessageBox::warning(this, QObject::tr("Warning"), QObject::tr("Station (%1) found with accepted configuration\nReplace (Yes/No)?").arg(rcvCidStationStr), QMessageBox::Yes | QMessageBox::No , QMessageBox::No);
//		if (ret == QMessageBox::Yes){
//вычитываем из исходной конфигурации секцию  "Stations" и заменяем в ней итем с принятой конфигурацией
//			ReplaceStationInfo(strCurrentStationsConfig, dstStation, rcvCidStation);
//			QMessageBox::information(this, QObject::tr("Info"), QObject::tr("Information of the station (CID%1) is replace").arg(rcvCidStation));
//		} else {
//вычисляем максимальный индекс станции, модифицируем cid принятой конфигурации на более высший и десериализуем
			newRcvCidStation = addStationInfo(strCurrentStationsConfig, dstStation);
			QMessageBox::information(this, QObject::tr("Info"), QObject::tr("Information of the station (CID%1) addeded with number: CID%2").arg(rcvCidStation).arg(newRcvCidStation));
//		}
	} else
		App::model()->stationsItem()->deserialize(dstStations);

/*	QJsonDocument d_temp = QJsonDocument(dstStations);
	QByteArray baRxConfiguration = d_temp.toJson();
	QString _fl = QString::fromLocal8Bit("rcv_cnf_1.txt");
	QFile* _file = new QFile(_fl);
    if (!_file->open(QIODevice::WriteOnly | QIODevice::Text)){
        return;
	}
	_file->write(baRxConfiguration);
	_file->close();
	delete _file;
	*/
}

void MainWindow::createMenuBarInstruments(){
QString _com_channel = QString::fromLocal8Bit("comport");
	ConnectionsParamStr = QString::fromLocal8Bit("");
	pMenuInstruments = new QMenu(ui->b_instruments);
	menuSetupConnections = pMenuInstruments->addMenu(iMenu::tr("Setup connections"));
//	menuSetupConnections = new QMenu(iMenu::tr("Setup connections"), pMenuInstruments);
	pTypeChannelAG = new QActionGroup(menuSetupConnections);
	pActionRs232Dlg = menuSetupConnections->addAction(IPS_STR("RS232"));
	connect(pActionRs232Dlg, SIGNAL(triggered()), this, SLOT(RS232SetupDlg()));
	pActionRs232Dlg->setCheckable(true);
	pTypeChannelAG->addAction(pActionRs232Dlg);
	pActionNetworkDlg = menuSetupConnections->addAction(IPS_STR("Network"));
	connect(pActionNetworkDlg, SIGNAL(triggered()), this, SLOT(NetworkSetupDlg()));
	pActionNetworkDlg->setCheckable(true);
	pTypeChannelAG->addAction(pActionNetworkDlg);
	pTypeChannelAG->setExclusive(true);
	_com_channel = App::settings()->value(QString::fromLocal8Bit("communication/channel"), QString::fromLocal8Bit("comport")).toString();
	ConnectionsParamStr = App::settings()->value(QString::fromLocal8Bit("communication/paramStr"), QString::fromLocal8Bit("")).toString();
	if(_com_channel.indexOf(QString::fromLocal8Bit("comport")) != -1){
		pActionRs232Dlg->setChecked(true);
		pActionRs232Dlg->setText(QObject::tr("RS232(%1)").arg(ConnectionsParamStr));
	} else {
		pActionNetworkDlg->setChecked(true);
		pActionNetworkDlg->setText(QObject::tr("Network(%1)").arg(ConnectionsParamStr));
	}
	pMenuInstruments->addAction(iMenu::tr("Configure"), this, SLOT(ConfigureDlg()));
	pMenuInstruments->addAction(iMenu::tr("FirmWare"), this, SLOT(FmWareDlg()));
	pMenuInstruments->addAction(iMenu::tr("Test"), IPS_FUNCT(IpsTestDialog().exec()))->setVisible(false);
	pMenuInstruments->addAction(iMenu::tr("Service"), this, SLOT(ServiceDlg()));
	pMenuInstruments->addAction(iMenu::tr("Debug"),  this, SLOT(DebugDlg()));

/*QString _com_channel = QString::fromLocal8Bit("comport");
	ConnectionsParamStr = QString::fromLocal8Bit("");
	pMenuInstruments = new QMenu(ui->b_instruments);
	pMenuInstruments->addAction(iMenu::tr("Check project"))->setVisible(false);
	pMenuInstruments->addAction(iMenu::tr("Check device"))->setVisible(false);
	pMenuInstruments->addSeparator();
	QMenu* menuSetupConnections = pMenuInstruments->addMenu(iMenu::tr("Setup connections"));
	pTypeChannelAG = new QActionGroup(menuSetupConnections);
	pActionRs232Dlg = menuSetupConnections->addAction(IPS_STR("RS232"));
	connect(pActionRs232Dlg, SIGNAL(triggered()), this, SLOT(RS232SetupDlg()));
	pActionRs232Dlg->setCheckable(true);
	pTypeChannelAG->addAction(pActionRs232Dlg);
	pActionNetworkDlg = menuSetupConnections->addAction(IPS_STR("Network"));
	connect(pActionNetworkDlg, SIGNAL(triggered()), this, SLOT(NetworkSetupDlg()));
	pActionNetworkDlg->setCheckable(true);
	pTypeChannelAG->addAction(pActionNetworkDlg);
	pTypeChannelAG->setExclusive(true);
	_com_channel = App::settings()->value(QString::fromLocal8Bit("communication/channel"), QString::fromLocal8Bit("comport")).toString();
	ConnectionsParamStr = App::settings()->value(QString::fromLocal8Bit("communication/paramStr"), QString::fromLocal8Bit("")).toString();
	if(_com_channel.indexOf(QString::fromLocal8Bit("comport")) != -1){
		pActionRs232Dlg->setChecked(true);
		pActionRs232Dlg->setText(QObject::tr("RS232(%1)").arg(ConnectionsParamStr));
	} else {
		pActionNetworkDlg->setChecked(true);
		pActionNetworkDlg->setText(QObject::tr("Network(%1)").arg(ConnectionsParamStr));
	}
	pMenuInstruments->addAction(iMenu::tr("Configure"), this, SLOT(ConfigureDlg()));
	pMenuInstruments->addAction(iMenu::tr("FirmWare"), this, SLOT(FmWareDlg()));
	pMenuInstruments->addAction(iMenu::tr("Test"), IPS_FUNCT(IpsTestDialog().exec()))->setVisible(false);
	pMenuInstruments->addAction(iMenu::tr("Service"), this, SLOT(ServiceDlg()));
	pMenuInstruments->addAction(iMenu::tr("Debug"),  this, SLOT(DebugDlg()));
	pMenuInstruments->addSeparator();
//       pMenuInstruments->addAction(iMenu::tr("FirmWare"), IPS_FUNCT(FirmwareDialog().exec()));
	pMenuInstruments->addAction(iMenu::tr("Programmer"), IPS_THIS_FUNCT(ProgrammerDialog(App::model()->stationsItem(), this).exec()))->setVisible(false);
	pMenuInstruments->addAction(iMenu::tr("Messages"), [] () {
           auto dlg = App::messages();
           LOG_FATAL(dlg, "dlg can not be nullptr");
           dlg->exec();})->setVisible(false);*/
}

bool MainWindow::findStation(QString strCurrentStationsConfig, int cid){
	int cidName;
	QJsonDocument doc = QJsonDocument::fromJson(strCurrentStationsConfig.toLocal8Bit());
	QJsonObject mapObj = doc.object();
	QJsonArray listStations = mapObj[QString::fromLocal8Bit("Stations")].toArray();
	foreach (const QJsonValue & value, listStations) {
		QJsonObject obj = value.toObject();
		cidName	= obj[QString::fromLocal8Bit("cid")].toInt();
		if (cidName == cid)
			return true;
	}
	return false;
}

void MainWindow::ReplaceStationInfo(QString strCurrentStationsConfig, QJsonObject rcvStationInfo, int cid){
	int cidName;
	QJsonObject dstStationsObj;
	QJsonArray dstListStations;
	QJsonDocument doc = QJsonDocument::fromJson(strCurrentStationsConfig.toLocal8Bit());
	QJsonObject mapObj = doc.object();
	QJsonArray listStations = mapObj[QString::fromLocal8Bit("Stations")].toArray();
	foreach (const QJsonValue & value, listStations) {
		QJsonObject obj = value.toObject();
		cidName	= obj[QString::fromLocal8Bit("cid")].toInt();
		if (cidName == cid){
			dstListStations.append(rcvStationInfo);
		} else {
			dstListStations.append(obj);
		}
	}
	dstStationsObj[QString::fromLocal8Bit("Stations")] = dstListStations;
//	dstStationsObj[QString::fromLocal8Bit("Groups")] = mapObj[QString::fromLocal8Bit("Groups")];
	App::model()->stationsItem()->clear();
	App::model()->stationsItem()->deserialize(dstStationsObj);
}

int MainWindow::addStationInfo(QString strCurrentStationsConfig, QJsonObject rcvStationInfo){
	int cidName, newCidName = 0;
	QJsonDocument doc = QJsonDocument::fromJson(strCurrentStationsConfig.toLocal8Bit());
	QJsonObject mapObj = doc.object();
	QJsonObject rcvObj = doc.object();
	QJsonArray listStations = mapObj[QString::fromLocal8Bit("Stations")].toArray();
	QJsonArray dstListStations;
	QJsonObject dstObj;
	foreach (const QJsonValue & value, listStations) {
		QJsonObject obj = value.toObject();
		cidName	= obj[QString::fromLocal8Bit("cid")].toInt();
		if (cidName > newCidName){
			newCidName = cidName;
		}
	}
	newCidName++;
	rcvObj[QString::fromLocal8Bit("cid")] = newCidName;
	rcvObj[QString::fromLocal8Bit("gateway")] = rcvStationInfo[QString::fromLocal8Bit("gateway")];
	rcvObj[QString::fromLocal8Bit("hash")] = rcvStationInfo[QString::fromLocal8Bit("hash")];
	rcvObj[QString::fromLocal8Bit("ip")] = rcvStationInfo[QString::fromLocal8Bit("ip")];
	rcvObj[QString::fromLocal8Bit("items")] = rcvStationInfo[QString::fromLocal8Bit("items")];
	rcvObj[QString::fromLocal8Bit("mask")] = rcvStationInfo[QString::fromLocal8Bit("mask")];
	rcvObj[QString::fromLocal8Bit("name")] = rcvStationInfo[QString::fromLocal8Bit("name")];
	rcvObj[QString::fromLocal8Bit("type")] = rcvStationInfo[QString::fromLocal8Bit("type")];
	rcvObj[QString::fromLocal8Bit("SN")] = rcvStationInfo[QString::fromLocal8Bit("SN")];
	dstListStations.append(rcvObj);
	dstObj[QString::fromLocal8Bit("Stations")] = dstListStations;
	App::model()->stationsItem()->deserialize(dstObj);
	return newCidName;
}
