#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "driver.h"


/** Declarations */
static void DriversAndTeamsCounter(int* drivers, int* teams, const char* details,SeasonStatus* status);
static void DriverAndTeamArrayDestroy (Driver* driver_array, Team* team_array, int number_of_drivers, int number_of_teams);
void SeasonDestroy(Season season);
int SeasonGetNumberOfDrivers(Season season);
static bool DriverIsNone(char* name, char* source );
SeasonStatus SeasonAddRaceResult(Season season, int* results);
Driver* SeasonGetDriversStandings(Season season);
static void DriversArrayToPointsArray(int *drivers_points, Driver *drivers_array, int number_of_drivers);
static int FindIndexOfMaxPointsDriver(Season season, int* points, int number_of_drivers);
static int FindLastPositionById(Season season, int id);


/** End of declarations*/

typedef struct season {
    int year;
    int number_of_teams;
    Team* team_array;
    int number_of_drivers;
    Driver* drivers_array;
    int* last_race_results_array;
} *Season;


SeasonStatus SeasonAddRaceResult(Season season, int* results){
    if (season==NULL || results==NULL){
        return SEASON_NULL_PTR;
    }
    for (int i=0;i<SeasonGetNumberOfDrivers(season);i++) {
        DriverAddRaceResult(season->drivers_array[results[i]-1],i+1);
        season->last_race_results_array[i] = results[i];
    }
    return SEASON_OK;
}

Driver* SeasonGetDriversStandings(Season season){
    assert(season!=NULL);
    Driver* drivers_standings = malloc(sizeof(*drivers_standings)*season->number_of_drivers); // todo: where do we free array?
    if(drivers_standings == NULL){
        return NULL;
    }
    int* drivers_points_array = malloc(sizeof(*drivers_points_array)*season->number_of_drivers);
    if(drivers_points_array == NULL){
        free(drivers_standings);
        return NULL;
    }
    /* drivers_points_array will hold the points of the drivers.
     * drivers_points_array[i] will contain the number of points of
     * the driver i.  */
    DriversArrayToPointsArray(drivers_points_array, season->drivers_array, season->number_of_drivers);
    /* Sorting the drivers by points. The driver with the highest score
     * will be stored at driver_standings[0], and so on.*/
    int driver_index;
    for(int i=0;i<season->number_of_drivers;i++){
        /* driver_index is holding the index of the driver who
         * has the greatest score. */
        driver_index = FindIndexOfMaxPointsDriver(season,drivers_points_array,season->number_of_drivers);
        drivers_standings[i] = season->drivers_array[driver_index];
    }
    free(drivers_points_array);
    return drivers_standings;
}
static void DriversArrayToPointsArray(int *drivers_points, Driver *drivers_array, int number_of_drivers){
    DriverStatus status;
    int points_of_driver_i;
    for(int i=0;i<number_of_drivers;i++){
        points_of_driver_i = DriverGetPoints(drivers_array[i],&status);
        if (status == DRIVER_STATUS_OK){
            drivers_points[i] = points_of_driver_i;
        }
        // todo: free memory in case of another status?
    }
}
static int FindIndexOfMaxPointsDriver(Season season, int* points, int number_of_drivers){
    int max = points[0];
    int index_of_max = 0;
    for(int i=1;i<number_of_drivers;i++){
        if(points[i]>max){
            max = points[i];
            index_of_max = i;
        }
        else if(points[i] == max){
            if(FindLastPositionById(season,i+1)<FindLastPositionById(season,index_of_max+1)){
                index_of_max = i;
            }
        }
    }
    points[index_of_max] = -1;
    return index_of_max;
}
static int FindLastPositionById(Season season, int id){
    for(int i=0;i<season->number_of_drivers;i++){
        if(season->last_race_results_array[i] == id)
            return i+1;
    }
    return 0;
}

/**
 ***** Function: SeasonCreate *****
 * @param status - Will hold success/fail of the function.
 * @param season_info - String containing input of teams and drivers.
 * @return A pointer to the season.
 */
Season SeasonCreate (SeasonStatus* status,const char* season_info){
    int drivers_index=0, teams_index=0, id=1, line_number=0;
    TeamStatus team_creation_status;
    DriverStatus driver_creation_status;
    Season new_season = malloc(sizeof(*new_season));
    if(new_season == NULL){
        *status = SEASON_MEMORY_ERROR;
        return NULL;
    }
    DriversAndTeamsCounter(&new_season->number_of_drivers,
                           &new_season->number_of_teams,season_info,status);
    if (*status==SEASON_MEMORY_ERROR){
        free(new_season);
        return NULL;
    }
    char* season_info_copy = malloc((strlen(season_info))+1);
    if(season_info_copy == NULL){ //If allocation fails frees all the allocated elements.
        *status = SEASON_MEMORY_ERROR;
        free(new_season);
        return NULL;
    }
    Team* teams_array = malloc(sizeof(*teams_array)*(new_season->number_of_teams));
    if (teams_array == NULL){ //If allocation fails frees all the allocated elements.
        free(new_season);
        free(season_info_copy);
        return NULL;
    }
    for (int i=0;i<new_season->number_of_teams;i++) {
        teams_array[i] = NULL;
    }
    Driver* drivers_array = malloc(sizeof(*drivers_array)*new_season->number_of_drivers);
    if (drivers_array == NULL){    //If allocation fails frees all the allocated elements.
        free(new_season);
        free(season_info_copy);
        free(teams_array);
        return NULL;
    }
    for (int j=0;j<new_season->number_of_drivers;j++) { //Setting array pointers to NULL which helps if
        drivers_array[j] = NULL;                           //memory allocation fails.
    }
    strcpy(season_info_copy,season_info);
    char* line = strtok(season_info_copy,"\n");
    new_season->year = atoi(line); // converting first line(string of year) to int.
    line = strtok(NULL,"\n");
    while(line != NULL){
        if(line_number++%3 == 0){   //Checks if the current line is a team name.
            teams_array[teams_index++] = TeamCreate(&team_creation_status,line);
            if (team_creation_status == TEAM_MEMORY_ERROR){ //If allocation fails frees all the allocated elements.
                DriverAndTeamArrayDestroy(drivers_array,teams_array,
                                          new_season->number_of_drivers,new_season->number_of_teams);
                free(new_season);
                free(season_info_copy);
                return NULL;
            }
        }
        else if(!DriverIsNone(line,"None")){  //Checks if the current line is a valid driver name.
            drivers_array[drivers_index++] = DriverCreate(&driver_creation_status,line,id++);
            DriverSetSeason(drivers_array[drivers_index-1],new_season); // Adding the driver to the season.
            DriverSetTeam(drivers_array[drivers_index-1],teams_array[teams_index-1]); // Adding the driver to the team.
            if(driver_creation_status == DRIVER_MEMORY_ERROR){ //If allocation fails frees all the allocated elements.
                DriverAndTeamArrayDestroy(drivers_array,teams_array,
                                          new_season->number_of_drivers,new_season->number_of_teams);
                free(new_season);
                free(season_info_copy);
                return NULL;
            }
        }
        line = strtok(NULL,"\n");
    }
    new_season->team_array = teams_array;
    new_season->drivers_array = drivers_array;
    int* last_race_results_array = malloc(sizeof(*last_race_results_array)*new_season->number_of_drivers);
    if(last_race_results_array == NULL) {
        DriverAndTeamArrayDestroy(drivers_array, teams_array, new_season->number_of_drivers, new_season->number_of_teams);
        free(season_info_copy);
        free(new_season);
    }
    new_season->last_race_results_array = last_race_results_array;
    free(season_info_copy);
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
    while (*name && *source){
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

