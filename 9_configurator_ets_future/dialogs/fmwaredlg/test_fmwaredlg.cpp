#include <QString>
#include <QTest>
#include <QtWidgets>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "test_fmwaredlg.h"

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

Test_FmwareDialog::Test_FmwareDialog(QWidget *parent)
    : QObject(parent)
{
	pFmWareDlg = new (FmWareDialog);
}

Test_FmwareDialog::~Test_FmwareDialog()
{
	delete pFmWareDlg;
}

void Test_FmwareDialog::Test_Constructions(){
	QVERIFY2(pFmWareDlg->get_b_close(), "Button b_close not created");
	QVERIFY2(pFmWareDlg->get_b_loadFirmFile(), "Button b_loadFirmFile not created");
	QVERIFY2(pFmWareDlg->get_b_ReadVersions(), "Button b_ReadVersions not created");
	QVERIFY2(pFmWareDlg->get_b_send(), "Button b_send not created");
	QVERIFY2(pFmWareDlg->get_b_rcv(), "Button b_rcv not created");
	QVERIFY2(pFmWareDlg->get_lineEdit_KeyAES(), "LineEdit lineEdit_KeyAES not created");
	QVERIFY2(pFmWareDlg->get_lineEdit_version(), "LineEdit lineEdit_version not created");
	QVERIFY2(pFmWareDlg->get_comboBox_TypeDev(), "ComboBox comboBox_TypeDev not created");
	QVERIFY2(pFmWareDlg->get_checkBox_all(), "CheckBox checkBox_all not created");
}

void Test_FmwareDialog::Test_SendFmware(){
}

#if defined(__clang__)
	#pragma clang diagnostic pop
#elif defined(__GNUC__) || defined(__GNUG__)
	#pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#endif
