/*
 * season.h
 *
 *  Created on: Apr 14, 2018
 *      Author: muhamadg
 */

#ifndef SEASON_H_
#define SEASON_H_

typedef struct season* Season;

#include"team.h"
#include"driver.h"


typedef enum seasonStatus {
	SEASON_OK,
	SEASON_MEMORY_ERROR,
	BAD_SEASON_INFO,
	SEASON_NULL_PTR} SeasonStatus;

Season SeasonCreate(SeasonStatus* status,const char* season_info);
void   SeasonDestroy(Season season);
Driver SeasonGetDriverByPosition(Season season, int position, SeasonStatus* status);
Driver* SeasonGetDriversStandings(Season season);
Team SeasonGetTeamByPosition(Season season, int position, SeasonStatus* status);
Team* SeasonGetTeamsStandings(Season season);
int SeasonGetNumberOfDrivers(Season season);
int SeasonGetNumberOfTeams(Season season);
SeasonStatus SeasonAddRaceResult(Season season, int* results);

#endif /* SEASON_H_ */