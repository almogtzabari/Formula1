#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>
#include "team.h"

/** Declarations */

static int DriverNumberIsValid(DriverNumber driver_number);

/** End of declarations */

typedef struct team{
        char* name;
        Driver first_driver;
        Driver second_driver;
}*Team;

/** test comment */

Team TeamCreate(TeamStatus* status, char* name){
    if(name!=NULL){
        *status = TEAM_MEMORY_ERROR;
        return NULL;
    }
    char* string = malloc(strlen(name)+1);
    /** String memory allocation failed. */
    if(string==NULL){
        *status = TEAM_MEMORY_ERROR;
        return NULL;
    }
    strcpy(string,name);
    Team team = malloc(sizeof(*team));
    /** Team memory allocation failed. */
    if(team == NULL){
        *status = TEAM_MEMORY_ERROR;
        free(string);
        return NULL;
    }
    team->name = string;
    team->first_driver = NULL; // Not sure.
    team->second_driver = NULL; // Not sure.
    *status = TEAM_STATUS_OK;
    return team;
}

void TeamDestroy(Team team){
    DriverDestroy(team->first_driver);
    DriverDestroy(team->second_driver);
    free(team->name);
    free(team);
}

TeamStatus TeamAddDriver(Team team, Driver driver){
    if(team == NULL || driver == NULL)
        return TEAM_NULL_PTR;
    /** There is no first driver yet. */
    if(team->first_driver == NULL){// no first driver
        team->first_driver = driver;
        return TEAM_STATUS_OK;
    }
    /** There is no second driver yet. */
    else if(team->second_driver == NULL){
            team->second_driver = driver;
            return TEAM_STATUS_OK;
    }
    /** Team already has 2 drivers. */
    return TEAM_FULL;
}

const char * TeamGetName(Team team){
    if(team == NULL){
        return NULL;
    }
    return team->name;
}

Driver TeamGetDriver(Team team, DriverNumber driver_number){
    /** Invalid input. */
    if(team == NULL || !DriverNumberIsValid(driver_number)){
        return NULL;
    }
    /** First Driver is needed. */
    if(driver_number == FIRST_DRIVER){
        return team->first_driver;
    }
    /** Second driver is needed. */
    else return team->second_driver;
}


int TeamGetPoints(Team team, TeamStatus *status){
    if(team == NULL || status == NULL){
        *status = TEAM_NULL_PTR;
        return NULL;
    }
    int points1 = DriverGetPoints(team->first_driver,status);
    int points2 = DriverGetPoints(team->second_driver,status);
    int sum = points1 + points2;
    *status = TEAM_STATUS_OK;
    return sum;
}


/** Static Functions*/
static int DriverNumberIsValid(DriverNumber driver_number){
    if(driver_number!=FIRST_DRIVER && driver_number!=SECOND_DRIVER){
        return 0;
    }
    return 1;
}
/** End of static functions */


