#ifndef _SKYMAP
#define _SKYMAP
#include <math.h>
struct SearchResult {
    double az;
    double alt;
};

struct DateTime {

    double year;
    double month;
    double day;
    double time;


};
struct ObserverPosition {
    double lat;
    double lng;
};
struct StarCoordinates {
    double ra;
    double dec;
};
struct SkyMap
{
    struct StarCoordinates star_coordinates;
    struct ObserverPosition observer_position;
    struct DateTime date_time;
};


double rad2deg(double value) {
    return value * 180.00 / 3.14159265358979;
}
double deg2rad(double value) {
    return value * 3.14159265358979 / 180.00;
}

double calculate_local_sidereal_time(double j2000, double time, double longitude) {
    double lst = 100.46 + 0.985647 * j2000 + longitude + 15.00 * time;
    if (lst < 0.00) {
        lst += 360.00;
    }
    else if (lst > 360.00) {
        lst -= 360.00;
    }
    return lst;
}
double calculate_j2000(struct DateTime* datetime) {
    double jd = 367.00 * datetime->year
        - floor(
            7.00 * (datetime->year + floor((datetime->month + 9.00) / 12.00)) / 4.00
        )
        - floor(
            3.00 * (floor((datetime->year + (datetime->month - 9.00) / 7.00) / 100.00)
                + 1.00)
            / 4.00
        )
        + floor(275.00 * datetime->month / 9.00)
        + datetime->day
        + 1721028.5
        + datetime->time / 24.00;
    return jd - 2451545.00;
}
double deg2h(double value) {
    return value / 15.00;
}
double h2deg(double value) {
    return value * 15.00;
}
double asind(double value) {
    return asin(rad2deg(value));
}
double acosd(double value) {
    return acos(rad2deg(value));
}
double calculate_hour_angle(double lst, double ra) {
    double ha = lst - ra;
    if (ha < 0.00) {
        ha += 360.00;
    }
    else if (ha > 360.00) {
        ha -= 360.00;
    }
    return ha;
}
struct SearchResult calculate_az_alt(double ha, double dec, double lat) {
    /*  math behind calculations -- conversion from HA and DEC to alt and  az
    sin(alt) = sin(DEC) * sin(LAT) + cos(DEC) * cos(LAT) * cos(HA)
    alt = asin(alt)
               sin(DEC) - sin(alt) * sin(LAT)
    cos(a) = ---------------------------------
                    cos(alt) * cos(LAT)
    a = acos(a)
    If sin(HA) is negative,then az = a, otherwise az = 360 - a */

    double sin_dec = sin(deg2rad(dec));
    double sin_ha = sin(deg2rad(ha));
    double sin_lat = sin(deg2rad(lat));
    double cos_dec = cos(deg2rad(dec));
    double cos_ha = cos(deg2rad(ha));
    double cos_lat = cos(deg2rad(lat));
    double sin_alt = (sin_dec * sin_lat) + (cos_dec * cos_lat * cos_ha);
    double alt = asin(sin_alt);
    double cos_alt = cos(alt);
    double cos_a = (sin_dec - sin_alt * sin_lat) / (cos_alt * cos_lat);
    double a = acos(cos_a);
    a = rad2deg(a);
    alt = rad2deg(alt);

    double _az = 0.00;
    if (sin_ha > 0.00) {
        _az = 360.00 - a;
    }
    else {
        _az = a;
    }
    struct SearchResult result;
    result.az = _az;
    result.alt = alt;
    return result;
}

struct SearchResult skymap_calculate(struct SkyMap* sm) {
    double j2000 = calculate_j2000(&(sm->date_time));
    double lst = calculate_local_sidereal_time(
        j2000,
        sm->date_time.time,
        sm->observer_position.lng
    );

    double ha = calculate_hour_angle(lst, sm->star_coordinates.ra);
    return calculate_az_alt(ha, sm->star_coordinates.dec, sm->observer_position.lat);
}

void skymap_set_observer(struct SkyMap* sm, struct ObserverPosition* observer_position)
{
    (*sm).observer_position = *observer_position;
}
void skymap_set_datetime(struct SkyMap* sm, struct DateTime* dt)
{
    (*sm).date_time = *dt;
}
void skymap_set_celestial_obj(struct SkyMap* sm, struct StarCoordinates* co)
{
    (*sm).star_coordinates = *co;
}


#endif