#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>
#include "team.h"

/** Declarations */
static int DriverNumberIsValid(DriverNumber driver_number);
/** End of declarations */

struct team{
        char* name;
        Driver first_driver;
        Driver second_driver;
};

/**
 ***** TeamCreate *****
 * Description: The function gets a string team's name and creates a new
 * team. If the create was a success a pointer to the team is returned,
 * else NULL.
 * @param status - Success/failure of the function (if fails - with cause).
 * @param name - name of the new team.
 * @return Pointer to the new team.
 * Note: The team is created without drivers (first and second drivers are
 * set to be NULL).
 */
Team TeamCreate(TeamStatus* status, char* name){
    if(name==NULL){
        if (status!=NULL){
            *status = TEAM_NULL_PTR;
        }
        return NULL;
    }
    char* string = malloc(strlen(name)+1);
    if(string==NULL){ // String memory allocation failed.
        if (status!=NULL){
            *status = TEAM_MEMORY_ERROR;
        }
        return NULL;
    }
    strcpy(string,name);
    Team team = malloc(sizeof(*team));
    if(team == NULL){ // Team memory allocation failed.
        if (status!=NULL){
            *status = TEAM_MEMORY_ERROR;
        }
        free(string);
        return NULL;
    }
    /* If we got here we can create the team. */
    team->name = string;
    team->first_driver = NULL;
    team->second_driver = NULL;
    if (status!=NULL){
        *status = TEAM_STATUS_OK;
    }
    return team;
}

/**
 ***** Function: TeamDestroy *****
 * Description: Freeing all allocated memory of 'team' including
 * its drivers.
 * @param team - Pointer to a 'team'.
 */
void TeamDestroy(Team team){
    if(team!=NULL){
        DriverDestroy(team->first_driver);
        DriverDestroy(team->second_driver);
        free(team->name);
        free(team);
    }
}

/**
 ***** Function: TeamAddDriver *****
 * Description: Adds 'driver' to 'team' if the team isn't full.
 * @param team - Pointer to a 'team'.
 * @param driver - Pointer to a 'driver'
 * @return Status - Success/failure of the function (if fails - with cause).
 */
TeamStatus TeamAddDriver(Team team, Driver driver){
    if(team == NULL || driver == NULL)
        return TEAM_NULL_PTR;
    if(team->first_driver == NULL){ // 'team' has no first driver yet.
        team->first_driver = driver;
        return TEAM_STATUS_OK;
    }
    else if(team->second_driver == NULL){ // 'team' has no second driver yet.
            team->second_driver = driver;
            return TEAM_STATUS_OK;
    }
    /* If we got here then 'team' already has 2 drivers. */
    return TEAM_FULL;
}

/**
 ***** Function: TeamGetName *****
 * Description: The function gets a pointer to a team and returns the name
 * of the team. if the pointer is null then the function returns null.
 * @param team - Pointer to the 'team'.
 * @return Name (char*) of the 'team'.
 */
const char * TeamGetName(Team team){
    if(team == NULL){
        return NULL;
    }
    return team->name;
}

/**
***** Function: TeamGetDriver *****
 * Description: The function gets a pointer to a team and a number of
 * driver and returns a pointer to that driver in the team. If the pointer
 * to the team is null or the number is not valid (enum - FIRST_DRIVER or
 * SECOND_DRIVER) then the function returns null.
* @param team - Pointer to a team.
* @param driver_number - First/second driver of the 'team'.
* @return Pointer to the driver.
*/
Driver TeamGetDriver(Team team, DriverNumber driver_number){
    /* Invalid input. */
    if(team == NULL || !DriverNumberIsValid(driver_number)){
        return NULL;
    }
    if(driver_number == FIRST_DRIVER){
        return team->first_driver;
    }
    else return team->second_driver;
}

/**
 ***** Function: TeamGetPoints *****
 * Description: The function gets a pointer to a team and a pointer of
 * status and returns the number of points of that team. If the pointer to
 * the team or the pointer to the status is null then the function returns
 * null and updating the status to 'TEAM_NULL_PTR'. If everything is okay
 * then status will be updated to 'TEAM_STATUS_OK'.
 * @param team - Pointer to a team.
 * @param status - Success/failure of the function (if fails - with cause).
 * @return Number of points the 'team' has.
 */
int TeamGetPoints(Team team, TeamStatus *status){
    int team_points=0;
    DriverStatus driver_status;
    if(team == NULL){
        if(status!=NULL){
            *status = TEAM_NULL_PTR;
        }
        /* 'team' is not a valid pointer, therefore has no points. */
        return 0;
    }
    int first_driver_points =DriverGetPoints(team->first_driver,
            &driver_status);
    /* Check if first driver's points is okay */
    if(driver_status == DRIVER_STATUS_OK){
        team_points+=first_driver_points;
    }
    int second_driver_points = DriverGetPoints(team->second_driver,
            &driver_status);
    /* Check if second driver's points is okay*/
    if(driver_status == DRIVER_STATUS_OK){
        team_points+=second_driver_points;
    }
    if(status!=NULL){
        *status = TEAM_STATUS_OK;
    }
    return team_points;
}

/** Static functions */
/**
 ***** Static Function: DriverNumberIsValid *****
 * Description: Checks if driver's number is FIRST_DRIVER or
 * SECOND_DRIVER (enum).
 * @param driver_number - (enum) number of driver.
 * @return 1 if the number is valid, else 0.
 */
static int DriverNumberIsValid(DriverNumber driver_number){
    if(driver_number!=FIRST_DRIVER && driver_number!=SECOND_DRIVER){
        return 0;
    }
    return 1;
}
/** End of static functions */


