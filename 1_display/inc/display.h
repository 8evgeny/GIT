#include <string>
#include <chrono>

class Display
{
public:
    Display();
    ~Display();

    void printToLcd(std::string str, std::string port);
    void clearDisplay();
    void display1(std::string serial, int ms);
    void display1_(std::string serial, int ms);
    void dutyFrame(std::pair<std::string, std::string> dt,
                  std::string numDev, std::string onDev,
                  std::string offDev, int ms);
    void diagnosticFrame(std::string num1, std::string num2,std::string num3,
        std::string ip1, std::string ip2, std::string ip3, int ms);
    void wait(int ms);

    void setLcdYellow(bool);
    void setLcdRed(bool);
    void setBuzzer(bool);

private:
    std::string port = "/dev/ttyACM0";
    std::string _lcdYellow;
    std::string _lcdRed;
    std::string _buzzer;
};
