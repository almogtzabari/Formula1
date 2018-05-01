#include <stdio.h>
#include "team.h"
#include "season.h"
#include "driver.h"
#include <malloc.h>

/** Team declarations */
void TeamTesting();
void TeamPrint(Team team, TeamStatus status);
void TeamPrintDriverStatus(TeamStatus status);
void TeamPrintCreateStatus(TeamStatus status);
/** End of team declarations */

/** Driver declarations */
void DriverTesting();
void DriverPrint(Driver driver);
void DriverPrintStatus(DriverStatus status);
/** End of driver declarations */




int main() {
//    TeamTesting();
    DriverTesting();
    return 0;
}


/** Team testing functions */
/* Testing: TeamCreate,    */
void TeamTesting(){
    /** Testing for Team and Driver. */
    printf("\n\nStarting TeamTesting() in:\n3...\n2...\n1...\n\n");
    TeamStatus team_status;
    DriverStatus driver_status;
    printf("Creating first driver...\n");
    Driver first_driver = DriverCreate(&driver_status,"Yohan Kfir",3115445); // Creating first driver.
    DriverPrintStatus(driver_status);
    printf("Creating second driver...\n");
    Driver second_driver = DriverCreate(&driver_status,"Moshe Daood",203548); // Creating second driver.
    DriverPrintStatus(driver_status);
    printf("Creating third driver...\n");
    Driver third_driver = DriverCreate(&driver_status,"Ben Weinsteffen",789156); // Creating third driver.
    DriverPrintStatus(driver_status);
    printf("Creating team...\n");
    Team team = TeamCreate(&team_status,"Ford"); // Creates a team.
    TeamPrintCreateStatus(team_status);
    printf("Adding first driver...\n");
    TeamStatus first_driver_status = TeamAddDriver(team,first_driver); // Adding first driver.
    TeamPrintDriverStatus(first_driver_status); // Add first driver to team success/fail.
    printf("Adding second driver...\n");
    TeamStatus second_driver_status = TeamAddDriver(team,second_driver); // Adding second driver.
    TeamPrintDriverStatus(second_driver_status); // Add second driver to team success/fail.
    printf("Adding second driver...\n");
    TeamStatus third_driver_status = TeamAddDriver(team,third_driver); // Attempt to add third driver to a team. Should fail.
    TeamPrintDriverStatus(second_driver_status); // Add third driver to team success/fail.

//    DriverDestroy(first_driver); // NOT SURE IF WORKING. Suppose to destroying first driver.
//    DriverDestroy(second_driver); // NOT SURE IF WORKING. Suppose to destroying second driver.
    printf("\nPrinting Team:\n\n");
    TeamPrint(team, team_status);
    printf("\nTeamTesting() is done\n");
    /** End of Testing for Team and Driver. */
}

void TeamPrint(Team team, TeamStatus status){
    printf("Team Name: %s\n",TeamGetName(team));
    Driver first_driver = TeamGetDriver(team,FIRST_DRIVER);
    Driver second_driver = TeamGetDriver(team,SECOND_DRIVER);
    printf("First Driver: %s\n",DriverGetName(first_driver));
    printf("ID: %d\n",DriverGetId(first_driver));
    printf("Second Driver: %s\n",DriverGetName(second_driver));
    printf("ID: %d\n",DriverGetId(second_driver));
}

void TeamPrintDriverStatus(TeamStatus status){
    switch(status){
        case TEAM_STATUS_OK: printf("Status: successfully added.\n\n"); break;
        case TEAM_MEMORY_ERROR: printf("Status: memory error.\n\n"); break;
        case TEAM_FULL: printf("Status: team is full.\n"); break;
        default: ; break;
    }
}

void TeamPrintCreateStatus(TeamStatus status){
    switch(status){
        case TEAM_STATUS_OK: printf("Team successfully created.\n\n"); break;
        case TEAM_MEMORY_ERROR: printf("Team not created: Memory Error.\n\n"); break;
        default: ; break;
    }
}
/** End of team testing functions */


/** Driver testing functions */
/* Testing: DriverCreate, DriverGetName, DriverGetId */
void DriverTesting() {
    printf("\n\nStarting DriverTesting() in:\n3...\n2...\n1...\n\n");
    DriverStatus driver_status;
    printf("Creating first driver...\n");
    Driver first_driver = DriverCreate(&driver_status,"Yohan Kfir",3115445); // Creating first driver.
    DriverPrintStatus(driver_status);
    printf("Creating second driver...\n");
    Driver second_driver = DriverCreate(&driver_status,"Moshe Daood",203548); // Creating second driver.
    DriverPrintStatus(driver_status);
    printf("Creating third driver...\n");
    Driver third_driver = DriverCreate(&driver_status,"Ben Weinsteffen",789156); // Creating third driver.
    DriverPrintStatus(driver_status);
    printf("\nPrinting Drivers:\n\n");
    printf("First Driver:\n");
    DriverPrint(first_driver);
    printf("Second Driver:\n");
    DriverPrint(second_driver);
    printf("Third Driver:\n");
    DriverPrint(third_driver);
}

void DriverPrint(Driver driver){
    DriverStatus status;
    printf("Driver Name: %s\n",DriverGetName(driver));
    printf("Driver ID: %d\n",DriverGetId(driver));
    printf("Driver Points: %d\n\n",DriverGetPoints(driver, &status));
}

void DriverPrintStatus(DriverStatus status){
    switch(status){
        case DRIVER_STATUS_OK: printf("Driver successfully created.\n\n"); break;
        case DRIVER_MEMORY_ERROR: printf("Driver not created: memory error.\n\n"); break;
        case INVALID_DRIVER: printf("Driver not created: pointer is NULL.\n\n"); break;
        default: ; break;
    }
}
/** End of driver testing functions */



