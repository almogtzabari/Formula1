/*
 * team.h
 */

#ifndef TEAM_H_
#define TEAM_H_

typedef struct team * Team;

#include"driver.h"

typedef enum teamStatus {
    TEAM_STATUS_OK,
    TEAM_MEMORY_ERROR,
    TEAM_NULL_PTR,
    TEAM_FULL} TeamStatus;
typedef enum driverNumber {FIRST_DRIVER, SECOND_DRIVER} DriverNumber;


Team TeamCreate(TeamStatus* status, char* name);
void TeamDestroy(Team team);
TeamStatus TeamAddDriver(Team team, Driver driver);
const char * TeamGetName(Team  team);
Driver TeamGetDriver(Team  team, DriverNumber driver_number);
int TeamGetPoints(Team  team, TeamStatus *status);

#endif /* TEAM_H_ */