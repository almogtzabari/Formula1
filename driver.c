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
 * @return return - NULL in case of failure or pointer to the created
 * driver in case of success.
 */
Driver DriverCreate(DriverStatus* status, char* driver_name, int driverId){
    if (driver_name==NULL){
        if (status!=NULL){
            *status=INVALID_DRIVER;
        }
        return NULL;
    }
    char* name = malloc(strlen(driver_name)+1);
    if (name==NULL){  // Checks if memory allocation failed.
        if (status!=NULL){
            *status=DRIVER_MEMORY_ERROR;
        }
        return NULL;
    }
    strcpy(name,driver_name);
    Driver driver = malloc(sizeof(*driver));
    if (driver==NULL){ // Checks if memory allocation failed.
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
    /* If we got here we can create a driver. */
    driver->id=driverId;
    driver->driver_name=name;
    driver->season_of_driver = NULL;
    driver->points = 0;
    driver->team = NULL; // On creatation 'driver' has no team.
    if (status!=NULL){
        *status=DRIVER_STATUS_OK;
    }
    return driver;
}

/**
 ***** Function : DriverSetTeam *****
 * Description: assign the given team to the given driver.
 * @param driver - A pointer to a driver.
 * @param team - A pointer to a team.
 */
void DriverSetTeam(Driver driver, Team team){
    if (driver==NULL){
        return;
    }
    driver->team=team;
}

/**
 ***** Function: DriverGetName *****
 * Description: gets a driver name.
 * @param driver - A pointer to a driver.
 * @return A pointer to a string contains the name of the driver.
 */
const char* DriverGetName(Driver driver){
    if (driver==NULL){
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
    if(driver==NULL){
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
    if (driver==NULL){
        return 0;
    }
    return driver->id;
}

/**
 ***** Function: DriverDestroy *****
 * Description: destroys all allocated memory of 'driver'.
 * @param driver - A pointer to a driver.
 */
void DriverDestroy(Driver driver){
    if(driver!=NULL){
        free(driver->driver_name);
        free(driver);
    }
}

/**
 ***** Function: DriverSetSeason *****
 * Description: assign the given season to the given driver.
 * @param driver - A pointer to a driver.
 * @param season - A pointer to a season.
 */
void DriverSetSeason(Driver driver, Season season){
    if (driver!=NULL && season!=NULL){ // Both Pointers are valid.
    driver->season_of_driver = season;
    driver->points=0; // Reset it's points.
    }
}

/**
 ***** Function: DriverAddRaceResult *****
 * Description: adds points to a driver by it's position.
 * @param driver - A pointer to a driver.
 * @param position - The driver's position in the last race.
 * @return - Success/failure of the function (if fails - with cause).
 */
DriverStatus DriverAddRaceResult(Driver driver, int position){
    if (driver == NULL){
        return INVALID_DRIVER;
    }
    if (driver->season_of_driver == NULL){
        return SEASON_NOT_ASSIGNED;
    }
    /* Checks if the given position is bigger than 0 and smaller than the
     * number of drivers in the season. */
    if (position < MIN_POSITION ||
        position > SeasonGetNumberOfDrivers(DriverGetSeason(driver))){
        return INVALID_POSITION;
    }
    /* Adds points to a driver according to it's position. */
    driver->points+=(SeasonGetNumberOfDrivers(driver->season_of_driver)-position);
    return DRIVER_STATUS_OK;
}

/**
 ***** Function: DriverGetPoints *****
 * Description: gets driver's points.
 * @param driver - A pointer to a driver.
 * @param status - Success/failure of the function (if fails - with cause).
 * @return - Number of points of a given driver.
 */
int DriverGetPoints(Driver driver, DriverStatus* status){
    if (driver==NULL){
        if (status!=NULL){
            *status = INVALID_DRIVER;
        }
        return 0;
    }
    if (status!=NULL) {
        *status = DRIVER_STATUS_OK;
    }
    return driver->points;
}

/** Static functions */
/**
 *****  Function: DriverGetSeason *****
 * @param driver - A pointer to a driver.
 * @return - The season of the driver.
 */
static Season DriverGetSeason(Driver driver){
    return driver->season_of_driver;
}
/** End of static functions */