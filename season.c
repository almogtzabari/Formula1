#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "driver.h"
#include <stdlib.h>



/** Declarations */
static void DriversAndTeamsCounter(int* drivers, int* teams, const char* details,SeasonStatus* status);
static void DriverAndTeamArrayDestroy (Driver* driver_array,
                                       Team* team_array, int number_of_drivers, int number_of_teams);
void SeasonDestroy(Season season);
int SeasonGetNumberOfDrivers(Season season);
static bool DriverIsNone(char* name, char* source );
SeasonStatus SeasonAddRaceResult(Season season, int* results);
Driver* SeasonGetDriversStandings(Season season);
static void DriversArrayToPointsArray(int *drivers_points,
                                      Driver *drivers_array, int number_of_drivers);
static int FindIndexOfMaxPointsDriver(Season season, int* points, int number_of_drivers);
static int FindLastPositionById(Season season, int id);
Team* SeasonGetTeamsStandings(Season season);
static int FindCurrentWinningTeam (Season season, int* points,
                                   int number_of_teams);
static int FindBestTeamDriverPosition (Season season,Team team);
Team SeasonGetTeamByPosition(Season season, int position,
                             SeasonStatus* status);
Driver SeasonGetDriverByPosition(Season season, int position,
                                 SeasonStatus* status);
static Driver* DriverArrayAllocation(Season season,
                                     char* season_info_copy ,Team* teams_array);
static Team* TeamArrayAllocation(Season season,char* season_info_copy);
static int* SeasonLastRaceResultsArrayAllocation(Season season,
                                                 char* season_info_copy);
static void SetDriverSeasonResources(char* driver_name, Driver* drivers_array,
                                     Team* team_array, int* id,DriverStatus* status, int* driver_index,
                                     int* team_index, Season season, char* season_info_copy);



/** End of declarations*/

struct season {
    int year;
    int number_of_teams;
    Team* team_array;
    int number_of_drivers;
    Driver* drivers_array;
    int* last_race_results_array;
};

/**
 ***** Function:SeasonAddResult****
 * Adding race results to all drivers in the current season.
 * @param season - A pointer to a season.
 * @param results - An array with results of a race.
 * @return Success/fail +reason of the function.
 */
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
/**
 ***** Function: SeasonGetDriversStandings*****
 * Sorts the drivers by their position according to the points gained till the function is called.
 * @param season - a pointer to a season.
 * @return Succses/fail + reason of the failure(if failed).
 */
Driver* SeasonGetDriversStandings(Season season){
    assert(season!=NULL);
    Driver* drivers_standings =
            malloc(sizeof(*drivers_standings)*season->number_of_drivers);
    if(drivers_standings == NULL){
        return NULL;
    }
    int* drivers_points_array =
            malloc(sizeof(*drivers_points_array)*season->number_of_drivers);
    if(drivers_points_array == NULL){
        free(drivers_standings);
        return NULL;
    }
    /* drivers_points_array will hold the points of the drivers.
     * drivers_points_array[i] will contain the number of points of
     * the driver i.  */
    DriversArrayToPointsArray(drivers_points_array,
                              season->drivers_array, season->number_of_drivers);
    /* Sorting the drivers by points. The driver with the highest score
     * will be stored at driver_standings[0], and so on.*/
    int driver_index;
    for(int i=0;i<season->number_of_drivers;i++){
        /* driver_index is holding the index of the driver who
         * has the greatest score. */
        driver_index = FindIndexOfMaxPointsDriver
                (season,drivers_points_array,season->number_of_drivers);
        drivers_standings[i] = season->drivers_array[driver_index];
    }
    free(drivers_points_array);
    return drivers_standings;
}
/**
 ***** Function:DriversArrayToPointsArray *****
 * Adding each driver's points to a given array.
 * @param drivers_points - An array of points of drivers. driver_points[i] contaions points of driver i.
 * @param drivers_array - A pointer to a driver array.
 * @param number_of_drivers - Number of drivers.
 */
static void DriversArrayToPointsArray(int *drivers_points,
                                      Driver *drivers_array, int number_of_drivers){
    assert(drivers_array!=NULL && drivers_points!=NULL);
    DriverStatus status;
    int points_of_driver_i;
    for(int i=0;i<number_of_drivers;i++){
        points_of_driver_i = DriverGetPoints(drivers_array[i],&status);
        if (status == DRIVER_STATUS_OK){
            drivers_points[i] = points_of_driver_i;
        }
    }
}

/**
 ***** Function:FindIndexOfMaxPointsDriver *****
 * Finds the driver with the highest number of points. In case there are 2 drivers
 * with equal number of points checks which driver got a better position in the last
 * race and returns its index.
 * @param season - A pointer to a season.
 * @param points - A pointer to an array of dirvers points array.
 * @param number_of_drivers - Number of drivers.
 * @return Index of the driver with max points.
 */
static int FindIndexOfMaxPointsDriver
        (Season season, int* points, int number_of_drivers){
    assert(season!=NULL && points!=NULL);
    int max = points[0];
    int index_of_max = 0;
    for(int i=1;i<number_of_drivers;i++){
        if(points[i]>max){
            max = points[i];
            index_of_max = i;
        }
        else if(points[i] == max){
            if(FindLastPositionById(season,i+1)<
               FindLastPositionById(season,index_of_max+1)){
                index_of_max = i;
            }
        }
    }
    points[index_of_max] = -1;
    return index_of_max;
}

/**
 * Function: FindLastPositionById *****
 * Finds the driver's position in the last race by his id.
 * @param season - A pointer to a season.
 * @param id
 * @return
 */
static int FindLastPositionById(Season season, int id){
    assert(season!=NULL);
    for(int i=0;i<season->number_of_drivers;i++){
        if(season->last_race_results_array[i] == id)
            return i+1;
    }
    return 0;
}

/**
 ***** Function: SeasonCreate *****
 * Creates a new season.
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
    Team* teams_array =TeamArrayAllocation(new_season,season_info_copy);
    if (teams_array == NULL){ //If allocation fails frees all the allocated elements.
        return NULL;
    }
    Driver* drivers_array = DriverArrayAllocation
            (new_season,season_info_copy,teams_array);
    if (drivers_array == NULL){
        return NULL;
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
            SetDriverSeasonResources(line,drivers_array,teams_array,&id,
                                     &driver_creation_status,&drivers_index, &teams_index,new_season,season_info_copy);
            if(driver_creation_status == DRIVER_MEMORY_ERROR){ //If allocation fails frees all the allocated elements.
                return NULL;
            }
        }
        line = strtok(NULL,"\n");
    }
    new_season->team_array = teams_array;
    new_season->drivers_array = drivers_array;
    new_season->last_race_results_array =
            SeasonLastRaceResultsArrayAllocation(new_season,season_info_copy);
    if(new_season->last_race_results_array == NULL) {
        DriverAndTeamArrayDestroy(drivers_array, teams_array,
                                  new_season->number_of_drivers, new_season->number_of_teams);
    }
    free(season_info_copy);
    return new_season;
}

static void SetDriverSeasonResources(char* driver_name, Driver* drivers_array,
                                     Team* team_array, int* id,DriverStatus* status, int* driver_index,
                                     int* team_index, Season season, char* season_info_copy){
    drivers_array[(*driver_index)++] = DriverCreate
            (status,driver_name,(*id)++);
    if(*status == DRIVER_MEMORY_ERROR){ //If allocation fails frees all the allocated elements.
        DriverAndTeamArrayDestroy(drivers_array,team_array,
                                  season->number_of_drivers,season->number_of_teams);
        free(season);
        free(season_info_copy);
        return;
    }
    DriverSetSeason(drivers_array[(*driver_index)-1],season); // Adding the driver to the season.
    DriverSetTeam(drivers_array[(*driver_index)-1],team_array[(*team_index)-1]); // Adding the driver to the team.
    TeamAddDriver(team_array[(*team_index)-1],drivers_array[(*driver_index)-1]);
}



/**
 ***** Function: DriversAndTeamsCounter *****
 * Counts the number of drivers and teams in the given string.
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

/**
 ***** Function:DriverAndTeamArrayDestroy *****
 * Destroys all the allocated memory of all the drivers and all the teams.
 * @param driver_array - A pointer to drivers array.
 * @param team_array - A pointer to teams array.
 * @param number_of_drivers - Number of drivers.
 * @param number_of_teams - Number of teams.
 */
static void DriverAndTeamArrayDestroy (Driver* driver_array, Team* team_array,
                                       int number_of_drivers, int number_of_teams) {
    assert(driver_array!=NULL && team_array!=NULL);
    for (int j = 0; j < number_of_teams; j++) {
        TeamDestroy(team_array[j]);
    }
    free(driver_array);
    free(team_array);
}

/**
 ***** Function: SeasonDestroy *****
 * Destroys all allocated memory of a season.
 * Description: freeing all allocated memory of season including all the
 * elements in array: drivers, teams and their pointers.
 * @param season - A pointer to a season.
 */
void SeasonDestroy(Season season) {
    DriverAndTeamArrayDestroy(season->drivers_array, season->team_array,
                              season->number_of_drivers,season->number_of_teams);
    free(season->last_race_results_array);
    free(season);
}

/**
 ***** Function: SeasonGetNumberOfDriver *****
 * Gets the number of drivers in a given season.
 * @param season - A pointer to a season.
 * @return
 */
int SeasonGetNumberOfDrivers(Season season){
    assert(season!=NULL);
    return season->number_of_drivers;
}

/**
 ***** Function : DriverIsNone *****
 * Checks if a new driver should be created.
 * @param name - A pointer to a driver's name.
 * @param source - A pointer to the string "None".
 * @return true if the driver's name is None else return false.
 */
static bool DriverIsNone(char* name, char* source ){
    assert(name!=NULL && source!=NULL);
    while (*name || *source){
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

/**
 ***** Function: SeasonGetNumberOfTeams *****
 * Gets the number of teams in the current season.
 * @param season - a pointer to a season.
 * @return number of teams of the season.
 */
int SeasonGetNumberOfTeams(Season season){
    assert(season!=NULL);
    return season->number_of_teams;
}

/**
 ***** Function : SeasonGetTeamStandings *****
 * Sorting the teams in the season by their points.
 * @param season - A pointer to a season.
 * @return a pointer to a sorted team array.
 */
Team* SeasonGetTeamsStandings(Season season){
    assert(season!=NULL);
    TeamStatus status;
    int index_of_current_winning_team=0;
    Team* sorted_team_array=
            malloc(sizeof(*sorted_team_array)*season->number_of_teams);
    if(sorted_team_array==NULL){
        return NULL;
    }
    int* team_points_array=
            malloc(sizeof(team_points_array)*season->number_of_teams);
    if(team_points_array==NULL){
        free(sorted_team_array);
        return NULL;
    }
    // Team points array will contain in index i the number of points of team
    //i in the teams array found in the season.
    for (int i=0;i<(season->number_of_teams);i++) {
        team_points_array[i]=TeamGetPoints(season->team_array[i],&status);
        if (status==TEAM_NULL_PTR){
            free(sorted_team_array);
            free(team_points_array);
            return NULL;
        }
    }
    for (int j=0;j<season->number_of_teams;j++) {
        index_of_current_winning_team=FindCurrentWinningTeam(
                season,team_points_array,season->number_of_teams);
        sorted_team_array[j]=season->team_array[index_of_current_winning_team];
    }
    free(team_points_array);
    return sorted_team_array;
}

/**
 ***** Function : FindCurrentWinningTeam *****
 * Finds the index of the team with the highest points. If two teams has equal number
 * of points checks which of the team's driver's position was highest and returns the
 * index of this team.
 * @param season - A pointer to a season.
 * @param points - A pointer to an array which contains the teams points.
 * @param number_of_teams - Number of teams in the season.
 * @return Winning team index.
 */
static int FindCurrentWinningTeam
        (Season season, int* points, int number_of_teams){
    assert(season!=NULL && points!=NULL);
    int winning_team_index=0;
    int max_team_points=points[0];
    for (int i=1;i<number_of_teams;i++) {
        if (points[i] > max_team_points) {
            max_team_points = points[i];
            winning_team_index = i;
        }
        else if (points[i] == max_team_points) {
            if (FindBestTeamDriverPosition(season,season->team_array[i]) <
                FindBestTeamDriverPosition(season,season->team_array[winning_team_index])) {
                winning_team_index = i;
            }
        }
    }
    points[winning_team_index]=-1;
    return winning_team_index;
}

/**
 ***** Function: FindBestTeamDriverPosition *****
 * Checks which of each team's drivers has the best position.
 * @param season - A pointer to a season.
 * @param team - A pointer to a team.
 * @return the position of the best team's driver.
 */
static int FindBestTeamDriverPosition (Season season,Team team){
    assert(season!=NULL && team!=NULL);
    int first_driver_position,second_driver_position;
    first_driver_position=
            (FindLastPositionById(season,DriverGetId(TeamGetDriver(team,FIRST_DRIVER))));
    second_driver_position=
            (FindLastPositionById(season,DriverGetId(TeamGetDriver(team,SECOND_DRIVER))));
    if(first_driver_position<second_driver_position){
        return first_driver_position;
    }
    return second_driver_position;
}

/**
 ***** Function : SeasonGetTeamByPosition *****
 * Returns a team pointer by it's rank.
 * @param season - a pointer to a season.
 * @param position - a position in the sorted teams array.
 * @param status - will hold success or failure.
 * @return a pointer to a team by its position in the sorted team array.
 */
Team SeasonGetTeamByPosition(Season season, int position, SeasonStatus* status){
    assert(season!=NULL && status!=NULL);
    if(position<1 || position>season->number_of_teams){
        *status=SEASON_NULL_PTR; //Check if legal, because its not supposed to be null ptr
        return NULL;
    }
    Team* sorted_team_array=SeasonGetTeamsStandings(season);
    return sorted_team_array[position-1];
}

/**
 ***** Function : SeasonGetDriverByPosition *****
 * Returns a drivers pointer by it's rank.
 * @param season - a pointer to season.
 * @param position - a position in the sorted drivers array.
 * @param status - will hold success or failure.
 * @return a pointer to a driver by its position in the sorted drivers array.
 */
Driver SeasonGetDriverByPosition(Season season, int position, SeasonStatus* status){
    assert(season!=NULL && status!=NULL);
    if(position<1 || position>season->number_of_drivers){
        *status=SEASON_NULL_PTR; //Check if legal, because its not supposed to be null ptr
        return NULL;
    }
    Driver* sorted_driver_array=SeasonGetDriversStandings(season);
    return sorted_driver_array[position-1];
}
/**
 ***** Function : DriverArrayAllocation *****
 * Allocates memory for the drivers array according to the number of drivers
 * and sets it to NULL.
 * @param season - A pointer to a season.
 * @param season_info_copy - A pointer to the season's info copy string.
 * @param teams_array - A pointer to the array of the teams in the current season.
 * @return a pointer to the allocated drivers array or NULL in case of memory allocation error.
 */
static Driver* DriverArrayAllocation
        (Season season,char* season_info_copy ,Team* teams_array){
    assert(season!=NULL && season_info_copy!=NULL && teams_array!=NULL);
    Driver* drivers_array =
            malloc(sizeof(*drivers_array)*season->number_of_drivers);
    if (drivers_array == NULL){    //If allocation fails frees all the allocated elements.
        free(season);
        free(season_info_copy);
        free(teams_array);
        return NULL;
    }
    for (int j=0;j<season->number_of_drivers;j++) { //Setting array pointers to NULL which helps if
        drivers_array[j] = NULL;                           //memory allocation fails.
    }
    return drivers_array;
}

/**
 ***** Function: TeamArrayAllocation *****
 * Allocates memory to the teams array according to the number of teams in the season,
 * and sets it to NULL.
 * @param season - a pointer to a season.
 * @param season_info_copy - A pointer to the season's info copy string.
 * @return a pointer to the allocated teams array or NULL in case of memory allocation error.
 */
static Team* TeamArrayAllocation(Season season,char* season_info_copy) {
    assert(season!=NULL && season_info_copy!=NULL);
    Team *teams_array = malloc(sizeof(*teams_array)*(season->number_of_teams));
    if (teams_array == NULL) {
        free(season);
        free(season_info_copy);
    }
    for (int i=0;i<season->number_of_teams;i++) {
        teams_array[i] = NULL;
    }
    return teams_array;
}

/**
 ***** Function : SeasonLastRaceResultsArrayAllocation *****
 * Allocates memory according to the number of drivers in the season which will
 * contain the last race results.
 * @param season - A pointer to a season.
 * @param season_info_copy - A pointer to the season's info copy string.
 * @return a pointer to the allocated results array or NULL in case of memory allocation error.
 */
static int* SeasonLastRaceResultsArrayAllocation(Season season,char* season_info_copy){
    assert(season!=NULL && season_info_copy!=NULL);
    int* last_race_results_array =
            malloc(sizeof(*last_race_results_array)*season->number_of_drivers);
    if(last_race_results_array == NULL) {
        free(season_info_copy);
        free(season);
    }
    return last_race_results_array;
}