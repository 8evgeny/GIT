#pragma once
#include <QFile>
#include "rapidjson/document.h"
#include "rapidjson/error/en.h"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
using namespace rapidjson;
using namespace std;
using namespace chrono_literals;

void parseJSON(QString & jsonData);
