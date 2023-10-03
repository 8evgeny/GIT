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

bool parseJSON(string & patchToFile);
quint32 CRC32(QString fileName);
quint32 CRC32Contents(QString DirectoryPatch);
