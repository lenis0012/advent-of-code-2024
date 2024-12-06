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

int add(struct object* a, struct object *b) {
    if (a->type == tnumber && b->type == tnumber) {
        return a->ivalue + b->ivalue;
    } else if (a->type == tfloat && b->type == tfloat) {
        return a->fvalue + b->fvalue;
    } else if (a->type == tfloat && b->type == tnumber) {
        return a->fvalue + b->ivalue;
    }else if (a->type == tnumber && b->type == tfloat) {
        return a->ivalue + b->fvalue;
    } else {
        exit(1);
    }
}

int main(int argc, char **argv) {
    int day = strtod(argv[1], nullptr);
    switch (day) {
    case 1: run_day1(); break;
    case 2: run_day2(); break;
    case 3: run_day3(); break;
    case 4: run_day4(); break;
    case 5: run_day5(); break;
    case 6: run_day6(); break;
        default:
            fprintf(stderr, "Not implemented day %d yet\n", day);
    }
}
