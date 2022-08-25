
#include <stdio.h>
#include "SkyMap.h"
int main()
{
    struct SkyMap _skymap;

    struct DateTime time_and_date_of_observation;

    time_and_date_of_observation.year = 2021.00;
    time_and_date_of_observation.month = 9.00;
    time_and_date_of_observation.day = 4.00;
    time_and_date_of_observation.time = 20.2; // UTC

    //Sirius
    struct StarCoordinates star_coordinates;
    star_coordinates.ra = 101.52;
    star_coordinates.dec = -16.7424;

    // Los Angeles
    struct ObserverPosition my_position;
    my_position.lat = 34.05;
    my_position.lng = -118.24358;

    SetSkyMapObserver(&_skymap, &my_position);
    SetSkyMapDateTime(&_skymap, &time_and_date_of_observation);
    SetSkyMapCelestialObj(&_skymap, &star_coordinates);

    struct SearchResult search_result = SkyMapCalculate(&_skymap);
    printf("alt %f, az %f", search_result.alt, search_result.az);
    return 0;
}
