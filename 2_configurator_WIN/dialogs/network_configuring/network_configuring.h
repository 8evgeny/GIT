#pragma once
#include "include/typedefs.h"
#include <QtCore\QSortFilterProxyModel.h>
#include <QtGui\QStandardItemModel.h>
#include <QtCore\QStringList.h>
#include <QtCore\QSaveFile.h>
#include <QtCore\QFile.h>
#include <QtNetwork\QNetworkInterface>
#include "../stringlist/string_list.h"
#include "network_configuring_pch.h"
#include "network_configuring_global.h"

IPS_UI(NetworkConfiguringDialog)

class NETWORK_CONFIGURING_DIALOG_SHARED_EXPORT NetworkConfiguringDialog final: public QDialog {
    Q_OBJECT
    IPS_DISABLE_COPY(NetworkConfiguringDialog)

 public:
    using inherited = QDialog;
    using class_name = NetworkConfiguringDialog;

 public:
    explicit NetworkConfiguringDialog(QWidget* parent = nullptr);
    ~NetworkConfiguringDialog() override;

 public:
    void instanceNetworkList();
    void insatnceEnable();
	void setCidList(QStringList _cidList);
	void setConfigureStr(QString _data);

 public slots:
	 void b_open_clicked();
	 void b_send_clicked();
 private:
    QString IP2Str_Dec( unsigned int _IP );
    Ui::NetworkConfiguringDialog* ui = nullptr;
	QStringList* pCidList;
	StringListDialog* dlgCidList;
};
