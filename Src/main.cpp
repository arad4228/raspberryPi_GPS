#include "CGPS.h"

int main(void)
{
    CGPS GPS = CGPS();
    std::pair<double, double> pairLocation;
    while(true)
    {
        pairLocation = GPS.getLocation();
        std::cout << "Latitude: " << pairLocation.first << " Longitude: " << pairLocation.second << std::endl;
        return 0;
    }
    return 0;
}