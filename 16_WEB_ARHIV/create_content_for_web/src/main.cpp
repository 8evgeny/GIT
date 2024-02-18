#include "main.h"
#include <pqxx/connection>
#include <pqxx/transaction>
#include <QThread>
#if 0

docker run -it --rm -d -p 8080:80 --name web -v ~/SOFT/Github/GIT/16_WEB_ARHIV/CONTENT:/usr/share/nginx/html nginx
                                    или
docker run -it --rm -d -p 8080:80 --name web2 -v ~/SOFT/Github/GIT/16_WEB_ARHIV/CONTENT:/usr/local/apache2/htdocs/ httpd:2.4
docker build -t  createwebcontent ~/SOFT/Github/GIT/16_WEB_ARHIV
Один раз дать права на запись других
sudo chmod -R ugo+rwx ~/SOFT/Github/GIT/16_WEB_ARHIV/CONTENT/content_for_web

После отработки контейнера, чтобы пересобрать - удалить под root файлы из content_for_web  или
sudo chown -R evg:evg ~/SOFT/Github/GIT/16_WEB_ARHIV/CONTENT/content_for_web && chmod -R ugo+rwx ~/SOFT/Github/GIT/16_WEB_ARHIV/CONTENT/content_for_web

docker run --name=createwebcontent --rm -v ~/SOFT/Github/GIT/16_WEB_ARHIV/CONTENT:/home/evg/CONTENT -v ~/SOFT/Github/GIT/16_WEB_ARHIV/Ниокр-Актуальные_документы:/home/evg/Ниокр-Актуальные_документы -v ~/SOFT/Github/GIT/16_WEB_ARHIV/Ниокр-Документы_по_обозначениям:/home/evg/Ниокр-Документы_по_обозначениям -v ~/SOFT/Github/GIT/16_WEB_ARHIV/Ниокр-Извещения_об_изменении:/home/evg/Ниокр-Извещения_об_изменении -v ~/SOFT/Github/GIT/16_WEB_ARHIV/Ниокр-Неактуальные_документы:/home/evg/Ниокр-Неактуальные_документы -v /etc/timezone:/etc/timezone:ro -v /etc/localtime:/etc/localtime:ro createwebcontent
docker system prune
cd ~/SOFT/Github/GIT/16_WEB_ARHIV && docker save createwebcontent > createwebcontent.tar
cd ~/SOFT/Github/GIT/16_WEB_ARHIV && docker load -i createwebcontent.tar
#endif
string WEB_content{"/home/evg/SOFT/Github/GIT/16_WEB_ARHIV/CONTENT/content_for_web/"};
uint numContent{0};

int main(int argc, char *argv[])
{
    cout <<  "start docker servises" << endl;
    string dockerStart = "docker-compose up -d 2>/dev/null";
    system(dockerStart.c_str());
    QThread::currentThread()->msleep(2000);

    workPSQL();

//    path archiv_path_zip{"/home/evg/SOFT/Github/GIT/16_WEB_ARHIV/Ниокр-Актуальные_документы"};
//    path archiv_path_zip{"/home/evg/SOFT/Github/GIT/16_WEB_ARHIV/_ERRORS"};
//    path archiv_path_zip{"/home/evg/SOFT/Github/GIT/16_WEB_ARHIV/_BAD"};
    path archiv_path_zip{"/home/evg/SOFT/Github/GIT/16_WEB_ARHIV/_TEST"};
    if (argc != 2){
//        cout << "Передается в качестве параметра путь: "<< archiv_path_zip << endl;
    }
    else{
        archiv_path_zip= argv[1];
    }

    string pathToExtractDirectory = "EXTRACT"; //в папке build
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

    vector<string> vectorZipFilesPath;
    vector<string> vectorZipFilesName;
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
            ++numContent;
            cout <<"Extract "<< numContent << ": "<< nameFromPath(vectorZipFilesPath[i]) << endl;
            extractZip(vectorZipFilesPath[i], vectorZipFilesName[i], pathToExtractDirectory); //распаковка одного zip файла
        }
    }
    cout<< "Всего разархивировано zip файлов: " << numContent<< endl;
    cout<< "Начинаем разбор разархивированных директорий." << endl;
//Разбор разархивированной директории
    vector<string> vectorJsonFilesPath;
    vector<string> errorJsonPatch;
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
    int errorParsingJson = 0;
    if (vectorJsonFilesPath.size()>0)
    {
        int num = 0;
        for (auto & patchJsonFile : vectorJsonFilesPath){
            ++num;
            cout <<endl<<"Seaching json "<< num << ": "<< patchJsonFile << endl;
            if(!parseJSON(patchJsonFile, archiv_path_zip)) //парсинг одного файла JSON
            {
                cout << "ОШИБКА JSON ФАЙЛА" << endl;
                ++errorParsingJson;
                errorJsonPatch.push_back(patchJsonFile);
            }
        }
    }
    cout<< endl<< "Всего разобрано json файлов: " << vectorJsonFilesPath.size()
       << endl << "Ошибок разбора: " << errorParsingJson<< endl;
    for (auto & patchJsonError : errorJsonPatch)
        cout <<  patchJsonError << endl;

}

string nameFromPath(path Patch){
    return Patch.filename();
}

void workPSQL(){

    string sql;
    try {
//Соединяемся
        connection* C = connectToDB("niokrDB", "postgres", "postgres", "127.0.0.1", "5432");
//Удаляем таблицу
          sql = "DROP TABLE IF EXISTS COMPANY;";
          work drop(*C);/* Create a transactional object. */
          drop.exec( sql ); /* Execute SQL query */
          drop.commit();

//Создаем таблицу
          sql = "CREATE TABLE IF NOT EXISTS COMPANY("
          "ID INT PRIMARY KEY     NOT NULL,"
          "NAME           TEXT    NOT NULL,"
          "AGE            INT     NOT NULL,"
          "ADDRESS        CHAR(50),"
          "SALARY         REAL );";
          work create(*C);/* Create a transactional object. */
          create.exec( sql ); /* Execute SQL query */
          create.commit();

//Заполняем таблицу
          sql = "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "
             "VALUES (1, 'Pratds', 32, 'California', 40000.00 ); "
             "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "
             "VALUES (2, 'Alks', 25, 'Texas', 25000.00 ); "
             "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)"
             "VALUES (3, 'Tedh', 23, 'Norway', 19000.00 );"
             "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)"
             "VALUES (4, 'Rahj', 25, 'Rich-Mond ', 95000.00 );";
          work fill(*C);/* Create a transactional object. */
          fill.exec( sql ); /* Execute SQL query */
          fill.commit();

//Удаляем запись
          work del(*C);
          sql = "DELETE from COMPANY where ID = 2";
          del.exec( sql );
          del.commit();
          cout << "Records deleted successfully" << endl;

//Изменяем запись
          work update(*C);
          sql = "UPDATE COMPANY set SALARY = 250000.00 where ID=1";
          /* Execute SQL query */
          update.exec( sql );
          update.commit();
          cout << "Records updated successfully" << endl;

//Извлекаем данные
           sql = "SELECT * from COMPANY";
           nontransaction N(*C); /* Create a non-transactional object. */
           result R( N.exec( sql ));/* Execute SQL query */
           /* List down all the records */
           for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
              cout << "ID = " << c[0].as<int>() << endl;
              cout << "Name = " << c[1].as<string>() << endl;
              cout << "Age = " << c[2].as<int>() << endl;
              cout << "Address = " << c[3].as<string>() << endl;
              cout << "Salary = " << c[4].as<float>() << endl;
           }
          cout << "Database operation successfully" << endl<< endl;

//Отсоединяемся
          disconnectFromDB(C);

       } catch (const std::exception &e) {
          cerr << e.what() << std::endl;
       }

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
    Connection->disconnect ();
    delete Connection;
}
