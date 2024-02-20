#include "main.h"

bool extractZip(string & patchToFile, string & fileName, string & patchToExtractDirectory)
{
// 7z x /home/evg/SOFT/Github/GIT/16_WEB_ARHIV/NIOKR_aktual_DOC/ГРЛМ.713121.001.zip -y -o"/home/evg/SOFT/Github/GIT/16_WEB_ARHIV/EXTRACT/ГРЛМ.713121.001"
    string createExtractFolder = "mkdir -p " + patchToExtractDirectory + "/" + "\"" + fileName + "\"";
    system(createExtractFolder.c_str());
    //-bsp0 -bso0  не выводим сообщения 7z
    // -mx=3 -mmt=4  задать режим работы "Fast" (x=3) работать в 4 потока; -mx=3 -mmt=4 (не ускоряет)
    string extract = "7z x \"" + patchToFile + "\" -y -bsp0 -bso0 -o\"" + patchToExtractDirectory + "/" + fileName+ "\"";
    system(extract.c_str());
    return true;
}

