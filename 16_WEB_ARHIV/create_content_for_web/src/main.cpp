﻿#include "main.h"
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
QString host;
QString host_remote;
QString database;
QString dumpDB;
QString user;
QString password;
int main(int argc, char *argv[])
{
    host="localhost";
    database="niokrDB";
    user="postgres";
    password="postgres";
    cout <<  "start docker servises" << endl;
    string dockerStart = "docker-compose up -d 2>/dev/null";
    system(dockerStart.c_str());
    QThread::currentThread()->msleep(2000);
//    create_table("test");


    char * sql;

    try {
          connection C("dbname = postgres user = postgres password = postgres hostaddr = 127.0.0.1 port = 5432");
          if (C.is_open()) {
             cout << "Opened database successfully: " << C.dbname() << endl;
          } else {
             cout << "Can't open database" << endl;
             return 1;
          }



          /* Create SQL statement */
          sql = "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "
             "VALUES (1, 'Pratds', 32, 'California', 40000.00 ); "
             "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "
             "VALUES (2, 'Alks', 25, 'Texas', 25000.00 ); "
             "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)"
             "VALUES (3, 'Tedh', 23, 'Norway', 19000.00 );"
             "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)"
             "VALUES (4, 'Rahj', 25, 'Rich-Mond ', 95000.00 );";

          work W(C);/* Create a transactional object. */
          W.exec( sql ); /* Execute SQL query */
          W.commit();

          cout << "Records created successfully" << endl;
          C.disconnect ();

       } catch (const std::exception &e) {
          cerr << e.what() << std::endl;
          return 1;
       }






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

void create_table(QString table_name) {

  CONNECT_TO_DB

  string create = "CREATE TABLE IF NOT EXISTS " + table_name.toStdString() +
                " (did    integer PRIMARY KEY GENERATED BY DEFAULT AS IDENTITY,name varchar(40) NOT NULL CHECK (name <> ''));"
                ;
  auto res = work.exec(create);
  cout << "create_table: " << table_name.toStdString() << endl;
}

void drop_table(QString table_name) {
  CONNECT_TO_DB
  string drop = "DROP TABLE IF EXISTS " + table_name.toStdString() +
                " CASCADE"
                ";";
  auto res = work.exec(drop);
  cout << "drop_table: " << table_name.toStdString() << endl;
}

//void receeveTrajectories() {
//  //функция получает номера траекторий в вектор
//  trajectories.clear();
//  CONNECT_TO_DB
//  string request = "SELECT * FROM " + table_trajectory.toStdString() + " ; ";
//  auto res = work.exec(request);
//  for (const auto& track : res) {
//    trajectories.push_back(track[0].as<int>());  // номер траектории

//    //    cout << track[0].as<int>() << endl;
//    //        if (track[0].as<int>() == 11 | track[0].as<int>() == 12 ) {
//    //          trajectories.push_back(track[0].as<int>());  //  отладка
//    //          cout << track[0].as<int>() << endl;
//    //        }
//  }
//  zab.number_tracks=trajectories.size();
//}


//string create6 = "CREATE TABLE " + table_violation_events.toStdString() +
//                 " (trj_violation_id SERIAL NOT NULL PRIMARY KEY, "
//                 " trj_id integer, "
//                 " trj_violation_num_internal integer, "
//                 " violation_begin bigint, "
//                 " violation_end bigint, "
//                 " green_on bigint, "
//                 " start_violation_survey1_x  double precision, "
//                 " start_violation_survey1_y  double precision, "
//                 " end_violation_survey1_x  double precision, "
//                 " end_violation_survey1_y  double precision, "
//                 " start_violation_survey2_x  double precision, "
//                 " start_violation_survey2_y  double precision, "
//                 " end_violation_survey2_x  double precision, "
//                 " end_violation_survey2_y  double precision, "
//                 " start_violation_recog_x  double precision, "
//                 " start_violation_recog_y  double precision, "
//                 " end_violation_recog_x  double precision, "
//                 " end_violation_recog_y  double precision, "
//                 " w  double precision, "
//                 " h  double precision); ";
//auto res12 = work.exec(create6);

//}

