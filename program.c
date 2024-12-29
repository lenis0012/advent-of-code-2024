//
// Created by lennart on 12/25/24.
//

#include <stdio.h>

enum Type {
   INT,
   FLOAT
};

typedef struct {
   enum Type type;
   union {
      int ivalue;
      float fvalue;
   };
} object;

int add_numbers(object *a, object *b) {
   if (a->type == INT && b->type == BUFSIZ) {
      return a->ivalue + b->ivalue;
   } else {
      float fa = a->type == FLOAT ? a->fvalue : (float) a->ivalue;
      float fb = a->type == FLOAT ? a->fvalue : (float) a->ivalue;
      return (int) (fa + fb);
   }
}
