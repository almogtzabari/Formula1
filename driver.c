#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>
#include "driver.h"

#define MIN_POSITION 1

/** Declarations */
static Season DriverGetSeason(Driver driver);
/** End of decalarations */

struct driver {
    int id;
    char* driver_name;
    Team team;
    int points;
    Season season_of_driver;
};

/**
 ***** Function: DriverCreate *****
 * Creates a new driver.
 * @param status - will hold success or fail of the function.
 * @param driver_name - A string which contains the driver's name.
 * @param driverId - A given id.
 * @return return - NULL in case of failure or pointer to the created driver in case of success.
 */
Driver DriverCreate(DriverStatus* status, char* driver_name, int driverId){
    if (driver_name==NULL){ // Checks if the given string is NULL.
        if (status!=NULL){ // Checks if status is NULL.
            *status=INVALID_DRIVER;
        }
        return NULL;
    }
    char* name = malloc(strlen(driver_name)+1);
    if (name==NULL){  // Checks if allocation failed.
        if (status!=NULL){
            *status=DRIVER_MEMORY_ERROR;
        }
        return NULL;
    }
    strcpy(name,driver_name);
    Driver driver = malloc(sizeof(*driver));
    if (driver==NULL){
        if (status!=NULL){
            *status=DRIVER_MEMORY_ERROR;
        }
        free(name);
        return NULL;
    }
    if(driverId<=0){ // Checks if id is vaild.
        if (status!=NULL){
            *status=INVALID_DRIVER;
        }
        free(driver);
        free(name);
        return NULL;
    }
    // If we got here we can create a driver.
    driver->id=driverId;  // Sets driver's id.
    driver->driver_name=name;
    driver->season_of_driver = NULL;
    driver->points = 0; // Resets driver's points to 0.
    driver->team = NULL;
    if (status!=NULL){
        *status=DRIVER_STATUS_OK;
    }
    return driver;
}
/**
 ***** Function : DriverSetTeam *****
 * Sets given team to a given driver.
 * @param driver - A pointer to a driver.
 * @param team - A pointer to a team.
 */
void DriverSetTeam(Driver driver, Team team){
    if (driver==NULL){ //Checks if driver is not NULL.
        return;
    }
    driver->team=team;
}

/**
 ***** Function: DriverGetName *****
 * Gets a driver name.
 * @param driver - A pointer to a driver.
 * @return A pointer to a string contains the name of the driver.
 */
const char* DriverGetName(Driver driver){
    if (driver==NULL){ //Checks if driver is not NULL.
        return NULL;
    }
    return driver->driver_name;
}

/**
 ***** Function: DriverGetTeam *****
 * Gets the driver's team pointer.
 * @param driver - A pointer to a driver.
 * @return - A pointer to a team if driver is not a NULL pointer.
 */
Team DriverGetTeam(Driver driver){
    if(driver==NULL){ //Checks if driver is not NULL.
        return NULL;
    }
    return driver->team;
}

/**
 ***** Function: DriverGetId *****
 * Gets the given driver's id.
 * @param driver - A pointer to a driver.
 * @return - ID of the driver.
 */
int DriverGetId(Driver driver){
    if (driver==NULL){ //Checks if driver is not NULL.
        return 0;
    }
    return driver->id;
}

/**
 ***** Function: DriverDestroy *****
 * Destroys all allocated memory of a given driver.
 * @param driver - A pointer to a driver.
 */
void DriverDestroy(Driver driver){
    if(driver!=NULL){ // If driver is not NULL we need to free all of it's resources.
        free(driver->driver_name);
        free(driver);
    }
}
/**
 ***** Function: DriverSetSeason *****
 * Sets a given season to a given driver.
 * @param driver - A pointer to a driver.
 * @param season - A pointer to a season.
 */
void DriverSetSeason(Driver driver, Season season){
    if (driver!=NULL && season!=NULL){ // If driver and season is not NULL we can set driver's season and
    driver->season_of_driver = season; // reset it's points.
    driver->points=0;
    }
}

/**
 ***** Function: DriverAddRaceResult *****
 * Adds points to a driver by it's position.
 * @param driver - A pointer to a driver.
 * @param position - The driver's position in the last race.
 * @return - Success/Failure of the function.
 */
DriverStatus DriverAddRaceResult(Driver driver, int position) {
    if (driver == NULL) { //Checks if driver is not NULL.
        return INVALID_DRIVER;
    }
    if (driver->season_of_driver == NULL) { // Checks if season is not NULL.
        return SEASON_NOT_ASSIGNED;
    }
    // Checks if the given position is not a negative number or bigger than the number of drivers.
    if (position < MIN_POSITION ||
        position > SeasonGetNumberOfDrivers(DriverGetSeason(driver))) {
        return INVALID_POSITION;
    }
    //Adds points to a driver by it's position.
    driver->points+=(SeasonGetNumberOfDrivers(driver->season_of_driver)-position);
    return DRIVER_STATUS_OK;
}

/**
 ***** Function: DriverGetPoints *****
 * Gets a driver's points.
 * @param driver - A pointer to a driver.
 * @param status - Will hold the success/fail of the function.
 * @return - number of points of a given driver.
 */
int DriverGetPoints(Driver driver, DriverStatus* status){
    if (driver==NULL){ //Checks if driver is not NULL.
        if (status!=NULL) { // Checks if status is not NULL.
            *status = INVALID_DRIVER;
        }
        return 0;
    }
    if (status!=NULL) {
        *status = DRIVER_STATUS_OK;
    }
    return driver->points;
}
/**
 ***** Function: DriverGetSeason *****
 * @param driver - A pointer to a driver.
 * @return - the season of the driver.
 */
static Season DriverGetSeason(Driver driver){
    return driver->season_of_driver;
}