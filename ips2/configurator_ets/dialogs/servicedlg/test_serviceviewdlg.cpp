#include <QString>
#include <QTest>
#include <QtWidgets>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "test_serviceviewdlg.h"

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

Test_ServiceViewDialog::Test_ServiceViewDialog(QWidget *parent)
    : QObject(parent)
{
	pServiceViewDialog = new (ServiceViewDialog);
}

Test_ServiceViewDialog::~Test_ServiceViewDialog()
{
	delete pServiceViewDialog;
}

void Test_ServiceViewDialog::Test_ServiceDialog(){
}

#if defined(__clang__)
	#pragma clang diagnostic pop
#elif defined(__GNUC__) || defined(__GNUG__)
	#pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#endif
