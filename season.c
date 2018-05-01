#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "driver.h"


/** Declarations */
static void mergeSort(Driver* drivers_array, int l, int r);
static void merge(Driver* drivers_array, int l, int m, int r);
static void driversAndTeamsCounter(int* drivers, int* teams, const char* details,SeasonStatus* status);
static void driverAndTeamArrayDestroy (Driver* driver_array, Team* team_array,
                                       int number_of_drivers, int number_of_teams);
void SeasonDestroy(Season season);
int SeasonGetNumberOfDrivers(Season season);
static bool DriverIsNone(char* name, char* source );

/** End of declarations*/

typedef struct season {
    int year;
    int number_of_teams;
    Team* team_array;
    int number_of_drivers;
    Driver* drivers_array;
} *Season;

/**
 ****** Function: SeasonGetDriverByPosition *****
 * @param season - Pointer to a season.
 * @param position - A number in the rank range.
 * @param status - Will hold success/fail of the function.
 * @return A pointer to the desired driver.
 */
Driver SeasonGetDriverByPosition(Season season, int position, SeasonStatus* status){
    /** Checks if the position is legal. */
    if(position<1 || position>(season->number_of_drivers)) {
        *status = INVALID_POSITION;
        return NULL;
    }
    /** Sorting drivers array of 'season' from lowest to highest rank
     * and returns a pointer to the array. */
    Driver* driver_array = SeasonGetDriversStandings(season);
    return driver_array[(season->number_of_drivers)-position];
}

/**
 ***** Function: SeasonGetDriverStandings *****
 * @param season - Pointer to a season.
 * @return A sorted array of drivers in the given season.
 */
Driver* SeasonGetDriversStandings(Season season){
    mergeSort(season->drivers_array, 0, season->number_of_drivers-1);
    return season->drivers_array;
}

/** Merge Sort */
/* TODO: check if the sort is okay. */
static void merge(Driver* drivers_array, int l, int m, int r)
{
    int i, j, k;
    DriverStatus status;
    int n1 = m - l + 1;
    int n2 =  r - m;

    /** create temp arrays */
    Driver* L = malloc(sizeof(Driver)*n1);
    Driver* R = malloc(sizeof(Driver)*n2);
    /* Copy data to temp arrays L and R */
    for (i = 0; i < n1; i++)
        L[i] = drivers_array[l + i];
    for (j = 0; j < n2; j++)
        R[j] = drivers_array[m + 1+ j];

    /** Merge the temp arrays back into drivers_array. */
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2)
    {
        if (DriverGetPoints(L[i],&status) <= DriverGetPoints(R[j],&status))
        {
            drivers_array[k] = L[i];
            i++;
        }
        else
        {
            drivers_array[k] = R[j];
            j++;
        }
        k++;
    }

    /* Copy the remaining elements of L[], if there
       are any */
    while (i < n1)
    {
        drivers_array[k] = L[i];
        i++;
        k++;
    }

    /* Copy the remaining elements of R[], if there
       are any */
    while (j < n2)
    {
        drivers_array[k] = R[j];
        j++;
        k++;
    }
    free(L);
    free(R);
}

/* l is for left index and r is right index of the
   sub-array of drivers_array to be sorted */
static void mergeSort(Driver* drivers_array, int l, int r)
{
    if (l < r)
    {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l+(r-l)/2;

        // Sort first and second halves
        mergeSort(drivers_array, l, m);
        mergeSort(drivers_array, m+1, r);
        merge(drivers_array, l, m, r);
    }
}
/** End of merge sort*/

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
    driversAndTeamsCounter(&new_season->number_of_drivers,
                           &new_season->number_of_teams,season_info,status);
    if (*status==SEASON_MEMORY_ERROR){
        free(new_season);
        return NULL;
    }
    int temp=strlen(season_info);
    char* season_info_copy=malloc((strlen(season_info))+1);
    if(season_info_copy==NULL){
        *status=SEASON_MEMORY_ERROR;
        free(new_season);
        return NULL;
    }
    Team* team_array=malloc(sizeof(*team_array)*(new_season->number_of_teams));
    if (team_array==NULL){
        free(new_season);
        free(season_info_copy);
        return NULL;
    }
    for (int i=0;i<new_season->number_of_teams;i++) {
        team_array[i]=NULL;
    }
    Driver* driver_array=malloc(sizeof(*driver_array)*new_season->number_of_drivers);
    if (driver_array==NULL){
        free(new_season);
        free(season_info_copy);
        free(team_array);
        return NULL;
    }
    for (int j=0;j<new_season->number_of_drivers;j++) {
        driver_array[j]=NULL;
    }
    strcpy(season_info_copy,season_info);
    char* season_details=strtok(season_info_copy,"\n");
    new_season->year=atoi(season_details);
    season_details=strtok(NULL,"\n");
    while(season_details!=NULL){
        if(line_number%3==0){
            team_array[current_team++]=TeamCreate(&team_creation_status,season_details);
            if (team_creation_status==TEAM_MEMORY_ERROR){
                driverAndTeamArrayDestroy(driver_array,team_array,
                                          new_season->number_of_drivers,new_season->number_of_teams);
                free(new_season);
                free(season_info_copy);
                return NULL;
            }
        }
        else if(!DriverIsNone(season_details,"None")){
            driver_array[current_driver++]=DriverCreate(&driver_creation_status,season_details,id++);
            if(driver_creation_status==DRIVER_MEMORY_ERROR){
                driverAndTeamArrayDestroy(driver_array,team_array,
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



static void driversAndTeamsCounter(int* drivers, int* teams,
                                   const char* details,SeasonStatus* status){
    int number_of_drivers=0, number_of_teams=0;
    int line_number=0;
    char* season_details_copy=malloc((strlen(details))+1);
    if(season_details_copy==NULL){
        *status=SEASON_MEMORY_ERROR;
        return;
    }
    strcpy(season_details_copy,details);
    char* line=strtok(season_details_copy,"\n");
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

static void driverAndTeamArrayDestroy (Driver* driver_array, Team* team_array,
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


void SeasonDestroy(Season season) {
    driverAndTeamArrayDestroy(season->drivers_array, season->team_array,
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