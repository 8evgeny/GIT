/********************************************************************************
** Form generated from reading UI file 'mdrfwuform.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MDRFWUFORM_H
#define UI_MDRFWUFORM_H

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
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MdrFwUForm
{
public:
    QAction *action_Qt;
    QAction *about;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_3;
    QComboBox *portBaudrateCB;
    QComboBox *portNameCB;
    QPushButton *openFilePB;
    QLineEdit *fwfnLE;
    QLineEdit *flasherfnLE;
    QLabel *sL;
    QLabel *label;
    QGroupBox *taskGB;
    QHBoxLayout *horizontalLayout_2;
    QCheckBox *dumpCB;
    QCheckBox *eraseCB;
    QCheckBox *programCB;
    QCheckBox *verifyCB;
    QCheckBox *runCB;
    QLineEdit *dumpfnLE;
    QPushButton *dumpfnPB;
    QPushButton *startStopPB;
    QComboBox *mcuSelectCB;
    QPlainTextEdit *t;
    QPushButton *flasherfnPB;
    QMenuBar *menuBar;
    QMenu *menu;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MdrFwUForm)
    {
        if (MdrFwUForm->objectName().isEmpty())
            MdrFwUForm->setObjectName(QString::fromUtf8("MdrFwUForm"));
        MdrFwUForm->resize(850, 700);
        QFont font;
        font.setPointSize(13);
        MdrFwUForm->setFont(font);
        action_Qt = new QAction(MdrFwUForm);
        action_Qt->setObjectName(QString::fromUtf8("action_Qt"));
        about = new QAction(MdrFwUForm);
        about->setObjectName(QString::fromUtf8("about"));
        centralWidget = new QWidget(MdrFwUForm);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QFont font1;
        font1.setPointSize(10);
        groupBox->setFont(font1);
        horizontalLayout_3 = new QHBoxLayout(groupBox);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        portBaudrateCB = new QComboBox(groupBox);
        portBaudrateCB->addItem(QString());
        portBaudrateCB->addItem(QString());
        portBaudrateCB->addItem(QString());
        portBaudrateCB->addItem(QString());
        portBaudrateCB->addItem(QString());
        portBaudrateCB->addItem(QString());
        portBaudrateCB->addItem(QString());
        portBaudrateCB->addItem(QString());
        portBaudrateCB->addItem(QString());
        portBaudrateCB->addItem(QString());
        portBaudrateCB->addItem(QString());
        portBaudrateCB->setObjectName(QString::fromUtf8("portBaudrateCB"));

        horizontalLayout_3->addWidget(portBaudrateCB);

        portNameCB = new QComboBox(groupBox);
        portNameCB->setObjectName(QString::fromUtf8("portNameCB"));

        horizontalLayout_3->addWidget(portNameCB);

        horizontalLayout_3->setStretch(0, 1);
        horizontalLayout_3->setStretch(1, 4);

        gridLayout->addWidget(groupBox, 7, 0, 1, 2);

        openFilePB = new QPushButton(centralWidget);
        openFilePB->setObjectName(QString::fromUtf8("openFilePB"));

        gridLayout->addWidget(openFilePB, 6, 0, 1, 1);

        fwfnLE = new QLineEdit(centralWidget);
        fwfnLE->setObjectName(QString::fromUtf8("fwfnLE"));

        gridLayout->addWidget(fwfnLE, 6, 1, 1, 1);

        flasherfnLE = new QLineEdit(centralWidget);
        flasherfnLE->setObjectName(QString::fromUtf8("flasherfnLE"));
        flasherfnLE->setEnabled(false);

        gridLayout->addWidget(flasherfnLE, 11, 1, 1, 1);

        sL = new QLabel(centralWidget);
        sL->setObjectName(QString::fromUtf8("sL"));
        sL->setFrameShape(QFrame::Box);

        gridLayout->addWidget(sL, 12, 1, 1, 1);

        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 2, 0, 1, 1);

        taskGB = new QGroupBox(centralWidget);
        taskGB->setObjectName(QString::fromUtf8("taskGB"));
        taskGB->setFont(font1);
        horizontalLayout_2 = new QHBoxLayout(taskGB);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        dumpCB = new QCheckBox(taskGB);
        dumpCB->setObjectName(QString::fromUtf8("dumpCB"));

        horizontalLayout_2->addWidget(dumpCB);

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


        gridLayout->addWidget(taskGB, 8, 0, 1, 2);

        dumpfnLE = new QLineEdit(centralWidget);
        dumpfnLE->setObjectName(QString::fromUtf8("dumpfnLE"));
        dumpfnLE->setEnabled(false);

        gridLayout->addWidget(dumpfnLE, 10, 1, 1, 1);

        dumpfnPB = new QPushButton(centralWidget);
        dumpfnPB->setObjectName(QString::fromUtf8("dumpfnPB"));
        dumpfnPB->setEnabled(false);

        gridLayout->addWidget(dumpfnPB, 9, 0, 2, 1);

        startStopPB = new QPushButton(centralWidget);
        startStopPB->setObjectName(QString::fromUtf8("startStopPB"));
        startStopPB->setCheckable(true);

        gridLayout->addWidget(startStopPB, 12, 0, 1, 1);

        mcuSelectCB = new QComboBox(centralWidget);
        mcuSelectCB->setObjectName(QString::fromUtf8("mcuSelectCB"));

        gridLayout->addWidget(mcuSelectCB, 2, 1, 1, 1);

        t = new QPlainTextEdit(centralWidget);
        t->setObjectName(QString::fromUtf8("t"));
        t->setFont(font1);
        t->setReadOnly(true);

        gridLayout->addWidget(t, 13, 0, 1, 2);

        flasherfnPB = new QPushButton(centralWidget);
        flasherfnPB->setObjectName(QString::fromUtf8("flasherfnPB"));
        flasherfnPB->setEnabled(false);

        gridLayout->addWidget(flasherfnPB, 11, 0, 1, 1);

        MdrFwUForm->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MdrFwUForm);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 850, 35));
        menu = new QMenu(menuBar);
        menu->setObjectName(QString::fromUtf8("menu"));
        MdrFwUForm->setMenuBar(menuBar);
        statusBar = new QStatusBar(MdrFwUForm);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MdrFwUForm->setStatusBar(statusBar);
        QWidget::setTabOrder(openFilePB, fwfnLE);
        QWidget::setTabOrder(fwfnLE, portBaudrateCB);
        QWidget::setTabOrder(portBaudrateCB, portNameCB);
        QWidget::setTabOrder(portNameCB, dumpCB);
        QWidget::setTabOrder(dumpCB, eraseCB);
        QWidget::setTabOrder(eraseCB, programCB);
        QWidget::setTabOrder(programCB, verifyCB);
        QWidget::setTabOrder(verifyCB, runCB);
        QWidget::setTabOrder(runCB, dumpfnPB);
        QWidget::setTabOrder(dumpfnPB, dumpfnLE);
        QWidget::setTabOrder(dumpfnLE, flasherfnPB);
        QWidget::setTabOrder(flasherfnPB, flasherfnLE);
        QWidget::setTabOrder(flasherfnLE, startStopPB);
        QWidget::setTabOrder(startStopPB, t);

        menuBar->addAction(menu->menuAction());
        menu->addAction(action_Qt);
        menu->addAction(about);

        retranslateUi(MdrFwUForm);

        portBaudrateCB->setCurrentIndex(7);


        QMetaObject::connectSlotsByName(MdrFwUForm);
    } // setupUi

    void retranslateUi(QMainWindow *MdrFwUForm)
    {
        action_Qt->setText(QApplication::translate("MdrFwUForm", "\320\236 Qt", nullptr));
        about->setText(QApplication::translate("MdrFwUForm", "\320\236 \320\277\321\200\320\276\320\263\321\200\320\260\320\274\320\274\320\265", nullptr));
        groupBox->setTitle(QApplication::translate("MdrFwUForm", "\320\222\321\213\320\261\320\276\321\200 \320\270 \320\267\320\260\320\264\320\260\320\275\320\270\320\265 \321\201\320\272\320\276\321\200\320\276\321\201\321\202\320\270 \320\272\320\276\320\274\320\274\321\203\320\275\320\270\320\272\320\260\321\206\320\270\320\276\320\275\320\275\320\276\320\263\320\276 \320\277\320\276\321\200\321\202\320\260", nullptr));
        portBaudrateCB->setItemText(0, QApplication::translate("MdrFwUForm", "1200", nullptr));
        portBaudrateCB->setItemText(1, QApplication::translate("MdrFwUForm", "2400", nullptr));
        portBaudrateCB->setItemText(2, QApplication::translate("MdrFwUForm", "4800", nullptr));
        portBaudrateCB->setItemText(3, QApplication::translate("MdrFwUForm", "9600", nullptr));
        portBaudrateCB->setItemText(4, QApplication::translate("MdrFwUForm", "19200", nullptr));
        portBaudrateCB->setItemText(5, QApplication::translate("MdrFwUForm", "38400", nullptr));
        portBaudrateCB->setItemText(6, QApplication::translate("MdrFwUForm", "57600", nullptr));
        portBaudrateCB->setItemText(7, QApplication::translate("MdrFwUForm", "115200", nullptr));
        portBaudrateCB->setItemText(8, QApplication::translate("MdrFwUForm", "230400", nullptr));
        portBaudrateCB->setItemText(9, QApplication::translate("MdrFwUForm", "460800", nullptr));
        portBaudrateCB->setItemText(10, QApplication::translate("MdrFwUForm", "921600", nullptr));

        openFilePB->setText(QApplication::translate("MdrFwUForm", "\320\244\320\260\320\271\320\273 \320\277\321\200\320\276\321\210\320\270\320\262\320\272\320\270", nullptr));
        fwfnLE->setPlaceholderText(QApplication::translate("MdrFwUForm", "\320\222\321\213\320\261\320\265\321\200\320\270\321\202\320\265 \321\204\320\260\320\271\320\273 \320\277\321\200\320\276\321\210\320\270\320\262\320\272\320\270", nullptr));
        flasherfnLE->setText(QString());
        flasherfnLE->setPlaceholderText(QApplication::translate("MdrFwUForm", "\320\236\321\201\321\202\320\260\320\262\321\214\321\202\320\265 \320\277\320\276\320\273\320\265 \320\277\321\203\321\201\321\202\321\213\320\274 \320\264\320\273\321\217 \320\270\321\201\320\277\320\276\320\273\321\214\320\267\320\276\320\262\320\260\320\275\320\270\321\217 \321\201\321\202\320\260\320\275\320\264\320\260\321\200\321\202\320\275\320\276\320\263\320\276  \321\204\320\273\320\265\321\210\320\265\321\200\320\260", nullptr));
        sL->setText(QApplication::translate("MdrFwUForm", "\320\245\320\276\320\264 \321\200\320\260\320\261\320\276\321\202\321\213", nullptr));
        label->setText(QApplication::translate("MdrFwUForm", "\320\234\320\270\320\272\321\200\320\276\320\272\320\276\320\275\321\202\321\200\320\276\320\273\320\273\320\265\321\200", nullptr));
        taskGB->setTitle(QApplication::translate("MdrFwUForm", "\320\227\320\260\320\264\320\260\321\207\320\270", nullptr));
        dumpCB->setText(QApplication::translate("MdrFwUForm", "\320\241\321\207\320\270\321\202\320\260\321\202\321\214", nullptr));
        eraseCB->setText(QApplication::translate("MdrFwUForm", "\320\241\321\202\320\265\321\200\320\265\321\202\321\214", nullptr));
        programCB->setText(QApplication::translate("MdrFwUForm", "\320\237\321\200\320\276\320\263\321\200\320\260\320\274\320\274\320\270\321\200\320\276\320\262\320\260\321\202\321\214", nullptr));
        verifyCB->setText(QApplication::translate("MdrFwUForm", "\320\241\321\200\320\260\320\262\320\275\320\270\321\202\321\214", nullptr));
        runCB->setText(QApplication::translate("MdrFwUForm", "\320\227\320\260\320\277\321\203\321\201\321\202\320\270\321\202\321\214", nullptr));
        dumpfnLE->setText(QString());
        dumpfnLE->setPlaceholderText(QApplication::translate("MdrFwUForm", "\320\222\320\262\320\265\320\264\320\270\321\202\320\265 \320\270\320\274\321\217 \321\204\320\260\320\271\320\273\320\260 \320\264\320\273\321\217 \321\201\320\276\321\205\321\200\320\260\320\275\320\265\320\275\320\270\321\217 \320\277\321\200\320\276\321\210\320\270\320\262\320\272\320\270", nullptr));
        dumpfnPB->setText(QApplication::translate("MdrFwUForm", "\320\244\320\260\320\271\320\273 \320\264\320\260\320\274\320\277\320\260", nullptr));
        startStopPB->setText(QApplication::translate("MdrFwUForm", "Start", nullptr));
        flasherfnPB->setText(QApplication::translate("MdrFwUForm", "\320\244\320\260\320\271\320\273 \321\204\320\273\320\265\321\210\320\265\321\200\320\260", nullptr));
        menu->setTitle(QApplication::translate("MdrFwUForm", "\320\241\320\277\321\200\320\260\320\262\320\272\320\260", nullptr));
        Q_UNUSED(MdrFwUForm);
    } // retranslateUi

};

namespace Ui {
    class MdrFwUForm: public Ui_MdrFwUForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MDRFWUFORM_H
