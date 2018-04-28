/*
 * driver.h
 */

#ifndef DRIVER_H_
#define DRIVER_H_

typedef struct driver * Driver;

#include"team.h"
#include"season.h"

typedef enum driverStatus {
    DRIVER_STATUS_OK,
    SEASON_NOT_ASSIGNED,
    DRIVER_MEMORY_ERROR,
    INVALID_DRIVER,
    INVALID_POSITION} DriverStatus;



Driver DriverCreate(DriverStatus* status, char* driver_name, int driverId);
void   DriverDestroy(Driver driver);
const char* DriverGetName(Driver driver);
int DriverGetId(Driver driver);
Team  DriverGetTeam(Driver driver);
void  DriverSetTeam(Driver driver, Team team);
void  DriverSetSeason(Driver driver, Season season);
DriverStatus DriverAddRaceResult(Driver driver, int position);
int DriverGetPoints(Driver driver, DriverStatus* status);



#endif /* DRIVER_H_ */