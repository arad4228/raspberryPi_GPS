#include <iostream>
#include <cstring>
#include <sstream>
#include <wiringSerial.h>
#include <wiringPi.h>

class CGPS
{
public:
    CGPS();
    CGPS(const char* strSerialPort);
    ~CGPS();
    std::pair<double, double> getLocation();

private:
    void extractLocation(const std::string& sentence, double& latitude, double& longitude);

    int gpsSerial = 0;
    std::string strSerialPort = "/dev/serial0";
};