#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "season.h"

Driver getDummyDriver() {
    return DriverCreate(NULL, "driver", 1);
}

Team getDummyTeam() {
    return TeamCreate(NULL, "team1");
}

Season getDummySeason() {
    char *seasonInfo = "\
2018\n\
Ferrari\n\
Sebastian Vettel\n\
Kimi Raikonen\n\
Mercedes\n\
Lewis Hamilton\n\
Valtteri Bottas\n\
RedBull Racing\n\
Daniel\n\
Max  Verstappen\n\
McLaren\n\
Fernando Alonso\n\
None\n\
";
    return SeasonCreate(NULL, seasonInfo);
}

void driverUnitTest() {
    DriverStatus status;
    int id = 1;
    char *name1 = "driver1";
    Driver driver1 = DriverCreate(&status, name1, id);
    if (status == DRIVER_STATUS_OK) {
        assert(driver1);
    } else if (status == DRIVER_MEMORY_ERROR) {
        assert(!driver1);
    } else {
        assert(false);
    }
    assert(!DriverGetId(NULL));
    assert(DriverGetId(driver1) == id);
    assert(strcmp(DriverGetName(driver1), name1) == 0);
    assert(!DriverGetName(NULL));
    assert(!DriverGetTeam(driver1));
    assert(!DriverGetTeam(NULL));
    Team team = getDummyTeam();
    assert(team);
    DriverSetTeam(NULL, NULL);
    assert(!DriverGetTeam(driver1));
    DriverSetTeam(driver1, NULL);
    assert(!DriverGetTeam(driver1));
    DriverSetTeam(NULL, team);
    assert(!DriverGetTeam(driver1));
    DriverSetTeam(driver1, team);
    assert(DriverGetTeam(driver1) == team);
    DriverSetTeam(driver1, NULL);
    assert(!DriverGetTeam(driver1));
    DriverSetTeam(driver1, team);
    TeamDestroy(team);
    assert(DriverGetPoints(driver1, NULL) == 0);
    assert(DriverAddRaceResult(driver1, 1) == SEASON_NOT_ASSIGNED);
    assert(DriverGetPoints(driver1, NULL) == 0);
    Season season = getDummySeason();
    assert(season);
    DriverSetSeason(NULL, NULL);
    DriverSetSeason(NULL, season);
    DriverSetSeason(driver1, NULL);
    DriverSetSeason(driver1, season);
    assert(DriverGetPoints(driver1, NULL) == 0);
    assert(DriverAddRaceResult(driver1, 1) == DRIVER_STATUS_OK);
    assert(DriverGetPoints(driver1, NULL) == 6);
    assert(DriverAddRaceResult(NULL, 1) == INVALID_DRIVER);
    assert(DriverGetPoints(driver1, NULL) == 6);
    assert(DriverAddRaceResult(driver1, 0) == INVALID_POSITION);
    assert(DriverGetPoints(driver1, NULL) == 6);
    assert(DriverAddRaceResult(driver1, -1) == INVALID_POSITION);
    assert(DriverGetPoints(driver1, NULL) == 6);
    assert(DriverAddRaceResult(driver1, 8) == INVALID_POSITION);
    assert(DriverGetPoints(driver1, NULL) == 6);
    assert(DriverAddRaceResult(driver1, 7) == DRIVER_STATUS_OK);
    assert(DriverGetPoints(driver1, NULL) == 6);
    DriverGetPoints(NULL, &status);
    assert(status == INVALID_DRIVER);
    DriverGetPoints(driver1, &status);
    assert(status == DRIVER_STATUS_OK);
    assert(DriverGetPoints(driver1, NULL) == 6);
    DriverSetSeason(NULL, season);
    assert(DriverGetPoints(driver1, NULL) == 6);
    DriverSetSeason(driver1, season);
    assert(DriverGetPoints(driver1, NULL) == 0);
    SeasonDestroy(season);
    DriverDestroy(driver1);
    driver1 = NULL;
    DriverDestroy(driver1);
    Driver driver2 = DriverCreate(&status, NULL, 2);
    assert(status == INVALID_DRIVER && !driver2);
    Driver driver3 = DriverCreate(&status, "driver3", 0);
    assert(status == INVALID_DRIVER && !driver3);
    Driver driver4 = DriverCreate(&status, "driver4", -1);
    assert(status == INVALID_DRIVER && !driver4);
    Driver driver5 = DriverCreate(&status, NULL, -1);
    assert(status == INVALID_DRIVER && !driver5);
    char *nameBefore = "driver6";
    Driver driver6 = DriverCreate(&status, nameBefore, 6);
    assert(driver6);
    assert(strcmp(DriverGetName(driver6), "driver6") == 0);
    nameBefore = "name after";
    assert(strcmp(DriverGetName(driver6), nameBefore) != 0);
    assert(strcmp(DriverGetName(driver6), "driver6") == 0);
    DriverDestroy(driver6);
    Driver driver7 = DriverCreate(NULL, "driver7", 7);
    DriverDestroy(driver7);
}

void teamUnitTest() {
    TeamStatus status;
    Team team1 = TeamCreate(&status, "team1");
    if (status == TEAM_STATUS_OK) {
        assert(team1);
    } else if (status == TEAM_MEMORY_ERROR) {
        assert(!team1);
    } else {
        assert(false);
    }
    assert(strcmp(TeamGetName(team1), "team1") == 0);
    assert(!TeamGetName(NULL));
    Driver driver1 = getDummyDriver();
    Driver driver2 = getDummyDriver();
    assert(driver1 && driver2);
    Season season = getDummySeason();
    assert(season);
    DriverSetSeason(driver1, season);
    DriverAddRaceResult(driver1, 1);
    assert(DriverGetPoints(driver1, NULL) == 6);
    DriverSetSeason(driver2, season);
    DriverAddRaceResult(driver2, 1);
    assert(DriverGetPoints(driver2, NULL) == 6);
    assert(!TeamGetDriver(NULL, FIRST_DRIVER));
    assert(!TeamGetDriver(team1, -1));
    assert(!TeamGetDriver(team1, 2));
    assert(!TeamGetDriver(team1, FIRST_DRIVER));
    assert(!TeamGetDriver(team1, SECOND_DRIVER));
    assert(TeamAddDriver(NULL, NULL) == TEAM_NULL_PTR);
    assert(!TeamGetDriver(team1, FIRST_DRIVER));
    assert(!TeamGetDriver(team1, SECOND_DRIVER));
    assert(TeamAddDriver(NULL, driver1) == TEAM_NULL_PTR);
    assert(!TeamGetDriver(team1, FIRST_DRIVER));
    assert(!TeamGetDriver(team1, SECOND_DRIVER));
    assert(TeamAddDriver(team1, NULL) == TEAM_NULL_PTR);
    assert(!TeamGetDriver(team1, FIRST_DRIVER));
    assert(!TeamGetDriver(team1, SECOND_DRIVER));
    assert(TeamGetPoints(NULL, NULL) == 0);
    assert(TeamGetPoints(NULL, &status) == 0);
    assert(status == TEAM_NULL_PTR);
    assert(TeamGetPoints(team1, NULL) == 0);
    assert(TeamGetPoints(team1, &status) == 0);
    assert(status == TEAM_STATUS_OK);
    assert(TeamAddDriver(team1, driver1) == TEAM_STATUS_OK);
    assert(TeamGetDriver(team1, FIRST_DRIVER) == driver1);
    assert(!TeamGetDriver(team1, SECOND_DRIVER));
    assert(TeamGetPoints(team1, &status) == 6);
    assert(status == TEAM_STATUS_OK);
    assert(TeamAddDriver(team1, driver2) == TEAM_STATUS_OK);
    assert(TeamGetDriver(team1, FIRST_DRIVER) == driver1);
    assert(TeamGetDriver(team1, SECOND_DRIVER) == driver2);
    assert(TeamGetPoints(team1, &status) == 12);
    assert(status == TEAM_STATUS_OK);
    assert(TeamAddDriver(team1, driver1) == TEAM_FULL);
    assert(TeamGetDriver(team1, FIRST_DRIVER) == driver1);
    assert(TeamGetDriver(team1, SECOND_DRIVER) == driver2);
    assert(TeamAddDriver(team1, driver1) == TEAM_FULL);
    assert(TeamGetDriver(team1, FIRST_DRIVER) == driver1);
    assert(TeamGetDriver(team1, SECOND_DRIVER) == driver2);
    SeasonDestroy(season);
    TeamDestroy(team1);
    team1 = NULL;
    TeamDestroy(team1);
    Team team2 = TeamCreate(NULL, "team2");
    TeamDestroy(team2);
    Team team3 = TeamCreate(&status, NULL);
    assert(status == TEAM_NULL_PTR && !team3);
    Team team4 = TeamCreate(NULL, NULL);
    assert(status == TEAM_NULL_PTR && !team4);
    char *nameBefore = "team5";
    Team team5 = TeamCreate(&status, nameBefore);
    assert(team5);
    assert(strcmp(TeamGetName(team5), "team5") == 0);
    nameBefore = "name after";
    assert(strcmp(TeamGetName(team5), nameBefore) != 0);
    assert(strcmp(TeamGetName(team5), "team5") == 0);
    TeamDestroy(team5);
}

void testDriverPosition(Driver driver, char *name, int points) {
    assert(driver);
    assert(strcmp(DriverGetName(driver), name) == 0);
    assert(DriverGetPoints(driver, NULL) == points);
}

void testDriverByPositionFunc(Season season, int position,
                              char *name, int points) {
    SeasonStatus status;
    Driver driver = SeasonGetDriverByPosition(season, position, &status);
    if (status == SEASON_OK) {
        testDriverPosition(driver, name, points);
    } else if (status == SEASON_MEMORY_ERROR) {
        assert(!driver);
    } else {
        assert(false);
    }
}

void testTeamPosition(Team team, char *name, int points) {
    assert(team);
    assert(strcmp(TeamGetName(team), name) == 0);
    assert(TeamGetPoints(team, NULL) == points);
}

void testTeamByPositionFunc(Season season, int position,
                            char *name, int points) {
    SeasonStatus status;
    Team team = SeasonGetTeamByPosition(season, position, &status);
    if (status == SEASON_OK) {
        testTeamPosition(team, name, points);
    } else if (status == SEASON_MEMORY_ERROR) {
        assert(!team);
    } else {
        assert(false);
    }
}

void seasonUnitTest() {
    SeasonStatus status;
    char *seasonInfo = "\
2018\n\
Ferrari\n\
Sebastian Vettel\n\
Kimi Raikonen\n\
Mercedes\n\
Lewis Hamilton\n\
Valtteri Bottas\n\
RedBull Racing\n\
Daniel\n\
Max  Verstappen\n\
McLaren\n\
Fernando Alonso\n\
None\n\
";
    Season season1 = SeasonCreate(&status, seasonInfo);
    if (status == SEASON_OK) {
        assert(season1);
    } else if (status == SEASON_MEMORY_ERROR) {
        assert(!season1);
    } else {
        assert(false);
    }
    assert(SeasonGetNumberOfTeams(season1) == 4);
    assert(!SeasonGetNumberOfTeams(NULL));
    assert(SeasonGetNumberOfDrivers(season1) == 7);
    assert(!SeasonGetNumberOfDrivers(NULL));
    int results1[7] = {1, 2, 3, 4, 5, 6, 7};
    assert(SeasonAddRaceResult(NULL, NULL) == SEASON_NULL_PTR);
    assert(SeasonAddRaceResult(season1, NULL) == SEASON_NULL_PTR);
    assert(SeasonAddRaceResult(NULL, results1) == SEASON_NULL_PTR);
    assert(SeasonAddRaceResult(season1, results1) == SEASON_OK);
    Driver driver = SeasonGetDriverByPosition(season1, 1, NULL);
    assert(driver);
    driver = SeasonGetDriverByPosition(NULL, 1, &status);
    assert(status == SEASON_NULL_PTR && !driver);
    driver = SeasonGetDriverByPosition(season1, 0, &status);
    assert(status == SEASON_NULL_PTR && !driver);
    driver = SeasonGetDriverByPosition(season1, 8, &status);
    assert(status == SEASON_NULL_PTR && !driver);
    assert(!SeasonGetDriversStandings(NULL));
    Driver *driverStandings = SeasonGetDriversStandings(season1);
    assert(driverStandings);
    testDriverPosition(driverStandings[0], "Sebastian Vettel", 6);
    testDriverPosition(driverStandings[1], "Kimi Raikonen", 5);
    testDriverPosition(driverStandings[2], "Lewis Hamilton", 4);
    testDriverPosition(driverStandings[3], "Valtteri Bottas", 3);
    testDriverPosition(driverStandings[4], "Daniel", 2);
    testDriverPosition(driverStandings[5], "Max  Verstappen", 1);
    testDriverPosition(driverStandings[6], "Fernando Alonso", 0);
    testDriverByPositionFunc(season1, 1, "Sebastian Vettel", 6);
    testDriverByPositionFunc(season1, 2, "Kimi Raikonen", 5);
    testDriverByPositionFunc(season1, 3, "Lewis Hamilton", 4);
    testDriverByPositionFunc(season1, 4, "Valtteri Bottas", 3);
    testDriverByPositionFunc(season1, 5, "Daniel", 2);
    testDriverByPositionFunc(season1, 6, "Max  Verstappen", 1);
    testDriverByPositionFunc(season1, 7, "Fernando Alonso", 0);
    Team team = SeasonGetTeamByPosition(season1, 1, NULL);
    assert(team);
    team = SeasonGetTeamByPosition(NULL, 1, &status);
    assert(status == SEASON_NULL_PTR && !team);
    team = SeasonGetTeamByPosition(season1, 0, &status);
    assert(status == SEASON_NULL_PTR && !team);
    team = SeasonGetTeamByPosition(season1, 5, &status);
    assert(status == SEASON_NULL_PTR && !team);
    assert(!SeasonGetTeamsStandings(NULL));
    Team *teamStandings = SeasonGetTeamsStandings(season1);
    testTeamPosition(teamStandings[0], "Ferrari", 11);
    testTeamPosition(teamStandings[1], "Mercedes", 7);
    testTeamPosition(teamStandings[2], "RedBull Racing", 3);
    testTeamPosition(teamStandings[3], "McLaren", 0);
    testTeamByPositionFunc(season1, 1, "Ferrari", 11);
    testTeamByPositionFunc(season1, 2, "Mercedes", 7);
    testTeamByPositionFunc(season1, 3, "RedBull Racing", 3);
    testTeamByPositionFunc(season1, 4, "McLaren", 0);
    free(driverStandings);
    free(teamStandings);
    int results2[7] = {3, 4, 1, 2, 5, 7, 6};
    assert(SeasonAddRaceResult(season1, results2) == SEASON_OK);
    driverStandings = SeasonGetDriversStandings(season1);
    teamStandings = SeasonGetTeamsStandings(season1);
    testDriverPosition(driverStandings[0], "Lewis Hamilton", 10);
    testDriverPosition(driverStandings[1], "Sebastian Vettel", 10);
    testDriverPosition(driverStandings[2], "Valtteri Bottas", 8);
    testDriverPosition(driverStandings[3], "Kimi Raikonen", 8);
    testDriverPosition(driverStandings[4], "Daniel", 4);
    testDriverPosition(driverStandings[5], "Fernando Alonso", 1);
    testDriverPosition(driverStandings[6], "Max  Verstappen", 1);
    testDriverByPositionFunc(season1, 1, "Lewis Hamilton", 10);
    testDriverByPositionFunc(season1, 2, "Sebastian Vettel", 10);
    testDriverByPositionFunc(season1, 3, "Valtteri Bottas", 8);
    testDriverByPositionFunc(season1, 4, "Kimi Raikonen", 8);
    testDriverByPositionFunc(season1, 5, "Daniel", 4);
    testDriverByPositionFunc(season1, 6, "Fernando Alonso", 1);
    testDriverByPositionFunc(season1, 7, "Max  Verstappen", 1);
    testTeamPosition(teamStandings[0], "Mercedes", 18);
    testTeamPosition(teamStandings[1], "Ferrari", 18);
    testTeamPosition(teamStandings[2], "RedBull Racing", 5);
    testTeamPosition(teamStandings[3], "McLaren", 1);
    testTeamByPositionFunc(season1, 1, "Mercedes", 18);
    testTeamByPositionFunc(season1, 2, "Ferrari", 18);
    testTeamByPositionFunc(season1, 3, "RedBull Racing", 5);
    testTeamByPositionFunc(season1, 4, "McLaren", 1);
    free(driverStandings);
    free(teamStandings);
    int results3[7] = {7, 1, 2, 3, 5, 4, 6};
    assert(SeasonAddRaceResult(season1, results3) == SEASON_OK);
    driverStandings = SeasonGetDriversStandings(season1);
    teamStandings = SeasonGetTeamsStandings(season1);
    testDriverPosition(driverStandings[0], "Sebastian Vettel", 15);
    testDriverPosition(driverStandings[1], "Lewis Hamilton", 13);
    testDriverPosition(driverStandings[2], "Kimi Raikonen", 12);
    testDriverPosition(driverStandings[3], "Valtteri Bottas", 9);
    testDriverPosition(driverStandings[4], "Fernando Alonso", 7);
    testDriverPosition(driverStandings[5], "Daniel", 6);
    testDriverPosition(driverStandings[6], "Max  Verstappen", 1);
    testDriverByPositionFunc(season1, 1, "Sebastian Vettel", 15);
    testDriverByPositionFunc(season1, 2, "Lewis Hamilton", 13);
    testDriverByPositionFunc(season1, 3, "Kimi Raikonen", 12);
    testDriverByPositionFunc(season1, 4, "Valtteri Bottas", 9);
    testDriverByPositionFunc(season1, 5, "Fernando Alonso", 7);
    testDriverByPositionFunc(season1, 6, "Daniel", 6);
    testDriverByPositionFunc(season1, 7, "Max  Verstappen", 1);
    testTeamPosition(teamStandings[0], "Ferrari", 27);
    testTeamPosition(teamStandings[1], "Mercedes", 22);
    testTeamPosition(teamStandings[2], "McLaren", 7);
    testTeamPosition(teamStandings[3], "RedBull Racing", 7);
    testTeamByPositionFunc(season1, 1, "Ferrari", 27);
    testTeamByPositionFunc(season1, 2, "Mercedes", 22);
    testTeamByPositionFunc(season1, 3, "McLaren", 7);
    testTeamByPositionFunc(season1, 4, "RedBull Racing", 7);
    free(driverStandings);
    free(teamStandings);
    SeasonDestroy(season1);
    season1 = NULL;
    SeasonDestroy(season1);
    Season season2 = SeasonCreate(NULL, seasonInfo);
    SeasonDestroy(season2);
    Season season3 = SeasonCreate(&status, NULL);
    assert(status == BAD_SEASON_INFO && !season3);
    seasonInfo = "\
2018\n\
None\n\
Driver1\n\
None\n\
";
    Season season4 = SeasonCreate(&status, seasonInfo);
    assert(SeasonGetNumberOfTeams(season4) == 1);
    assert(SeasonGetNumberOfDrivers(season4) == 1);
    SeasonDestroy(season4);
}

void exampleTest() {
    DriverStatus driver_status;
    TeamStatus team_status;
    Driver Lewis = DriverCreate(&driver_status, "Lewis", 44);
    Team Mercedes = TeamCreate(&team_status, "Mercedes");
    DriverSetTeam(Lewis, Mercedes);
    assert(strcmp(DriverGetName(Lewis), "Lewis") == 0);
    assert(strcmp(TeamGetName(DriverGetTeam(Lewis)), "Mercedes") == 0);
    DriverDestroy(Lewis);
    TeamDestroy(Mercedes);

    /*now lets try to create a season
    all drivers/teams created above are not related to the season.*/

    SeasonStatus season_status;
    char *season_info = "\
2018\n\
Ferrari\n\
Sebastian Vettel\n\
Kimi Raikonen\n\
Mercedes\n\
Lewis Hamilton\n\
Valtteri Bottas\n\
RedBull Racing\n\
Daniel\n\
Max  Verstappen\n\
McLaren\n\
Fernando Alonso\n\
None\n\
";
    Season season = SeasonCreate(&season_status, season_info);
    int race_results[7] = {7, 1, 3, 2, 4, 5, 6};
    SeasonAddRaceResult(season, race_results);
    SeasonDestroy(season);
}

int main() {
    driverUnitTest();
    teamUnitTest();
    seasonUnitTest();
    exampleTest();
    return 0;
}