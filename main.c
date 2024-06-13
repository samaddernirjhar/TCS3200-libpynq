/*
By Nirjhar Samadder
TCS-3200 colour sensor - for TU/e 5EID0
Example code for how to run it. This code assumues you are in posession of the libpynq library for the PYNQ Z2 board.
*/
#include <libpynq.h>
#include <stdio.h>
#include <time.h>
#include "pulsecounter.h"

// Pinmap to arduino headers
#define OUT IO_AR8
#define OE IO_AR10
#define S0 IO_AR4
#define S1 IO_AR5
#define S2 IO_AR6
#define S3 IO_AR7

// Generalized parameters
#define NUM_READINGS 30
#define DELAY_BETWEEN_READINGS_MS 20
#define NUM_MIDDLE_VALUES 10

void sortArray(uint32_t arr[], int n) {
    int i, j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                uint32_t temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

uint32_t averageMiddleValues(uint32_t arr[], int num_values) {
    int start_index = (NUM_READINGS - num_values) / 2;
    uint32_t sum = 0;
    for (int i = start_index; i < start_index + num_values; i++) {
        sum += arr[i];
    }
    return sum / num_values;
}

void takeReadings(uint32_t readings[], uint8_t s2, uint8_t s3) {
    for (int i = 0; i < NUM_READINGS; i++) {
        gpio_set_level(S2, s2);
        gpio_set_level(S3, s3);

        pulsecounter_reset_count(PULSECOUNTER0);
        sleep_msec(DELAY_BETWEEN_READINGS_MS);

        uint32_t timestamp;
        readings[i] = pulsecounter_get_count(PULSECOUNTER0, &timestamp);
        sleep_msec(DELAY_BETWEEN_READINGS_MS);
    }
    sortArray(readings, NUM_READINGS);
}

void colour(void) {
    uint32_t redReadings[NUM_READINGS];
    uint32_t greenReadings[NUM_READINGS];
    uint32_t blueReadings[NUM_READINGS];

    // RED
    takeReadings(redReadings, GPIO_LEVEL_LOW, GPIO_LEVEL_LOW);
    uint32_t redAverage = averageMiddleValues(redReadings, NUM_MIDDLE_VALUES);
    printf("RED: %d\n", redAverage);

    // GREEN
    takeReadings(greenReadings, GPIO_LEVEL_HIGH, GPIO_LEVEL_HIGH);
    uint32_t greenAverage = averageMiddleValues(greenReadings, NUM_MIDDLE_VALUES);
    printf("GREEN: %d\n", greenAverage);

    // BLUE
    takeReadings(blueReadings, GPIO_LEVEL_LOW, GPIO_LEVEL_HIGH);
    uint32_t blueAverage = averageMiddleValues(blueReadings, NUM_MIDDLE_VALUES);
    printf("BLUE: %d\n", blueAverage);
}

/*
    @return 0 on successful execution.
*/
int main(void) {
    pynq_init();
    gpio_init();

    gpio_set_direction(OE, GPIO_DIR_OUTPUT);
    gpio_set_direction(S0, GPIO_DIR_OUTPUT);
    gpio_set_direction(S1, GPIO_DIR_OUTPUT);
    gpio_set_direction(S2, GPIO_DIR_OUTPUT);
    gpio_set_direction(S3, GPIO_DIR_OUTPUT);
    gpio_set_direction(OUT, GPIO_DIR_INPUT);

    gpio_set_level(S0, GPIO_LEVEL_HIGH);
    gpio_set_level(S1, GPIO_LEVEL_LOW);

    // Enable the sensor by setting OE to low
    gpio_set_level(OE, GPIO_LEVEL_LOW);
    
    switchbox_set_pin(OUT, SWB_TIMER_IC0);
    pulsecounter_init(PULSECOUNTER0);
    pulsecounter_set_edge(PULSECOUNTER0, GPIO_LEVEL_HIGH);

    while (1) {
        colour();
        sleep_msec(50);
        printf("-------------------\n");
    }

    pulsecounter_destroy(PULSECOUNTER0);
    pynq_destroy();
    gpio_destroy();
    return 0;
}
