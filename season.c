#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>
#include "driver.h"


typedef struct season {
    int year;
    int number_of_teams;
    Team* team_array;
    int number_of_drivers;
    Driver* driver_array
} *Season;