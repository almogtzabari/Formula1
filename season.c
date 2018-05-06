#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "driver.h"
#include <stdlib.h>



/** Declarations */
static void DriversAndTeamsCounter(int* drivers, int* teams,
                                   const char* details,SeasonStatus* status);
static bool DriverIsNone(char* name, char* source );
static void DriversArrayToPointsArray(int *drivers_points,
                                      Driver *drivers_array, int number_of_drivers);
static int FindIndexOfMaxPointsDriver(Season season,
                                      int* points, int number_of_drivers);
static int FindLastPositionById(Season season, int id);
static int FindCurrentWinningTeam (Season season, int* points,
                                   int number_of_teams);
static int FindBestTeamDriverPosition (Season season,Team team);
static Driver* DriverArrayAllocation(Season season,Team* teams_array);
static Team* TeamArrayAllocation(Season season);
static int* SeasonLastRaceResultsArrayAllocation(Season season);
static void SetDriversInSeason(char* driver_name, Driver* drivers_array,
                                     Team* team_array, int* id,DriverStatus* status, int* driver_index,
                                     int* team_index, Season season, char* season_info_copy);
static void SeasonDriversAndTeamsCreation (const char* season_info,
                                           SeasonStatus* status,Season season);
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
 * Description: adding race results to all drivers in the current season.
 * @param season - A pointer to a season.
 * @param results - An array with results of a race.
 * @return - Success/fail +reason of the function.
 */
SeasonStatus SeasonAddRaceResult(Season season, int* results){
    if (season==NULL || results==NULL){ // Checks if season or results is NULL.
        return SEASON_NULL_PTR;
    }
    for (int i=0;i<SeasonGetNumberOfDrivers(season);i++) {
        /* Add points to each driver by it's id and position in race. */
        DriverAddRaceResult(season->drivers_array[results[i]-1],i+1);
        season->last_race_results_array[i] = results[i];  // Copies the last race results.
    }
    return SEASON_OK;
}

/**
 ***** Function: SeasonGetDriversStandings*****
 * Description: sorts the drivers by their position according to the points gained till the function is called.
 * @param season - a pointer to a season.
 * @return - Succses/fail + reason of the failure(if failed).
 */
Driver* SeasonGetDriversStandings(Season season){
    if (season==NULL){
        return NULL;
    }
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
 ***** Function: SeasonCreate *****
 * Description: creates a new season.
 * @param status - Will hold success/fail of the function.
 * @param season_info - String containing input of teams and drivers.
 * @return - A pointer to the season.
 */
Season SeasonCreate (SeasonStatus* status,const char* season_info){
    if (season_info==NULL){
        if(status!=NULL){
            *status=BAD_SEASON_INFO;
        }
        return NULL;
    }
    SeasonStatus season_allocation_status;
    Season new_season = malloc(sizeof(*new_season));
    if(new_season == NULL){
        if(status!=NULL) {
            *status = SEASON_MEMORY_ERROR;
        }
        return NULL;
    }
    /* Counts the number of teams and drivers in the season */
    DriversAndTeamsCounter(&new_season->number_of_drivers,
                           &new_season->number_of_teams,season_info,&season_allocation_status);
    if ((season_allocation_status==SEASON_MEMORY_ERROR )){
        if (status!=NULL){
            *status=season_allocation_status;
        }
        free(new_season);
        return NULL;
    }
    new_season->team_array = TeamArrayAllocation(new_season);
    if (new_season->team_array == NULL){ //If allocation fails frees all the allocated elements.
        if (status!=NULL){
            *status=SEASON_MEMORY_ERROR;
        }
        return NULL;
    }
    new_season->drivers_array = DriverArrayAllocation
            (new_season,new_season->team_array);
    if (new_season->drivers_array == NULL){
        if (status!=NULL){
            *status=SEASON_MEMORY_ERROR;
        }
        return NULL;
    }
    /* If we got here we can create teams and drivers */
    SeasonDriversAndTeamsCreation(season_info,
                                  &season_allocation_status,new_season);
            if(season_allocation_status==SEASON_MEMORY_ERROR){
                return NULL;
    }
    new_season->last_race_results_array =
            SeasonLastRaceResultsArrayAllocation(new_season);
    if(new_season->last_race_results_array == NULL) {
        SeasonDestroy(new_season);
        return NULL;
    }
    if(status!=NULL){
        *status=season_allocation_status;
    }
    return new_season;
}

/**
 ***** Function: SeasonDestroy *****
 * Description: freeing all allocated memory of season including all the
 * elements in array: drivers, teams and their pointers.
 * @param season - A pointer to a season.
 */
void SeasonDestroy(Season season) {
    if(season==NULL){
        return;
    }
    for (int j = 0; j < season->number_of_teams; j++) {
        TeamDestroy((season->team_array)[j]); // Destroys all teams and their drivers.
    }
    free(season->drivers_array);
    free(season->team_array);
    free(season->last_race_results_array);
    free(season);
}

/**
 ***** Function: SeasonGetNumberOfDrivers *****
 * Description: gets the number of drivers in a given season.
 * @param season - A pointer to a season.
 * @return - Number of drivers in the season.
 */
int SeasonGetNumberOfDrivers(Season season){
    if (season==NULL){
        return 0;
    }
    return season->number_of_drivers;
}

/**
 ***** Function: SeasonGetNumberOfTeams *****
 * Description: gets the number of teams in the current season.
 * @param season - a pointer to a season.
 * @return number of teams of the season.
 */
int SeasonGetNumberOfTeams(Season season){
    if(season==NULL){
        return 0;
    }
    return season->number_of_teams;
}

/**
 ***** Function : SeasonGetTeamStandings *****
 *Description: sorting the teams in the season by their points.
 * @param season - A pointer to a season.
 * @return - A pointer to a sorted team array.
 */
Team* SeasonGetTeamsStandings(Season season){
    if(season==NULL){
        return NULL;
    }
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
    /* Team points array will contain in index i the number of points of team
      i in the teams array found in the season. */
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
        sorted_team_array[j]=
                season->team_array[index_of_current_winning_team];
    }
    free(team_points_array);
    return sorted_team_array;
}

/**
 ***** Function : SeasonGetTeamByPosition *****
 * Description: returns a team pointer by it's rank.
 * @param season - A pointer to a season.
 * @param position - A position in the sorted teams array.
 * @param status - Will hold success or failure.
 * @return - A pointer to a team by its position in the sorted team array.
 */
Team SeasonGetTeamByPosition(Season season, int position,
                             SeasonStatus* status){
    if (season==NULL){
        if(status!=NULL){ // Updates status only if it is not a NULL pointer.
            *status=SEASON_NULL_PTR;
        }
        return NULL;
    }
    if(position<1 || position>season->number_of_teams){
        if(status!=NULL){
            *status=SEASON_NULL_PTR;
        }
        return NULL;
    }
    /* Sorted_team_array will hold a new (new pointer) array sorted by each team's points */
    Team* sorted_team_array=SeasonGetTeamsStandings(season);
    if(status!=NULL){
        *status=SEASON_OK;
    }
    Team team=sorted_team_array[position-1];
    /* Searches for the wanted team in the team array found in season
         by comparing teams names. */
    for (int i=0;i<season->number_of_teams;i++) {
        if(!strcmp(TeamGetName(team),TeamGetName(season->team_array[i]))){
            team=season->team_array[i];
            free(sorted_team_array);
            return team;
        }
    }
    return NULL; // Shouldn't get here.
}

/**
 ***** Function : SeasonGetDriverByPosition *****
 * Description: returns a drivers pointer by it's rank.
 * @param season - A pointer to season.
 * @param position - A position in the sorted drivers array.
 * @param status - Will hold success or failure.
 * @return - A pointer to a driver by its position in the sorted drivers array.
 */
Driver SeasonGetDriverByPosition(Season season, int position,
                                 SeasonStatus* status){
    if (season==NULL){
        if(status!=NULL){
            *status=SEASON_NULL_PTR;
        }
        return NULL;
    }
    if(position<1 || position>season->number_of_drivers){
        *status=SEASON_NULL_PTR;
        return NULL;
    }
    /* Sorted_driver_array will hold a new (new pointer) array sorted by each driver's points */
    Driver* sorted_driver_array=SeasonGetDriversStandings(season);
    int id = DriverGetId(sorted_driver_array[position-1]);
    /* Searches for the wanted driver in the driver array found in season
         by comparing drivers ids. */
    free(sorted_driver_array);
    for(int i=0;i<season->number_of_drivers;i++){
        if(DriverGetId(season->drivers_array[i])==id){
            if(status!=NULL){
                *status=SEASON_OK;
            }
            return season->drivers_array[i];
        }
    }
    return NULL; //Shouldn't get here.
}

/** Static functions */
/**
 ***** Static function : DriverArrayAllocation *****
 *  Description: allocates memory for the drivers array according to the number of drivers
 * and sets it's elements to NULL.
 * @param season - A pointer to a season.
 * @param season_info_copy - A pointer to the season's info copy string.
 * @param teams_array - A pointer to the array of the teams in the current season.
 * @return - A pointer to the allocated drivers array or NULL in case of memory allocation error.
 */
static Driver* DriverArrayAllocation(Season season ,Team* teams_array){
    assert(season!=NULL && teams_array!=NULL);
    Driver* drivers_array =
            malloc(sizeof(*drivers_array)*season->number_of_drivers);
    if (drivers_array == NULL){    //If allocation fails frees all the allocated elements.
        free(season);
        free(teams_array);
        return NULL;
    }
    for (int j=0;j<season->number_of_drivers;j++) { //Setting array pointers to NULL which helps if
        drivers_array[j] = NULL;                           //memory allocation fails.
    }
    return drivers_array;
}

/**
 ***** Static function: TeamArrayAllocation *****
 * Description: allocates memory to the teams array according to the number of teams in the season,
 * and sets it's elements to NULL.
 * @param season - A pointer to a season.
 * @param season_info_copy - A pointer to the season's info copy string.
 * @return - A pointer to the allocated teams array or NULL in case of memory allocation error.
 */
static Team* TeamArrayAllocation(Season season) {
    assert(season!=NULL);
    Team *teams_array =
            malloc(sizeof(*teams_array)*(season->number_of_teams));
    if (teams_array == NULL) {
        free(season);
    }
    for (int i=0;i<season->number_of_teams;i++) {
        teams_array[i] = NULL;
    }
    return teams_array;
}

/**
 ***** Static Function : SeasonLastRaceResultsArrayAllocation *****
 * Description: allocates memory according to the number of drivers in the season which will
 * contain the last race results.
 * @param season - A pointer to a season.
 * @param season_info_copy - A pointer to the season's info copy string.
 * @return - A pointer to the allocated results array or NULL in case of memory allocation error.
 */
static int* SeasonLastRaceResultsArrayAllocation(Season season){
    assert(season!=NULL);
    int* last_race_results_array =
            malloc(sizeof(*last_race_results_array)*season->number_of_drivers);
    return last_race_results_array;
}

/**
 ***** Static function:DriversArrayToPointsArray *****
 * Description: adding each driver's points to a given array.
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
 ***** Static function: FindBestTeamDriverPosition *****
 * Description: checks which of each team's drivers has the best position.
 * @param season - A pointer to a season.
 * @param team - A pointer to a team.
 * @return - The position of the best team's driver.
 */
static int FindBestTeamDriverPosition (Season season,Team team){
    assert(season!=NULL && team!=NULL);
    int first_driver_position,second_driver_position;
    /* Gets the position of each driver found in the team*/
    first_driver_position=
            (FindLastPositionById(season,DriverGetId
                    (TeamGetDriver(team,FIRST_DRIVER))));
    second_driver_position=
            (FindLastPositionById(season,DriverGetId(
                    TeamGetDriver(team,SECOND_DRIVER))));
    /* Returns the driver that finished the last race first */
    if(first_driver_position<second_driver_position){
        return first_driver_position;
    }
    return second_driver_position;
}

/**
 ***** Static function:FindIndexOfMaxPointsDriver *****
 * Description: finds the driver with the highest number of points. In case there are 2 drivers
 * with equal number of points checks which driver got a better position in the last
 * race and returns its index.
 * @param season - A pointer to a season.
 * @param points - A pointer to an array of dirvers points array.
 * @param number_of_drivers - Number of drivers.
 * @return - Index of the driver with max points.
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
            /* If two drivers has an equal number of points, compares their
               position in the last race */
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
 * Static function: FindLastPositionById *****
 * Description: finds the driver's position in the last race by his id.
 * @param season - A pointer to a season.
 * @param id - A driver's id.
 * @return - The position of the driver in the last race.
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
 ***** Static function : SeasonDriversAndTeamsCreation *****
 * Description: assings the year to a season and creates drivers and teams
 * and assigns them to teams and drivers array.
 * @param season_info - A pointer to a string which contains season info.
 * @param status - Status of the function which holds success or failure.
 * @param season - A pointer to a season.
 */
static void SeasonDriversAndTeamsCreation (const char* season_info,
                                           SeasonStatus* status,Season season){
    /* drivers_index and team_index represent the indexes in which a driver\team
       should be added. Line number counts the number of lines starting from the
       first line which a team's name appears (right after line of the year) */
    int drivers_index=0, teams_index=0, id=1, line_number=0;
    TeamStatus team_creation_status;
    DriverStatus driver_creation_status;
    char* season_info_copy = malloc((strlen(season_info))+1);
    if(season_info_copy == NULL){ //If allocation fails frees all the allocated elements.
        if (status!=NULL) {
            *status = SEASON_MEMORY_ERROR;
        }
        SeasonDestroy(season);
        return;
    }
    strcpy(season_info_copy,season_info);
    char* line = strtok(season_info_copy,"\n");
    season->year = atoi(line); // converting first line(string of year) to int.
    line = strtok(NULL,"\n");
    while(line != NULL){
        if(line_number++%3 == 0){   //Checks if the current line is a team name.
            season->team_array[teams_index++] =
                    TeamCreate(&team_creation_status,line);
            if (team_creation_status == TEAM_MEMORY_ERROR){ //If allocation fails frees all the allocated elements.
                SeasonDestroy(season);
                free(season_info_copy);
                return;
            }
        }
        else if(!DriverIsNone(line,"None")){  //Checks if the current line is a valid driver name.
            SetDriversInSeason(line,season->drivers_array,season->team_array,
                               &id,&driver_creation_status,&drivers_index,&teams_index,season,
                               season_info_copy);
            if(driver_creation_status == DRIVER_MEMORY_ERROR){ //If allocation fails frees all the allocated elements.
                return;
            }
        }
        line = strtok(NULL,"\n"); //Line will now hold the next line.
    }
    free(season_info_copy);
}

/**
 ***** Static Function : SetDriversInSeason *****
 * Description: creates drivers, putting them in drivers array, sets their season, adding
 * them to their team and setting them to their team.
 * @param driver_name - Driver's name.
 * @param drivers_array - A pointer to drivers array.
 * @param team_array - A pointer to teams array.
 * @param id - Id of driver.
 * @param status - Driver creation status.
 * @param driver_index - Position in driver array.
 * @param team_index - Position in team array.
 * @param season - A pointer to season.
 * @param season_info_copy - Copy of season info string.
 */
static void SetDriversInSeason(char* driver_name, Driver* drivers_array,
                               Team* team_array, int* id,DriverStatus* status, int* driver_index,
                               int* team_index, Season season, char* season_info_copy){
    drivers_array[(*driver_index)++] = DriverCreate
            (status,driver_name,(*id)++);
    if(*status == DRIVER_MEMORY_ERROR){ //If allocation fails frees all the allocated elements.
        SeasonDestroy(season);
        free(season_info_copy);
        return;
    }
    DriverSetSeason(drivers_array[(*driver_index)-1],season); // Adding the driver to the season.
    if(TeamAddDriver(team_array[(*team_index)-1],
                     drivers_array[(*driver_index)-1]) == TEAM_STATUS_OK){ // Driver successfully added to team.
        DriverSetTeam(drivers_array[(*driver_index)-1],
                      team_array[(*team_index)-1]); // Setting driver team.
    }
}

/**
 ***** Static Function: DriversAndTeamsCounter *****
 * Description: counts the number of drivers and teams in the given string.
 * @param drivers - Will hold the number of drivers.
 * @param teams - Will hold the number of teams.
 * @param details - String input that contains the teams and drivers.
 * @param status - Success/fail.
 */
static void DriversAndTeamsCounter(int* drivers, int* teams,
                                   const char* details,SeasonStatus* status){
    assert(drivers!=NULL && teams!=NULL && details!=NULL);
    int number_of_drivers=0, number_of_teams=0;
    int line_number=0;
    char* season_details_copy=malloc((strlen(details))+1);
    if(season_details_copy==NULL){
        *status = SEASON_MEMORY_ERROR;
        return;
    }
    strcpy(season_details_copy,details);
    char* line = strtok(season_details_copy,"\n");
    /* Skips the first line which contains the year of the season. */
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
    *status = SEASON_OK;
}

/**
 ***** Static function : DriverIsNone *****
 * Description: checks if a new driver should be created.
 * @param name - A pointer to a driver's name.
 * @param source - A pointer to the string "None".
 * @return - True if the driver's name is None else return false.
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
 ***** Static Function : FindCurrentWinningTeam *****
 * Description: finds the index of the team with the highest points. If two teams has equal number
 * of points checks which of the team's driver's position was highest and returns the
 * index of this team.
 * @param season - A pointer to a season.
 * @param points - A pointer to an array which contains the teams points.
 * @param number_of_teams - Number of teams in the season.
 * @return - Winning team's index.
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
            /* If two teams has equal number of points checks which
               of the team's best drivers has a better position in
               the last race */
            if (FindBestTeamDriverPosition(season,season->team_array[i]) <
                FindBestTeamDriverPosition
                        (season,season->team_array[winning_team_index])) {
                winning_team_index = i;
            }
        }
    }
    points[winning_team_index]=-1;
    return winning_team_index;
}

/** End of static functions */