//
// Created by wo on 5/19/16.
//

#ifndef RRR_LLIST_H
#define RRR_LLIST_H

#include <stdlib.h>

#ifdef DEBUG
#define INIT_CHUNK_SIZE 2
#else
#define INIT_CHUNK_SIZE 32
#endif

#ifdef DEBUG
#define MAX_CHUNK_SIZE 4
#else
#define MAX_CHUNK_SIZE 5120
#endif

struct __r_aarr_chunk;

struct r_aarr
{
  struct __r_aarr_chunk* head;
  struct __r_aarr_chunk* tail;
  size_t size;
  size_t alloc_size;
  size_t unit_size;
};

struct __r_aarr_chunk
{
  void* d;
  size_t alloc_size;
  int full;
  struct __r_aarr_chunk* next;
};

void
make_aarr(struct r_aarr* l, size_t unit_size, size_t init_size);

#define __aarr_inner_idx(idx) ((idx) % MAX_CHUNK_SIZE)

#define aarr_push(aarr, type, valuep) do {\
  try_resize_chunks(aarr);\
  ((type*) ((aarr)->tail->d))[__aarr_inner_idx((aarr)->size)] = *(valuep);\
  ++((aarr)->size);\
  if ((aarr)->size % MAX_CHUNK_SIZE == 0) {\
    (aarr)->tail->full = 1;\
  }\
} while (0)

#define aarr_idx(aarr, type, idx) ((type*) (idx_chunk(aarr, idx)->d))[__aarr_inner_idx(idx)]

#define aarr_enum(aarr, type, en, it) do {\
  size_t ___ ## en = 0;\
  size_t en;\
  struct __r_aarr_chunk* ___for_chk = (aarr)->head;\
  while (___for_chk != 0) {\
    size_t lmt = ___for_chk->full ? MAX_CHUNK_SIZE : ((aarr)->size % MAX_CHUNK_SIZE);\
    for (size_t ___for_for_i = 0; ___for_for_i < lmt; ++___for_for_i) {\
      ++___ ## en;\
      en = ___ ## en - 1;\
      type* it = &(((type*) (___for_chk->d))[___for_for_i]);

#define aarr_for(aarr, type, it) aarr_enum(aarr, type, ___en, it)

#define aarr_end \
    }\
    ___for_chk = ___for_chk->next;\
  }\
} while (0);

void
free_aarr(struct r_aarr* l);

struct __r_aarr_chunk*
idx_chunk(struct r_aarr* l, size_t idx);

void
try_resize_chunks(struct r_aarr* l);

#define new_aarr(l, u, isz)\
  struct r_aarr* l = (struct r_aarr*) malloc(sizeof(struct r_aarr));\
  make_aarr(l, sizeof(u), isz)

#define del_aarr(l)\
  free_aarr(l);\
  free(l);\
  l = 0;

#endif //RRR_LLIST_H
