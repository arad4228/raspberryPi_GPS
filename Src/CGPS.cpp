#include "CGPS.h"

CGPS::CGPS()
{
    if(wiringPiSetup() == -1)
    {
        std::cerr << "wiringPiSetup Failed" << std::endl;
        exit(-1);
    }

    if((this->gpsSerial = serialOpen(this->strSerialPort.data(), 9600)) < 0)
    {
        std::cerr << "GPS 시리얼 포트 열기 실패!" << std::endl;
        exit(-1);
    }
}

CGPS::CGPS(const char* strSerialPort)
{
    if(wiringPiSetup() == -1)
    {
        std::cerr << "wiringPiSetup Failed" << std::endl;
        exit(-1);
    }

    this->strSerialPort.clear();
    this->strSerialPort.append(strSerialPort);

    if((this->gpsSerial = serialOpen(strSerialPort, 9600)) < 0)
    {
        std::cerr << "GPS 시리얼 포트 열기 실패!" << std::endl;
        exit(-1);
    }
}

CGPS::~CGPS()
{

}

std::pair<double, double> CGPS::getLocation()
{
    bool findLocation = false;
    std::pair<double, double> pairLocation;

    while(serialDataAvail(this->gpsSerial))
    {   
        char data = serialGetchar(this->gpsSerial);
        if(data == '$')
        {
            std::string strSerialData;
            while(data != '\n')
            {
                data = serialGetchar(this->gpsSerial);
                strSerialData += data;
            }
            if(strstr(strSerialData.data(), "GPGGA") != 0)
                continue;
            
            extractLocation(strSerialData, pairLocation.first, pairLocation.second);
        }
    }
    return pairLocation;
}

void CGPS::extractLocation(const std::string& strSerialData, double& latitude, double& longitude)
{
    std::istringstream iss(strSerialData);
    std::string token;
    int count = 0;

    while(std::getline(iss, token, ',')) 
    {
        try {
            if(count == 2)
                latitude = std::stod(token);
            else if (count == 4)
                longitude = std::stod(token);
            } catch (const std::exception& e) {
                if(count == 2)
                    latitude = 0.0;
                else
                    longitude = 0.0;
            }
        count++;
    }
}