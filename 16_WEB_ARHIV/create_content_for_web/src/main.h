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


#define BUFSIZE 300000

using namespace rapidjson;
using namespace std;
using namespace chrono_literals;
using namespace std::filesystem;

bool parseJSON(string & patchToFile, const path & archiv_path_zip);
bool extractZip(string & patchToFile, string & fileName, string & patchToExtractDirectory);

quint32 CRC32(QString fileName);
string CRC32Contents(QString DirectoryPatch);
string createHTML(vector <string> & content );
void createQR(string nameQR, string originText);
string currentDateTime();
string createStringForQr (string oboznachenieIkodDokumenta,
                          string changeNumStr,
                          string changeNotificationNum,
                          string storageDataStr,
                          string litera,
                          string controlSummOrigin,
                          string controlSummParts
                          );
