#include <QFile>
#include <iostream>
#include <filesystem>
#include <fstream>

using namespace std;
using namespace std::filesystem;

void createQR(string nameQR, string originText)
{
    string CMD1 = "qrencode -s 6 -l M -o ";
    string CMD2 = nameQR;
    string CMD3 = originText;
    string CMD = CMD1 +  CMD2 + " " + CMD3;
    system (CMD.c_str());
}

int main(int argc, char *argv[]){

    createQR(argv[1], argv[2]);

return 0;
}
