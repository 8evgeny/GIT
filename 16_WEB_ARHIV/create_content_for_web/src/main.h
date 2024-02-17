#pragma once
#include <QFile>
#include "rapidjson/document.h"
#include "rapidjson/error/en.h"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <filesystem>
#include <fstream>
#include <unordered_set>
#include <pqxx/pqxx>

#define BUFSIZE 300000

using namespace rapidjson;
using namespace std;
using namespace chrono_literals;
using namespace std::filesystem;
using namespace pqxx;
#define CONNECT_TO_DB                                              \
  string connections_to_db = "host=" + host.toStdString() +        \
                             " dbname=" + database.toStdString() + \
                             " user=" + user.toStdString();        \
  pqxx::connection connect(connections_to_db);                     \
  pqxx::nontransaction work(connect);

bool parseJSON(string & patchToFile, const path & archiv_path_zip);
bool extractZip(string & patchToFile, string & fileName, string & patchToExtractDirectory);

quint32 CRC32(QString fileName);
string CRC32Contents(QString DirectoryPatch);
string createHTML(vector <string> & content, vector<pair<string,string>> & oldZipData);
void createQR(string nameQR, string originText);
string currentDateTime();
string nameFromPath(path);
string createStringForQr (string oboznachenieIkodDokumenta,
                          string changeNumStr,
                          string changeNotificationNum,
                          string storageDataStr,
                          string litera,
                          string controlSummOrigin,
                          string controlSummParts
                          );
void create_table(QString table_name);
