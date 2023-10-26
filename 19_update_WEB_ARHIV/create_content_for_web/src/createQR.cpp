#include "main.h"

extern string WEB_content;
extern uint numFolderForWebContent;

string createStringForQr (string oboznachenieIkodDokumenta,
                          string changeNumStr,
                          string changeNotificationNum,
                          string storageDataStr,
                          string litera,
                          string controlSummOrigin,
                          string controlSummParts
                          )
{
//    ГРЛМ.741138.003 | изм.0 | К.001-2023 | 2023-05-29 | Лит.нет | CRC-32.pdf=1F349207 | CRC-32.zip\Contents=3C3DE8F8

    string separator = "\" | \"";
    string LiteraQr{"Лит."};
    if (litera == "")
        LiteraQr.append("нет");
    else
        LiteraQr.append(litera);

    string result = oboznachenieIkodDokumenta + separator +
            "изм."+changeNumStr + separator +
            changeNotificationNum + separator +
            storageDataStr  + separator +
            LiteraQr + separator+
            "CRC-32.pdf="+controlSummOrigin + separator +
            "CRC-32.zip\\Contents=" + controlSummParts;
//    cout<<result<<endl;
    return result;
}

void createQR(string nameQR, string originText)
{
#ifdef WIN32
    string CMD1 = "qrcodecon -s 6 -l M -o ";
    string CMD2 = nameQR+".png";
#else
    string CMD1 = "qrencode -s 6 -l M -o ";
    string CMD2 = WEB_content + to_string(numFolderForWebContent) + "/" + nameQR+".png";
#endif
    string CMD3 = originText;
    string CMD = CMD1 +  CMD2 + " " + CMD3;
    system (CMD.c_str());
}
