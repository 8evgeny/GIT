#include <QString>
#include "main/classes/base_types/station_types.h"
#include "fmwaredlg.h"
#include "ui_fmwaredlg.h"

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

FmWareDialog::FmWareDialog(QWidget *parent, CModeExchange mode)
    : QDialog(parent, Qt::FramelessWindowHint)
    , ui(new Ui::FmWareDialog)
{
    ui->setupUi(this);
	connect(ui->b_send, SIGNAL(clicked()), this, SLOT(b_send_clicked()));
	connect(ui->b_rcv, SIGNAL(clicked()), this, SLOT(b_rcv_clicked()));
	connect(ui->comboBox_TypeDev, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBox_TypeDev_currentIndexChanged(int)));
	connect(ui->checkBox_all, SIGNAL(stateChanged(int)), this, SLOT(checkBox_all_stateChanged(int)));
	connect(ui->b_loadFirmFile, SIGNAL(clicked()), this, SLOT(b_loadFirmFile_clicked()));
	connect(ui->b_ReadVersions, SIGNAL(clicked()), this, SLOT(b_ReadVersions_clicked()));
	globalConfigureStr = QString::fromLocal8Bit("");
	fmware_filename = QString::fromLocal8Bit("");
	ModeExchange = mode;
	portName = QString::fromLocal8Bit("");
	isDisableCloseWindow = false;
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
	RxUDPPort = 12341; 
	fmware_filename = pSettings.value(QString::fromLocal8Bit("FmwareFile"), QString::fromLocal8Bit("")).toString();
    ui->l_path->setText(QObject::tr("Path to the firmware file: %1").arg(fmware_filename));
}

FmWareDialog::~FmWareDialog()
{
    delete ui;
}

void FmWareDialog::comboBox_TypeDev_currentIndexChanged(int _idx){
	Q_UNUSED(_idx);
	updateListCID();
}

void FmWareDialog::setTitle(QString _title){
	ui->l_title->setText(_title);
}

void FmWareDialog::setMode(CModeExchange _mode){
	ModeExchange = _mode;
	if (ModeExchange == RS232Mode){
		ui->lineEdit_KeyAES->setEnabled(false);
		ui->label_KeyAES->setEnabled(false);
		ui->checkBox_all->setEnabled(false);
	}
	updateListCID();
}

void FmWareDialog::checkBox_all_stateChanged(int _state){
	Q_UNUSED(_state);
	if (ui->checkBox_all->isChecked()){
		ui->checkBox_all->setText(QObject::tr("unchecked all"));
		for(int _i = 0; _i < listCB.count(); _i++){
			listCB.at(_i)->setChecked(true);
		}
	} else {
		ui->checkBox_all->setText(QObject::tr("checked all"));
		for(int _i = 0; _i < listCB.count(); _i++){
			listCB.at(_i)->setChecked(false);
		}
	}
}

void FmWareDialog::updateListCID(){
	int cidName, typedev, filterIdx;
	QString _str;
//очистка таблицы
	ui->tableWidget_DeviceInfoState->setRowCount(0);
	for(int _i = 0; _i < listCB.count(); _i++){
		delete listCB.at(_i);
	}
	listCB.clear();
	for(int _i = 0; _i < listRB.count(); _i++){
		delete listRB.at(_i);
	}
	listRB.clear();
	listCids.clear();
	filterIdx = ui->comboBox_TypeDev->currentIndex();
	QJsonDocument doc = QJsonDocument::fromJson(globalConfigureStr.toLocal8Bit());
	QJsonObject mapObj = doc.object();
	QJsonArray listStations = mapObj[QString::fromLocal8Bit("Stations")].toArray();
	foreach (const QJsonValue & value, listStations) {
		QJsonObject obj = value.toObject();
		cidName	= obj[QString::fromLocal8Bit("cid")].toInt();
		typedev	= obj[QString::fromLocal8Bit("type")].toInt();
		_str.setNum(cidName);
		_str.prepend(QString::fromLocal8Bit("CID"));
		switch (filterIdx){
			case 0://all
				listCids.append(_str);
				break;
			case 1://PDO
				if (
					(((StationType)typedev) == StationType::PDO16) || 
					(((StationType)typedev) == StationType::PDO16N) || 
					(((StationType)typedev) == StationType::PDO32) ||
					(((StationType)typedev) == StationType::PDO32N) ||
					(((StationType)typedev) == StationType::PDO48)
					)
					listCids.append(_str);
				break;
			case 2://UPV
				if (
					(((StationType)typedev) == StationType::UPV2) || 
					(((StationType)typedev) == StationType::UPV4) || 
					(((StationType)typedev) == StationType::UPV6) ||
					(((StationType)typedev) == StationType::UPV2N) ||
					(((StationType)typedev) == StationType::UPV4N) ||
					(((StationType)typedev) == StationType::UPV_A)
					)
					listCids.append(_str);
				break;
			case 3://SL
				if ((((StationType)typedev) == StationType::SL1) ||
					(((StationType)typedev) == StationType::SL2)
					)
					listCids.append(_str);
				break;
			case 4://UM
				if (((StationType)typedev) == StationType::UM_300_2)
					listCids.append(_str);
				break;
			default:
				listCids.append(_str);
				break;
		}
	}
	ui->tableWidget_DeviceInfoState->setRowCount(listCids.count());
	for (int _i = 0; _i < listCids.count(); _i++){
		ui->tableWidget_DeviceInfoState->setItem(_i, 0, new QTableWidgetItem(listCids.at(_i)));
		if (ModeExchange == RS232Mode){
			QRadioButton* _rb = new QRadioButton();
			listRB.append(_rb);
			ui->tableWidget_DeviceInfoState->setCellWidget(_i, 1, _rb);
		} else {
			QCheckBox* _cb = new QCheckBox();
			listCB.append(_cb);
			ui->tableWidget_DeviceInfoState->setCellWidget(_i, 1, _cb);
		}
	}
	ui->tableWidget_DeviceInfoState->resizeColumnsToContents();
	ui->tableWidget_DeviceInfoState->setColumnWidth(1, 85);
	ui->tableWidget_DeviceInfoState->setColumnWidth(2, 100);
	ui->tableWidget_DeviceInfoState->setColumnWidth(3, 90);
	ui->tableWidget_DeviceInfoState->resizeRowsToContents();
	ui->tableWidget_DeviceInfoState->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void FmWareDialog::setRS232Name(QString _portname){
	portName = _portname;
}

void FmWareDialog::setSelfIP(QString _selfip){
	selfIP = _selfip;
}

void FmWareDialog::b_send_clicked()
{
	isDisableCloseWindow = true;
	bool _ok;
	QStringList _selected_items;
	QJsonDocument doc;
	QJsonObject mapObj;
	QJsonArray listStations;
	int _rx_type_dev;
	StationType _current_type_dev;
	QString _targetIP;
	qint64 _fmware_size;
	int _rc;
	int _rd;
    unsigned int _block_size = 0;
	unsigned int _length_wr_block;
	int count_errors = 0;
	QIcon* _icn;
	QToolButton* _tb;
//проверка на введение номера версии прошивки
	if (ui->lineEdit_version->text().isEmpty()){
		QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("Set version firmware number"));
		isDisableCloseWindow = false;
		return;
	}
	version = ui->lineEdit_version->text().toInt(&_ok);
//проверка на выбор файла прошивки
	if (fmware_filename.isEmpty()){
		QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("Select firmware file"));
		isDisableCloseWindow = false;
		return;
	}
//проверка на максимально допустимый размер файла прошивки
	if (QFileInfo(fmware_filename).size() > MAX_FMWARE_SIZE){
		QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("Selected file is too long"));
		isDisableCloseWindow = false;
		return;
	}
	doc = QJsonDocument::fromJson(globalConfigureStr.toLocal8Bit());
	mapObj = doc.object();
	listStations = mapObj[QString::fromLocal8Bit("Stations")].toArray();
//открываем файл прошивки и копируем данные в буфер для передачи
	QFile _fmwareFile(fmware_filename);
	if (!_fmwareFile.open(QIODevice::ReadOnly)){
		QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("File firmware is not open"));
		isDisableCloseWindow = false;
		return;
	}
	_fmwareFile.read(buff_tx, MAX_FMWARE_SIZE); 
	_fmware_size = QFileInfo(fmware_filename).size();
//проверка на выбор устройства для прошивки
	for(int _i = 0; _i < ui->tableWidget_DeviceInfoState->rowCount(); _i++){
		if (ModeExchange == RS232Mode){
			if (listRB.at(_i)->isChecked())
				_selected_items.append(listCids.at(_i));
		} else
		if (ModeExchange == NetworkMode){
			if (listCB.at(_i)->isChecked())
				_selected_items.append(listCids.at(_i));
		}
		_icn = new QIcon(QIcon(IPS_STR(":/svg/plus_icon.svg")));
		_tb = new QToolButton();
		_tb->setIcon(*_icn); 
		ui->tableWidget_DeviceInfoState->setCellWidget(_i, 2, _tb);
//		if (ui->tableWidget_DeviceInfoState->cellWidget(_i, 2) != NULL)
//			ui->tableWidget_DeviceInfoState->removeCellWidget(_i, 2);
	}
	if (_selected_items.count() == 0){
		QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("Select device for firmware"));
		isDisableCloseWindow = false;
		return;
	}
	pdSendConfig->show();
	isCancelWrite = false;
#ifdef __IS_WRITE_LOG__
	QString _fl = QString::fromLocal8Bit("Log.txt");
	QFile* _file = new QFile(_fl);
    if (!_file->open(QIODevice::WriteOnly | QIODevice::Text)){
        return;
	}
	_file->close();
	delete _file;
#endif
	if (ui->checkBox_UpdateSysInfo->isChecked() == false){
		pdSendConfig->setLabelText(QObject::tr("Send firmware device..."));
	} else {
		pdSendConfig->setLabelText(QObject::tr("Update firmware system info device..."));
	}
	for(int _i = 0; _i < _selected_items.count(); _i++){
//		btnCancel->setEnabled(false);
		if (isCancelWrite){
			pdSendConfig->close();
			isDisableCloseWindow = false;
			return;
		}
		pb->setMaximum(_selected_items.count());
		pdSendConfig->setValue(_i);
		pCommunicationDialog = new CommunicationDialog(this);
		pCommunicationDialog->setMode(ModeExchange);
		if (ModeExchange == RS232Mode){
			pCommunicationDialog->setRs232PortName(portName);
		} else
		if (ModeExchange == NetworkMode){
			pCommunicationDialog->setRxUDPPort(RxUDPPort); 
			pCommunicationDialog->setSelfIP(selfIP);
			pCommunicationDialog->setAesKey((uint8_t*)ui->lineEdit_KeyAES->text().toLocal8Bit().data(), 16);
			_targetIP = getIPFromCid(&listStations, _selected_items.at(_i));
			pCommunicationDialog->setTargetIP(_targetIP);
		}
		if (pCommunicationDialog->Open() == false){
			QMessageBox::critical(this, QObject::tr("Error"), pCommunicationDialog->getLastErrorString());
			delete pCommunicationDialog;
			isDisableCloseWindow = false;
			return;
		}
		connect(this, SIGNAL(sigCancel()), pCommunicationDialog, SLOT(cancel_transactions()), Qt::QueuedConnection);
//====считаем тип подключенного устройства====
//		pdSendConfig->setLabelText(QObject::tr("Get type device..."));
		_rx_type_dev = 0;
		if (!pCommunicationDialog->fnGetCFGUI(reinterpret_cast<unsigned int*>(&_rx_type_dev))){
			if (_selected_items.count() > 1){
				drawCidStatus(_selected_items.at(_i), false);
				count_errors++;
				disconnect(this, SIGNAL(sigCancel()), pCommunicationDialog, SLOT(cancel_transactions()));
				pCommunicationDialog->Close();
				delete pCommunicationDialog;
				continue;
			} else {
				drawCidStatus(_selected_items.at(_i), false);
				QMessageBox::critical(this, QObject::tr("Error"), pCommunicationDialog->getLastErrorString());
				disconnect(this, SIGNAL(sigCancel()), pCommunicationDialog, SLOT(cancel_transactions()));
				pCommunicationDialog->Close();
				delete pCommunicationDialog;
				pdSendConfig->close();
				isDisableCloseWindow = false;
				return;
			}
		}
//====проверка типа подключенного прибора====
		_current_type_dev = (StationType)getTypeFromCid(&listStations, _selected_items.at(_i));
		switch(_current_type_dev){
			case StationType::UM_300_2:
				if ((StationType)_rx_type_dev != StationType::UM_300_2){
					if (_selected_items.count() > 1){
						drawCidStatus(_selected_items.at(_i), false);
						count_errors++;
						disconnect(this, SIGNAL(sigCancel()), pCommunicationDialog, SLOT(cancel_transactions()));
						pCommunicationDialog->Close();
						delete pCommunicationDialog;
						continue;
					} else {
						drawCidStatus(_selected_items.at(_i), false);
						QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("the connected device does not match the type of the selected device "));
						disconnect(this, SIGNAL(sigCancel()), pCommunicationDialog, SLOT(cancel_transactions()));
						pCommunicationDialog->Close();
						delete pCommunicationDialog;
						pdSendConfig->close();
						isDisableCloseWindow = false;
						return;
					}
				}
				break;
			case StationType::PDO16:
			case StationType::PDO16N:
			case StationType::PDO32:
			case StationType::PDO32N:
			case StationType::PDO48:
				if (((StationType)_rx_type_dev != StationType::PDO16) && 
					((StationType)_rx_type_dev != StationType::PDO16N) && 
					((StationType)_rx_type_dev != StationType::PDO32) &&
					((StationType)_rx_type_dev != StationType::PDO32N) &&
					((StationType)_rx_type_dev != StationType::PDO48)
					){
					if (_selected_items.count() > 1){
//						drawCidStatus(_selected_items.at(_i), false);
//						count_errors++;
//						disconnect(this, SIGNAL(sigCancel()), pCommunicationDialog, SLOT(cancel_transactions()));
//						pCommunicationDialog->Close();
//						delete pCommunicationDialog;
//						continue;
					} else {
//						drawCidStatus(_selected_items.at(_i), false);
//						QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("the connected device does not match the type of the selected device "));
//						disconnect(this, SIGNAL(sigCancel()), pCommunicationDialog, SLOT(cancel_transactions()));
//						pCommunicationDialog->Close();
//						delete pCommunicationDialog;
//						pdSendConfig->close();
//						isDisableCloseWindow = false;
//						return;
					}
				}
				break;
			case StationType::UPV2:
			case StationType::UPV4:
			case StationType::UPV6:
			case StationType::UPV2N:
			case StationType::UPV4N:
			case StationType::UPV_A:
				if (((StationType)_rx_type_dev != StationType::UPV2) && 
					((StationType)_rx_type_dev != StationType::UPV4) && 
					((StationType)_rx_type_dev != StationType::UPV6) &&
					((StationType)_rx_type_dev != StationType::UPV2N) && 
					((StationType)_rx_type_dev != StationType::UPV4N) && 
					((StationType)_rx_type_dev != StationType::UPV_A)
					){
					if (_selected_items.count() > 1){
						drawCidStatus(_selected_items.at(_i), false);
						count_errors++;
						disconnect(this, SIGNAL(sigCancel()), pCommunicationDialog, SLOT(cancel_transactions()));
						pCommunicationDialog->Close();
						delete pCommunicationDialog;
						continue;
					} else {
						drawCidStatus(_selected_items.at(_i), false);
						QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("the connected device does not match the type of the selected device "));
						disconnect(this, SIGNAL(sigCancel()), pCommunicationDialog, SLOT(cancel_transactions()));
						pCommunicationDialog->Close();
						delete pCommunicationDialog;
						pdSendConfig->close();
						isDisableCloseWindow = false;
						return;
					}
				}
				break;
			case StationType::SL1:
			case StationType::SL2:
				if (((StationType)_rx_type_dev != StationType::SL1) &&
					((StationType)_rx_type_dev != StationType::SL2)
					){
					if (_selected_items.count() > 1){
						drawCidStatus(_selected_items.at(_i), false);
						count_errors++;
						disconnect(this, SIGNAL(sigCancel()), pCommunicationDialog, SLOT(cancel_transactions()));
						pCommunicationDialog->Close();
						delete pCommunicationDialog;
						continue;
					} else {
						drawCidStatus(_selected_items.at(_i), false);
						QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("the connected device does not match the type of the selected device "));
						disconnect(this, SIGNAL(sigCancel()), pCommunicationDialog, SLOT(cancel_transactions()));
						pCommunicationDialog->Close();
						delete pCommunicationDialog;
						pdSendConfig->close();
						isDisableCloseWindow = false;
						return;
					}
				}
				break;
			default:
				if (_selected_items.count() > 1){
					drawCidStatus(_selected_items.at(_i), false);
					count_errors++;
					disconnect(this, SIGNAL(sigCancel()), pCommunicationDialog, SLOT(cancel_transactions()));
					pCommunicationDialog->Close();
					delete pCommunicationDialog;
					continue;
				} else {
					drawCidStatus(_selected_items.at(_i), false);
					QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("Selected device unknown type"));
					disconnect(this, SIGNAL(sigCancel()), pCommunicationDialog, SLOT(cancel_transactions()));
					pCommunicationDialog->Close();
					delete pCommunicationDialog;
					pdSendConfig->close();
					isDisableCloseWindow = false;
					return;
				}
				break;
		}
//====записываем версию прошивки====
//инициализируем начало записи прошивки в ОЗУ устройства
//		pdSendConfig->setLabelText(QObject::tr("Set firmware on..."));
		if (!pCommunicationDialog->fnFwwareOn()){
			if (_selected_items.count() > 1){
				drawCidStatus(_selected_items.at(_i), false);
				count_errors++;
				disconnect(this, SIGNAL(sigCancel()), pCommunicationDialog, SLOT(cancel_transactions()));
				pCommunicationDialog->Close();
				delete pCommunicationDialog;
				continue;
			} else {
				drawCidStatus(_selected_items.at(_i), false);
				QMessageBox::critical(this, QObject::tr("Error"), pCommunicationDialog->getLastErrorString());
				disconnect(this, SIGNAL(sigCancel()), pCommunicationDialog, SLOT(cancel_transactions()));
				pCommunicationDialog->Close();
				delete pCommunicationDialog;
				pdSendConfig->close();
				isDisableCloseWindow = false;
				return;
			}
		}
//====установка системной информации конфигурации====
		pCommunicationDialog->crc32();
		txCRCFmware	= pCommunicationDialog->ProcessCRC(buff_tx, _fmware_size);
//		pdSendConfig->setLabelText(QObject::tr("Set system info configure device..."));
		if (!pCommunicationDialog->fnFwwareSetSysInfo(_fmware_size, txCRCFmware, version)){
			if (_selected_items.count() > 1){
				drawCidStatus(_selected_items.at(_i), false);
				count_errors++;
				disconnect(this, SIGNAL(sigCancel()), pCommunicationDialog, SLOT(cancel_transactions()));
				pCommunicationDialog->Close();
				delete pCommunicationDialog;
				continue;
			} else {
				drawCidStatus(_selected_items.at(_i), false);
				QMessageBox::critical(this, QObject::tr("Error"), pCommunicationDialog->getLastErrorString());
				disconnect(this, SIGNAL(sigCancel()), pCommunicationDialog, SLOT(cancel_transactions()));
				pCommunicationDialog->Close();
				delete pCommunicationDialog;
				pdSendConfig->close();
				isDisableCloseWindow = false;
				return;
			}
		}
		if (ui->checkBox_UpdateSysInfo->isChecked() == false){
//передача прошивки в ОЗУ устройства
//			pdSendConfig->setLabelText(QObject::tr("Set firmware data block..."));
			if (ModeExchange == RS232Mode){
				_block_size = FMWARE_BLOCK_SIZE_RS232;
			} else
			if (ModeExchange == NetworkMode){
				_block_size = FMWARE_BLOCK_SIZE_ETH;
			}
			_rc = _fmware_size/_block_size;
			_rd = _fmware_size % _block_size;
			pdSendConfig->setMaximum(100);
//			btnCancel->setEnabled(true);
			for(int _j = 0; _j < _rc; _j++){
				if (isCancelWrite){
					emit sigCancel();
					if (!pCommunicationDialog->fnFwwareOff()){
						QMessageBox::critical(this, QObject::tr("Error"), pCommunicationDialog->getLastErrorString());
					}
					disconnect(this, SIGNAL(sigCancel()), pCommunicationDialog, SLOT(cancel_transactions()));
					pCommunicationDialog->Close();
					delete pCommunicationDialog;
					pdSendConfig->close();
					isDisableCloseWindow = false;
					return;
				}
				_length_wr_block = _j;
				_length_wr_block *= _block_size;
				if (!pCommunicationDialog->fnFwwareSetBlock(&buff_tx[_length_wr_block], _length_wr_block, _block_size)){
					if (_selected_items.count() > 1){
						drawCidStatus(_selected_items.at(_i), false);
						count_errors++;
						break;
					} else {
						drawCidStatus(_selected_items.at(_i), false);
						QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("%1\n number block=%2").arg(pCommunicationDialog->getLastErrorString()).arg(_j));
						disconnect(this, SIGNAL(sigCancel()), pCommunicationDialog, SLOT(cancel_transactions()));
						pCommunicationDialog->Close();
						delete pCommunicationDialog;
						pdSendConfig->close();
						isDisableCloseWindow = false;
						return;
					}
				}
				pdSendConfig->setValue(((_j*100)/(_rc+1)));
				qApp->processEvents();
			}
			_length_wr_block = _rc;
			_length_wr_block *= _block_size;
			if (!pCommunicationDialog->fnFwwareSetBlock(&buff_tx[_length_wr_block], _length_wr_block,  _rd)){
				if (_selected_items.count() > 1){
					drawCidStatus(_selected_items.at(_i), false);
					count_errors++;
					disconnect(this, SIGNAL(sigCancel()), pCommunicationDialog, SLOT(cancel_transactions()));
					pCommunicationDialog->Close();
					delete pCommunicationDialog;
					continue;
				} else {
					drawCidStatus(_selected_items.at(_i), false);
					QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("%1\nlast block").arg(pCommunicationDialog->getLastErrorString()));
					disconnect(this, SIGNAL(sigCancel()), pCommunicationDialog, SLOT(cancel_transactions()));
					pCommunicationDialog->Close();
					delete pCommunicationDialog;
					pdSendConfig->close();
					isDisableCloseWindow = false;
					return;
				}
			}
			pdSendConfig->setValue(100);
//команда инициализации перезаписи прошивки из ОЗУ во FLASH устройства
		}
		btnCancel->setEnabled(false);
//		pdSendConfig->setLabelText(QObject::tr("Write fmware..."));
		if (!pCommunicationDialog->fnFwwareStartWrite()){
			if (_selected_items.count() > 1){
				drawCidStatus(_selected_items.at(_i), false);
				count_errors++;
				disconnect(this, SIGNAL(sigCancel()), pCommunicationDialog, SLOT(cancel_transactions()));
				pCommunicationDialog->Close();
				delete pCommunicationDialog;
				continue;
			} else {
				drawCidStatus(_selected_items.at(_i), false);
				QMessageBox::critical(this, QObject::tr("Error"), pCommunicationDialog->getLastErrorString());
				disconnect(this, SIGNAL(sigCancel()), pCommunicationDialog, SLOT(cancel_transactions()));
				pCommunicationDialog->Close();
				delete pCommunicationDialog;
				pdSendConfig->close();
				isDisableCloseWindow = false;
				return;
			}
		}
		drawCidStatus(_selected_items.at(_i), true);
		disconnect(this, SIGNAL(sigCancel()), pCommunicationDialog, SLOT(cancel_transactions()));
		pCommunicationDialog->Close();
		delete pCommunicationDialog;
	}
	if (count_errors){
		QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("Send firmware with errors!!!"));
	} else {
		QMessageBox::information(this, QObject::tr("Info"), QObject::tr("Send firmware succefulle!!!"));
	}
	pdSendConfig->close();
	isDisableCloseWindow = false;
}

void FmWareDialog::b_rcv_clicked(){
}

void FmWareDialog::drawCidStatus(QString _selected_cid, bool _isOkStatus){
	QIcon* _icn;
	QToolButton* _tb;
	for(int _j = 0; _j <  ui->tableWidget_DeviceInfoState->rowCount(); _j++){
		if (listCids.at(_j) == _selected_cid){
			if (_isOkStatus)
				_icn = new QIcon(QIcon(IPS_STR(":/svg/status_ok.svg"))); else
				_icn = new QIcon(QIcon(IPS_STR(":/svg/status_error.svg")));
			_tb = new QToolButton();
			_tb->setIcon(*_icn); 
			ui->tableWidget_DeviceInfoState->setCellWidget(_j, 2, _tb);
		}
	}
}

void FmWareDialog::setConfigureStr(QString _data){
	globalConfigureStr = _data;
	QJsonDocument doc = QJsonDocument::fromJson(globalConfigureStr.toLocal8Bit());
	QJsonObject mapSrcObj = doc.object();
	QJsonObject srcGlobalParameters = mapSrcObj[QString::fromLocal8Bit("Global parameters")].toObject();
	ui->lineEdit_KeyAES->setText(srcGlobalParameters[QString::fromLocal8Bit("key AES")].toString());
	RxUDPPort = srcGlobalParameters[QString::fromLocal8Bit("port configuration")].toInt(); 
	updateListCID();
}

void FmWareDialog::pdSendConfig_cancel(){
	QMessageBox::StandardButton ret = QMessageBox::warning(this, QObject::tr("Warning"), QObject::tr("This process is not finished\nReject process(Yes/No)"), QMessageBox::Yes | QMessageBox::No);
	if (ret == QMessageBox::Yes){
		isCancelWrite = true;
	}
}

QString FmWareDialog::getIPFromCid(QJsonArray* StationList, QString _cid){
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

void FmWareDialog::b_loadFirmFile_clicked(){
//	static Settings _settings;
    auto const& file = QFileDialog::getOpenFileName(this, QObject::tr("Open firmware file...")
                                    , QDir::currentPath(), QObject::tr("Firmware files (*.bin)"));        
	if (!file.isEmpty()){
        ui->l_path->setText(QObject::tr("Path to the firmware file: %1").arg(file));
		fmware_filename = file;
		pSettings.setValue(QString::fromLocal8Bit("FmwareFile"), fmware_filename);
	}
}

int FmWareDialog::getTypeFromCid(QJsonArray* StationList, QString _cid){
	int _result = 0;
	QJsonObject mapStation;
	int cidName;
	QString str;
	for(int _i = 0; _i < StationList->count(); _i++){
		mapStation = StationList->at(_i).toObject();
		cidName	= mapStation[QString::fromLocal8Bit("cid")].toInt();
		str.setNum(cidName);
		if (_cid.indexOf(str) != -1){
			return mapStation[QString::fromLocal8Bit("type")].toInt();
		}
	}
	return _result;
}

void FmWareDialog::b_ReadVersions_clicked(){
	isDisableCloseWindow = true;
	QStringList _selected_items;
	QJsonDocument doc;
	QJsonObject mapObj;
	QJsonArray listStations;
	QString _targetIP;
	QIcon* _icn;
	QToolButton* _tb;
	int count_errors = 0;
	doc = QJsonDocument::fromJson(globalConfigureStr.toLocal8Bit());
	mapObj = doc.object();
	listStations = mapObj[QString::fromLocal8Bit("Stations")].toArray();
	for(int _i = 0; _i < ui->tableWidget_DeviceInfoState->rowCount(); _i++){
		if (ModeExchange == RS232Mode){
			if (listRB.at(_i)->isChecked())
				_selected_items.append(listCids.at(_i));
		} else
		if (ModeExchange == NetworkMode){
			if (listCB.at(_i)->isChecked())
				_selected_items.append(listCids.at(_i));
		}
		_icn = new QIcon(QIcon(IPS_STR(":/svg/plus_icon.svg")));
		_tb = new QToolButton();
		_tb->setIcon(*_icn); 
		ui->tableWidget_DeviceInfoState->setCellWidget(_i, 2, _tb);
//		if (ui->tableWidget_DeviceInfoState->cellWidget(_i, 2) != NULL)
//			ui->tableWidget_DeviceInfoState->removeCellWidget(_i, 2);
	}
	if (_selected_items.count() == 0){
		QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("Select device for firmware"));
		isDisableCloseWindow = false;
		return;
	}
	pdSendConfig->setValue(0);
	pdSendConfig->show();
	btnCancel->setEnabled(true);
	pdSendConfig->setMaximum(_selected_items.count());
	pb->setMaximum(_selected_items.count());
	isCancelWrite = false;
#ifdef __IS_WRITE_LOG__
	QString _fl = QString::fromLocal8Bit("Log.txt");
	QFile* _file = new QFile(_fl);
    if (!_file->open(QIODevice::WriteOnly | QIODevice::Text)){
        return;
	}
	_file->close();
	delete _file;
#endif
	pdSendConfig->setLabelText(QObject::tr("Get version firmware..."));
	for(int _i = 0; _i < _selected_items.count(); _i++){
		if (isCancelWrite){
			isDisableCloseWindow = false;
			pdSendConfig->hide();
			pdSendConfig->close();
			break;
		}
		pdSendConfig->show();
		pCommunicationDialog = new CommunicationDialog(this);
		pCommunicationDialog->setMode(ModeExchange);
		if (ModeExchange == RS232Mode){
			pCommunicationDialog->setRs232PortName(portName);
		} else
		if (ModeExchange == NetworkMode){
			pCommunicationDialog->setRxUDPPort(RxUDPPort); 
			pCommunicationDialog->setSelfIP(selfIP);
			pCommunicationDialog->setAesKey((uint8_t*)ui->lineEdit_KeyAES->text().toLocal8Bit().data(), 16);
			_targetIP = getIPFromCid(&listStations, _selected_items.at(_i));
			pCommunicationDialog->setTargetIP(_targetIP);
		}
		if (pCommunicationDialog->Open() == false){
			QMessageBox::critical(this, QObject::tr("Error"), pCommunicationDialog->getLastErrorString());
			delete pCommunicationDialog;
			pdSendConfig->hide();
			isDisableCloseWindow = false;
			return;
		}
		connect(this, SIGNAL(sigCancel()), pCommunicationDialog, SLOT(cancel_transactions()), Qt::QueuedConnection);
//инициализируем начало работы с firmware
		if (!pCommunicationDialog->fnFwwareOn()){
			if (_selected_items.count() == 1){
				QMessageBox::critical(this, QObject::tr("Error"), pCommunicationDialog->getLastErrorString());
			}
			disconnect(this, SIGNAL(sigCancel()), pCommunicationDialog, SLOT(cancel_transactions()));
			pCommunicationDialog->Close();
			delete pCommunicationDialog;
			pdSendConfig->setValue(_i+1);
			drawCidStatus(_selected_items.at(_i), false);
			count_errors++;
			continue;
		}
//читаем версию устройства
		if (!pCommunicationDialog->fnFmwareGetVersion(&version)){
			if (_selected_items.count() == 1){
				QMessageBox::critical(this, QObject::tr("Error"), pCommunicationDialog->getLastErrorString());
			}
			disconnect(this, SIGNAL(sigCancel()), pCommunicationDialog, SLOT(cancel_transactions()));
			pCommunicationDialog->Close();
			delete pCommunicationDialog;
			pdSendConfig->setValue(_i+1);
			drawCidStatus(_selected_items.at(_i), false);
			continue;
		}
//деинициализируем работы с firmware
		if (!pCommunicationDialog->fnFwwareOff()){
			if (_selected_items.count() == 1){
				QMessageBox::critical(this, QObject::tr("Error"), pCommunicationDialog->getLastErrorString());
			}
			disconnect(this, SIGNAL(sigCancel()), pCommunicationDialog, SLOT(cancel_transactions()));
			pCommunicationDialog->Close();
			delete pCommunicationDialog;
			pdSendConfig->setValue(_i+1);
			drawCidStatus(_selected_items.at(_i), false);
			continue;
		}
		pdSendConfig->setValue(_i+1);
//команда инициализации перезаписи прошивки из ОЗУ во FLASH устройства
		for(int _j = 0; _j <  ui->tableWidget_DeviceInfoState->rowCount(); _j++){
			if (listCids.at(_j) == _selected_items.at(_i)){
				_icn = new QIcon(QIcon(IPS_STR(":/svg/status_ok.svg")));
				_tb = new QToolButton();
				_tb->setIcon(*_icn); 
				ui->tableWidget_DeviceInfoState->setCellWidget(_j, 2, _tb);
				ui->tableWidget_DeviceInfoState->setItem(_j, 3, new QTableWidgetItem(QString::number(version)));
			}
		}
		disconnect(this, SIGNAL(sigCancel()), pCommunicationDialog, SLOT(cancel_transactions()));
		pCommunicationDialog->Close();
		delete pCommunicationDialog;
	}
	if (count_errors){
		QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("Read version firmware devices with errors!!!"));
	} else {
		QMessageBox::information(this, QObject::tr("Info"), QObject::tr("Read version firmware devices succefulle!!!"));
	}
	pdSendConfig->hide();
	pdSendConfig->close();
	isDisableCloseWindow = false;
}

QPushButton* FmWareDialog::get_b_close(){
	return ui->b_close;
}

QPushButton* FmWareDialog::get_b_loadFirmFile(){
	return ui->b_loadFirmFile;
}

QPushButton* FmWareDialog::get_b_ReadVersions(){
	return ui->b_ReadVersions;
}

QPushButton* FmWareDialog::get_b_send(){
	return ui->b_send;
}

QPushButton* FmWareDialog::get_b_rcv(){
	return ui->b_rcv;
}

QLineEdit*	 FmWareDialog::get_lineEdit_KeyAES(){
	return ui->lineEdit_KeyAES;
}

QLineEdit*	 FmWareDialog::get_lineEdit_version(){
	return ui->lineEdit_version;
}

QComboBox*	 FmWareDialog::get_comboBox_TypeDev(){
	return ui->comboBox_TypeDev;
}

QCheckBox*	 FmWareDialog::get_checkBox_all(){
	return ui->checkBox_all;
}

void FmWareDialog::setStateTestingMode(bool _isTestMode){
	isTestMode = _isTestMode;
}

bool FmWareDialog::isTestResult(){
	return testResult;
}

QString FmWareDialog::getTestMsg(){
	return testMsg;
}

void FmWareDialog::closeEvent( QCloseEvent *ev){
	Q_UNUSED(ev);
	if (!isDisableCloseWindow){
		accept(); 
	} else {
        ev->ignore(); 
//		QDialog::closeEvent(ev);
	}
}

#if defined(__clang__)
	#pragma clang diagnostic pop
#elif defined(__GNUC__) || defined(__GNUG__)
	#pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#endif
