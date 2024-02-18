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
void drop_table(QString table_name);
void workPSQL();
connection* connectToDB(string dbname, string user, string password, string hostaddr, string port);
void disconnectFromDB(connection* Connection);
void transactionToDB(connection* conn, string req);
result nontransactionToDB(connection* conn, string req);
