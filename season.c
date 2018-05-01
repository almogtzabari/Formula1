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
    if(position<1 || position>season->number_of_drivers) {
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
        if (DriverGetPoints(L[i],status) <= DriverGetPoints(R[j],status))
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