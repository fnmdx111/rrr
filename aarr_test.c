//
// Created by wo on 5/19/16.
//

#include "libs/generic/aarr.h"
#include <stdio.h>
#include <time.h>


struct my_type_t
{
  float a, b, c;
  float x, y, z;
};

#define TN 1000000

void
create_type(struct my_type_t* ret)
{
  ret->a = random() * 1.f / RAND_MAX;
  ret->b = random() * 1.f / RAND_MAX;
  ret->c = random() * 1.f / RAND_MAX;
  ret->x = random() * 1.f / RAND_MAX;
  ret->y = random() * 1.f / RAND_MAX;
  ret->z = random() * 1.f / RAND_MAX;
}

int
is_equal(struct my_type_t* a, struct my_type_t* b)
{
  if (a->a == b->a && a->b == b->b && a->c == b->c
    && a->x == b->x && a->y == b->y && a->z == b->z) {
    return 1;
  }

  return 0;
}

struct my_type_t arr[TN];

int
main()
{
  new_aarr(test, struct my_type_t, 1);

  for (size_t i = 0; i < TN; ++i) {
    create_type(&arr[i]);
    aarr_push(test, struct my_type_t, &arr[i]);
  }

  struct my_type_t x1 = aarr_idx(test, struct my_type_t, 0);
  struct my_type_t x2 = aarr_idx(test, struct my_type_t, 1);
  struct my_type_t x10 = aarr_idx(test, struct my_type_t, 100000);

  if (is_equal(&x1, &(arr[0]))) {
    printf("0 passed test.\n");
  }
  if (is_equal(&x2, &(arr[1]))) {
    printf("1 passed test.\n");
  }
  if (is_equal(&x10, &(arr[100000]))) {
    printf("10 passed test.\n");
  }

  time_t t1 = clock();
  int passed = 1;
  aarr_enum(test, struct my_type_t, i, it)
    if (!is_equal(it, &(arr[i]))) {
      passed = 0;
    }
  aarr_end

  time_t t2 = clock();
  printf("%d in %lf\n", passed, (t2 - t1) * 1.0 / CLOCKS_PER_SEC);


  del_aarr(test);

  return 0;
}
