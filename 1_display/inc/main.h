#pragma once
#include <string>
#include <vector>
#include <memory>
#include "display.h"

std::pair<std::string,std::string> datetime();
void test(int scr);
void diagnosticFrame(std::shared_ptr<Display> lcd);
