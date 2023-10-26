#include "main.h"

bool extractZip(string & patchToFile, string & fileName, string & patchToExtractDirectory)
{
// 7z x /home/evg/SOFT/Github/GIT/16_WEB_ARHIV/Ниокр-Актуальные_документы/ГРЛМ.713121.001.zip -y -o"/home/evg/SOFT/Github/GIT/16_WEB_ARHIV/EXTRACT/ГРЛМ.713121.001"
    string createExtractFolder = "mkdir " + patchToExtractDirectory + "/" + fileName;
    system(createExtractFolder.c_str());
    string extract = "7z x " + patchToFile + " -y -o\"" + patchToExtractDirectory + "/" + fileName+ "\"";
    system(extract.c_str());
    return true;
}

