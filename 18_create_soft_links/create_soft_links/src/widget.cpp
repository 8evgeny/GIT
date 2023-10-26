#include "widget.h"
#include "ui_widget.h"
#include "main.h"

Widget::Widget(QWidget *parent) : QWidget(parent) , ui(new Ui::Widget) {
  ui->setupUi(this);

    readConfig("../config.ini");
    parseInFolder();
    addPdfItemsToList();


}

Widget::~Widget() {
 delete ui;
}

void Widget::readConfig(const char* conf_file) {
    cout<<"reading config.ini file\n"<<endl;
    po::variables_map vm;
    po::options_description patches("patchesToDirectories");
    patches.add_options()
          ("patchesToDirectories.niokrPoOboznacheniyam", po::value<string>(), "")
          ("patchesToDirectories.niokrFoldersToSoftLinks", po::value<string>(), "");
  po::options_description desc("Allowed options");
  desc.add(patches);
  try {
    po::parsed_options parsed = po::parse_config_file<char>(conf_file, desc, true);  //флаг true разрешает незарегистрированные опции !
    po::store(parsed, vm);
  } catch (const po::reading_file& e) {
    std::cout << "Error: " << e.what() << std::endl;
  }
  po::notify(vm);
  config["niokrPoOboznacheniyam"] = vm["patchesToDirectories.niokrPoOboznacheniyam"].as<string>();
  config["niokrFoldersToSoftLinks"] = vm["patchesToDirectories.niokrFoldersToSoftLinks"].as<string>();
  cout << "niokrPoOboznacheniyam: \t" << config["niokrPoOboznacheniyam"] << endl;
  cout << "niokrFoldersToSoftLinks: \t" << config["niokrFoldersToSoftLinks"] << endl<<endl;
}

void Widget::parseInFolder()
{
    const path path{config["niokrPoOboznacheniyam"]};
    const unordered_set<string> pe_extensions{ ".PDF", ".pdf" };
    cout << endl <<"Patch for PDF search: "<< path << endl;

    auto iterator = recursive_directory_iterator{ path, directory_options::skip_permission_denied };
    for(const auto& entry : iterator) {
      try {
        if(!entry.is_regular_file())
          continue;
        const auto& extension = entry.path().extension().string();
        const auto is_pe = pe_extensions.find(extension) != pe_extensions.end();
        if(!is_pe)
          continue;
        ifstream file{ entry.path() };
        string patch = entry.path().string();
        vectorPDF.push_back(patch);
      } catch(const exception& e) {
        cerr << "Error reading " << entry.path().string() << ": " << e.what() << endl;
      }
    }
    cout<<"PDF документов в папке: " << vectorPDF.size()<<endl;

}

void Widget::addPdfItemsToList()
{
    uint numDoc = 0;
    for (auto & i:vectorPDF)
    {
        ++numDoc;
        QListWidgetItem *item = new QListWidgetItem;
        auto pos = i.find_last_of('/');
        string name = to_string(numDoc).append(".  ").append(i.substr(pos + 1, i.size() - pos - 5));
        item->setText(tr(name.c_str()));
        item->setToolTip(i.c_str());
//        item->setStatusTip("Сообщение в строку статуса");
//        item->setWhatsThis("Подсказка \"Что это?\"");
        ui->listWidget->addItem(item);
    }

}

void Widget::on_pushButton_clicked() {
 new QListWidgetItem(tr("Элемент 1"),ui->listWidget);
 QString item2Title ("Элемент 2");
 QListWidgetItem *item2 = new QListWidgetItem;
 item2->setText(item2Title);
 item2->setIcon(QIcon(":/images/new.png"));
 item2->setToolTip("Всплывающая подсказка");
 item2->setStatusTip("Сообщение в строку статуса");
 item2->setWhatsThis("Подсказка \"Что это?\"");
 ui->listWidget->addItem(item2);
}

void Widget::on_pushButton_2_clicked() {
 static Qt::SortOrder order = Qt::AscendingOrder;
 ui->listWidget->sortItems(order);
 if (order == Qt::AscendingOrder) order = Qt::DescendingOrder;
 else order = Qt::AscendingOrder;
}

void Widget::on_pushButton_3_clicked() {
 QListWidgetItem *item = ui->listWidget->currentItem(); //получить текущий
 if (!item) {
  QMessageBox::information(this,tr("Не могу получить currentItem"),"NULL");
 }
 else {
  QMessageBox::information(this,tr("Получение currentItem"),item->text());
 }
 QListWidgetItem *item2 = ui->listWidget->item(ui->listWidget->currentRow()); //получить по индексу
 if (!item2) {
  QMessageBox::information(this,tr("Не могу получить элемент по индексу"),"NULL");
 }
 else {
  QMessageBox::information(this,tr("Получение по индексу"),item->text());
 }
}

void Widget::on_pushButton_4_clicked() {
 QListWidgetItem *item = ui->listWidget->currentItem();
 if (!item) {
  QMessageBox::information(this,tr("Ошибка"),tr("Не выбран элемент в списке"));
  return;
 }
 item->setFlags (item->flags () | Qt::ItemIsEditable);
}

void Widget::on_listWidget_itemChanged(QListWidgetItem *item) {
 if (!item) return;
 QString text = item->text();
 ui->listWidget->blockSignals(true);
 if (text.contains(QRegExp("^[\\w\\s]+$"))) item->setText(text);
  //Только алфафитно-цифровые или разделители
 else item->setText(this->previousText);
 ui->listWidget->blockSignals(false);
}

void Widget::on_listWidget_itemDoubleClicked(QListWidgetItem *item) {
 if (item) this->previousText = item->text();
}

void Widget::on_pushButton_5_clicked() {
 QListWidgetItem *item = ui->listWidget->item(ui->listWidget->currentRow());
 if (!item) {
  QMessageBox::information(this,tr("Ошибка"),tr("Не выбран элемент в списке"));
  return;
 }
 ui->listWidget->takeItem (ui->listWidget->currentRow());
}

void Widget::on_pushButton_6_clicked() {
 ui->listWidget->clear();
}

void Widget::on_pushButton_7_clicked() {
 static QAbstractItemView::SelectionMode mode = QAbstractItemView::SingleSelection;
 if (mode == QAbstractItemView::SingleSelection) mode = QAbstractItemView::MultiSelection;
 else mode = QAbstractItemView::SingleSelection;
 ui->listWidget->setSelectionMode(mode);
}

void Widget::on_pushButton_8_clicked() {
 QList <QListWidgetItem *> selected = ui->listWidget->selectedItems();
 if (selected.size()==0)  {
  QMessageBox::information(this,tr("Ошибка"),tr("Нет выбранных элементов"));
  return;
 }
 QString str("");
 foreach(QListWidgetItem *item, selected) {
  str += item->text() + "\n";
 }
 QMessageBox::information(this,tr("Выбранные элементы"),str);
}

void Widget::on_pushButton_9_clicked() {
 QFile file("data.txt");
 if (!file.open(QFile::WriteOnly | QIODevice::Text)) {
  QMessageBox::critical(this,tr("Ошибка"),tr("Не могу записать в data.txt"));
  return;
 }
 for (int row = 0; row < ui->listWidget->count(); row++) {
  QListWidgetItem *item = ui->listWidget->item(row);
  QTextStream out(&file);
  out << item->text() << "\n";
 }
 file.close();
}


void Widget::on_pushButton_10_clicked() {
 QFile file("data.txt");
 if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
  QMessageBox::critical(this,tr("Ошибка"),tr("Не могу прочитать data.txt"));
  return;
 }
 QTextStream in(&file);
 while (!in.atEnd()) {
  QString line = in.readLine();
  QListWidgetItem *item = new QListWidgetItem;
  item->setText(line);
  ui->listWidget->addItem(item);
 }
 file.close();
}
