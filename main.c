/*
By Nirjhar Samadder
TCS-3200 colour sensor - for TU/e 5EID0
Example code for how to run it. This code assumues you are in posession of the libpynq library for the PYNQ Z2 board.
*/
#include <libpynq.h>
#include <stdio.h>
#include <time.h>

// Pinmap to arduino headers
#define OUT IO_AR4
#define OE IO_AR5
#define S0 IO_AR0
#define S1 IO_AR1
#define S2 IO_AR2
#define S3 IO_AR3


/*
    @return The measured pulse width in microseconds.
*/
uint32_t pulseIn(uint8_t pin, uint8_t state) {
    struct timespec start, end;
    uint32_t pulse_out = 0;

    // Wait for the pin to enter the desired state
    while (gpio_get_level(pin) != state) {}

    // Measure pulse width
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    while (gpio_get_level(pin) == state) {
        clock_gettime(CLOCK_MONOTONIC_RAW, &end);
        pulse_out = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;
    }

    return pulse_out;
}


void colour(void) {
    int redFrequency = 0;
    int greenFrequency = 0;
    int blueFrequency = 0;
    int clearFrequency = 0;

    // RED
    gpio_set_level(S2, GPIO_LEVEL_LOW);
    gpio_set_level(S3, GPIO_LEVEL_LOW);
    redFrequency = pulseIn(OUT, GPIO_LEVEL_LOW);
    printf("RED: %d\n", redFrequency);

    // GREEN
    gpio_set_level(S2, GPIO_LEVEL_HIGH);
    gpio_set_level(S3, GPIO_LEVEL_HIGH);
    greenFrequency = pulseIn(OUT, GPIO_LEVEL_LOW);
    printf("GREEN: %d\n", greenFrequency);

    // BLUE
    gpio_set_level(S2, GPIO_LEVEL_LOW);
    gpio_set_level(S3, GPIO_LEVEL_HIGH);
    blueFrequency = pulseIn(OUT, GPIO_LEVEL_LOW);
    printf("BLUE: %d\n", blueFrequency);

    // CLEAR(NO FILTER)
    gpio_set_level(S2, GPIO_LEVEL_HIGH);
    gpio_set_level(S3, GPIO_LEVEL_LOW);
    clearFrequency = pulseIn(OUT, GPIO_LEVEL_LOW);
    printf("CLEAR: %d\n", clearFrequency);
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

    // Set frequency scaling, for 100% both high, for 20% use high-low, and for 2% use low-high
    gpio_set_level(S0, GPIO_LEVEL_LOW);
    gpio_set_level(S1, GPIO_LEVEL_HIGH);

    // Enable the sensor by setting OE to low
    gpio_set_level(OE, GPIO_LEVEL_LOW);

    while (1) {
        colour();
        sleep_msec(200);
        printf("-------------------\n");
    }

    pynq_destroy();
    gpio_destroy();
    return 0;
}
