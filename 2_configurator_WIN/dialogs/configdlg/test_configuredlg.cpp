#include <QString>
#include <QTest>
#include <QtWidgets>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "test_configuredlg.h"

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

Test_ConfigureDialog::Test_ConfigureDialog(QWidget *parent)
    : QObject(parent)
{
	pConfigDlg = new (ConfigureDialog);
}

Test_ConfigureDialog::~Test_ConfigureDialog()
{
	delete pConfigDlg;
}

void Test_ConfigureDialog::Test_Constructions(){
	QVERIFY2(pConfigDlg->getBtn_b_setSN(), "Button b_setSN not created");
	QVERIFY2(pConfigDlg->getBtn_b_GetAES(), "Button b_GetAES() not created");
	QVERIFY2(pConfigDlg->getBtn_b_SetAES(), "Button b_SetAES() not created");
	QVERIFY2(pConfigDlg->getBtn_b_send(), "Button b_send() not created");
	QVERIFY2(pConfigDlg->getBtn_b_rcv(), "Button b_rcv() not created");
	QVERIFY2(pConfigDlg->getLN_SN(), "LineEdit lineEdit_SN not created");
	QVERIFY2(pConfigDlg->getLN_KeyAes(), "LineEdit lineEdit_KeyAES not created");
	QVERIFY2(pConfigDlg->getLN_m_settings(), "LineEdit m_settings not created");
	QVERIFY2(pConfigDlg->getLN_IPStation(), "LineEdit lineEdit_IPStation not created");
//	QEXPECT_FAIL("", "Button b_setSN not created", Continue);
//	QVERIFY(pConfigDlg->getBtn_b_setSN());
}

void Test_ConfigureDialog::Test_b_SetSN_clicked(){
	pConfigDlg->setStateTestingMode(true);
	pConfigDlg->setMode(NetworkMode);
	pConfigDlg->setSelfIP(QString::fromLocal8Bit("127.0.0.1"));
	pConfigDlg->getLN_IPStation()->setText(QString::fromLocal8Bit("127.0.0.1"));
	pConfigDlg->getLN_KeyAes()->setText(QString::fromLocal8Bit("000102030405060708090A0B0C0D0E0F"));
	pConfigDlg->b_SetSN_clicked();
	if (!pConfigDlg->isTestResult()){
		QFAIL(pConfigDlg->getTestMsg().toLocal8Bit().constData());
	}
	pConfigDlg->setStateTestingMode(false);
}

void Test_ConfigureDialog::Test_b_GetAES_clicked(){
	pConfigDlg->setStateTestingMode(true);
	pConfigDlg->setMode(NetworkMode);
	pConfigDlg->setSelfIP(QString::fromLocal8Bit("127.0.0.1"));
	pConfigDlg->getLN_IPStation()->setText(QString::fromLocal8Bit("127.0.0.1"));
	pConfigDlg->getLN_KeyAes()->setText(QString::fromLocal8Bit("000102030405060708090A0B0C0D0E0F"));
	pConfigDlg->b_GetAES_clicked();
	if (!pConfigDlg->isTestResult()){
		QFAIL(pConfigDlg->getTestMsg().toLocal8Bit().constData());
	}
	pConfigDlg->setStateTestingMode(false);
}

void Test_ConfigureDialog::Test_b_SetAES_clicked(){
	pConfigDlg->setStateTestingMode(true);
	pConfigDlg->setMode(NetworkMode);
	pConfigDlg->setSelfIP(QString::fromLocal8Bit("127.0.0.1"));
	pConfigDlg->getLN_IPStation()->setText(QString::fromLocal8Bit("127.0.0.1"));
	pConfigDlg->getLN_KeyAes()->setText(QString::fromLocal8Bit("000102030405060708090A0B0C0D0E0F"));
	pConfigDlg->b_SetAES_clicked();
	if (!pConfigDlg->isTestResult()){
		QFAIL(pConfigDlg->getTestMsg().toLocal8Bit().constData());
	}
	pConfigDlg->setStateTestingMode(false);
}

#if defined(__clang__)
	#pragma clang diagnostic pop
#elif defined(__GNUC__) || defined(__GNUG__)
	#pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#endif
