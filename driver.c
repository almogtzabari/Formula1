#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>
#include "driver.h"

#define MIN_POSITION 1

/** Declarations */

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
 * @param driverId - Driver id.
 * @return returns NULL in case of failure or pointer to the created driver in case of success.
 */
Driver DriverCreate(DriverStatus* status, char* driver_name, int driverId){
    assert(status!=NULL && driver_name!=NULL);
    char* name = malloc(strlen(driver_name)+1);
    if (name==NULL){
        *status=DRIVER_MEMORY_ERROR;
        return NULL;
    }
    strcpy(name,driver_name);
    Driver driver = malloc(sizeof(*driver));
    if (driver==NULL){
        *status=DRIVER_MEMORY_ERROR;
        free(name);
        return NULL;
    }
    if(driverId<=0){
        *status=DRIVER_MEMORY_ERROR;
        free(name);
        return NULL;
    }
    driver->id=driverId;
    driver->driver_name=name;
    driver->season_of_driver = NULL;
    driver->points = 0;
    *status=DRIVER_STATUS_OK;
    return driver;
}
/**
 ***** Function : DriverSetTeam *****
 * Sets given team to a given driver.
 * @param driver A pointer to a driver.
 * @param team A pointer to a team.
 */
void DriverSetTeam(Driver driver, Team team){
    assert(driver!=NULL && team!=NULL);
    driver->team=team;
}

/** Tested: Yes
 ***** Function: DriverGetName *****
 * Gets a driver name.
 * @param driver - A pointer to a driver.
 * @return Name of the driver.
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
 * @return A pointer to a team if driver is not a NULL pointer.
 */
Team DriverGetTeam(Driver driver){
    if(driver==NULL){
        return NULL;
    }
    return driver->team;
}

/** Tested: Yes
 ***** Function: DriverGetId *****
 * Gets the given driver's id.
 * @param driver - A pointer to a driver.
 * @return ID of the driver.
 */
int DriverGetId(Driver driver){
    if (driver==NULL){
        return 0;
    }
    return driver->id;
}

/**
 ***** Function: DriverDestroy *****
 * Destroys the allocated memory of all resources of a given driver.
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
 * Sets a given season to a given driver.
 * @param driver - A pointer to a driver.
 * @param season - A pointer to a season.
 */
void DriverSetSeason(Driver driver, Season season){
    assert(driver!=NULL && season!=NULL);
    driver->season_of_driver = season;
    driver->points=0;
}

/**
 ***** Function: DriverAddRaceResult *****
 * Adds points to a driver by it's position.
 * @param driver - A pointer to a driver.
 * @param position - The driver's position in the last race.
 * @return Success/Failure of the function.
 */
DriverStatus DriverAddRaceResult(Driver driver, int position){
    if(driver == NULL){
        return INVALID_DRIVER;
    }
    if (position<MIN_POSITION){
        return INVALID_POSITION;
    }
    if(driver->season_of_driver==NULL){
        return SEASON_NOT_ASSIGNED;
    }
    driver->points+=(SeasonGetNumberOfDrivers(driver->season_of_driver)-position);
    return DRIVER_STATUS_OK;
}

/** Tested: Yes
 ***** Function: DriverGetPoints *****
 * Gets a driver's points.
 * @param driver - A pointer to a driver.
 * @param status - Will hold the success/fail of the function.
 * @return number of points of 'driver'.
 */
int DriverGetPoints(Driver driver, DriverStatus* status){
    assert(status!=NULL);
    if (driver==NULL){
        *status=INVALID_DRIVER;
        return 0;
    }
    *status=DRIVER_STATUS_OK;
    return driver->points;
}