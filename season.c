#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "driver.h"


/** Declarations */
static void mergeSort(Driver* drivers_array, int l, int r);
static void merge(Driver* drivers_array, int l, int m, int r);
static void DriversAndTeamsCounter(int* drivers, int* teams, const char* details,SeasonStatus* status);
static void DriverAndTeamArrayDestroy (Driver* driver_array, Team* team_array,
                                       int number_of_drivers, int number_of_teams);
void SeasonDestroy(Season season);
int SeasonGetNumberOfDrivers(Season season);
static bool DriverIsNone(char* name, char* source );
int SeasonGetNumberOfTeams(Season season);
SeasonStatus SeasonAddRaceResult(Season season, int* results);
Driver* SeasonGetDriversStandings(Season season);

/** End of declarations*/

typedef struct season {
    int year;
    int number_of_teams;
    Team* team_array;
    int number_of_drivers;
    Driver* drivers_array;
} *Season;


Driver* SeasonGetDriversStandings(Season season){
    Driver* sorted_driver_array=malloc(
            sizeof(*sorted_driver_array)*SeasonGetNumberOfDrivers(season));

    return season->drivers_array;
}

SeasonStatus SeasonAddRaceResult(Season season, int* results){
    if (season==NULL || results==NULL){
        *results=SEASON_NULL_PTR;
        return SEASON_NULL_PTR;
    }
    for (int i=0;i<SeasonGetNumberOfDrivers(season);i++) {
        DriverAddRaceResult(season->drivers_array[results[i]-1],i+1);
    }
    return SEASON_OK;
}

/**
 ***** Function: SeasonCreate *****
 * @param status - Will hold success/fail of the function.
 * @param season_info - String containing input of teams and drivers.
 * @return A pointer to the season.
 */
Season SeasonCreate (SeasonStatus* status,const char* season_info){
    int current_driver=0, current_team=0, id=1, line_number=0;
    TeamStatus team_creation_status=TEAM_STATUS_OK;
    DriverStatus driver_creation_status=DRIVER_STATUS_OK;
    Season new_season=malloc(sizeof(*new_season));
    if(new_season==NULL){
        *status=SEASON_MEMORY_ERROR;
        return NULL;
    }
    DriversAndTeamsCounter(&new_season->number_of_drivers,
                           &new_season->number_of_teams,season_info,status);
    if (*status==SEASON_MEMORY_ERROR){
        free(new_season);
        return NULL;
    }
    char* season_info_copy=malloc((strlen(season_info))+1);
    if(season_info_copy==NULL){ //If allocation fails frees all the allocated elements.
        *status=SEASON_MEMORY_ERROR;
        free(new_season);
        return NULL;
    }
    Team* team_array=malloc(sizeof(*team_array)*(new_season->number_of_teams));
    if (team_array==NULL){ //If allocation fails frees all the allocated elements.
        free(new_season);
        free(season_info_copy);
        return NULL;
    }
    for (int i=0;i<new_season->number_of_teams;i++) {
        team_array[i]=NULL;
    }
    Driver* driver_array=malloc(sizeof(*driver_array)*new_season->number_of_drivers);
    if (driver_array==NULL){    //If allocation fails frees all the allocated elements.
        free(new_season);
        free(season_info_copy);
        free(team_array);
        return NULL;
    }
    for (int j=0;j<new_season->number_of_drivers;j++) { //Setting array pointers to NULL which helps if
        driver_array[j]=NULL;                           //memory allocation fails.
    }
    strcpy(season_info_copy,season_info);
    char* season_details=strtok(season_info_copy,"\n");
    new_season->year=atoi(season_details);
    season_details=strtok(NULL,"\n");
    while(season_details!=NULL){
        if(line_number%3==0){   //Checks if the current line is a team name.
            team_array[current_team++]=TeamCreate(&team_creation_status,season_details);
            if (team_creation_status==TEAM_MEMORY_ERROR){ //If allocation fails frees all the allocated elements.
                DriverAndTeamArrayDestroy(driver_array,team_array,
                                          new_season->number_of_drivers,new_season->number_of_teams);
                free(new_season);
                free(season_info_copy);
                return NULL;
            }
        }
        else if(!DriverIsNone(season_details,"None")){  //Checks if the current line is a valid driver name.
            driver_array[current_driver++]=DriverCreate(&driver_creation_status,season_details,id++);
            if(driver_creation_status==DRIVER_MEMORY_ERROR){ //If allocation fails frees all the allocated elements.
                DriverAndTeamArrayDestroy(driver_array,team_array,
                                          new_season->number_of_drivers,new_season->number_of_teams);
                free(new_season);
                free(season_info_copy);
                return NULL;
            }
        }
        season_details=strtok(NULL,"\n");
        line_number++;
    }
    free(season_info_copy);
    new_season->team_array=team_array;
    new_season->drivers_array=driver_array;
    return new_season;
}

/**
 ***** Function: DriversAndTeamsCounter *****
 * @param drivers - Will hold the number of drivers.
 * @param teams - Will hold the number of teams.
 * @param details - String input that contains the teams and drivers.
 * @param status - Success/fail.
 */
static void DriversAndTeamsCounter(int* drivers, int* teams,
                                   const char* details,SeasonStatus* status){
    int number_of_drivers=0, number_of_teams=0;
    int line_number=0;
    char* season_details_copy=malloc((strlen(details))+1);
    if(season_details_copy==NULL){
        *status=SEASON_MEMORY_ERROR;
        return;
    }
    strcpy(season_details_copy,details);
    char* line = strtok(season_details_copy,"\n");
    line=strtok(NULL,"\n");
    while(line!=NULL) {
        if (line_number % 3 == 0) {
            number_of_teams++;
        }
        else if (!DriverIsNone(line,"None")) {
            number_of_drivers++;
        }
        line = strtok(NULL,"\n");
        line_number++;
    }
    *drivers = number_of_drivers;
    *teams = number_of_teams;
    free(season_details_copy);
}

static void DriverAndTeamArrayDestroy (Driver* driver_array, Team* team_array,
                                       int number_of_drivers, int number_of_teams) {
    for (int i = 0; i < number_of_drivers; i++) {
        DriverDestroy(driver_array[i]);
    }
    for (int j = 0; j < number_of_teams; j++) {
        TeamDestroy(team_array[j]);
    }
    free(driver_array);
    free(team_array);
}

/**
 ***** Function: SeasonDestroy *****
 * Description: freeing all allocated memory of season including all the
 * elements in array: drivers, teams and their pointers.
 * @param season - A pointer to a season.
 */
void SeasonDestroy(Season season) {
    DriverAndTeamArrayDestroy(season->drivers_array, season->team_array,
                              season->number_of_drivers,
                              season->number_of_teams);
    free(season);
}

int SeasonGetNumberOfDrivers(Season season){
            assert(season!=NULL);
            return season->number_of_drivers;
}

static bool DriverIsNone(char* name, char* source ){
    while (*name){
        if (*name==*source){
            name++;
            source++;
        }
        else {
            return false;
        }
    }
    return true;
}

int SeasonGetNumberOfTeams(Season season){
    assert(season!=NULL);
    return season->number_of_teams;
}

