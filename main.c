#include <stdio.h>
#include "team.h"
#include "season.h"
#include "driver.h"
#include <malloc.h>

/** Team declarations */
void TeamTesting();
void TeamPrint(Team team, TeamStatus team_status);
void TeamPrintAddDriverStatus(TeamStatus status);
void TeamPrintCreateStatus(TeamStatus status);
/** End of team declarations */

/** Driver declarations */
void DriverTesting();
void DriverPrint(Driver driver);
void DriverPrintCreateStatus(DriverStatus status);
/** End of driver declarations */

/** Season declarations */
void SeasonTesting();
void SeasonPrint(Season season);
/** End of season declarations */


int main() {
//    TeamTesting();
//    printf("------------------------------------------------------------");
//    DriverTesting();
//    printf("------------------------------------------------------------");
    SeasonTesting();
    return 0;
}


/***** Team testing functions *****/
/* Testing: TeamCreate, TeamGetPoints, TeamAddDriver,     */
void TeamTesting(){
    printf("\n\nStarting TeamTesting() in:\n3...\n2...\n1...\n\n");
    TeamStatus team_status;
    DriverStatus driver_status;
    printf("Creating first driver...\n");
    Driver first_driver = DriverCreate(&driver_status,"Yohan Kfir",3115445); // Creating first driver.
    DriverPrintCreateStatus(driver_status);
    printf("Creating second driver...\n");
    Driver second_driver = DriverCreate(&driver_status,"Moshe Daood",203548); // Creating second driver.
    DriverPrintCreateStatus(driver_status);
    printf("Creating third driver...\n");
    Driver third_driver = DriverCreate(&driver_status,"Ben Weinsteffen",789156); // Creating third driver.
    DriverPrintCreateStatus(driver_status);
    printf("Creating team...\n");
    Team team = TeamCreate(&team_status,"Ford"); // Creates a team.
    TeamPrintCreateStatus(team_status);
    printf("Adding first driver...\n");
    TeamStatus first_driver_status = TeamAddDriver(team,first_driver); // Adding first driver.
    TeamPrintAddDriverStatus(first_driver_status); // Add first driver to team success/fail.
    printf("Adding second driver...\n");
    TeamStatus second_driver_status = TeamAddDriver(team,second_driver); // Adding second driver.
    TeamPrintAddDriverStatus(second_driver_status); // Add second driver to team success/fail.
    printf("Adding third driver...\n");
    TeamStatus third_driver_status = TeamAddDriver(team,third_driver); // Attempt to add third driver to a team. Should fail.
    TeamPrintAddDriverStatus(third_driver_status); // Add third driver to team success/fail.
//    DriverDestroy(first_driver); // NOT SURE IF WORKING. Suppose to destroying first driver.
//    DriverDestroy(second_driver); // NOT SURE IF WORKING. Suppose to destroying second driver.
    printf("\nPrinting Team:\n\n");
    TeamPrint(team, team_status);
    printf("\nTeamTesting() is done\n");
}

/** Printing team details: name,points,drivers. */
void TeamPrint(Team team, TeamStatus team_status){
    DriverStatus driver_status;
    printf("Team Name: %s\n",TeamGetName(team));
    int team_points = TeamGetPoints(team,&team_status);
    printf("Team Points: %d\n", team_points);
    printf("Drivers:\n\n");
    DriverPrint(TeamGetDriver(team,FIRST_DRIVER));
    DriverPrint(TeamGetDriver(team,SECOND_DRIVER));
}

/** Printing if adding a driver to a team was a success/fail. */
void TeamPrintAddDriverStatus(TeamStatus status){
    switch(status){
        case TEAM_STATUS_OK: printf("Driver successfully added.\n\n"); break;
        case TEAM_MEMORY_ERROR: printf("Driver not added: memory error.\n\n"); break;
        case TEAM_FULL: printf("Driver not added: team is full.\n"); break;
        default: ; break;
    }
}

/** Printing if creating a team was a success/fail. */
void TeamPrintCreateStatus(TeamStatus status){
    switch(status){
        case TEAM_STATUS_OK: printf("Team successfully created.\n\n"); break;
        case TEAM_MEMORY_ERROR: printf("Team not created: Memory Error.\n\n"); break;
        default: ; break;
    }
}
/***** End of team testing functions *****/


/***** Driver testing functions *****/
/* Testing: DriverCreate, DriverGetName, DriverGetId, DriverGetPoints */
void DriverTesting() {
    printf("\n\nStarting DriverTesting() in:\n3...\n2...\n1...\n\n");
    DriverStatus driver_status;
    printf("Creating first driver...\n");
    Driver first_driver = DriverCreate(&driver_status,"Yohan Kfir",3115445); // Creating first driver.
    DriverPrintCreateStatus(driver_status);
    printf("Creating second driver...\n");
    Driver second_driver = DriverCreate(&driver_status,"Moshe Daood",203548); // Creating second driver.
    DriverPrintCreateStatus(driver_status);
    printf("Creating third driver...\n");
    Driver third_driver = DriverCreate(&driver_status,"Ben Weinsteffen",789156); // Creating third driver.
    DriverPrintCreateStatus(driver_status);
    printf("\nPrinting Drivers:\n\n");
    printf("First Driver:\n");
    DriverPrint(first_driver);
    printf("Second Driver:\n");
    DriverPrint(second_driver);
    printf("Third Driver:\n");
    DriverPrint(third_driver);
}

/** Printing driver details: name,id,points. */
void DriverPrint(Driver driver){
    DriverStatus status;
    printf("Driver Name: %s\n",DriverGetName(driver));
    printf("Driver ID: %d\n",DriverGetId(driver));
    int points = DriverGetPoints(driver, &status);
    if (status == DRIVER_STATUS_OK) {
        printf("Driver Points: %d\n\n", points);
    }
    else printf("Error calculating points\n\n");
}

/** Printing if creating a driver was a success/fail. */
void DriverPrintCreateStatus(DriverStatus status){
    switch(status){
        case DRIVER_STATUS_OK: printf("Driver successfully created.\n\n"); break;
        case DRIVER_MEMORY_ERROR: printf("Driver not created: memory error.\n\n"); break;
        case INVALID_DRIVER: printf("Driver not created: pointer is NULL.\n\n"); break;
        default: ; break;
    }
}
/***** End of driver testing functions *****/

/***** Season testing functions *****/
void SeasonTesting(){
    SeasonStatus season_status;
    char* string = "2018\nFerrari\nSebastian Vettel\nKimi Raikonen\nMercedes\nLewis Hamilton\nValtteri Bottas\nRedBull Racing\nDaniel\nMax Verstappen\nMcLaren\nFernando Alonso\nNone";
    Season season = SeasonCreate(&season_status,string);
    int test_results [7] = {2,3,1,5,4,7,6};
    SeasonAddRaceResult(season,test_results);
    SeasonPrint(season);
}

void SeasonPrint(Season season){
    int number_of_teams = SeasonGetNumberOfTeams(season);
    int number_of_drivers = SeasonGetNumberOfDrivers(season);
    DriverStatus status;
    printf("Number of teams in season: %d\n",number_of_teams);
//    Team* teams_array = SeasonGetTeamsStandings(season);
    Driver* drivers_array = SeasonGetDriversStandings(season);
    for(int i=0;i<number_of_drivers;i++){
        Driver driver = drivers_array[i];
        printf("driver name: %s\n",DriverGetName(driver));
        printf("Points: %d\n",DriverGetPoints(driver,&status));
    }
    printf("Number of drivers in season: %d\n",number_of_drivers);
}
/***** End of season testing functions *****/

