#include "main.h"

string currentDateTime() {
std::time_t t = std::time(nullptr);
std::tm* now = std::localtime(&t);
char buffer[128];
strftime(buffer, sizeof(buffer), "%d-%m-%Y %X", now);
return buffer;
}
