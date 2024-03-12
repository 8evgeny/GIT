#include "main.h"
#include <QThread>

#ifdef enablePOKO
#include "Poco/Data/Session.h"
#include "Poco/Data/PostgreSQL/Connector.h"
#include <Poco/ActiveRecord/Context.h>
#include <Poco/Data/PostgreSQL/PostgreSQLException.h>
#include <vector>
#include <iostream>
using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;
struct Person
{
    std::string name;
    std::string address;
    int         age;
};
#endif

#ifdef enablePQXX
#include <pqxx/connection>
#include <pqxx/transaction>
connection* ConnectionToDB;
#endif

#if 0

docker run -it --rm -d -p 8080:80 --name web -v ~/SOFT/Github/GIT/16_WEB_ARHIV/CONTENT:/usr/share/nginx/html nginx
                                    или
docker run -it --rm -d -p 8080:80 --name web2 -v ~/SOFT/Github/GIT/16_WEB_ARHIV/CONTENT:/usr/local/apache2/htdocs/ httpd:2.4
docker build -t  createwebcontent ~/SOFT/Github/GIT/16_WEB_ARHIV
Один раз дать права на запись других
sudo chmod -R ugo+rwx ~/SOFT/Github/GIT/16_WEB_ARHIV/CONTENT/content_for_web

После отработки контейнера, чтобы пересобрать - удалить под root файлы из content_for_web  или
sudo chown -R evg:evg ~/SOFT/Github/GIT/16_WEB_ARHIV/CONTENT/content_for_web && chmod -R ugo+rwx ~/SOFT/Github/GIT/16_WEB_ARHIV/CONTENT/content_for_web

docker run --name=createwebcontent --rm -v ~/SOFT/Github/GIT/16_WEB_ARHIV/CONTENT:/home/evg/CONTENT -v ~/SOFT/Github/GIT/16_WEB_ARHIV/NIOKR_aktual_DOC:/home/evg/NIOKR_aktual_DOC -v ~/SOFT/Github/GIT/16_WEB_ARHIV/NIOKR_IUL:/home/evg/NIOKR_IUL -v ~/SOFT/Github/GIT/16_WEB_ARHIV/NIOKR_messages:/home/evg/NIOKR_messages -v ~/SOFT/Github/GIT/16_WEB_ARHIV/NIOKR_old_DOC:/home/evg/NIOKR_old_DOC -v /etc/timezone:/etc/timezone:ro -v /etc/localtime:/etc/localtime:ro createwebcontent
docker system prune
cd ~/SOFT/Github/GIT/16_WEB_ARHIV && docker save createwebcontent > createwebcontent.tar
cd ~/SOFT/Github/GIT/16_WEB_ARHIV && docker load -i createwebcontent.tar
#endif
string WEB_content{"/home/evg/SOFT/Github/GIT/16_WEB_ARHIV/CONTENT/content_for_web/"};
uint numZipFiles{0};
uint numErrorPrint{1};
bool printDebug=false;
uint errorParsingJson{0};
uint numFolderForWebContent{1};
string nameFileForDeleteIfJsonNoGood;

int main(int argc, char *argv[])
{
#ifdef printDebugInfo
    printDebug=true;
#endif

    cout <<  "start docker servises" << endl<< endl;
    string dockerStart = "docker-compose up -d 2>/dev/null";
    system(dockerStart.c_str());
    QThread::currentThread()->msleep(2000);
#ifdef enablePQXX
//    testPQXX();
#endif
#ifdef enablePOKO
    testPOCO_Psql();
#endif
//    path archiv_path_zip{"/home/evg/Desktop/NIOKR_aktual_DOC"};
//    path archiv_path_zip{"/home/evg/tempArchiv/NIOKR_aktual_DOC"}; //Дома
    path archiv_path_zip{"/home/evg/SOFT/Github/GIT/16_WEB_ARHIV/NIOKR_aktual_DOC"}; //Для виртуалки
//    path archiv_path_zip{"/home/evg/SOFT/Github/GIT/16_WEB_ARHIV/_TEST"};

    if (argc == 2){ archiv_path_zip= argv[1];}
    cout << "Путь к архиву zip: "<< archiv_path_zip << endl;
    string pathToExtractDirectory = "../EXTRACT"; //в папке build
    const path archiv_path_extracted{pathToExtractDirectory};
//Разархивирование исходного контента
//Очищаем директорию для извлечения
    string eraseDir = " rm -rf ";
    eraseDir.append(archiv_path_extracted);
    eraseDir.append("/");
    system(eraseDir.c_str());

//Очищаем директорию web контента
    string eraseWebDir = " rm -rf ";
    eraseWebDir.append(WEB_content);
    system(eraseWebDir.c_str());
    string createWebContentDir = "mkdir ";
    createWebContentDir.append(WEB_content);
    system(createWebContentDir.c_str());

    std::vector<string> vectorZipFilesPath;
    std::vector<string> vectorZipFilesName;
    cout << "Patch for zip search: "<<archiv_path_zip << endl;
    const unordered_set<string> zip_extensions{ ".zip" };
    auto iteratorZip = recursive_directory_iterator{ archiv_path_zip, directory_options::skip_permission_denied };
    for(const auto& entry : iteratorZip) {
      try {
        if(!entry.is_regular_file())
          continue;
        const auto& extension = entry.path().extension().string();
        const auto is_zip = zip_extensions.find(extension) != zip_extensions.end();
        if(!is_zip)
          continue;
        string patch = entry.path().string();
//        auto posBlank = patch.find(" "); //Борьба с пробелом (Пути файлов уже не содержат пробелов)
//        patch.insert(posBlank,"\\");
        string name = entry.path().filename();
        vectorZipFilesPath.push_back(patch);
        name.pop_back();//убираем .zip
        name.pop_back();
        name.pop_back();
        name.pop_back();
        vectorZipFilesName.push_back(name);
      } catch(const exception& e) {
        cerr << "Error reading " << entry.path().string() << ": " << e.what() << endl;
      }
    }

    if (vectorZipFilesPath.size()>0)
    {
        for (auto i = 0; i < vectorZipFilesPath.size();++i ){
            ++numZipFiles;
            cout <<"Extract "<< numZipFiles << ": "<< nameFromPath(vectorZipFilesPath[i]) << endl;
            extractZip(vectorZipFilesPath[i], vectorZipFilesName[i], pathToExtractDirectory); //распаковка одного zip файла
        }
    }
    cout<< "Всего разархивировано zip файлов: " << numZipFiles<< endl;

//Создаем таблицу для хранения web контента
    ConnectionToDB = connectToDB("niokrDB", "postgres", "postgres", "127.0.0.1", "5432");
    string sql;
//Удаляем таблицу WEB_CONTENT
    sql = "DROP TABLE IF EXISTS WEB_CONTENT;";
    transactionToDB(ConnectionToDB, sql);

//Создаем таблицу WEB_CONTENT
    sql = "CREATE TABLE IF NOT EXISTS WEB_CONTENT("
                 "folder                        TEXT,"
                 "oboznachenieIkodDokumenta     TEXT,"
                 "naimenovanieIzdeliya          TEXT,"
                 "naimenovanieDokumenta         TEXT,"
                 "changeNumStr                  TEXT,"
                 "notificationDataStr           TEXT,"
                 "controlSummOrigin             TEXT,"
                 "infoOrderList                 TEXT,"
                 "created                       TEXT"
          ");";
    try {
        transactionToDB(ConnectionToDB, sql);

    } catch (const std::exception &e) {
        cerr << e.what() << std::endl;
    }

    cout<<"createTableForWebContent"<<endl<<endl;


//    cout<< "Начинаем разбор разархивированных директорий." << endl << endl;
//Разбор разархивированной директории
    std::vector<string> vectorJsonFilesPath;
    std::vector<string> errorJsonPatch;
    const unordered_set<string> json_extensions{ ".json" };
    auto iteratorJson = recursive_directory_iterator{ archiv_path_extracted, directory_options::skip_permission_denied };
    for(const auto& entry : iteratorJson) {
      try {
        if(!entry.is_regular_file())
          continue;
        const auto& extension = entry.path().extension().string();
        const auto is_json = json_extensions.find(extension) != json_extensions.end();
        if(!is_json)
          continue;
        string patch = entry.path().string();
        vectorJsonFilesPath.push_back(patch);
      } catch(const exception& e) {
        cerr << "Error reading " << entry.path().string() << ": " << e.what() << endl;
      }
    }

    if (vectorJsonFilesPath.size()>0)
    {
        int num = 0;
        for (auto & patchJsonFile : vectorJsonFilesPath){
            ++num;
            if (printDebug)
                cout <<endl<<"Seaching json "<< num << ": "<< patchJsonFile << endl;
            if(!parseJSON(patchJsonFile, archiv_path_zip)) //парсинг одного файла JSON
            {
      //Удаляем папку с контентом nameFileForDeleteIfJsonNoGood
                string remove = "rm -rf ../EXTRACT/" + nameFileForDeleteIfJsonNoGood;
                system(remove.c_str());
//                for (int i = 0; i < 11; ++i) { //Удаляем конец строки /index.json
//                    patchJsonFile.pop_back();
//                }
                auto pos1 = patchJsonFile.find_last_of("/");
                patchJsonFile.replace(pos1, 11, "");
                auto pos2 = patchJsonFile.find_last_of("/");
                patchJsonFile.replace(0, pos2 + 1, "");
                cout << numErrorPrint <<". Ошибка json: " << patchJsonFile <<endl;
                ++numErrorPrint;
                ++errorParsingJson;
                errorJsonPatch.push_back(patchJsonFile);
            }
        }
    }
    cout<< endl<< "Всего разобрано json файлов: " << vectorJsonFilesPath.size()
         << endl << "Web архив документов: " << vectorJsonFilesPath.size() - errorParsingJson<< endl
       << endl << "Ошибок json: " << errorParsingJson<< endl;
        for (auto & patchJsonError : errorJsonPatch){
        cout <<  patchJsonError << endl;
        }
        cout << endl;

//Удаляем папку EXTRACT (если там что осталось)
    string remove = "rm -rf ../EXTRACT/";
    system(remove.c_str());

//Все папки с контентом сформированы - формирую файл numDoc (Теперь не нужен после файлов сортировок)
//    QFile frow((WEB_content + "/" + "numDoc").c_str());
//    frow.open(QIODevice::WriteOnly);
//    frow.write(to_string(numZipFiles - errorParsingJson).c_str());
//    frow.close();

//Формирую строку с датой: Сводный перечень документов на хранении по состоянию на ____
    QFile fdate((WEB_content + "/" + "date").c_str());
    fdate.open(QIODevice::WriteOnly);
    string date = "АРХИВ КОНСТРУКТОРСКОЙ ДОКУМЕНТАЦИИ НИОКР на ";
    date.append(currentDateTime());
    fdate.write(date.c_str());
    fdate.close();

//Формируем данные с разными критериями сортировки (эти 5 файлов будут подгружаться из JS)
    sort("oboznachenieikoddokumenta ASC");
    sort("naimenovanieizdeliya ASC, notificationdatastr DESC");
    sort("naimenovaniedokumenta ASC, notificationdatastr DESC");
    sort("changenumstr DESC, oboznachenieikoddokumenta ASC");
    sort("notificationdatastr DESC, oboznachenieikoddokumenta ASC");

    disconnectFromDB(ConnectionToDB);
}

void sort(string rule){
    string sql = "SELECT * from web_content ORDER BY " + rule;
    result data = nontransactionToDB(ConnectionToDB, sql);
    //Записываем отсортированные данные в файл
    string nameFileSort;
    if(rule == "oboznachenieikoddokumenta ASC")
        nameFileSort = "ruleSort_1";
    if(rule == "naimenovanieizdeliya ASC, notificationdatastr DESC")
        nameFileSort = "ruleSort_2";
    if(rule == "naimenovaniedokumenta ASC, notificationdatastr DESC")
        nameFileSort = "ruleSort_3";
    if(rule == "changenumstr DESC, oboznachenieikoddokumenta ASC")
        nameFileSort = "ruleSort_4";
    if(rule == "notificationdatastr DESC, oboznachenieikoddokumenta ASC")
        nameFileSort = "ruleSort_5";


    QFile fout((WEB_content + nameFileSort).c_str());
    fout.open(QIODevice::WriteOnly);
    QByteArray ba;
    for (result::const_iterator c = data.begin(); c != data.end(); ++c) {
//        ba.append(QString::fromStdString(c[0].as<string>())); ba.append("~~"); //folder
//        ba.append(QString::fromStdString(c[1].as<string>())); ba.append("~~"); //oboznachenieIkodDokumenta
//        ba.append(QString::fromStdString(c[2].as<string>())); ba.append("~~"); //naimenovanieIzdeliya
//        ba.append(QString::fromStdString(c[3].as<string>())); ba.append("~~"); //naimenovanieDokumenta
//        ba.append(QString::fromStdString(c[4].as<string>())); ba.append("~~"); //changeNumStr
//        ba.append(QString::fromStdString(c[5].as<string>())); ba.append("~~"); //notificationDataStr
//        ba.append(QString::fromStdString(c[6].as<string>())); ba.append("~~"); //controlSummOrigin
//        ba.append(QString::fromStdString(c[7].as<string>())); ba.append("~~"); //infoOrderList
//        ba.append(QString::fromStdString(c[8].as<string>())); ba.append('\n'); //creator

//Пока пишем только номера папок и далее JS  подгрузит контент каждого ряда
        ba.append(QString::fromStdString(c[0].as<string>())); ba.append('\n'); //folder
    }
    fout.write(ba);
    fout.close();
}

string nameFromPath(path Patch){
    return Patch.filename();
}
#ifdef enablePQXX
void testPQXX(){
    cout << "testPQXX " << endl;
    ConnectionToDB = connectToDB("niokrDB", "postgres", "postgres", "127.0.0.1", "5432");
    string sql;
    try {
//Удаляем таблицу
          sql = "DROP TABLE IF EXISTS COMPANY;";
          transactionToDB(ConnectionToDB, sql);
//Создаем таблицу
          sql = "CREATE TABLE IF NOT EXISTS COMPANY("
          "ID INT PRIMARY KEY     NOT NULL,"
          "NAME           TEXT    NOT NULL,"
          "AGE            INT     NOT NULL,"
          "ADDRESS        CHAR(50),"
          "SALARY         REAL );";
          transactionToDB(ConnectionToDB, sql);
//Заполняем таблицу
          sql = "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "
             "VALUES (1, 'Pratds', 32, 'California', 40000.00 ); "
             "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "
             "VALUES (2, 'Alks', 25, 'Texas', 25000.00 ); "
             "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)"
             "VALUES (3, 'Tedh', 23, 'Norway', 19000.00 );"
             "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)"
             "VALUES (4, 'Rahj', 25, 'Rich-Mond ', 95000.00 );";
           transactionToDB(ConnectionToDB, sql);
//Удаляем запись
          sql = "DELETE from COMPANY where ID = 2";
          transactionToDB(ConnectionToDB, sql);
          cout << "Records deleted successfully" << endl;
//Изменяем запись
          sql = "UPDATE COMPANY set SALARY = 250000.00 where ID=1";
          transactionToDB(ConnectionToDB, sql);
          cout << "Records updated successfully" << endl;
//Извлекаем данные
           sql = "SELECT * from COMPANY";
           result data = nontransactionToDB(ConnectionToDB, sql);
           /* List down all the records */
           for (result::const_iterator c = data.begin(); c != data.end(); ++c) {
              cout << "ID = " << c[0].as<int>() << endl;
              cout << "Name = " << c[1].as<string>() << endl;
              cout << "Age = " << c[2].as<int>() << endl;
              cout << "Address = " << c[3].as<string>() << endl;
              cout << "Salary = " << c[4].as<float>() << endl;
           }
          cout << "Database operation successfully" << endl;

       } catch (const std::exception &e) {
          cerr << e.what() << std::endl;
       }

disconnectFromDB(ConnectionToDB);

}
connection* connectToDB(string dbname, string user, string password, string hostaddr, string port){
    const string connect =
                " dbname = " + dbname +
                " user = " + user +
                " password = " + password +
                " hostaddr = " + hostaddr +
                " port = " + port;
    connection *Connection = new connection(connect);
    try {
          if (Connection->is_open()) {
             cout << "Opened database successfully: " << Connection->dbname() << endl;
          } else {
             cout << "Can't open database " << dbname << endl;
          }
    } catch (const std::exception &e) {
       cerr << e.what() << std::endl;
    }
    return Connection;
}
void disconnectFromDB(connection* Connection){
    string name = Connection->dbname();
    Connection->disconnect ();
    delete Connection;
    cout << "disconnect from database " << name << endl<< endl;
}
void transactionToDB(connection* conn, string req){
    work W(*conn);
    W.exec(req);
    W.commit();
}
result nontransactionToDB(connection* conn, string req){
    nontransaction N(*conn); /* Create a non-transactional object. */
    result R( N.exec( req ));/* Execute SQL query */
    return R;
}

#endif

#ifdef enablePOKO
void testPOCO_Psql(){
    cout << "testPOCO_Psql " << endl;
    // register PostgreSQL connector
    Poco::Data::PostgreSQL::Connector::registerConnector();
    // create a session
    Poco::Data::Session session(Poco::Data::PostgreSQL::Connector::KEY, "host=127.0.0.1 user=postgres password=postgres dbname=niokrDB port=5432");
    try {
        session << "DROP TABLE IF EXISTS Person", now;
        // (re)create table
        session << "CREATE TABLE Person (Name VARCHAR(30), Address VARCHAR, Age INTEGER)", now;

        // insert some rows
        Person person =
        {
            "Bart Simpson",
            "Springfield",
            12
        };

        Statement insert(session);
        insert << "INSERT INTO Person VALUES($1, $2, $3)",
            use(person.name),
            use(person.address),
            use(person.age);
        insert.execute();

        person.name    = "Lisa Simpson";
        person.address = "Springfield";
        person.age     = 10;

        insert.execute();

    //     a simple query
        Statement select(session);
        select << "SELECT Name, Address, Age FROM Person",
            into(person.name),
            into(person.address),
            into(person.age),
            range(0, 1); //  iterate over result set one row at a time

        while (!select.done())
        {
            select.execute();
            std::cout << person.name << " " << person.address << " " << person.age << std::endl;
        }
        std::cout << std::endl;
    } catch (Poco::Data::PostgreSQL::StatementException &e) {
                std::cout << e.displayText()  << std::endl;
        }
}
#endif
