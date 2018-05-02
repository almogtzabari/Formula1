#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>
#include "driver.h"

#define MIN_POSITION 1

typedef struct driver {
    int id;
    char* driver_name;
    Team team;
    int points;
    Season season_of_driver;
} *Driver;


Driver DriverCreate(DriverStatus* status, char* driver_name, int driverId){
    if(driver_name==NULL){
        *status=DRIVER_MEMORY_ERROR;
        return NULL;
    }
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
    driver->id=driverId;
    driver->driver_name=name;
    driver->season_of_driver = NULL;
    driver->points = 0;
    *status=DRIVER_STATUS_OK;
    return driver;
}

void DriverSetTeam(Driver driver, Team team){
    assert(driver!=NULL && team!=NULL);
    driver->team=team;
}
/** Tested: Yes
 ***** Function: DriverGetName *****
 * @param driver - A pointer to a driver.
 * @return Name of the driver.
 */
const char* DriverGetName(Driver driver){
    if (driver==NULL){
        return NULL;
    }
    return driver->driver_name;
}

Team DriverGetTeam(Driver driver){
    if(driver==NULL){
        return NULL;
    }
    return driver->team;
}

/** Tested: Yes
 ***** Function: DriverGetId *****
 * @param driver - A pointer to a driver.
 * @return ID of the driver.
 */
int DriverGetId(Driver driver){
    if (driver==NULL){
        return 0;
    }
    return driver->id;
}

void DriverDestroy(Driver driver){
    free(driver->driver_name);
    free(driver);
}

void DriverSetSeason(Driver driver, Season season){
    assert(driver!=NULL);
    assert(season!=NULL);
    driver->season_of_driver = season;
    driver->points=0;
}

DriverStatus DriverAddRaceResult(Driver driver, int position){
    if (position<MIN_POSITION){
        return INVALID_POSITION;
    }
    if(driver->season_of_driver==NULL){
        return SEASON_NOT_ASSIGNED;
    }
    if(driver == NULL){
        return INVALID_DRIVER;
    }
    driver->points+=(SeasonGetNumberOfDrivers(driver->season_of_driver)-position);
    return DRIVER_STATUS_OK;
}

/** Tested: Yes
 ***** Function: DriverGetPoints *****
 * @param driver - A pointer to a driver.
 * @param status - Will hold the success/fail of the function.
 * @return number of points of 'driver'.
 */
int DriverGetPoints(Driver driver, DriverStatus* status){
    if (driver==NULL){
        *status=INVALID_DRIVER;
        return 0;
    }
    *status=DRIVER_STATUS_OK;
    return driver->points;
}