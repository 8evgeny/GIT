/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_Qt;
    QAction *about;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QGroupBox *mcuSelectGB;
    QHBoxLayout *horizontalLayout;
    QRadioButton *mcu9xRB;
    QRadioButton *mcu1RB;
    QRadioButton *mcu4RB;
    QRadioButton *mcuVC1RB;
    QPushButton *dumpfnPB;
    QComboBox *portNameCB;
    QLineEdit *dumpfnLE;
    QLineEdit *fwfnLE;
    QPushButton *openFilePB;
    QComboBox *portBaudrateCB;
    QPushButton *flasherfnPB;
    QLineEdit *flasherfnLE;
    QPushButton *touchMCUPB;
    QLabel *sL;
    QPlainTextEdit *t;
    QGroupBox *taskGB;
    QHBoxLayout *horizontalLayout_2;
    QCheckBox *loadCB;
    QCheckBox *eraseCB;
    QCheckBox *programCB;
    QCheckBox *verifyCB;
    QCheckBox *runCB;
    QMenuBar *menuBar;
    QMenu *menu;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(626, 566);
        QFont font;
        font.setPointSize(13);
        MainWindow->setFont(font);
        action_Qt = new QAction(MainWindow);
        action_Qt->setObjectName(QString::fromUtf8("action_Qt"));
        about = new QAction(MainWindow);
        about->setObjectName(QString::fromUtf8("about"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        mcuSelectGB = new QGroupBox(centralWidget);
        mcuSelectGB->setObjectName(QString::fromUtf8("mcuSelectGB"));
        QFont font1;
        font1.setPointSize(10);
        mcuSelectGB->setFont(font1);
        horizontalLayout = new QHBoxLayout(mcuSelectGB);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        mcu9xRB = new QRadioButton(mcuSelectGB);
        mcu9xRB->setObjectName(QString::fromUtf8("mcu9xRB"));
        mcu9xRB->setChecked(true);

        horizontalLayout->addWidget(mcu9xRB);

        mcu1RB = new QRadioButton(mcuSelectGB);
        mcu1RB->setObjectName(QString::fromUtf8("mcu1RB"));

        horizontalLayout->addWidget(mcu1RB);

        mcu4RB = new QRadioButton(mcuSelectGB);
        mcu4RB->setObjectName(QString::fromUtf8("mcu4RB"));

        horizontalLayout->addWidget(mcu4RB);

        mcuVC1RB = new QRadioButton(mcuSelectGB);
        mcuVC1RB->setObjectName(QString::fromUtf8("mcuVC1RB"));

        horizontalLayout->addWidget(mcuVC1RB);


        gridLayout->addWidget(mcuSelectGB, 0, 0, 1, 2);

        dumpfnPB = new QPushButton(centralWidget);
        dumpfnPB->setObjectName(QString::fromUtf8("dumpfnPB"));
        dumpfnPB->setEnabled(false);

        gridLayout->addWidget(dumpfnPB, 4, 0, 2, 1);

        portNameCB = new QComboBox(centralWidget);
        portNameCB->setObjectName(QString::fromUtf8("portNameCB"));

        gridLayout->addWidget(portNameCB, 2, 1, 1, 1);

        dumpfnLE = new QLineEdit(centralWidget);
        dumpfnLE->setObjectName(QString::fromUtf8("dumpfnLE"));
        dumpfnLE->setEnabled(false);

        gridLayout->addWidget(dumpfnLE, 5, 1, 1, 1);

        fwfnLE = new QLineEdit(centralWidget);
        fwfnLE->setObjectName(QString::fromUtf8("fwfnLE"));

        gridLayout->addWidget(fwfnLE, 1, 1, 1, 1);

        openFilePB = new QPushButton(centralWidget);
        openFilePB->setObjectName(QString::fromUtf8("openFilePB"));

        gridLayout->addWidget(openFilePB, 1, 0, 1, 1);

        portBaudrateCB = new QComboBox(centralWidget);
        portBaudrateCB->addItem(QString());
        portBaudrateCB->addItem(QString());
        portBaudrateCB->addItem(QString());
        portBaudrateCB->addItem(QString());
        portBaudrateCB->addItem(QString());
        portBaudrateCB->addItem(QString());
        portBaudrateCB->addItem(QString());
        portBaudrateCB->addItem(QString());
        portBaudrateCB->setObjectName(QString::fromUtf8("portBaudrateCB"));

        gridLayout->addWidget(portBaudrateCB, 2, 0, 1, 1);

        flasherfnPB = new QPushButton(centralWidget);
        flasherfnPB->setObjectName(QString::fromUtf8("flasherfnPB"));
        flasherfnPB->setEnabled(false);

        gridLayout->addWidget(flasherfnPB, 6, 0, 1, 1);

        flasherfnLE = new QLineEdit(centralWidget);
        flasherfnLE->setObjectName(QString::fromUtf8("flasherfnLE"));
        flasherfnLE->setEnabled(false);

        gridLayout->addWidget(flasherfnLE, 6, 1, 1, 1);

        touchMCUPB = new QPushButton(centralWidget);
        touchMCUPB->setObjectName(QString::fromUtf8("touchMCUPB"));
        touchMCUPB->setCheckable(true);

        gridLayout->addWidget(touchMCUPB, 7, 0, 1, 1);

        sL = new QLabel(centralWidget);
        sL->setObjectName(QString::fromUtf8("sL"));
        sL->setFrameShape(QFrame::Box);

        gridLayout->addWidget(sL, 7, 1, 1, 1);

        t = new QPlainTextEdit(centralWidget);
        t->setObjectName(QString::fromUtf8("t"));
        t->setFont(font1);
        t->setReadOnly(true);

        gridLayout->addWidget(t, 8, 0, 1, 2);

        taskGB = new QGroupBox(centralWidget);
        taskGB->setObjectName(QString::fromUtf8("taskGB"));
        taskGB->setFont(font1);
        horizontalLayout_2 = new QHBoxLayout(taskGB);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        loadCB = new QCheckBox(taskGB);
        loadCB->setObjectName(QString::fromUtf8("loadCB"));

        horizontalLayout_2->addWidget(loadCB);

        eraseCB = new QCheckBox(taskGB);
        eraseCB->setObjectName(QString::fromUtf8("eraseCB"));

        horizontalLayout_2->addWidget(eraseCB);

        programCB = new QCheckBox(taskGB);
        programCB->setObjectName(QString::fromUtf8("programCB"));

        horizontalLayout_2->addWidget(programCB);

        verifyCB = new QCheckBox(taskGB);
        verifyCB->setObjectName(QString::fromUtf8("verifyCB"));
        verifyCB->setChecked(true);

        horizontalLayout_2->addWidget(verifyCB);

        runCB = new QCheckBox(taskGB);
        runCB->setObjectName(QString::fromUtf8("runCB"));

        horizontalLayout_2->addWidget(runCB);


        gridLayout->addWidget(taskGB, 3, 0, 1, 2);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 626, 26));
        menu = new QMenu(menuBar);
        menu->setObjectName(QString::fromUtf8("menu"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());
        menu->addAction(action_Qt);
        menu->addAction(about);

        retranslateUi(MainWindow);

        portBaudrateCB->setCurrentIndex(4);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MdrFwUart v.0.1.5", nullptr));
        action_Qt->setText(QApplication::translate("MainWindow", "\320\236 Qt", nullptr));
        about->setText(QApplication::translate("MainWindow", "\320\236 \320\277\321\200\320\276\320\263\321\200\320\260\320\274\320\274\320\265", nullptr));
        mcuSelectGB->setTitle(QApplication::translate("MainWindow", "\320\222\321\213\320\261\320\265\321\200\320\270\321\202\320\265 \320\274\320\270\320\272\321\200\320\276\320\272\320\276\320\275\321\202\321\200\320\276\320\273\320\273\320\265\321\200", nullptr));
        mcu9xRB->setText(QApplication::translate("MainWindow", "1986\320\222\320\2259x", nullptr));
        mcu1RB->setText(QApplication::translate("MainWindow", "1986\320\222\320\2251, 1986\320\222\320\2253", nullptr));
        mcu4RB->setText(QApplication::translate("MainWindow", "1986\320\222\320\2254", nullptr));
        mcuVC1RB->setText(QApplication::translate("MainWindow", "1901\320\222\320\2461\320\242", nullptr));
        dumpfnPB->setText(QApplication::translate("MainWindow", "\320\244\320\260\320\271\320\273 \320\264\320\260\320\274\320\277\320\260", nullptr));
        dumpfnLE->setText(QString());
        dumpfnLE->setPlaceholderText(QApplication::translate("MainWindow", "\320\222\320\262\320\265\320\264\320\270\321\202\320\265 \320\270\320\274\321\217 \321\204\320\260\320\271\320\273\320\260 \320\264\320\273\321\217 \321\201\320\276\321\205\321\200\320\260\320\275\320\265\320\275\320\270\321\217 \320\277\321\200\320\276\321\210\320\270\320\262\320\272\320\270", nullptr));
        fwfnLE->setPlaceholderText(QApplication::translate("MainWindow", "\320\222\321\213\320\261\320\265\321\200\320\270\321\202\320\265 \321\204\320\260\320\271\320\273 \320\277\321\200\320\276\321\210\320\270\320\262\320\272\320\270", nullptr));
        openFilePB->setText(QApplication::translate("MainWindow", "\320\244\320\260\320\271\320\273 \320\277\321\200\320\276\321\210\320\270\320\262\320\272\320\270", nullptr));
        portBaudrateCB->setItemText(0, QApplication::translate("MainWindow", "9600", nullptr));
        portBaudrateCB->setItemText(1, QApplication::translate("MainWindow", "19200", nullptr));
        portBaudrateCB->setItemText(2, QApplication::translate("MainWindow", "38400", nullptr));
        portBaudrateCB->setItemText(3, QApplication::translate("MainWindow", "57600", nullptr));
        portBaudrateCB->setItemText(4, QApplication::translate("MainWindow", "115200", nullptr));
        portBaudrateCB->setItemText(5, QApplication::translate("MainWindow", "230400", nullptr));
        portBaudrateCB->setItemText(6, QApplication::translate("MainWindow", "460800", nullptr));
        portBaudrateCB->setItemText(7, QApplication::translate("MainWindow", "921600", nullptr));

        flasherfnPB->setText(QApplication::translate("MainWindow", "\320\244\320\260\320\271\320\273 \321\204\320\273\320\265\321\210\320\265\321\200\320\260", nullptr));
        flasherfnLE->setText(QString());
        flasherfnLE->setPlaceholderText(QApplication::translate("MainWindow", "\320\236\321\201\321\202\320\260\320\262\321\214\321\202\320\265 \320\277\320\276\320\273\320\265 \320\277\321\203\321\201\321\202\321\213\320\274 \320\264\320\273\321\217 \320\270\321\201\320\277\320\276\320\273\321\214\320\267\320\276\320\262\320\260\320\275\320\270\321\217 \321\201\321\202\320\260\320\275\320\264\320\260\321\200\321\202\320\275\320\276\320\263\320\276  \321\204\320\273\320\265\321\210\320\265\321\200\320\260", nullptr));
        touchMCUPB->setText(QApplication::translate("MainWindow", "Start", nullptr));
        sL->setText(QApplication::translate("MainWindow", "\320\245\320\276\320\264 \321\200\320\260\320\261\320\276\321\202\321\213", nullptr));
        taskGB->setTitle(QApplication::translate("MainWindow", "\320\227\320\260\320\264\320\260\321\207\320\270", nullptr));
        loadCB->setText(QApplication::translate("MainWindow", "\320\241\321\207\320\270\321\202\320\260\321\202\321\214", nullptr));
        eraseCB->setText(QApplication::translate("MainWindow", "\320\241\321\202\320\265\321\200\320\265\321\202\321\214", nullptr));
        programCB->setText(QApplication::translate("MainWindow", "\320\237\321\200\320\276\320\263\321\200\320\260\320\274\320\274\320\270\321\200\320\276\320\262\320\260\321\202\321\214", nullptr));
        verifyCB->setText(QApplication::translate("MainWindow", "\320\241\321\200\320\260\320\262\320\275\320\270\321\202\321\214", nullptr));
        runCB->setText(QApplication::translate("MainWindow", "\320\227\320\260\320\277\321\203\321\201\321\202\320\270\321\202\321\214", nullptr));
        menu->setTitle(QApplication::translate("MainWindow", "\320\241\320\277\321\200\320\260\320\262\320\272\320\260", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
