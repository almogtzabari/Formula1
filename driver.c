#include "driver.h"

typedef struct driver {
    int id;
    char* driver_name;
    char* team_name;
    int points;
    Season season_of_driver;
} *Driver;

int DriverGetPoints(Driver driver, DriverStatus* status){
    if(driver == NULL)
        return
}