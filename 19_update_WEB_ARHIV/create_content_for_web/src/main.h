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
#include <boost/program_options.hpp>

#define BUFSIZE 300000

using namespace rapidjson;
using namespace std;
using namespace chrono_literals;
using namespace std::filesystem;
namespace po = boost::program_options;

bool parseJSON(string & patchToFile, const path & archiv_path_zip);
bool extractZip(string & patchToFile, string & fileName, string & patchToExtractDirectory);

quint32 CRC32(QString fileName);
string CRC32Contents(QString DirectoryPatch);
string createHTML(vector <string> & content, vector<pair<string,string>> & oldZipData);
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
map<string, string> readConfig(const char* conf_file);
void createMainWebContent(map<string, string> &config);
string nameFromPath(string path);
std::string replaceAll( std::string const& name, std::string const& before, std::string const& after );
