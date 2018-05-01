#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>
#include "team.h"

/** Declarations */

static int DriverNumberIsValid(DriverNumber driver_number);
void TeamPrint(Team team);

/** End of declarations */

typedef struct team{
        char* name;
        Driver first_driver;
        Driver second_driver;
}*Team;

/**
 ***** TeamCreate *****
 * @param status - Success/fail.
 * @param name - name of the new team.
 * @return Pointer to the new team.
 * Note: The team is created without drivers (first and second drivers are
 * set to be NULL).
 */
Team TeamCreate(TeamStatus* status, char* name){
    if(name==NULL){
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

/**
 ***** Function: TeamDestroy *****
 * Description: Freeing all allocated memory of 'team' including
 * its drivers.
 * @param team - Pointer to a 'team'.
 */
void TeamDestroy(Team team){
    DriverDestroy(team->first_driver);
    DriverDestroy(team->second_driver);
    free(team->name);
    free(team);
}

/**
 ***** Function: TeamAddDriver *****
 * Description: Adds 'driver' to 'team' if the team isn't full.
 * @param team - Pointer to a 'team'.
 * @param driver - Pointer to a 'driver'
 * @return Status - Success/fail +reason of the function.
 */
TeamStatus TeamAddDriver(Team team, Driver driver){
    if(team == NULL || driver == NULL)
        return TEAM_NULL_PTR;
    /** There is no first driver yet. */
    if(team->first_driver == NULL){
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

/** Tested: Yes
 ***** Function: TeamGetName *****
 * @param team - Pointer to the 'team'.
 * @return Name (string) of the 'team'.
 */
const char * TeamGetName(Team team){
    if(team == NULL){
        return NULL;
    }
    return team->name;
}

/** Tested: Yes
***** Function: TeamGetDriver *****
* @param team - Pointer to a team.
* @param driver_number - First/second driver of the 'team'.
* @return Pointer to the driver.
*/
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

/**
 ***** Function: TeamGetPoints *****
 * @param team - Pointer to a team.
 * @param status - Will hold Success/fail +reason of the function.
 * @return Number of points the 'team' has.
 */
int TeamGetPoints(Team team, TeamStatus *status){
    int sum=0;
    DriverStatus driver_status;
    if(team == NULL || status == NULL){
        *status = TEAM_NULL_PTR;
        return 0;
    }
    int points1 = DriverGetPoints(team->first_driver,&driver_status);
    /** Check if first driver's points is okay*/
    if(driver_status == DRIVER_STATUS_OK){
        sum+=points1;
    }
    int points2 = DriverGetPoints(team->second_driver,&driver_status);
    /** Check if second driver's points is okay*/
    if(driver_status == DRIVER_STATUS_OK){
        sum+=points2;
    }
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


