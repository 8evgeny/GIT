#include "about.h"
#include "ui_about.h"
#include "include/tr.h"
#include "main/classes/program.h"

AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent, Qt::FramelessWindowHint)
    , ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    ui->l_version->setText(program::fullVersion());
    ui->l_about->setText(IPS_STR("Software \"IPS Configurator\" for digital system multifunctional\ndispatch communication and alerts GIT-Comm IPS."));
    ui->l_contacts->setText(IPS_STR("Address: prosp. Vernadskogo, 94, korp. 5, Moscow, 119571 <br />"
                                   "Phone: +7 (495) 223-0725 <br /> "
                                    "E-mail: <a style=\"color:#272e3a; text-decoration:none;\" href=\"mailto:git@git-holding.ru\">git@git-holding.ru</a> <br /> "
                                    "<a style=\"color:#272e3a; text-decoration:none;\" href=\"https://www.git-holding.ru/\">www.git-holding.ru</a>"));
    ui->l_contacts->setOpenExternalLinks(true);
}
AboutDialog::~AboutDialog()
{
    delete ui;
}
