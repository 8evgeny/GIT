#include <string>
#include <chrono>

class Display
{
public:
    Display();
    ~Display();

    void printToLcd(std::string str, std::string port);
    void clearDisplay();
    void display1(int ms);
    void display2(int ms);
    void display3(std::pair<std::string, std::string> dt,
                  std::string numDev, std::string onDev,
                  std::string offDev, int ms);
    void display4(std::string num1, std::string num2,std::string num3,
        std::string ip1, std::string ip2, std::string ip3, int ms);
    void wait(int ms);

private:
std::string port = "/dev/ttyACM0";

};
