//
// Created by lennart on 12/3/24.
//

#pragma once

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifndef LIST_NAME
#define LIST_NAME list
#endif

#ifndef LIST_ELEMENT
#define LIST_ELEMENT int
#endif

#define LIST_CONCAT_(a, b) a ## _ ## b
#define LIST_CONCAT(a, b) LIST_CONCAT_(a, b)
#define LIST_T LIST_CONCAT(LIST_NAME, t)
#define LIST_FUNC(name) LIST_CONCAT(LIST_NAME, name)

typedef struct {
    LIST_ELEMENT *elements;
    unsigned int size, capacity;
} LIST_T;

static void LIST_FUNC(init)(LIST_T *list) {
    list->capacity = 16;
    list->size = 0;
    list->elements = malloc(sizeof(LIST_ELEMENT) * list->capacity);
}

static LIST_T* LIST_FUNC(new)() {
    LIST_T *list = malloc(sizeof(LIST_T));
    LIST_FUNC(init)(list);
    return list;
}

static LIST_T* LIST_FUNC(clone)(LIST_T *from) {
    LIST_T *to = LIST_FUNC(new)();
    to->capacity = from->capacity;
    to->size = from->size;
    to->elements = malloc(to->capacity * sizeof(LIST_ELEMENT));
    memcpy(to->elements, from->elements, to->size * sizeof(LIST_ELEMENT));
}

static void LIST_FUNC(fini)(LIST_T *list) {
    free(list->elements);
}

static LIST_T* LIST_FUNC(destroy)(LIST_T *list) {
    LIST_FUNC(fini)(list);
    free(list);
}

static LIST_ELEMENT LIST_FUNC(get)(LIST_T *list, unsigned int index) {
    assert(list->size > index);
    return list->elements[index];
}

static void LIST_FUNC(add)(LIST_T *list, LIST_ELEMENT value) {
    if (list->size >= list->capacity) {
        list->elements = realloc(list->elements, (list->capacity = list->capacity * 2) * sizeof(LIST_ELEMENT));
    }

    list->elements[list->size++] = value;
}

static void LIST_FUNC(remove_at)(LIST_T *list, int index) {
    assert(list->size > index);
    memmove(&list->elements[index + 1], &list->elements[index], (list->size - index - 1) * sizeof(LIST_ELEMENT));
}

// Undefine all variables
#undef LIST_FUNC
#undef LIST_T
#undef LIST_ELEMENT
#undef LIST_NAME
#undef LIST_CONCAT
#undef LIST_CONCAT_

