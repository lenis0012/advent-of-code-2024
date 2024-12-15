//
// Created by lennart on 12/3/24.
//

#include <stdlib.h>
#include <string.h>
#include <assert.h>

/*
 * Array-backed list implementation.
 * Base functions: add(V), get(I), insert(I, V), remove_at(), clear()
 * define LIST_EQ(a, b) for search extensions: index_of()
 *
 * Copies all data when inserting before end of list.
 * To avoid this, define LIST_UNORDERED
 */

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

static void LIST_FUNC(destroy)(LIST_T *list) {
    LIST_FUNC(fini)(list);
    free(list);
}

static void LIST_FUNC(clear)(LIST_T *list) {
    list->size = 0;
}

static LIST_ELEMENT LIST_FUNC(get)(LIST_T *list, unsigned int index) {
    assert(list->size > index);
    return list->elements[index];
}

/**
* returns -1 when not found
*/
#ifdef LIST_EQ
static int LIST_FUNC(index_of)(LIST_T *list, LIST_ELEMENT target) {
    for (int i = 0; i < list->size; i++) {
        if (LIST_EQ(list->elements[i], target)) return i;
    }

    return -1;
}
#endif

static void LIST_FUNC(add)(LIST_T *list, LIST_ELEMENT value) {
    if (list->size >= list->capacity) {
        list->elements = realloc(list->elements, (list->capacity = list->capacity * 2) * sizeof(LIST_ELEMENT));
    }

    list->elements[list->size++] = value;
}

static void LIST_FUNC(insert)(LIST_T *list, unsigned int index, LIST_ELEMENT value) {
    if (list->size >= list->capacity) {
        list->elements = realloc(list->elements, (list->capacity = list->capacity * 2) * sizeof(LIST_ELEMENT));
    }

    memmove(&list->elements[index + 1], &list->elements[index], (list->size++ - index) * sizeof(LIST_ELEMENT));
    list->elements[index] = value;
}

#define LIST_UNORDERED
static void LIST_FUNC(remove_at)(LIST_T *list, int index) {
    assert(list->size > index);
#ifdef LIST_UNORDERED
    list->elements[index] = list->elements[--list->size];
#else
    memmove(&list->elements[index + 1], &list->elements[index], (list->size - index - 1) * sizeof(LIST_ELEMENT));
#endif
}

// Undefine all variables
#undef LIST_FUNC
#undef LIST_UNORDERED
#undef LIST_T
#undef LIST_ELEMENT
#undef LIST_NAME
#undef LIST_EQ
#undef LIST_CONCAT
#undef LIST_CONCAT_

