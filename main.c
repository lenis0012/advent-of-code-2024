//
// Created by lennart on 12/2/24.
//


#include <stdio.h>
#include <stdlib.h>

extern void run_day1();
extern void run_day2();
extern void run_day3();
extern void run_day4();

int main(int argc, char **argv) {
    int day = strtod(argv[1], nullptr);
    switch (day) {
    case 1: run_day1(); break;
    case 2: run_day2(); break;
    case 3: run_day3(); break;
    case 4: run_day4(); break;
        default:
            fprintf(stderr, "Not implemented day %d yet\n");
    }
}
