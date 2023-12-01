#include "CGPS.h"

CGPS::CGPS()
{
    if((this->gpsSerial = serialOpen(this->strSerialPort.data(), 9600)) < 0)
    {
        std::cerr << "GPS 시리얼 포트 열기 실패!" << std::endl;
        exit(-1);
    }

    if(wiringPiSetup() == -1)
    {
        std::cerr << "wiringPiSetup Failed" << std::endl;
        exit(-1);
    }

    this->IsitGGAstring=0;
    this->GGA_index=0;
    this->is_GGA_received_completely=0;
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
    this->IsitGGAstring=0;
    this->GGA_index=0;
    this->is_GGA_received_completely=0;
}

CGPS::~CGPS()
{

}

std::pair<double, double> CGPS::getLocation()
{
    std::pair<double, double> pairLocation;
    char strGPS[100];
    char strGGA_Code[3]={0, };
    char data;
    while(true)
    {   
        if(serialDataAvail(this->gpsSerial))
        {
            data = serialGetchar(this->gpsSerial);
            if(data == '$')
            {
                this->IsitGGAstring=0;
                this->GGA_index=0;
            }
            else if(this->IsitGGAstring == 1)
            {
                strGPS[this->GGA_index++] = data;
                if(data == '\r')
                    this->is_GGA_received_completely = 1;
            }
            else if(strGGA_Code[0] == 'G' && strGGA_Code[1] == 'G' && strGGA_Code[2] == 'A')
            {
                this->IsitGGAstring = 1;
		        strGGA_Code[0]= 0;
                strGGA_Code[0]= 0;
		        strGGA_Code[0]= 0;
            }
            else
            {
                strGGA_Code[0]= strGGA_Code[1]; 
		        strGGA_Code[1]= strGGA_Code[2];
		        strGGA_Code[2]= data;
            }
        }
        if(this->is_GGA_received_completely == 1)
        {
            std::string strSerialData(strGPS);
            extractLocation(strSerialData, pairLocation.first, pairLocation.second);
            this->is_GGA_received_completely=0;
            break;
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
            if(count == 1)
            {
                double tempLatitude = std::stod(token);
                latitude = (int)tempLatitude / 100;
                tempLatitude = tempLatitude - (latitude * 100);
		latitude+= tempLatitude/60.0;
            }
            else if (count == 3)
            {
                double templongitude = std::stod(token);
             	longitude = (int)templongitude/100;
                templongitude = templongitude - (longitude * 100);
		longitude+=templongitude/60.0;
            }
            } catch (const std::exception& e) {
                if(count == 2)
                    latitude = 0.0;
                else
                    longitude = 0.0;
            }
        count++;
    }
}
