//
// Created by lennart on 12/2/24.
//


#include <stdio.h>
#include <stdlib.h>

extern void run_day1();
extern void run_day2();
extern void run_day3();
extern void run_day4();
extern void run_day5();
extern void run_day6();
extern void run_day7();
extern void run_day8();
extern void run_day9();
extern void run_day10();
extern void run_day11();
extern void run_day12();
extern void run_day13();

struct object {
    enum type {
        tnumber,
        tfloat,
        tstring
    } type;
    union {
        float fvalue;
        int ivalue;
        char* strvalue;
    };
};

int main(int argc, char **argv) {
    int day = strtod(argv[1], nullptr);
    switch (day) {
    case 1: run_day1(); break;
    case 2: run_day2(); break;
    case 3: run_day3(); break;
    case 4: run_day4(); break;
    case 5: run_day5(); break;
    case 6: run_day6(); break;
    case 7: run_day7(); break;
    case 8: run_day8(); break;
    case 9: run_day9(); break;
    case 10: run_day10(); break;
    case 11: run_day11(); break;
    case 12: run_day12(); break;
    case 13: run_day13(); break;
        default:
            fprintf(stderr, "Not implemented day %d yet\n", day);
    }
}
