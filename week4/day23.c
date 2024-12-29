//
// Created by lennart on 12/23/24.
//

#include <aoc.h>
#include <limits.h>

#define FNV_BASIS 2166136261
#define FNV_PRIME 16777619

static int hash_str(char *str) {
    int hash = FNV_BASIS;
    while (*str != '\0') {
        hash = hash ^ *(str++);
        hash = hash * FNV_PRIME;
    }
    return hash;
}

#define LIST_NAME computers
#define LIST_ELEMENT char*
#define LIST_EQ(a, b) (strcmp(a, b) == 0)
#include "list.h"

#define MAP_NAME networks
#define MAP_K char*
#define MAP_V computers_t*
#define MAP_HASH(k) hash_str(k)
#define MAP_EQ(ka, kb) (strcmp(ka, kb) == 0)
#include "hashmap.h"

static networks_t *networks;

static void parse_input() {
    networks = networks_create(1 << 4);

    while (!aoc_eof()) {
        char a[3];
        char b[3];
        aoc_scan("%2s-%2s\n", &a[0], &b[0]);

        // Link b to a
        computers_t *computers = networks_get(networks, a);
        if (computers == nullptr) {
            computers = computers_new();
            networks_put(networks, strdup(a), computers);
        }
        computers_add(computers, strdup(b));

        // Link a to b
        computers = networks_get(networks, b);
        if (computers == nullptr) {
            computers = computers_new();
            networks_put(networks, strdup(b), computers);
        }
        computers_add(computers, strdup(a));
    }

}

static int error_rate(computers_t *list, unsigned int exclude) {
    int errors = 0;
    for (int i = 0; i < list->size; i++) {
        if (i == exclude) continue;

        char *in = computers_get(list, i);
        computers_t *ib = networks_get(networks, in);
        for (int j = 0; j < list->size; j++) {
            if (i == j || j == exclude) {
                continue;
            }

            char *jn = computers_get(list, j);
            if (computers_index_of(ib, jn) == -1) {
                errors++;
            }
        }
    }

    return errors;
}

static int str_compare(const void* a, const void* b)
{
    return strcmp(*(const char**)a, *(const char**)b);
}

void run_day23() {
    aoc_begin();

    // part 1
    parse_input();
    int result = 0;
    networks_node_t *nw = nullptr;
    while ((nw = networks_next(networks, nw)) != nullptr) {
        char *computer = nw->key;
        computers_t *buddies = nw->value;
        if (computer[0] != 't' || buddies->size < 2) {
            continue;
        }

        int parties = 0;
        for (int i = 0; i < buddies->size - 1; i++) {
            char *ibuddy = computers_get(buddies, i);
            computers_t *ibuddies = networks_get(networks, ibuddy);
            if (computers_index_of(ibuddies, computer) == -1 || ibuddies->size <= 2) continue; // optimization

            for (int j = i + 1; j < buddies->size; j++) {
                char *jbuddy = computers_get(buddies, j);
                computers_t *jbuddies = networks_get(networks, jbuddy);

                // Check if computers form a party
                if (
                     computers_index_of(ibuddies, jbuddy) != -1
                    && computers_index_of(jbuddies, computer) != -1 && computers_index_of(jbuddies, ibuddy) != -1
                ) {
                    // printf("%s %s %s\n", computer, ibuddy, jbuddy);
                    parties++;
                }
            }
        }

        // printf("%s <> %u = %d\n", computer, buddies->size, parties);
        buddies->size = 0; // Clear buddies to prevent double counting
        result += parties;
    }
    printf("Result: %d\n", result);

    // Part 2
    aoc_rewind();
    parse_input();
    nw = nullptr;

    int best_size = 0;
    computers_t *best = nullptr;
    while ((nw = networks_next(networks, nw)) != nullptr) {
        char *computer = nw->key;
        computers_t *buddies = networks_get(networks, computer);
        if (computer[0] != 't') {
            continue;
        }

        computers_t *party = computers_new();
        computers_add(party, computer);
        for (int i = 0; i < buddies->size; i++) {
            computers_add(party, computers_get(buddies, i));
        }

        while (party->size > 1) {
            int error = error_rate(party, party->size);
            if (error == 0) {
                break;
            }

            int le = INT_MAX;
            int li = 0;
            for (int i = 0; i < party->size; i++) {
                error = error_rate(party, i);
                if (error < le) {
                    li = i;
                    le = error;
                }
            }

            computers_remove_at(party, li);
        }

        if (party->size > best_size) {
            if (best != nullptr) computers_destroy(best);
            best_size = party->size;
            best = party;
        } else {
            computers_destroy(party);
        }
    }

    printf("Result: ");
    qsort((void*) best->elements, best->size, sizeof(char*), (__compar_fn_t) str_compare);
    for (int i = 0; i < best->size; i++) {
        printf(i < best->size - 1 ? "%s," : "%s\n", best->elements[i]);
    }

    aoc_end();
}
