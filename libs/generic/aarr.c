//
// Created by wo on 5/19/16.
//

#include <assert.h>
#include <string.h>
#include "aarr.h"
#include "../portable.h"

struct __r_aarr_chunk*
make_chunk(size_t unit_size)
{
  struct __r_aarr_chunk* ret;
  posix_memalign((void**) &ret, 16, sizeof(struct __r_aarr_chunk));

  assert(ret != 0);

  ret->d = calloc(INIT_CHUNK_SIZE, unit_size);
  ret->alloc_size = INIT_CHUNK_SIZE;
  ret->next = 0;
  ret->full = 0;

  return ret;
}

void
free_chunk(struct __r_aarr_chunk* chunk)
{
  free(chunk->d);
  free(chunk);
}

struct __r_aarr_chunk*
resize_chunk(struct __r_aarr_chunk* o, size_t unit_size, size_t new_size)
{
  struct __r_aarr_chunk* ret;
  posix_memalign((void**) &ret, 16, sizeof(struct __r_aarr_chunk));
  assert(ret != 0);

  ret->d = calloc(new_size, unit_size);
  memcpy(ret->d, o->d, o->alloc_size * unit_size);
  ret->alloc_size = new_size;
  ret->next = 0;
  ret->full = o->full;

  free_chunk(o);
  return ret;
}

void
make_aarr(struct r_aarr* l, size_t unit_size, size_t init_size)
{
  l->tail = l->head = make_chunk(unit_size);
  l->size = 0;
  l->alloc_size = INIT_CHUNK_SIZE;
  l->unit_size = unit_size;
}

void
new_chunk(struct r_aarr* l)
{
  /**
   * Cannot use this in `make_aarr`, because l->tail is NULL on creation.
   */
  struct __r_aarr_chunk* chunk = make_chunk(l->unit_size);
  l->tail->next = chunk;
  l->tail = chunk;
  l->alloc_size += INIT_CHUNK_SIZE;
}


struct __r_aarr_chunk*
idx_chunk(struct r_aarr* l, size_t idx)
{
  size_t chunk_n = idx / MAX_CHUNK_SIZE;
  struct __r_aarr_chunk* chk = l->head;
  for (size_t i = 0; i < chunk_n; ++i, chk = chk->next);

  return chk;
}

void
resize_last_chunk(struct r_aarr* l)
{
  if (l->head->next == 0) {
    /**
     * The `aarr` has only one chunk.
     */
    l->tail = resize_chunk(l->tail, l->unit_size, l->tail->alloc_size << 1);
    l->head = l->tail;
    l->alloc_size += l->tail->alloc_size >> 1;
  } else {
    struct __r_aarr_chunk* s_tail = 0;
    struct __r_aarr_chunk* cur = l->head;

    while (cur->next->next != 0) {
      cur = cur->next;
    }
    s_tail = cur;

    assert(s_tail->next == l->tail);
    s_tail->next = resize_chunk(l->tail, l->unit_size,
                                l->tail->alloc_size << 1);
    l->tail = s_tail->next;
    l->alloc_size += l->tail->alloc_size >> 1;
  }
}

void
try_resize_chunks(struct r_aarr* l)
{
  if (l->tail->full) {
    new_chunk(l);
  }

  size_t tail_sz = l->size % MAX_CHUNK_SIZE;
  if (tail_sz == l->tail->alloc_size) {
    resize_last_chunk(l);
  }
}

void
free_aarr(struct r_aarr* l)
{
  struct __r_aarr_chunk* cur = l->head;

  while (cur) {
    struct __r_aarr_chunk* next = cur->next;
    free_chunk(cur);
    cur = next;
  }
}