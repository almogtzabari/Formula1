#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>
#include "driver.h"


typedef struct season {
    int year;
    int number_of_teams;
    Team* team_array;
    int number_of_drivers;
    Driver* drivers_array
} *Season;

/**
 ****** Function: SeasonGetDriverByPosition *****
 * @param season - Pointer to a season.
 * @param position
 * @param status
 * @return
 */
Driver SeasonGetDriverByPosition(Season season, int position, SeasonStatus* status){
    Driver* driver_array = season->driver_array;
    for (int i = 0; i < ; ++i) {
        if(driver_array[i])
    }
}
/**
 ***** Function: SeasonGetDriverStandings *****
 * @param season - Pointer to a season.
 * @return Sorted array of drivers.
 */
Driver* SeasonGetDriversStandings(Season season){
    Driver* drivers_array = malloc(sizeof(Driver)*(season->number_of_drivers));
    CopyDriversArray(season->drivers_array,drivers_array,season->number_of_drivers);
}


static void CopyDriversArray(Driver* src, Driver* dest, int number_of_drivers){
    for (int i = 0; i <number_of_drivers ; ++i) {
        dest[i] = src[i];
    }
}