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
using namespace rapidjson;
using namespace std;
using namespace chrono_literals;
using namespace std::filesystem;

bool parseJSON(string & patchToFile);
