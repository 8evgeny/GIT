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
        ui->listWidget->addItem(item);
    }

}

void Widget::on_listWidget_itemDoubleClicked(QListWidgetItem *item) {
    string textItem = item->text().toStdString().substr();
    auto pos = textItem.find_first_of('.');
    qDebug()<<"add Item   "<<textItem.substr(pos + 3).c_str();
    QListWidgetItem *itemList2 = new QListWidgetItem;
    string nameDoc = textItem.substr(pos + 3);
    itemList2->setText(nameDoc.c_str());
    ui->listWidget_2->addItem(itemList2);

    string tmp = config["niokrPoOboznacheniyam"];
    tmp.append("/").append(nameDoc).append(".PDF");
    string pathOrigin{};
    //Проверяем есть ли такой файл
    QFile file(tmp.c_str());
    if (file.exists())
    {
        pathOrigin.append(config["niokrPoOboznacheniyam"]).append("/").append(nameDoc).append(".PDF ");
    }
    else //Меняем на pdf
    {
        pathOrigin.append(config["niokrPoOboznacheniyam"]).append("/").append(nameDoc).append(".pdf ");
    }
    createSimLink(pathOrigin, nameDoc);
}

void Widget::createSimLink(string pathOrigin, string nameSimLink)
{
    string cmd = "cd ";
    cmd.append(config["niokrFoldersToSoftLinks"]).append(" && ln -s -f ").append(pathOrigin).append(nameSimLink);
    system(cmd.c_str());
}

void Widget::on_listWidget_2_itemDoubleClicked(QListWidgetItem *item) {
    string textItem = item->text().toStdString().substr();
    ui->listWidget_2->takeItem (ui->listWidget_2->currentRow());
    qDebug()<<"delete Item"<<textItem.c_str();
}

void Widget::on_pushButton_Save_clicked() {
    QString path{config["niokrFoldersToSoftLinks"].c_str()};
    path.append("/.documents");
    QFile file(path);
    if (!file.open(QFile::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this,tr("Ошибка"),tr("Не могу записать в data.txt"));
        return;
    }
    for (int row = 0; row < ui->listWidget_2->count(); row++) {
        QListWidgetItem *item = ui->listWidget_2->item(row);
        QTextStream out(&file);
        out << item->text() << "\n";
    }
    file.close();
    close();
}

void Widget::on_pushButton_Load_clicked() {
    QString path{config["niokrFoldersToSoftLinks"].c_str()};
    path.append("/.documents");
    ui->listWidget_2->clear();//Сперва очищаем
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this,tr("Ошибка"),tr("Не могу прочитать data.txt"));
        return;
    }
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(line);
        ui->listWidget_2->addItem(item);
    }
    file.close();
}


