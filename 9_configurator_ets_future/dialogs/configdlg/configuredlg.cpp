#include <QString>
#include "../communication/communication.h"
#include "../cryptodlg/cryptodlg.h"
#include "configuredlg.h"
#include "ui_configuredlg.h"

#if defined(__clang__)
	#pragma clang diagnostic push
	#pragma clang diagnostic warning "-Wconversion"
	#pragma clang diagnostic warning "-Wsign-conversion"
	#pragma clang diagnostic warning "-Wold-style-cast"
#elif defined(__GNUC__) || defined(__GNUG__)
	#pragma GCC diagnostic push
	#pragma GCC diagnostic warning "-Wconversion"
	#pragma GCC diagnostic warning "-Wsign-conversion"
	#pragma GCC diagnostic warning "-Wold-style-cast"
#elif defined(_MSC_VER)
#endif

ConfigureDialog::ConfigureDialog(QWidget *parent, CModeExchange mode)
    : QDialog(parent, Qt::FramelessWindowHint)
    , ui(new Ui::ConfigureDialog)
{
    ui->setupUi(this);
	connect(ui->b_open, SIGNAL(clicked()), this, SLOT(b_open_clicked()));
	connect(ui->b_send, SIGNAL(clicked()), this, SLOT(b_send_clicked()));
	connect(ui->b_rcv, SIGNAL(clicked()), this, SLOT(b_rcv_clicked()));
	connect(ui->b_setSN, SIGNAL(clicked()), this, SLOT(b_SetSN_clicked()));
	connect(ui->b_GetAES, SIGNAL(clicked()), this, SLOT(b_GetAES_clicked()));
	connect(ui->b_SetAES, SIGNAL(clicked()), this, SLOT(b_SetAES_clicked()));

	globalConfigureStr = QString::fromLocal8Bit("");
	isRcvConfigure = false;
	ModeExchange = mode;
	portName = QString::fromLocal8Bit("");
	pdSendConfig = new QProgressDialog(QObject::tr("Read version devices"), QObject::tr("Cancel"), 0, 0);
	pdSendConfig->setValue(0);
	pdSendConfig->setMinimumDuration(50);
	pb = new QProgressBar();
	QString stylePB;
	stylePB = QString::fromLocal8Bit("QProgressBar {");
	stylePB += QString::fromLocal8Bit("border: 2px solid grey;");
	stylePB += QString::fromLocal8Bit("border-radius: 5px;");
	stylePB += QString::fromLocal8Bit("text-align: center;");
	stylePB += QString::fromLocal8Bit("}");
	stylePB += QString::fromLocal8Bit("QProgressBar::chunk {");
	stylePB += QString::fromLocal8Bit("background-color: #05B8CC;");
	stylePB += QString::fromLocal8Bit("width: 20px;}");
	pb->setStyleSheet(stylePB);
	pdSendConfig->setBar(pb);
	btnCancel = new QPushButton();
	btnCancel->setText(QObject::tr("Cancel"));
	connect(btnCancel, SIGNAL(clicked()), this, SLOT(pdSendConfig_cancel()));
	pdSendConfig->setCancelButton(btnCancel);
	pdSendConfig->setWindowModality(Qt::ApplicationModal);
	pdSendConfig->hide();
	pdSendConfig->close();
	targetIP = QString::fromLocal8Bit("");
	RxUDPPort = 12341;
}

ConfigureDialog::~ConfigureDialog()
{
    delete ui;
}

void ConfigureDialog::setTitle(QString _title){
	ui->l_title->setText(_title);
}

void ConfigureDialog::setMode(CModeExchange _mode){
	ModeExchange = _mode;
	if (ModeExchange != RS232Mode){
		ui->lineEdit_SN->setEnabled(false);
		ui->b_setSN->setEnabled(false);
		ui->b_GetAES->setEnabled(false);
	}
}

void ConfigureDialog::setRS232Name(QString _portname){
	portName = _portname;
}

void ConfigureDialog::setSelfIP(QString _selfip){
	selfIP = _selfip;
}

void ConfigureDialog::b_open_clicked()
{
	int filterIdx = ui->comboBox_TypeDev->currentIndex();
	int cidName;
	int typedev;
	QString _str;
	filteredCidList.clear();
	QJsonDocument doc = QJsonDocument::fromJson(globalConfigureStr.toLocal8Bit());
	QJsonObject mapObj = doc.object();
	QJsonArray listStations = mapObj[QString::fromLocal8Bit("Stations")].toArray();
	foreach (const QJsonValue & value, listStations) {
		QJsonObject obj = value.toObject();
		cidName	= obj[QString::fromLocal8Bit("cid")].toInt();
		typedev	= obj[QString::fromLocal8Bit("type")].toInt();
		_str.setNum(cidName);
		_str.prepend(QString::fromLocal8Bit("CID"));
		if (filterIdx == 0){
			filteredCidList.append(_str);
		} else {
			if ((typedev-1) == filterIdx)
				filteredCidList.append(_str);
		}
	}
	dlgCidList = new StringListDialog(QObject::tr("Destiations"), filteredCidList);
    if (dlgCidList->exec() == QDialog::Accepted) {               
		ui->m_settings->setText(dlgCidList->current());
		foreach (const QJsonValue & value, listStations) {
			QJsonObject obj = value.toObject();
			cidName	= obj[QString::fromLocal8Bit("cid")].toInt();
			_str.setNum(cidName);
			_str.prepend(QString::fromLocal8Bit("CID"));
			if (_str.compare(dlgCidList->current()) == 0){
				ui->lineEdit_SN->setText(obj[QString::fromLocal8Bit("SN")].toString());
				ui->lineEdit_IPStation->setText(obj[QString::fromLocal8Bit("ip")].toString());
				typeDeviceConfiguring = obj[QString::fromLocal8Bit("type")].toInt();
			}
		}
    }
	delete dlgCidList;
}

void ConfigureDialog::b_send_clicked()
{
	QMessageBox::StandardButton ret;
	if (ModeExchange == RS232Mode){
		if (portName.isEmpty()){
			QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("Select RS232 port for configure"));
			return;
		}
	} else
	if (ModeExchange == NetworkMode){
		if (selfIP.isEmpty()){
			QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("Select self IP address for configure"));
			return;
		}
	}
	if (ui->m_settings->text() == QString::fromLocal8Bit("")){
		QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("Selected CID device for configure"));
		return;
	}
	sendConfigStr = ConvertToConfigureStr(ui->m_settings->text());
	if (sendConfigStr.isEmpty()){
		QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("Configuration is empty"));
		return;
	}
	QByteArray _baTxConfiguration = sendConfigStr.toLocal8Bit();
	txSizeConfig = _baTxConfiguration.count();
	if (txSizeConfig > MAX_CONFIG_SIZE){
		QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("Configuration is too long"));
		return;
	} else 
	if (txSizeConfig == 0){
		QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("Configuration is too small"));
		return;
	}

	QString _flCnf = QString::fromLocal8Bit("cnf.txt");
	QFile* _fileCnf = new QFile(_flCnf);
    if (!_fileCnf->open(QIODevice::WriteOnly | QIODevice::Text)){
        return;
	}
	_fileCnf->write(_baTxConfiguration);
	_fileCnf->close();
	delete _fileCnf;
//	return;

	QString _str;
    int _rc = 0;
    int _rd = 0;
	unsigned int _length_wr_block;
    unsigned int _block_size = 0;
	if (ModeExchange == RS232Mode){
		_rc = int(txSizeConfig/CONFIG_BLOCK_SIZE_RS232);
		_rd = txSizeConfig % CONFIG_BLOCK_SIZE_RS232;
		_block_size = CONFIG_BLOCK_SIZE_RS232;
	} else
	if (ModeExchange == NetworkMode){
		_rc = int(txSizeConfig/CONFIG_BLOCK_SIZE_ETH);
		_rd = txSizeConfig % CONFIG_BLOCK_SIZE_ETH;
		_block_size = CONFIG_BLOCK_SIZE_ETH;
	}
	for(int _i = 0; _i < txSizeConfig; _i++){
		test_buff_tx[_i] = _baTxConfiguration.at(_i);
	}

	pCommunicationDialog = new CommunicationDialog(this);
	pCommunicationDialog->setMode(ModeExchange);
	if (ModeExchange == RS232Mode){
		pCommunicationDialog->setRs232PortName(portName);
	} else
	if (ModeExchange == NetworkMode){
		pCommunicationDialog->setRxUDPPort(RxUDPPort);
		pCommunicationDialog->setSelfIP(selfIP);
		targetIP = ui->lineEdit_IPStation->text();
		pCommunicationDialog->setTargetIP(targetIP);
		pCommunicationDialog->setAesKey((uint8_t*)ui->lineEdit_KeyAES->text().toLocal8Bit().data(), 16);
	}
	if (pCommunicationDialog->Open() == false){
		QMessageBox::critical(this, QObject::tr("Error"), pCommunicationDialog->getLastErrorString());
		delete pCommunicationDialog;
		return;
	}
	connect(this, SIGNAL(sigCancel()), pCommunicationDialog, SLOT(cancel_transactions()), Qt::QueuedConnection);
	isCancelWrite = false;
	pdSendConfig->show();
//===протокол передачи конфигурации====
#ifdef __IS_WRITE_LOG__
	QString _fl = QString::fromLocal8Bit("Log.txt");
	QFile* _file = new QFile(_fl);
    if (!_file->open(QIODevice::WriteOnly | QIODevice::Text)){
        return;
	}
	_file->close();
	delete _file;
#endif
	this->setEnabled(false);
//===чтение типа подключенного устройства====
	pdSendConfig->setLabelText(QObject::tr("Get type device..."));
	int _rx_type_dev = 0;
	if (!pCommunicationDialog->fnGetCFGUI(reinterpret_cast<unsigned int*>(&_rx_type_dev))){
		QMessageBox::critical(this, QObject::tr("Error"), pCommunicationDialog->getLastErrorString());
		disconnect(this, SIGNAL(sigCancel()), pCommunicationDialog, SLOT(cancel_transactions()));
		pCommunicationDialog->Close();
		delete pCommunicationDialog;
		pdSendConfig->close();
		this->setEnabled(true);
		return;
	}
//====проверка типа подключенного устройства====
	if (_rx_type_dev != typeDeviceConfiguring){
		if (_rx_type_dev == (int)StationType::Unknown) {
			ret = QMessageBox::question(this, QApplication::applicationName(), QObject::tr("Connected device unknown type\nContinue configuring? Yes/No"),
				QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
			if (ret == QMessageBox::No) {
				disconnect(this, SIGNAL(sigCancel()), pCommunicationDialog, SLOT(cancel_transactions()));
				pCommunicationDialog->Close();
				delete pCommunicationDialog;
				pdSendConfig->close();
				this->setEnabled(true);
				return;
			}
		} else {
//			QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("Unsupported type device\nSelected device: %1, connected device: %2").arg(typeDeviceConfiguring).arg(_rx_type_dev));
//			disconnect(this, SIGNAL(sigCancel()), pCommunicationDialog, SLOT(cancel_transactions()));
//			pCommunicationDialog->Close();
//			delete pCommunicationDialog;
//			pdSendConfig->close();
//			this->setEnabled(true);
//			return;
		}
	}
//====установка инициализации процесса конфигурирования====
	pdSendConfig->setLabelText(QObject::tr("Set config on..."));
	if (!pCommunicationDialog->fnConfigOn()){
		QMessageBox::critical(this, QObject::tr("Error"), pCommunicationDialog->getLastErrorString());
		disconnect(this, SIGNAL(sigCancel()), pCommunicationDialog, SLOT(cancel_transactions()));
		pCommunicationDialog->Close();
		delete pCommunicationDialog;
		pdSendConfig->close();
		this->setEnabled(true);
		return;
	}
//====установка системной информации конфигурации====
	pCommunicationDialog->crc32();
	txCRCConfig	= pCommunicationDialog->ProcessCRC(test_buff_tx, txSizeConfig);
	pdSendConfig->setLabelText(QObject::tr("Set system info configure device..."));
	if (!pCommunicationDialog->fnConfigSetSysInfo(txSizeConfig, txCRCConfig, version)){
		QMessageBox::critical(this, QObject::tr("Error"), pCommunicationDialog->getLastErrorString());
		disconnect(this, SIGNAL(sigCancel()), pCommunicationDialog, SLOT(cancel_transactions()));
		pCommunicationDialog->Close();
		delete pCommunicationDialog;
		pdSendConfig->close();
		this->setEnabled(true);
		return;
	}
	btnCancel->setEnabled(true);
//====передача блока данных конфигурации====
	pdSendConfig->setLabelText(QObject::tr("Set config data block..."));
	for(int _i = 0; _i < _rc; _i++){
		if (isCancelWrite){
			if (!pCommunicationDialog->fnConfigOff()){
				QMessageBox::critical(this, QObject::tr("Error"), pCommunicationDialog->getLastErrorString());
			}
			emit sigCancel();
			disconnect(this, SIGNAL(sigCancel()), pCommunicationDialog, SLOT(cancel_transactions()));
			pCommunicationDialog->Close();
			delete pCommunicationDialog;
			pdSendConfig->close();
			this->setEnabled(true);
			return;
		}
		_length_wr_block = _i;
		_length_wr_block *= _block_size;
		if (!pCommunicationDialog->fnConfigSetBlock(&test_buff_tx[_length_wr_block], _length_wr_block, _block_size)){
			QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("%1\n number block=%2").arg(pCommunicationDialog->getLastErrorString()).arg(_i));
			disconnect(this, SIGNAL(sigCancel()), pCommunicationDialog, SLOT(cancel_transactions()));
			pCommunicationDialog->Close();
			delete pCommunicationDialog;
			pdSendConfig->close();
			this->setEnabled(true);
			return;
		}
		pdSendConfig->setValue(((_i*100)/(_rc+1)));
		qApp->processEvents();
	}
	_length_wr_block = _rc;
	_length_wr_block *= _block_size;
	if (!pCommunicationDialog->fnConfigSetBlock(&test_buff_tx[_length_wr_block], _length_wr_block,  _rd)){
		QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("%1\nlast block").arg(pCommunicationDialog->getLastErrorString()));
		disconnect(this, SIGNAL(sigCancel()), pCommunicationDialog, SLOT(cancel_transactions()));
		pCommunicationDialog->Close();
		delete pCommunicationDialog;
		pdSendConfig->close();
		this->setEnabled(true);
		return;
	}
	pdSendConfig->setValue(100);
	btnCancel->setEnabled(false);
//====запуск записи конфигурации====
	pdSendConfig->setLabelText(QObject::tr("Write configure..."));
	if (!pCommunicationDialog->fnConfigStartWrite()){
		QMessageBox::critical(this, QObject::tr("Error"), pCommunicationDialog->getLastErrorString());
		disconnect(this, SIGNAL(sigCancel()), pCommunicationDialog, SLOT(cancel_transactions()));
		pCommunicationDialog->Close();
		delete pCommunicationDialog;
		pdSendConfig->close();
		this->setEnabled(true);
		return;
	}
	QMessageBox::information(this, QObject::tr("Info"), QObject::tr("Send configuration succefulle!!!"));
	disconnect(this, SIGNAL(sigCancel()), pCommunicationDialog, SLOT(cancel_transactions()));
	pCommunicationDialog->Close();
	delete pCommunicationDialog;
	pdSendConfig->close();
	this->setEnabled(true);
}

void ConfigureDialog::b_rcv_clicked(){
	targetIP = ui->lineEdit_IPStation->text();
	if (ModeExchange == RS232Mode){
		if (portName.isEmpty()){
			QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("Select RS232 port for configure"));
			return;
		}
	} else
	if (ModeExchange == NetworkMode){
		if (selfIP.isEmpty()){
			QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("Select self IP address for configure"));
			return;
		}
		if (targetIP.isEmpty()){
			QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("Select target IP address for configure"));
			return;
		}
	}
    int _rc = 0;
    unsigned int _rd = 0;
    unsigned int _length_wr_block = 0;
    unsigned int _block_size = 0;

	isCancelWrite = false;
	pCommunicationDialog = new CommunicationDialog(this);
	pCommunicationDialog->setMode(ModeExchange);
	if (ModeExchange == RS232Mode){
		pCommunicationDialog->setRs232PortName(portName);
	} else
	if (ModeExchange == NetworkMode){
		pCommunicationDialog->setRxUDPPort(RxUDPPort);
		pCommunicationDialog->setSelfIP(selfIP);
		targetIP = ui->lineEdit_IPStation->text();
		pCommunicationDialog->setTargetIP(targetIP);
		pCommunicationDialog->setAesKey((uint8_t*)ui->lineEdit_KeyAES->text().toLocal8Bit().data(), 16);
	}
	if (pCommunicationDialog->Open() == false){
		QMessageBox::critical(this, QObject::tr("Error"), pCommunicationDialog->getLastErrorString());
		delete pCommunicationDialog;
		return;
	}
	connect(this, SIGNAL(sigCancel()), pCommunicationDialog, SLOT(cancel_transactions()), Qt::QueuedConnection);
	isCancelWrite = false;
	btnCancel->setEnabled(false);
	pdSendConfig->show();
//=====протокол чтения конфигурации из устройства=====
#ifdef __IS_WRITE_LOG__
	QString _fl = QString::fromLocal8Bit("Log.txt");
	QFile* _file = new QFile(_fl);
    if (!_file->open(QIODevice::WriteOnly | QIODevice::Text)){
        return;
	}
	_file->close();
	delete _file;
#endif
	this->setEnabled(false);
//====установка инициализации процесса конфигурирования====
	pdSendConfig->setLabelText(QObject::tr("Set config on..."));
	if (!pCommunicationDialog->fnConfigOn()){
		QMessageBox::critical(this, QObject::tr("Error"), pCommunicationDialog->getLastErrorString());
		disconnect(this, SIGNAL(sigCancel()), pCommunicationDialog, SLOT(cancel_transactions()));
		pCommunicationDialog->Close();
		delete pCommunicationDialog;
		pdSendConfig->close();
		this->setEnabled(true);
		return;
	}
//====чтение системной информации о конфигурации====
	pdSendConfig->setLabelText(QObject::tr("Get system info config..."));
	if (!pCommunicationDialog->fnConfigGetSysInfo(&rxSizeConfig, &rxCRCConfig, (unsigned int*)&version)){
		QMessageBox::critical(this, QObject::tr("Error"), pCommunicationDialog->getLastErrorString());
		disconnect(this, SIGNAL(sigCancel()), pCommunicationDialog, SLOT(cancel_transactions()));
		pCommunicationDialog->Close();
		delete pCommunicationDialog;
		pdSendConfig->close();
		this->setEnabled(true);
		return;
	}
	if ((rxSizeConfig == 0) || (rxSizeConfig > MAX_CONFIG_SIZE)){
		QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("Config size incorrect"));
		disconnect(this, SIGNAL(sigCancel()), pCommunicationDialog, SLOT(cancel_transactions()));
		pCommunicationDialog->Close();
		delete pCommunicationDialog;
		pdSendConfig->close();
		this->setEnabled(true);
		return;
	}
	if (ModeExchange == RS232Mode){
		_rc = int(rxSizeConfig/CONFIG_BLOCK_SIZE_RS232);
		_rd = rxSizeConfig % CONFIG_BLOCK_SIZE_RS232;
		_block_size = CONFIG_BLOCK_SIZE_RS232;
	} else
	if (ModeExchange == NetworkMode){
		_rc = int(rxSizeConfig/CONFIG_BLOCK_SIZE_ETH);
		_rd = rxSizeConfig % CONFIG_BLOCK_SIZE_ETH;
		_block_size = CONFIG_BLOCK_SIZE_ETH;
	}
//====чтение блоков данных конфигурации====
	pdSendConfig->setLabelText(QObject::tr("Read config data block..."));
	memset(test_buff_rx, 0, 1024*1024);
	btnCancel->setEnabled(true);
	for(int _i = 0; _i < _rc; _i++){
		if (isCancelWrite){
			if (!pCommunicationDialog->fnConfigOff()){
				QMessageBox::critical(this, QObject::tr("Error"), pCommunicationDialog->getLastErrorString());
			}
			emit sigCancel();
			disconnect(this, SIGNAL(sigCancel()), pCommunicationDialog, SLOT(cancel_transactions()));
			pCommunicationDialog->Close();
			delete pCommunicationDialog;
			pdSendConfig->close();
			this->setEnabled(true);
			return;
		}
		_length_wr_block = _i;
		_length_wr_block *= _block_size;
		if (!pCommunicationDialog->fnConfigGetBlock(&test_buff_rx[_length_wr_block], _length_wr_block, _block_size)){
			QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("%1\n number block=%2").arg(pCommunicationDialog->getLastErrorString()).arg(_i));
			disconnect(this, SIGNAL(sigCancel()), pCommunicationDialog, SLOT(cancel_transactions()));
			pCommunicationDialog->Close();
			delete pCommunicationDialog;
			pdSendConfig->close();
			this->setEnabled(true);
			return;
		}
		pdSendConfig->setValue(((_i*100)/(_rc+1)));
		qApp->processEvents();
	}
	_length_wr_block = _rc;
	_length_wr_block *= _block_size;
	if (!pCommunicationDialog->fnConfigGetBlock(&test_buff_rx[_length_wr_block], _length_wr_block, _rd)){
		QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("%1\nlast block").arg(pCommunicationDialog->getLastErrorString()));
		disconnect(this, SIGNAL(sigCancel()), pCommunicationDialog, SLOT(cancel_transactions()));
		pCommunicationDialog->Close();
		delete pCommunicationDialog;
		pdSendConfig->close();
		this->setEnabled(true);
		return;
	}
	pdSendConfig->setValue(100);
	btnCancel->setEnabled(false);
	pCommunicationDialog->crc32();
	memset((char*)&test_buff_rx[_rc*_block_size+_rd], 0, (1024*1024)-(_rc*_block_size+_rd));
	unsigned int _rxCRCConfigCalc = pCommunicationDialog->ProcessCRC(test_buff_rx, rxSizeConfig);
	QString _sCRCcalc;
	_sCRCcalc.setNum(_rxCRCConfigCalc, 16);
	QString _sCRC;
	_sCRC.setNum(rxCRCConfig, 16);
	QString _sSize;
	_sSize.setNum(rxSizeConfig, 10);
//====проверка контрольной суммы принятой конфигурации====
	if (_rxCRCConfigCalc != rxCRCConfig){
		QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("Mismatch CRC receive configure\nrx:0x%1, calc:0x%2\nsize:%3").arg(_sCRC).arg(_sCRCcalc).arg(_sSize));
		disconnect(this, SIGNAL(sigCancel()), pCommunicationDialog, SLOT(cancel_transactions()));
		pCommunicationDialog->Close();
		delete pCommunicationDialog;
		pdSendConfig->close();
		this->setEnabled(true);
		return;
	}
//====чтение серийного номера изделия====
	pdSendConfig->setLabelText(QObject::tr("Get config s/n..."));
	QString _rxSN;
	if (!pCommunicationDialog->fnGetSN(&_rxSN)){
		QMessageBox::critical(this, QObject::tr("Error"), pCommunicationDialog->getLastErrorString());
		disconnect(this, SIGNAL(sigCancel()), pCommunicationDialog, SLOT(cancel_transactions()));
		pCommunicationDialog->Close();
		delete pCommunicationDialog;
		pdSendConfig->close();
		this->setEnabled(true);
		return;
	}
//====передача остановки процесса конфигурирования====
	pdSendConfig->setLabelText(QObject::tr("Set config off..."));
	if (!pCommunicationDialog->fnConfigOff()){
		QMessageBox::critical(this, QObject::tr("Error"), pCommunicationDialog->getLastErrorString());
		disconnect(this, SIGNAL(sigCancel()), pCommunicationDialog, SLOT(cancel_transactions()));
		pCommunicationDialog->Close();
		delete pCommunicationDialog;
		pdSendConfig->close();
		this->setEnabled(true);
		return;
	}
	QMessageBox::information(this, QObject::tr("Info"), QObject::tr("Receive configuration succefulle!!!"));
	disconnect(this, SIGNAL(sigCancel()), pCommunicationDialog, SLOT(cancel_transactions()));
	pCommunicationDialog->Close();
	delete pCommunicationDialog;
	pdSendConfig->close();
	rcvConfigStr = QString::fromLocal8Bit(test_buff_rx);
	updateSNInRcvConfig(_rxSN);
	isRcvConfigure = true;
	this->setEnabled(true);
/*	QByteArray baRxConfiguration = rcvConfigStr.toLocal8Bit();
	QString _fl = QString::fromLocal8Bit("rcv_cnf.txt");
	QFile* _file = new QFile(_fl);
    if (!_file->open(QIODevice::WriteOnly | QIODevice::Text)){
        return;
	}
	_file->write(baRxConfiguration);
	_file->close();
	delete _file;*/
}

void ConfigureDialog::updateSNInRcvConfig(QString _sn){
	QJsonDocument doc = QJsonDocument::fromJson(rcvConfigStr.toLocal8Bit());
	QJsonObject mapSrcObj = doc.object();
	QJsonObject srcGlobalParameters = mapSrcObj[QString::fromLocal8Bit("Global parameters")].toObject();
	srcGlobalParameters[QString::fromLocal8Bit("SN")] = _sn;
	mapSrcObj[QString::fromLocal8Bit("Global parameters")] = srcGlobalParameters;
	rcvConfigStr = QString::fromLocal8Bit(QJsonDocument(mapSrcObj).toJson());
}

void ConfigureDialog::setConfigureStr(QString _data){
	globalConfigureStr = _data;
	QJsonDocument doc = QJsonDocument::fromJson(globalConfigureStr.toLocal8Bit());
	QJsonObject mapSrcObj = doc.object();
	QJsonObject srcGlobalParameters = mapSrcObj[QString::fromLocal8Bit("Global parameters")].toObject();
	encKey = srcGlobalParameters[QString::fromLocal8Bit("key AES")].toString();
	ui->lineEdit_KeyAES->setText(encKey);
	RxUDPPort = srcGlobalParameters[QString::fromLocal8Bit("port configuration")].toInt();

/*	baTxConfiguration = _data.toLocal8Bit();
	QString _fl = QString::fromLocal8Bit("1.txt");
	QFile* _file = new QFile(_fl);
    if (!_file->open(QIODevice::WriteOnly | QIODevice::Text)){
        return;
	}
	_file->write(baTxConfiguration);
	_file->close();
	delete _file;*/
}

void ConfigureDialog::pdSendConfig_cancel(){
	QMessageBox::StandardButton ret = QMessageBox::warning(this, QObject::tr("Warning"), QObject::tr("This process is not finished\nReject process(Yes/No)"), QMessageBox::Yes | QMessageBox::No);
	if (ret == QMessageBox::Yes)
		isCancelWrite = true;
}

QString ConfigureDialog::ConvertToConfigureStr(QString _cid){
	QString str, str2;
	int cidName;
//	bool _ok;
	QJsonObject dstObject;
	QJsonObject dstGlobalParameters;
	QJsonObject srcGlobalParameters;
	QJsonObject srcMapItems;
	QJsonObject cnfSubObj;
	QJsonObject srcMapObj;
	QJsonArray srcListActiveStation;
	QJsonArray listGroupsCalls;
	QJsonArray listButtonSettings;
	QJsonDocument doc = QJsonDocument::fromJson(globalConfigureStr.toLocal8Bit());
	QJsonObject mapSrcObj = doc.object();
	
	srcGlobalParameters = mapSrcObj[QString::fromLocal8Bit("Global parameters")].toObject();
	dstGlobalParameters[QString::fromLocal8Bit("ip doc")] = srcGlobalParameters[QString::fromLocal8Bit("ip doc")];
    dstGlobalParameters[QString::fromLocal8Bit("port log doc")] = srcGlobalParameters[QString::fromLocal8Bit("port log doc")];
	dstGlobalParameters[QString::fromLocal8Bit("port rtp doc")] = srcGlobalParameters[QString::fromLocal8Bit("port rtp doc")];
	dstGlobalParameters[QString::fromLocal8Bit("key AES")] = srcGlobalParameters[QString::fromLocal8Bit("key AES")];
	dstGlobalParameters[QString::fromLocal8Bit("version")] = mapSrcObj[QString::fromLocal8Bit("version")];
	version = mapSrcObj[QString::fromLocal8Bit("version")].toInt();

    QJsonArray listStations = mapSrcObj[QString::fromLocal8Bit("Stations")].toArray();
	foreach (const QJsonValue & value, listStations) {
		QJsonObject obj = value.toObject();
		cidName	= obj[QString::fromLocal8Bit("cid")].toInt();
		str.setNum(cidName);
		str.prepend(QString::fromLocal8Bit("CID"));
		if (_cid.indexOf(str) != -1){
			targetIP = obj[QString::fromLocal8Bit("ip")].toString();
			ui->lineEdit_IPStation->setText(targetIP);
			dstGlobalParameters[QString::fromLocal8Bit("type")] = obj[QString::fromLocal8Bit("type")];
			dstGlobalParameters[QString::fromLocal8Bit("cid")] = obj[QString::fromLocal8Bit("cid")];
			dstGlobalParameters[QString::fromLocal8Bit("gateway")] = obj[QString::fromLocal8Bit("gateway")];
			dstGlobalParameters[QString::fromLocal8Bit("ip")] = obj[QString::fromLocal8Bit("ip")];
			dstGlobalParameters[QString::fromLocal8Bit("mask")] = obj[QString::fromLocal8Bit("mask")];
			dstGlobalParameters[QString::fromLocal8Bit("name")] = obj[QString::fromLocal8Bit("name")];
			dstGlobalParameters[QString::fromLocal8Bit("hash")] = obj[QString::fromLocal8Bit("hash")];
			dstGlobalParameters[QString::fromLocal8Bit("SN")] = obj[QString::fromLocal8Bit("SN")];
			srcMapItems = obj[QString::fromLocal8Bit("items")].toObject();
			srcListActiveStation = srcMapItems[QString::fromLocal8Bit("activestation")].toArray();
			for(int _j = 0; _j < srcListActiveStation.count(); _j++){
				srcMapObj = srcListActiveStation.at(_j).toObject();
				cnfSubObj[QString::fromLocal8Bit("id")] = srcMapObj[QString::fromLocal8Bit("id")];
				cnfSubObj[QString::fromLocal8Bit("function")] = srcMapObj[QString::fromLocal8Bit("function")];
				cnfSubObj[QString::fromLocal8Bit("name")] = srcMapObj[QString::fromLocal8Bit("name")];
				cnfSubObj[QString::fromLocal8Bit("priority")] = srcMapObj[QString::fromLocal8Bit("priority")];
				cnfSubObj[QString::fromLocal8Bit("destination")] = srcMapObj[QString::fromLocal8Bit("destination")];
				str2 = cnfSubObj[QString::fromLocal8Bit("destination")].toString();
				if (str2.indexOf(QString::fromLocal8Bit("CID")) != -1){
					cnfSubObj[QString::fromLocal8Bit("destination_ip")] = getIPFromCid(&listStations, srcMapObj[QString::fromLocal8Bit("destination")].toString());
				} else
				if (str2.indexOf(QString::fromLocal8Bit("GID")) != -1){
					cnfSubObj[QString::fromLocal8Bit("destination_ip")] = QString::fromLocal8Bit("");
//					addGroupCallForGid(&listGroupsCalls, str2);
				}
				listButtonSettings.append(cnfSubObj);
			}
		}
	}
	dstObject[QString::fromLocal8Bit("Global parameters")] = dstGlobalParameters;
	dstObject[QString::fromLocal8Bit("activebuttons")] = listButtonSettings;
//	dstObject[QString::fromLocal8Bit("Groups_Calls")] = listGroupsCalls;
	return QString::fromLocal8Bit(QJsonDocument(dstObject).toJson(QJsonDocument::Compact));
}

QString ConfigureDialog::getIPFromCid(QJsonArray* StationList, QString _cid){
	QJsonObject mapStation;
	int cidName;
	QString str;
	for(int _i = 0; _i < StationList->count(); _i++){
		mapStation = StationList->at(_i).toObject();
		cidName	= mapStation[QString::fromLocal8Bit("cid")].toInt();
		str.setNum(cidName);
		if (_cid.indexOf(str) != -1){
			return mapStation[QString::fromLocal8Bit("ip")].toString();
		}
	}
	return QString::fromLocal8Bit("");
}

void ConfigureDialog::addGroupCallForGid(QJsonArray* GroupCallList, QString _gid){
	QString str, strHashStation;
	QJsonDocument doc = QJsonDocument::fromJson(globalConfigureStr.toLocal8Bit());
	QJsonObject mapObj = doc.object();
	QJsonObject cnfObj;
	QJsonObject mapGroups = mapObj[QString::fromLocal8Bit("Groups")].toObject();
	QJsonArray listGroupCalls = mapGroups[QString::fromLocal8Bit("groups_calls")].toArray();
	QJsonArray listStations = mapObj[QString::fromLocal8Bit("Stations")].toArray();
	QJsonArray listHashStations;
	QJsonArray cnfListStations;
	foreach (const QJsonValue & value, listGroupCalls) {
		QJsonObject obj = value.toObject();
		str.setNum(obj[QString::fromLocal8Bit("id")].toInt());
		str.prepend(QString::fromLocal8Bit("GID"));
		if ((str.indexOf(_gid) != -1) && (checkGidInGroupCallList(GroupCallList, obj[QString::fromLocal8Bit("id")].toInt()) == false)){
			cnfObj[QString::fromLocal8Bit("id")] = obj[QString::fromLocal8Bit("id")];
			cnfObj[QString::fromLocal8Bit("name")] = obj[QString::fromLocal8Bit("name")];
			listHashStations = obj[QString::fromLocal8Bit("listStations")].toArray();
			foreach (const QJsonValue & value1, listHashStations) {
				strHashStation = value1[QString::fromLocal8Bit("hashStation")].toString();
				getStationInfoFromHash(&cnfListStations, &listStations, strHashStation);
			}
			cnfObj[QString::fromLocal8Bit("listStations")] = cnfListStations;
			GroupCallList->append(cnfObj);
			return;
		}
	}
}

void ConfigureDialog::getStationInfoFromHash(QJsonArray* _dstList, QJsonArray* pStationArray, QString _hash){
	QJsonObject statInfo;
	for(int _i = 0; _i < pStationArray->count(); _i++){
		QJsonObject _obj = pStationArray->at(_i).toObject();
		if (_hash.indexOf(_obj[QString::fromLocal8Bit("hash")].toString()) != -1){
			statInfo[QString::fromLocal8Bit("ip")] = _obj[QString::fromLocal8Bit("ip")];
			statInfo[QString::fromLocal8Bit("cid")] = _obj[QString::fromLocal8Bit("cid")];
			statInfo[QString::fromLocal8Bit("hash")] = _obj[QString::fromLocal8Bit("hash")];
			_dstList->append(statInfo);
			return;
		}
	}
}

bool ConfigureDialog::checkGidInGroupCallList(QJsonArray* _groupCallList, int _gid){
	int _id;
	for(int _i = 0; _i < _groupCallList->count(); _i++){
		QJsonObject obj = _groupCallList->at(_i).toObject();
		_id = obj[QString::fromLocal8Bit("id")].toInt();
		if (_id == _gid)
			return true;
	}
	return false;
}

QString ConfigureDialog::getConfigureStr(){
	return rcvConfigStr;
}

bool ConfigureDialog::isAcceptLoaddedConfigure(){
	return isRcvConfigure;
}

void ConfigureDialog::closeEvent( QCloseEvent *ev){
	Q_UNUSED(ev);
	QMessageBox::StandardButton ret;
	if (isRcvConfigure){
		ret = QMessageBox::question( this,  QApplication::applicationName(), QObject::tr(" Receive new configure\nAccept? Yes/No"),
									QMessageBox::Yes | QMessageBox::No , QMessageBox::No );
		 if (ret == QMessageBox::No)
			isRcvConfigure = false;
	}
	accept();
}

void ConfigureDialog::b_SetSN_clicked(){
	targetIP = ui->lineEdit_IPStation->text();
	if (ModeExchange == RS232Mode){
		if (portName.isEmpty()){
			if (isTestMode){
				testResult = false;
				testMsg = QString::fromLocal8Bit("Select RS232 port for configure");
			} else {
				QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("Select RS232 port for configure"));
			}
			return;
		}
	} else
	if (ModeExchange == NetworkMode){
		if (selfIP.isEmpty()){
			if (isTestMode){
				testResult = false;
				testMsg = QString::fromLocal8Bit("Select self IP address for configure");
			} else {
				QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("Select self IP address for configure"));
			}
			return;
		}
		if (targetIP.isEmpty()){
			QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("Select target IP address for configure"));
			return;
		}
	}
	pCommunicationDialog = new CommunicationDialog(this);
	pCommunicationDialog->setMode(ModeExchange);
	if (ModeExchange == RS232Mode){
		pCommunicationDialog->setRs232PortName(portName);
	} else
	if (ModeExchange == NetworkMode){
		pCommunicationDialog->setRxUDPPort(RxUDPPort);
		pCommunicationDialog->setSelfIP(selfIP);
		targetIP = ui->lineEdit_IPStation->text();
		pCommunicationDialog->setTargetIP(targetIP);
		pCommunicationDialog->setAesKey((uint8_t*)ui->lineEdit_KeyAES->text().toLocal8Bit().data(), 16);
	}
	if (pCommunicationDialog->Open() == false){
		if (isTestMode){
			testResult = false;
			testMsg = QString::fromLocal8Bit("Communication dialog not initialize");
		} else {
			QMessageBox::critical(this, QObject::tr("Error"), pCommunicationDialog->getLastErrorString());
			delete pCommunicationDialog;
		}
		return;
	}
	if (isTestMode){
		testResult = true;
		return;
	}
	this->setEnabled(false);
	if (!pCommunicationDialog->fnConfigOn()){
		QMessageBox::critical(this, QObject::tr("Error"), pCommunicationDialog->getLastErrorString());
		pCommunicationDialog->Close();
		delete pCommunicationDialog;
		this->setEnabled(true);
		return;
	}
	if (!pCommunicationDialog->fnSetSN(ui->lineEdit_SN->text())){
		QMessageBox::critical(this, QObject::tr("Error"), pCommunicationDialog->getLastErrorString());
	} else {
		if (!pCommunicationDialog->fnConfigStartWrite()){
			QMessageBox::critical(this, QObject::tr("Error"), pCommunicationDialog->getLastErrorString());
		} else {
			QMessageBox::information(this, QObject::tr("Info"), QObject::tr("Serial number is set"));
		}
	}
	pCommunicationDialog->Close();
	delete pCommunicationDialog;
	this->setEnabled(true);
}

void ConfigureDialog::b_GetAES_clicked(){
	QString _str;
	targetIP = ui->lineEdit_IPStation->text();
	if (ModeExchange == RS232Mode){
		if (portName.isEmpty()){
			if (isTestMode){
				testResult = false;
				testMsg = QString::fromLocal8Bit("Select RS232 port for configure");
			} else {
				QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("Select RS232 port for configure"));
			}
			return;
		}
	} else
	if (ModeExchange == NetworkMode){
		if (selfIP.isEmpty()){
			if (isTestMode){
				testResult = false;
				testMsg = QString::fromLocal8Bit("Select self IP address for configure");
			} else {
				QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("Select self IP address for configure"));
			}
			return;
		}
		if (targetIP.isEmpty()){
			QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("Select target IP address for configure"));
			return;
		}
	}
	pCommunicationDialog = new CommunicationDialog(this);
	pCommunicationDialog->setMode(ModeExchange);
	if (ModeExchange == RS232Mode){
		pCommunicationDialog->setRs232PortName(portName);
	} else
	if (ModeExchange == NetworkMode){
		pCommunicationDialog->setRxUDPPort(RxUDPPort);
		pCommunicationDialog->setSelfIP(selfIP);
		targetIP = ui->lineEdit_IPStation->text();
		pCommunicationDialog->setTargetIP(targetIP);
		pCommunicationDialog->setAesKey((uint8_t*)ui->lineEdit_KeyAES->text().toLocal8Bit().data(), 16);
	}
	if (pCommunicationDialog->Open() == false){
		if (isTestMode){
			testResult = false;
			testMsg = QString::fromLocal8Bit("Communication dialog not initialize");
		} else {
			QMessageBox::critical(this, QObject::tr("Error"), pCommunicationDialog->getLastErrorString());
			delete pCommunicationDialog;
		}
		return;
	}
	if (isTestMode){
		testResult = true;
		return;
	}
	this->setEnabled(false);
	if (!pCommunicationDialog->fnConfigOn()){
		QMessageBox::critical(this, QObject::tr("Error"), pCommunicationDialog->getLastErrorString());
		pCommunicationDialog->Close();
		delete pCommunicationDialog;
		this->setEnabled(true);
		return;
	}
	if (!pCommunicationDialog->fnGetKeyAes(&_str)){
		QMessageBox::critical(this, QObject::tr("Error"), pCommunicationDialog->getLastErrorString());
	} else {
		if (!pCommunicationDialog->fnConfigOff()){
			QMessageBox::critical(this, QObject::tr("Error"), pCommunicationDialog->getLastErrorString());
			pCommunicationDialog->Close();
			delete pCommunicationDialog;
			this->setEnabled(true);
			return;
		}
		QMessageBox::information(this, QObject::tr("Info"), QObject::tr("Aes key is get"));
	}
	ui->lineEdit_KeyAES->setText(_str);
	pCommunicationDialog->Close();
	delete pCommunicationDialog;
	this->setEnabled(true);
}

void ConfigureDialog::b_SetAES_clicked(){
//	QMessageBox::StandardButton ret;
	targetIP = ui->lineEdit_IPStation->text();
	QString _aes_key = ui->lineEdit_KeyAES->text();
	if (ModeExchange == RS232Mode){
		if (portName.isEmpty()){
			if (isTestMode){
				testResult = false;
				testMsg = QString::fromLocal8Bit("Select RS232 port for configure");
			} else {
				QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("Select RS232 port for configure"));
			}
			return;
		}
	} else
	if (ModeExchange == NetworkMode){
		if (selfIP.isEmpty()){
			if (isTestMode){
				testResult = false;
				testMsg = QString::fromLocal8Bit("Select self IP address for configure");
			} else {
				QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("Select self IP address for configure"));
			}
			return;
		}
		if (targetIP.isEmpty()){
			QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("Select target IP address for configure"));
			return;
		}
		if ((_aes_key.isEmpty()) || (_aes_key.length() < 32)){
			QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("Encryption key empty or incorrect"));
			return;
		}
/*		ret = QMessageBox::question( this,  QApplication::applicationName(), QObject::tr("the current encryption key will be changed.\nChange? (Yes/No)"),
									QMessageBox::Yes | QMessageBox::No , QMessageBox::No );
		 if (ret == QMessageBox::No)
			return;*/
	}
	this->setEnabled(false);
	pCommunicationDialog = new CommunicationDialog(this);
	pCommunicationDialog->setMode(ModeExchange);
	if (ModeExchange == RS232Mode){
		pCommunicationDialog->setRs232PortName(portName);
	} else
	if (ModeExchange == NetworkMode){
		pCommunicationDialog->setRxUDPPort(RxUDPPort);
		pCommunicationDialog->setSelfIP(selfIP);
		targetIP = ui->lineEdit_IPStation->text();
		pCommunicationDialog->setTargetIP(targetIP);
		pCommunicationDialog->setAesKey((uint8_t*)_aes_key.toLocal8Bit().data(), 16);
	}
	if (pCommunicationDialog->Open() == false){
		if (isTestMode){
			testResult = false;
			testMsg = QString::fromLocal8Bit("Communication dialog not initialize");
		} else {
			QMessageBox::critical(this, QObject::tr("Error"), pCommunicationDialog->getLastErrorString());
			delete pCommunicationDialog;
		}
		this->setEnabled(true);
		return;
	}
	if (isTestMode){
		testResult = true;
		this->setEnabled(true);
		return;
	}
	if (!pCommunicationDialog->fnConfigOn()){
		QMessageBox::critical(this, QObject::tr("Error"), pCommunicationDialog->getLastErrorString());
		pCommunicationDialog->Close();
		delete pCommunicationDialog;
		this->setEnabled(true);
		return;
	}
	if (!pCommunicationDialog->fnSetKeyAes(ui->lineEdit_KeyAES->text())){
		QMessageBox::critical(this, QObject::tr("Error"), pCommunicationDialog->getLastErrorString());
	} else {
		if (!pCommunicationDialog->fnConfigStartWrite()){
			QMessageBox::critical(this, QObject::tr("Error"), pCommunicationDialog->getLastErrorString());
		} else {
			QMessageBox::information(this, QObject::tr("Info"), QObject::tr("Aes key is set"));
		}
	}
	pCommunicationDialog->Close();
	delete pCommunicationDialog;
	this->setEnabled(true);
}

QLineEdit* ConfigureDialog::getLN_SN(){
	return ui->lineEdit_SN;
}

QLineEdit* ConfigureDialog::getLN_KeyAes(){
	return ui->lineEdit_KeyAES;
}

QLineEdit* ConfigureDialog::getLN_m_settings(){
	return ui->m_settings;
}

QLineEdit* ConfigureDialog::getLN_IPStation(){
	return ui->lineEdit_IPStation;
}

QToolButton* ConfigureDialog::getBtn_b_setSN(){
	return ui->b_setSN;
//	return nullptr;
}

QToolButton* ConfigureDialog::getBtn_b_GetAES(){
	return ui->b_GetAES;
}

QToolButton* ConfigureDialog::getBtn_b_SetAES(){
	return ui->b_SetAES;
}

QPushButton* ConfigureDialog::getBtn_b_send(){
	return ui->b_send;
}

QPushButton* ConfigureDialog::getBtn_b_rcv(){
	return ui->b_rcv;
}

void ConfigureDialog::setStateTestingMode(bool _isTestMode){
	isTestMode = _isTestMode;
}

bool ConfigureDialog::isTestResult(){
	return testResult;
}

QString ConfigureDialog::getTestMsg(){
	return testMsg;
}

#if defined(__clang__)
	#pragma clang diagnostic pop
#elif defined(__GNUC__) || defined(__GNUG__)
	#pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#endif
